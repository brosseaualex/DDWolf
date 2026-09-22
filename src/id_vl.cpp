// ID_VL.C

#include <string.h>
#include "wl_def.h"
#include "sdl_wrap.h"

#pragma hdrstop

// Uncomment the following line, if you get destination out of bounds
// assertion errors and want to ignore them during debugging
//#define IGNORE_BAD_DEST

#ifdef IGNORE_BAD_DEST
#undef assert
#define assert(x) \
    if (!(x))     \
    return
#define assert_ret(x) \
    if (!(x))         \
    return 0
#else
#define assert_ret(x) assert(x)
#endif

SDL_Surface* screen = NULL;
SDL_Surface* screenBuffer = NULL;

#ifdef SAVE_GAME_SCREENSHOT
SDL_Surface* lastGameSurface = NULL;
#endif

#if SDL_MAJOR_VERSION == 2
Uint32 windowFlags = SDL_WINDOW_OPENGL | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_RESIZABLE;
Uint32 rendererFlags = SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC;
SDL_DisplayMode displayMode;
SDL_Window* window = NULL;
SDL_Renderer* renderer = NULL;
SDL_Texture* texture = NULL;
boolean borderlessFs = true;
boolean vsyncEnabled = true;
#elif SDL_MAJOR_VERSION == 1
Uint32 windowFlags = SDL_SWSURFACE | SDL_FULLSCREEN;
int desktopWidth = 0;
int desktopHeight = 0;
boolean doubleBufferingEnabled = true;
#endif

boolean fullScreen = true;

const unsigned ORIGINAL_SCREEN_WIDTH = 320, ORIGINAL_SCREEN_HEIGHT = 200, DEFAULT_SCREEN_WIDTH = 320, DEFAULT_SCREEN_HEIGHT = 200;

unsigned screenResW, screenResH;
unsigned scaleFactor, scaleOffsetX, scaleOffsetY;
unsigned screenWidth, screenHeight, rescaledWidth, rescaledHeight, screenPitch, bufferPitch;
unsigned picHorizAdjust, picVertAdjust, printHorizAdjust, printVertAdjust;

int screenBits = -1; // use "best" color depth according to libSDL

boolean screenfaded;
unsigned bordercolor;

uint32_t* ylookup;

SDL_Color palette1[256], palette2[256];
SDL_Color curpal[256];

#define CASSERT(x) extern int ASSERT_COMPILE[((x) != 0) * 2 - 1];
#define RGB(r, g, b)                                  \
    {                                                 \
        (r) * 255 / 63, (g)*255 / 63, (b)*255 / 63, 0 \
    }

SDL_Color gamepal[] = {
#ifdef SPEAR
#include "sodpal.inc"
#else
#include "wolfpal.inc"
#endif
};

CASSERT(lengthof(gamepal) == 256)

#ifdef SPEAR
const char* title = "Spear of Destiny";
#else
const char* title = "Wolfenstein 3D";
#endif

//===========================================================================

/*
=======================
=
= VL_Shutdown
=
=======================
*/

void VL_Shutdown(void) {
	SDL_FreeSurface(screenBuffer);
#if SDL_MAJOR_VERSION == 2
	SDL_DestroyRenderer(renderer);
	SDL_DestroyWindow(window);
	SDL_DestroyTexture(texture);
#endif
	free(ylookup);
	free(pixelangle);
	free(wallheight);
	screenBuffer = NULL;
	ylookup = NULL;
	pixelangle = NULL;
	wallheight = NULL;
#if defined(USE_FLOORCEILINGTEX) || defined(USE_CLOUDSKY)
	free(spanstart);
	spanstart = NULL;
#endif
}

/*
=======================
=
= VL_SetVGAPlaneMode
=
=======================
*/
void VL_SetVGAPlaneMode(void)
{
#if SDL_MAJOR_VERSION == 2
	uint32_t a, r, g, b;
#elif SDL_MAJOR_VERSION == 1
	SDL_WM_SetCaption(title, NULL);
#endif

	// Ensure saved screen/window configuration (if it exists)
	// are loaded before generating the window.
	ReadDisplayConfig();

#if SDL_MAJOR_VERSION == 2
	if (param_novsync || !vsyncEnabled)
		rendererFlags &= ~SDL_RENDERER_PRESENTVSYNC;
#endif

	if (param_forcewindowed)
		fullScreen = false;

	if (param_resx > 0)
		screenResW = param_resx;

	if (param_resy > 0)
		screenResH = param_resy;

	if (screenResW <= 0)
		screenResW = DEFAULT_SCREEN_WIDTH;

	if (screenResH <= 0)
		screenResH = DEFAULT_SCREEN_HEIGHT;

	screenWidth = screenResW;
	screenHeight = screenResH;

#if SDL_MAJOR_VERSION == 2
	window = SDL_CreateWindow(title, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, screenWidth, screenHeight, windowFlags);
	SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ARGB8888, &screenBits, &r, &g, &b, &a);

	screen = SDL_CreateRGBSurface(0, screenWidth, screenHeight, screenBits, r, g, b, a);
	if (!screen) {
		printf("Unable to set %ix%ix%i video mode: %s\n", screenWidth, screenHeight, screenBits, SDL_GetError());
		exit(1);
	}

	screenBuffer = SDL_CreateRGBSurface(0, screenWidth, screenHeight, 8, 0, 0, 0, 0);
#elif SDL_MAJOR_VERSION == 1
	if (screenBuffer)
	{
		SDL_FreeSurface(screenBuffer);
		screenBuffer = NULL;
	}

	screenBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, screenWidth, screenHeight, 8, 0, 0, 0, 0);
#endif

	if (!screenBuffer)
	{
		printf("Unable to create screen buffer surface: %s\n", SDL_GetError());
		exit(1);
	}

#if SDL_MAJOR_VERSION == 2
	renderer = SDL_CreateRenderer(window, -1, rendererFlags);

	SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
	SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");
	SDL_ShowCursor(SDL_DISABLE);
	SDL_SetPaletteColors(screen->format->palette, gamepal, 0, 256);
#endif

	memcpy(curpal, gamepal, sizeof(SDL_Color) * 256);

#if SDL_MAJOR_VERSION == 2
	SDL_SetPaletteColors(screenBuffer->format->palette, gamepal, 0, 256);
	texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, screenWidth, screenHeight);
#elif SDL_MAJOR_VERSION == 1
	if (screenBuffer->format && screenBuffer->format->palette)
		SDL_SetColors(screenBuffer, gamepal, 0, 256);

	memset(screenBuffer->pixels, 0, screenBuffer->pitch * screenHeight);
#endif

	VL_SetDisplayResolution(screenWidth, screenHeight);

	NewViewSize(viewsize);

	IN_MouseGrab();
}

/*
=============================================================================

						PALETTE OPS

		To avoid snow, do a WaitVBL BEFORE calling these

=============================================================================
*/

/*
=================
=
= VL_ConvertPalette
=
=================
*/

void VL_ConvertPalette(byte* srcpal, SDL_Color* destpal, int numColors) {
	int i;

	for (i = 0; i < numColors; i++) {
		destpal[i].r = *srcpal++ * 255 / 63;
		destpal[i].g = *srcpal++ * 255 / 63;
		destpal[i].b = *srcpal++ * 255 / 63;
	}
}

/*
=================
=
= VL_FillPalette
=
=================
*/

void VL_FillPalette(int red, int green, int blue) {
	int i;
	SDL_Color pal[256];

	for (i = 0; i < 256; i++) {
		pal[i].r = red;
		pal[i].g = green;
		pal[i].b = blue;
	}

	VL_SetPalette(pal, true);
}

//===========================================================================

/*
=================
=
= VL_SetColor
=
=================
*/
void VL_SetColor(int color, int red, int green, int blue)
{
	SDL_Color col = { (Uint8)red, (Uint8)green, (Uint8)blue
#if SDL_MAJOR_VERSION == 2
		, 255
#endif
	};

	curpal[color] = col;

#if SDL_MAJOR_VERSION == 2
	if (screenBits == 8 && screen && screen->format && screen->format->palette)
		SDL_SetPaletteColors(screen->format->palette, &col, color, 1);
	else if (screenBuffer && screenBuffer->format && screenBuffer->format->palette)
		SDL_SetPaletteColors(screenBuffer->format->palette, &col, color, 1);
#elif SDL_MAJOR_VERSION == 1
	if (screenBuffer && screenBuffer->format && screenBuffer->format->palette)
		SDL_SetColors(screenBuffer, &col, color, 1);

	if (screen && screen->format && screen->format->palette)
		SDL_SetColors(screen, &col, color, 1);
#endif
}

//===========================================================================

/*
=================
=
= VL_GetColor
=
=================
*/

void VL_GetColor(int color, int* red, int* green, int* blue) {
	SDL_Color* col = &curpal[color];
	*red = col->r;
	*green = col->g;
	*blue = col->b;
}

//===========================================================================

/*
=================
=
= VL_SetPalette
=
=================
*/
void VL_SetPalette(SDL_Color* palette, bool forceupdate)
{
	if (!palette)
		return;

	memcpy(curpal, palette, sizeof(SDL_Color) * 256);

#if SDL_MAJOR_VERSION == 2
	if (screenBits == 8)
	{
		if (screen && screen->format && screen->format->palette)
			SDL_SetPaletteColors(screen->format->palette, palette, 0, 256);
	}
	else
	{
		if (screenBuffer && screenBuffer->format && screenBuffer->format->palette)
			SDL_SetPaletteColors(screenBuffer->format->palette, palette, 0, 256);

		if (forceupdate && screenBuffer)
		{
			if (screen)
				SDL_BlitSurface(screenBuffer, NULL, screen, NULL);

			Present(screenBuffer);
		}
	}
#elif SDL_MAJOR_VERSION == 1
	if (screenBuffer && screenBuffer->format && screenBuffer->format->palette)
		SDL_SetColors(screenBuffer, palette, 0, 256);

	if (screen && screen->format && screen->format->palette)
		SDL_SetColors(screen, palette, 0, 256);

	if (forceupdate && screenBuffer)
		Present(screenBuffer);
#endif
}

//===========================================================================

/*
=================
=
= VL_GetPalette
=
=================
*/

void VL_GetPalette(SDL_Color* palette) {
	memcpy(palette, curpal, sizeof(SDL_Color) * 256);
}

//===========================================================================

/*
=================
=
= VL_FadeOut
=
= Fades the current palette to the given color in the given number of steps
=
=================
*/

void VL_FadeOut(int start, int end, int red, int green, int blue, int steps) {
#ifdef NO_TIME_WASTE
	steps = 0;
#endif

	int i, j, orig, delta;
	SDL_Color* origptr, * newptr;

	red = red * 255 / 63;
	green = green * 255 / 63;
	blue = blue * 255 / 63;

	VL_WaitVBL(1);
	VL_GetPalette(palette1);
	memcpy(palette2, palette1, sizeof(SDL_Color) * 256);

	//
	// fade through intermediate frames
	//
	for (i = 0; i < steps; i++) {
		origptr = &palette1[start];
		newptr = &palette2[start];
		for (j = start; j <= end; j++) {
			orig = origptr->r;
			delta = red - orig;
			newptr->r = orig + delta * i / steps;
			orig = origptr->g;
			delta = green - orig;
			newptr->g = orig + delta * i / steps;
			orig = origptr->b;
			delta = blue - orig;
			newptr->b = orig + delta * i / steps;
			origptr++;
			newptr++;
		}

#if (SDL_MAJOR_VERSION == 2)
		if (screenBits == 8)
#elif (SDL_MAJOR_VERSION == 1)
		if (!doubleBufferingEnabled || screenBits == 8)
#endif
			VL_SetPalette(palette2, true);
	}

	//
	// final color
	//
	VL_FillPalette(red, green, blue);

	screenfaded = true;
}

/*
=================
=
= VL_FadeIn
=
=================
*/

void VL_FadeIn(int start, int end, SDL_Color* palette, int steps) {
#ifdef NO_TIME_WASTE
	steps = 0;
#endif

	int i, j, delta;

	VL_WaitVBL(1);
	VL_GetPalette(palette1);
	memcpy(palette2, palette1, sizeof(SDL_Color) * 256);

	//
	// fade through intermediate frames
	//
	for (i = 0; i < steps; i++) {
		for (j = start; j <= end; j++) {
			delta = palette[j].r - palette1[j].r;
			palette2[j].r = palette1[j].r + delta * i / steps;
			delta = palette[j].g - palette1[j].g;
			palette2[j].g = palette1[j].g + delta * i / steps;
			delta = palette[j].b - palette1[j].b;
			palette2[j].b = palette1[j].b + delta * i / steps;
		}

#if (SDL_MAJOR_VERSION == 2)
		if (screenBits == 8)
#elif (SDL_MAJOR_VERSION == 1)
		if (!doubleBufferingEnabled || screenBits == 8)
#endif		
			VL_WaitVBL(1);
		VL_SetPalette(palette2, true);
	}

	//
	// final color
	//
	VL_SetPalette(palette, true);
	screenfaded = false;
}

/*
=============================================================================

							PIXEL OPS

=============================================================================
*/

byte* VL_LockSurface(SDL_Surface* surface) {
	if (SDL_MUSTLOCK(surface)) {
		if (SDL_LockSurface(surface) < 0)
			return NULL;
	}
	return (byte*)surface->pixels;
}

void VL_UnlockSurface(SDL_Surface* surface) {
	if (SDL_MUSTLOCK(surface)) {
		SDL_UnlockSurface(surface);
	}
}

/*
=================
=
= VL_Plot
=
=================
*/

void VL_Plot(int x, int y, int color) {
	byte* dest;

	assert(x >= 0 && (unsigned)x < screenWidth && y >= 0 && (unsigned)y < screenHeight &&
		"VL_Plot: Pixel out of bounds!");

	dest = VL_LockSurface(screenBuffer);
	if (dest == NULL)
		return;

	dest[ylookup[y] + x] = color;

	VL_UnlockSurface(screenBuffer);
}

/*
=================
=
= VL_GetPixel
=
=================
*/

byte VL_GetPixel(int x, int y)
{
	byte col;

	assert_ret(x >= 0 && (unsigned)x < screenWidth && y >= 0 && (unsigned)y < screenHeight && "VL_GetPixel: Pixel out of bounds!");

	if (!VL_LockSurface(screenBuffer))
		return 0;

	col = ((byte*)screenBuffer->pixels)[ylookup[y] + x];

	VL_UnlockSurface(screenBuffer);

	return col;
}
#ifdef SAVE_GAME_SCREENSHOT
byte VL_GetPixel(SDL_Surface* surface, int x, int y) {
	byte col;

	assert_ret(x >= 0 && (unsigned)x < screenWidth && y >= 0 && (unsigned)y < screenHeight && "VL_GetPixel: Pixel out of bounds!");

	if (!VL_LockSurface(surface))
		return 0;

	col = ((byte*)surface->pixels)[ylookup[y] + x];

	VL_UnlockSurface(surface);

	return col;
}

/*
==============================
=
= VL_DuplicateSurface
=
= Deep copies SDL_Surface
=
==============================
*/
SDL_Surface* VL_DuplicateSurface(SDL_Surface* surf) {
	SDL_Surface* cpy;
	cpy = (SDL_Surface*)malloc(sizeof(SDL_Surface));
	memcpy((SDL_Surface*)cpy, (SDL_Surface*)surf, sizeof(SDL_Surface));
	cpy->format = (SDL_PixelFormat*)malloc(sizeof(SDL_PixelFormat));
	memcpy((SDL_PixelFormat*)cpy->format, (SDL_PixelFormat*)surf->format,
		sizeof(SDL_PixelFormat));
	size_t size = surf->pitch * surf->h;
	cpy->pixels = malloc(size);
	memcpy((Uint8*)cpy->pixels, (Uint8*)surf->pixels, size * sizeof(Uint8));
	return cpy;
}

/*
==============================
=
= VL_DrawPixel
=
= Draws the pixel onto the surface
=
==============================
*/
void VL_DrawPixel(SDL_Surface* surface, int x, int y, Uint32 pixel) {
	int bpp = surface->format->BytesPerPixel;
	/* Here p is the address to the pixel we want to set */
	Uint8* p = (Uint8*)surface->pixels + y * surface->pitch + x * bpp;

	switch (bpp) {
	case 1:
		*p = pixel;
		break;

	case 2:
		*(Uint16*)p = pixel;
		break;

	case 3:
		if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
			p[0] = (pixel >> 16) & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = pixel & 0xff;
		}
		else {
			p[0] = pixel & 0xff;
			p[1] = (pixel >> 8) & 0xff;
			p[2] = (pixel >> 16) & 0xff;
		}
		break;

	case 4:
		*(Uint32*)p = pixel;
		break;
	}
}

/*
=================================
=
= VL_ScaleSurface
=
= Creates a surface to scaled width, then scales it accordingly
= Width and Height can be any size, not multiples of 320x200
=
=================================
*/
SDL_Surface* VL_ScaleSurface(SDL_Surface* surface, Uint16 width, Uint16 height) {
	if (!surface || !width || !height)
		return nullptr;

	SDL_Surface* scaledSurface = SDL_CreateRGBSurface(surface->flags, width, height, 8, 0, 0, 0, 0);

#if SDL_MAJOR_VERSION == 2
	SDL_SetPaletteColors(scaledSurface->format->palette, gamepal, 0, 256);
#elif SDL_MAJOR_VERSION == 1
	SDL_SetPalette(scaledSurface, SDL_LOGPAL | SDL_PHYSPAL, gamepal, 0, 256);
#endif

	double stretchFactorX = static_cast<double>(width) / static_cast<double>(surface->w);
	double stretchFactorY = static_cast<double>(height) / static_cast<double>(surface->h);

	for (Sint32 y = 0; y < surface->h; y++) {
		for (Sint32 x = 0; x < surface->w; x++) {
			for (Sint32 o_y = 0; o_y < stretchFactorY; ++o_y) {
				for (Sint32 o_x = 0; o_x < stretchFactorX; ++o_x) {
					VL_DrawPixel(scaledSurface, static_cast<Sint32>(stretchFactorX * x) + o_x,
						static_cast<Sint32>(stretchFactorY * y) + o_y, VL_GetPixel(surface, x, y));
				}
			}
		}
	}

	return scaledSurface;
}

/*
=================
=
= VL_SetSaveGameSlot
=
=================
*/
void VL_SetSaveGameSlot() {
	lastGameSurface = VL_ScaleSurface(VL_DuplicateSurface(screenBuffer), 128, 80);
}

#endif

/*
=================
=
= VL_Hlin
=
=================
*/

void VL_Hlin(unsigned x, unsigned y, unsigned width, int color) {
	byte* dest;

	assert(x >= 0 && x + width <= screenWidth && y >= 0 && y < screenHeight && "VL_Hlin: Destination rectangle out of bounds!");

	dest = VL_LockSurface(screenBuffer);
	if (dest == NULL)
		return;

	memset(dest + ylookup[y] + x, color, width);

	VL_UnlockSurface(screenBuffer);
}

/*
=================
=
= VL_Vlin
=
=================
*/

void VL_Vlin(int x, int y, int height, int color) {
	byte* dest;

	assert(x >= 0 && (unsigned)x < screenWidth && y >= 0 && (unsigned)y + height <= screenHeight &&
		"VL_Vlin: Destination rectangle out of bounds!");

	dest = VL_LockSurface(screenBuffer);
	if (dest == NULL)
		return;

	dest += ylookup[y] + x;

	while (height--) {
		*dest = color;
		dest += bufferPitch;
	}

	VL_UnlockSurface(screenBuffer);
}

/*
=================
=
= VL_Bar
=
=================
*/

void VL_Bar(int x, int y, int width, int height, int color) {
	VL_BarScaledCoord(scaleFactor * x, scaleFactor * y, scaleFactor * width, scaleFactor * height, color);
}

void VL_BarScaledCoord(int scx, int scy, int scwidth, int scheight, int color) {
	byte* dest;

	// --- CLIP COORDINATES TO SCREEN BOUNDS ---
	if (scx < 0) {
		scwidth += scx; // Reduce width by negative offset
		scx = 0;
	}
	if (scy < 0) {
		scheight += scy; // Reduce height by negative offset
		scy = 0;
	}
	if (scx >= (int)screenWidth || scy >= (int)screenHeight || scwidth <= 0 || scheight <= 0) {
		return; // Completely off screen, nothing to draw
	}
	if (scx + scwidth > (int)screenWidth) {
		scwidth = (int)screenWidth - scx;
	}
	if (scy + scheight > (int)screenHeight) {
		scheight = (int)screenHeight - scy;
	}
	// ------------------------------------------

	dest = VL_LockSurface(screenBuffer);
	if (dest == NULL)
		return;

	dest += ylookup[scy] + scx;

	while (scheight--) {
		memset(dest, color, scwidth);
		dest += bufferPitch;
	}
	VL_UnlockSurface(screenBuffer);
}

/*
============================================================================

							MEMORY OPS

============================================================================
*/

/*
===================
=
= VL_SurfaceToByteArray
=
= This function converts the pixel data from an SDL surface into a byte array.
=
===================
*/

void VL_SurfaceToByteArray(SDL_Surface* surface, byte* byteArray) {
	if (!surface || !byteArray) {
		return;
	}

	int width = surface->w;
	int height = surface->h;
	VL_LockSurface(surface);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			byte colorIndex = ((byte*)surface->pixels)[y * surface->pitch + x];
			byteArray[y * width + x] = colorIndex;
		}
	}

	VL_UnlockSurface(surface);
}

/*
===================
=
= VL_DePlaneVGA
=
= Unweave a VGA graphic to simplify drawing
=
===================
*/

void VL_DePlaneVGA(byte* source, int width, int height) {
	int x, y, plane;
	word size, pwidth;
	byte* temp, * dest, * srcline;

	size = width * height;

	if (width & 3)
		Quit("DePlaneVGA: width not divisible by 4!");

	temp = (byte*)SafeMalloc(size);

	//
	// munge pic into the temp buffer
	//
	srcline = source;
	pwidth = width >> 2;

	for (plane = 0; plane < 4; plane++) {
		dest = temp;

		for (y = 0; y < height; y++) {
			for (x = 0; x < pwidth; x++)
				*(dest + (x << 2) + plane) = *srcline++;

			dest += width;
		}
	}

	//
	// copy the temp buffer back into the original source
	//
	memcpy(source, temp, size);

	free(temp);
}

/*
=================
=
= VL_MemToScreenScaledCoord
=
= Draws a block of data to the screen with scaling according to scaleFactor.
=
=================
*/

void VL_MemToScreen(byte* source, int width, int height, int x, int y) {
	VL_MemToScreenScaledCoord(source, width, height, scaleFactor * x, scaleFactor * y);
}

void VL_MemToScreenScaledCoord(byte* source, int width, int height, int destx, int desty) {
	byte* dest;
	int i, j, sci, scj;
	unsigned m, n;

	if (!screenBuffer || (unsigned)destx >= screenWidth || (unsigned)desty >= screenHeight)
		return;

	dest = VL_LockSurface(screenBuffer);
	if (dest == NULL)
		return;

	for (j = 0, scj = 0; j < height; j++, scj += scaleFactor) {
		unsigned py = scj + desty;
		if (py >= screenHeight) break;

		for (i = 0, sci = 0; i < width; i++, sci += scaleFactor) {
			unsigned px = sci + destx;

			byte col = source[(j * width) + i];

			for (m = 0; m < scaleFactor; m++) {
				unsigned targetY = py + m;
				if (targetY >= screenHeight) break;

				for (n = 0; n < scaleFactor; n++) {
					unsigned targetX = px + n;
					if (targetX >= screenWidth) break;

					dest[ylookup[targetY] + targetX] = col;
				}
			}
		}
	}
	VL_UnlockSurface(screenBuffer);
}

/*
=================
=
= VL_MemToScreenScaledCoord
=
= Draws a part of a block of data to the screen.
= The block has the size origwidth*origheight.
= The part at (srcx, srcy) has the size width*height
= and will be painted to (destx, desty) with scaling according to scaleFactor.
=
=================
*/

void VL_MemToScreenScaledCoord2(byte* source, int origwidth, int origheight, int srcx, int srcy,
	int destx, int desty, int width, int height) {
	byte* dest;
	int i, j, sci, scj;
	unsigned m, n;

	if (!screenBuffer || (unsigned)destx >= screenWidth || (unsigned)desty >= screenHeight)
		return;

	dest = VL_LockSurface(screenBuffer);
	if (dest == NULL)
		return;

	for (j = 0, scj = 0; j < height; j++, scj += scaleFactor) {
		unsigned py = scj + desty;
		if (py >= screenHeight) break;

		for (i = 0, sci = 0; i < width; i++, sci += scaleFactor) {
			unsigned px = sci + destx;
			if (px >= screenWidth) break;

			if ((j + srcy) >= origheight || (i + srcx) >= origwidth)
				continue;

			byte col = source[((j + srcy) * origwidth) + (i + srcx)];

			for (m = 0; m < scaleFactor; m++) {
				unsigned targetY = py + m;
				if (targetY >= screenHeight) break;

				for (n = 0; n < scaleFactor; n++) {
					unsigned targetX = px + n;
					if (targetX >= screenWidth) break;

					dest[ylookup[targetY] + targetX] = col;
				}
			}
		}
	}
	VL_UnlockSurface(screenBuffer);
}

//==========================================================================

/*
=================
=
= VL_ScreenToScreen
=
=================
*/

void VL_ScreenToScreen(SDL_Surface* source, SDL_Surface* dest) {
	SDL_BlitSurface(source, NULL, dest, NULL);
}

/*
=================
=
= VL_GetFirstColoredPixel
=
= This function is mainly to resolve issues around the coloring of the
= VW_Bar showing messages in the Signon screen when using HD scaling.
=
= By default, it used to pick the pixel at 0,0 in the Signon screen
= (a red pixel) to color the bar accordingly.
=
= With HD, scaling, the first pixel of the screen is not the correct place to look.
=
= In order to be able to achieve the same result, regardless of scaling, we now look
= for the first colored pixel on the screen.
=
= This is far from being super efficient but it should always work, regardless of the scaling.
=
=================
*/

byte VL_GetFirstColoredPixel(SDL_Surface* surface)
{
	byte color = 0;

	int colorX = 0;
	int colorY = 0;

	SDL_LockSurface(surface);
	Uint8* pixels = static_cast<Uint8*>(surface->pixels);

	int width = surface->w;
	int height = surface->h;
	Uint8 pixel = 0;

	for (int y = 0; y < height; ++y) {
		if (pixel != 0)
			break;

		for (int x = 0; x < width; ++x) {
			pixel = pixels[y * width + x];

			if (pixel != 0 && pixel != SDL_MapRGB(surface->format, 0, 0, 0)) {
				colorX = x;
				colorY = y;
				break;
			}
		}
	}

	color = ((byte*)surface->pixels)[ylookup[colorY] + colorX];

	SDL_UnlockSurface(surface);

	return color;
}

void VL_CreateYlookup(void)
{
	if (!screenBuffer)
		return;

	if (ylookup)
	{
		free(ylookup);
		ylookup = NULL;
	}

	ylookup = (uint32_t*)malloc(sizeof(uint32_t) * screenBuffer->h);
	if (!ylookup)
	{
		printf("VL_CreateYlookup: Out of memory!\n");
		exit(1);
	}

	bufferPitch = screenBuffer->pitch;

	for (int y = 0; y < screenBuffer->h; y++)
		ylookup[y] = (uint32_t)(y * screenBuffer->pitch);
}

/*
=================
=
= VL_UpdateUIScale
=
=================
*/
void VL_UpdateUIScale(int newWidth, int newHeight)
{
	unsigned i = 0;

	screenPitch = screen->pitch;
	bufferPitch = screenBuffer->pitch;

	screenWidth = newWidth;
	screenHeight = newHeight;

	int scaleX = screenWidth / ORIGINAL_SCREEN_WIDTH;
	int scaleY = screenHeight / ORIGINAL_SCREEN_HEIGHT;

	scaleFactor = (scaleX < scaleY) ? scaleX : scaleY;

	if (scaleFactor < 1) scaleFactor = 1;
	if (screenHeight / ORIGINAL_SCREEN_HEIGHT < scaleFactor)
		scaleFactor = screenHeight / ORIGINAL_SCREEN_HEIGHT;

	printHorizAdjust = picHorizAdjust / scaleFactor;
	printVertAdjust = picVertAdjust / scaleFactor;

	scaleOffsetX = (screenWidth - scaleFactor * ORIGINAL_SCREEN_WIDTH) / (2 * scaleFactor);
	scaleOffsetY = (screenHeight - scaleFactor * ORIGINAL_SCREEN_HEIGHT) / (2 * scaleFactor);

	rescaledWidth = screenWidth / scaleFactor;
	rescaledHeight = screenHeight / scaleFactor;

	ylookup = (uint32_t*)SafeMalloc(screenHeight * sizeof(*ylookup));
	pixelangle = (short*)SafeMalloc(screenWidth * sizeof(*pixelangle));
	wallheight = (int16_t*)SafeMalloc(screenWidth * sizeof(*wallheight));

#if defined(USE_FLOORCEILINGTEX) || defined(USE_CLOUDSKY)
	spanstart = (int16_t*)SafeMalloc((screenHeight / 2) * sizeof(*spanstart));
#endif

	for (i = 0; i < screenHeight; i++)
		ylookup[i] = i * bufferPitch;
}

#if SDL_MAJOR_VERSION == 1
/*
=================
=
= VL_GetTargetAspectRatio
=
=================
*/
float VL_GetTargetAspectRatio(int srcW, int srcH)
{
	if ((srcW == 320 && srcH == 200) || (srcW == 640 && srcH == 400) || (srcW == 1280 && srcH == 800))
		return 4.0f / 3.0f;
	return (float)srcW / (float)srcH;
}
/*
=================
=
= VL_CacheDesktopResolution
=
=================
*/
void VL_CacheDesktopResolution(void)
{
	if (desktopWidth <= 0 || desktopHeight <= 0)
	{
		const SDL_VideoInfo* info = SDL_GetVideoInfo();
		if (info && info->current_w > 0 && info->current_h > 0)
		{
			desktopWidth = info->current_w;
			desktopHeight = info->current_h;
		}
		else
		{
			desktopWidth = 1920;
			desktopHeight = 1080;
		}
	}
}
#endif

/*
=================
=
= VL_ApplyDisplaySettings
=
=================
*/
void VL_ApplyDisplaySettings(void)
{
#if SDL_MAJOR_VERSION == 2
	if (!window || !renderer)
		return;

	if (fullScreen)
	{
		if (borderlessFs)
		{
			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
		}
		else
		{
			SDL_DisplayMode desktopMode;
			if (SDL_GetDesktopDisplayMode(0, &desktopMode) == 0)
				SDL_SetWindowDisplayMode(window, &desktopMode);

			SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN);
		}
	}
	else
	{
		SDL_SetWindowFullscreen(window, 0);
		SDL_SetWindowSize(window, screenWidth, screenHeight);
		SDL_SetWindowPosition(window, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}

	SDL_RenderSetVSync(renderer, vsyncEnabled ? true : false);
#elif SDL_MAJOR_VERSION == 1
	//SDL1 way of forcing the window to center on the screen
#if defined(_WIN32)
	putenv("SDL_VIDEO_WINDOW_POS=center");
#else
	setenv("SDL_VIDEO_WINDOW_POS", "center", 1);
#endif

	windowFlags = SDL_SWSURFACE;

	VL_CacheDesktopResolution();

	int targetW = screenWidth;
	int targetH = screenHeight;

	if (fullScreen)
	{
		windowFlags |= SDL_FULLSCREEN;
		targetW = desktopWidth;
		targetH = desktopHeight;
	}

	screen = SDL_SetVideoMode(targetW, targetH, 0, windowFlags);
	if (!screen)
	{
		screen = SDL_SetVideoMode(screenWidth, screenHeight, 0, windowFlags);
		if (!screen)
		{
			printf("VL_ApplyDisplaySettings: Failed to set video mode (%dx%d)!\n", targetW, targetH);
			exit(1);
		}
	}

	if (screen && screen->format && screen->format->palette)
		SDL_SetColors(screen, curpal, 0, 256);

	VL_CreateYlookup();

	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
	SDL_UpdateRect(screen, 0, 0, 0, 0);
#endif
}

/*
=================
=
= VL_SetDisplayResolution
=
=================
*/
void VL_SetDisplayResolution(int newWidth, int newHeight)
{
	SDL_Color tempPalette[256];
	bool keepPalette = false;

	if (screenBuffer && screenBuffer->format && screenBuffer->format->palette)
	{
		memcpy(tempPalette, screenBuffer->format->palette->colors, sizeof(SDL_Color) * 256);
		keepPalette = true;
	}

	screenWidth = newWidth;
	screenHeight = newHeight;

	if (screenBuffer)
	{
		SDL_FreeSurface(screenBuffer);
		screenBuffer = NULL;
	}

#if SDL_MAJOR_VERSION == 2
	screenBuffer = SDL_CreateRGBSurface(0, screenWidth, screenHeight, 8, 0, 0, 0, 0);
#elif SDL_MAJOR_VERSION == 1
	screenBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, screenWidth, screenHeight, 8, 0, 0, 0, 0);
#endif

	if (!screenBuffer)
	{
		printf("VL_SetDisplayResolution: Failed to create screenBuffer surface!\n");
		exit(1);
	}

#if SDL_MAJOR_VERSION == 2
	if (screenBuffer->format && !screenBuffer->format->palette)
	{
		SDL_PixelFormat* fmt = screenBuffer->format;
		SDL_SetPixelFormatPalette(fmt, SDL_AllocPalette(256));
	}

	if (keepPalette && screenBuffer->format && screenBuffer->format->palette)
		SDL_SetPaletteColors(screenBuffer->format->palette, tempPalette, 0, 256);

	VL_CreateYlookup();
#elif SDL_MAJOR_VERSION == 1
	if (keepPalette && screenBuffer->format && screenBuffer->format->palette)
		SDL_SetColors(screenBuffer, tempPalette, 0, 256);
#endif

	memset(screenBuffer->pixels, 0, screenBuffer->pitch * screenHeight);

	VL_ApplyDisplaySettings();

#if SDL_MAJOR_VERSION == 2
	if (texture)
	{
		SDL_DestroyTexture(texture);
		texture = NULL;
	}

	if (renderer)
	{
		SDL_RenderSetLogicalSize(renderer, 0, 0);

		texture = SDL_CreateTexture(
			renderer,
			SDL_PIXELFORMAT_ARGB8888,
			SDL_TEXTUREACCESS_STREAMING,
			screenWidth,
			screenHeight
		);

		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
	}
#endif

	VL_UpdateUIScale(screenWidth, screenHeight);

	screenResW = screenWidth;
	screenResH = screenHeight;

	NewViewSize(viewsize);
}