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

boolean fullscreen = true;
boolean usedoublebuffering = true;
boolean disablehdres = false;
boolean disableratiofix = false;
unsigned originalScreenWidth = 320;
unsigned originalScreenHeight = 240;
unsigned defaultScreenWidth = 640; // DO NOT CHANGE
unsigned defaultScreenHeight = 480; // DO NOT CHANGE
unsigned screenWidth = 640;
unsigned screenHeight = 480;
int screenBits = -1; // use "best" color depth according to libSDL
int scalingOffsetX = 0; // Used with HD scaling to calculate and center screens
int scalingOffsetY = 0;
int ratioCorrection = 20;

SDL_Surface *screen = NULL;
unsigned screenPitch;

SDL_Surface *screenBuffer = NULL;
unsigned bufferPitch;

#ifdef SAVE_GAME_SCREENSHOT
SDL_Surface *lastGameSurface = NULL;
#endif

#if SDL_MAJOR_VERSION == 2
SDL_Window *window = NULL;
SDL_Renderer *renderer = NULL;
SDL_Texture *texture = NULL;
#endif

int scaleFactor;
int rescaledWidth;
int rescaledHeight;

boolean screenfaded;
unsigned bordercolor;

uint32_t *ylookup;

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

int picHorizAdjust;
int picVertAdjust;
int printHorizAdjust;
int printVertAdjust;

CASSERT(lengthof(gamepal) == 256)

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
#if defined(USE_FLOORCEILINGTEX) || defined(USE_CLOUDSKY)
    free(spanstart);

    spanstart = NULL;
#endif
    screenBuffer = NULL;
    ylookup = NULL;
    pixelangle = NULL;
    wallheight = NULL;
}

/*
=======================
=
= VL_SetVGAPlaneMode
=
=======================
*/

void VL_SetVGAPlaneMode(void) {
    int i;
    uint32_t a, r, g, b;

    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);
#ifdef SPEAR
    const char* title = "Spear of Destiny";
#else
    const char *title = "Wolfenstein 3D";
#endif

#if SDL_MAJOR_VERSION == 1
    SDL_WM_SetCaption(title, NULL);

    if (screenBits == -1)
    {
        const SDL_VideoInfo* vidInfo = SDL_GetVideoInfo();
        screenBits = vidInfo->vfmt->BitsPerPixel;
    }

    screen = SDL_SetVideoMode(screenWidth, screenHeight, screenBits,
        (usedoublebuffering ? SDL_HWSURFACE | SDL_DOUBLEBUF : 0) | (screenBits == 8 ? SDL_HWPALETTE : 0) | (fullscreen ? SDL_FULLSCREEN : 0));
    if (!screen)
    {
        printf("Unable to set %ix%ix%i video mode: %s\n", screenWidth,
            screenHeight, screenBits, SDL_GetError());
        exit(1);
    }
    if ((screen->flags & SDL_DOUBLEBUF) != SDL_DOUBLEBUF)
        usedoublebuffering = false;
    SDL_ShowCursor(SDL_DISABLE);

    SDL_SetColors(screen, gamepal, 0, 256);
    memcpy(curpal, gamepal, sizeof(SDL_Color) * 256);

    screenBuffer = SDL_CreateRGBSurface(SDL_SWSURFACE, screenWidth,
        screenHeight, 8, 0, 0, 0, 0);
    if (!screenBuffer)
    {
        printf("Unable to create screen buffer surface: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_SetColors(screenBuffer, gamepal, 0, 256);
#else
    if (disableratiofix)
    {
        originalScreenHeight = 200;
        defaultScreenHeight = 400;
        ratioCorrection = 0;
    }

    if (fullscreen && !disablehdres)
    {
        screenWidth = displayMode.w;
        screenHeight = displayMode.h;
    }

    window = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight,
                              (fullscreen ? SDL_WINDOW_FULLSCREEN : 0) | SDL_WINDOW_OPENGL);

    SDL_PixelFormatEnumToMasks(SDL_PIXELFORMAT_ARGB8888, &screenBits, &r, &g, &b, &a);

    if(disablehdres)
    {
        screenWidth = defaultScreenWidth;
        screenHeight = defaultScreenHeight;
    }

    screen = SDL_CreateRGBSurface(0, screenWidth, screenHeight, screenBits, r, g, b, a);

    if (!screen) {
        printf("Unable to set %ix%ix%i video mode: %s\n", screenWidth, screenHeight, screenBits, SDL_GetError());
        exit(1);
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    SDL_SetRenderDrawBlendMode(renderer, SDL_BLENDMODE_BLEND);
    SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "0");

    SDL_ShowCursor(SDL_DISABLE);

    SDL_SetPaletteColors(screen->format->palette, gamepal, 0, 256);
    memcpy(curpal, gamepal, sizeof(SDL_Color) * 256);

    screenBuffer = SDL_CreateRGBSurface(0, screenWidth,
                                        screenHeight, 8, 0, 0, 0, 0);
    if (!screenBuffer) {
        printf("Unable to create screen buffer surface: %s\n", SDL_GetError());
        exit(1);
    }
    SDL_SetPaletteColors(screenBuffer->format->palette, gamepal, 0, 256);

    texture = SDL_CreateTexture(renderer,
                                SDL_PIXELFORMAT_ARGB8888,
                                SDL_TEXTUREACCESS_STREAMING,
                                screenWidth, screenHeight);
#endif

    screenPitch = screen->pitch;
    bufferPitch = screenBuffer->pitch;

    scaleFactor = screenWidth / originalScreenWidth;

    if (screenHeight / originalScreenHeight < scaleFactor)
        scaleFactor = screenHeight / originalScreenHeight;

#ifdef _DEBUG
    printf("VL_SetVGAPlaneMode - scaleFactor: %d\n", scaleFactor);
#endif

    printHorizAdjust = picHorizAdjust / scaleFactor;
    printVertAdjust = picVertAdjust / scaleFactor;

    scalingOffsetX = (screenWidth - scaleFactor * originalScreenWidth) / (2 * scaleFactor);
    scalingOffsetY = (screenHeight - scaleFactor * originalScreenHeight) / (2 * scaleFactor) + ratioCorrection;

    rescaledWidth = screenWidth / scaleFactor;
    rescaledHeight = screenHeight / scaleFactor;

#ifdef _DEBUG
    printf("VL_SetVGAPlaneMode - screenOffsetX: %d\n", scalingOffsetX);
    printf("VL_SetVGAPlaneMode - screenOffsetY: %d\n", scalingOffsetY);
#endif
    ylookup = (uint32_t *) SafeMalloc(screenHeight * sizeof(*ylookup));
    pixelangle = (short *) SafeMalloc(screenWidth * sizeof(*pixelangle));
    wallheight = (int16_t *) SafeMalloc(screenWidth * sizeof(*wallheight));
#if defined(USE_FLOORCEILINGTEX) || defined(USE_CLOUDSKY)
    spanstart = (int16_t *) SafeMalloc((screenHeight / 2) * sizeof(*spanstart));
#endif

    for (i = 0; i < screenHeight; i++)
        ylookup[i] = i * bufferPitch;
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

void VL_ConvertPalette(byte *srcpal, SDL_Color *destpal, int numColors) {
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

void VL_SetColor(int color, int red, int green, int blue) {
    SDL_Color col = {(Uint8) red, (Uint8) green, (Uint8) blue};
    curpal[color] = col;

    if (screenBits == 8)
#if SDL_MAJOR_VERSION == 1
        SDL_SetPalette(screen, SDL_PHYSPAL, &col, color, 1);
    else
    {
        SDL_SetPalette(screenBuffer, SDL_LOGPAL, &col, color, 1);
        SDL_BlitSurface(screenBuffer, NULL, screen, NULL);

        SDL_Flip(screen);
#else
        SDL_SetPaletteColors(screen->format->palette, &col, color, 1);
    else {
        SDL_SetPaletteColors(screenBuffer->format->palette, &col, color, 1);
        SDL_BlitSurface(screenBuffer, NULL, screen, NULL);

        SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer, screenBuffer);
        SDL_RenderCopy(renderer, texture, NULL, NULL);
        SDL_RenderPresent(renderer);
        SDL_DestroyTexture(texture);
#endif
    }
}

//===========================================================================

/*
=================
=
= VL_GetColor
=
=================
*/

void VL_GetColor(int color, int *red, int *green, int *blue) {
    SDL_Color *col = &curpal[color];
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

void VL_SetPalette(SDL_Color *palette, bool forceupdate) {
    memcpy(curpal, palette, sizeof(SDL_Color) * 256);

    if (screenBits == 8)
#if SDL_MAJOR_VERSION == 1
        SDL_SetPalette(screen, SDL_PHYSPAL, palette, 0, 256);
    else
    {
        SDL_SetPalette(screenBuffer, SDL_LOGPAL, palette, 0, 256);
        if (forceupdate)
        {
            SDL_BlitSurface(screenBuffer, NULL, screen, NULL);

            SDL_Flip(screen);
#else
        SDL_SetPaletteColors(screen->format->palette, palette, 0, 256);
    else {
        SDL_SetPaletteColors(screenBuffer->format->palette, palette, 0, 256);
        if (forceupdate) {
            SDL_BlitSurface(screenBuffer, NULL, screen, NULL);

            Present(screen);
#endif
        }
    }
}

//===========================================================================

/*
=================
=
= VL_GetPalette
=
=================
*/

void VL_GetPalette(SDL_Color *palette) {
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
    SDL_Color *origptr, *newptr;

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

        if (!usedoublebuffering || screenBits == 8)
            VL_WaitVBL(1);
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

void VL_FadeIn(int start, int end, SDL_Color *palette, int steps) {
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

        if (!usedoublebuffering || screenBits == 8)
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

byte *VL_LockSurface(SDL_Surface *surface) {
    if (SDL_MUSTLOCK(surface)) {
        if (SDL_LockSurface(surface) < 0)
            return NULL;
    }
    return (byte *) surface->pixels;
}

void VL_UnlockSurface(SDL_Surface *surface) {
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
    byte *dest;

    assert(x >= 0 && (unsigned) x < screenWidth && y >= 0 && (unsigned) y < screenHeight &&
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
void VL_DrawPixel(SDL_Surface *surface, int x, int y, Uint32 pixel) {
    int bpp = surface->format->BytesPerPixel;
    /* Here p is the address to the pixel we want to set */
    Uint8 *p = (Uint8 *) surface->pixels + y * surface->pitch + x * bpp;

    switch (bpp) {
        case 1:
            *p = pixel;
            break;

        case 2:
            *(Uint16 *) p = pixel;
            break;

        case 3:
            if (SDL_BYTEORDER == SDL_BIG_ENDIAN) {
                p[0] = (pixel >> 16) & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = pixel & 0xff;
            } else {
                p[0] = pixel & 0xff;
                p[1] = (pixel >> 8) & 0xff;
                p[2] = (pixel >> 16) & 0xff;
            }
            break;

        case 4:
            *(Uint32 *) p = pixel;
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
SDL_Surface *VL_ScaleSurface(SDL_Surface *surface, Uint16 width, Uint16 height) {
    if (!surface || !width || !height)
        return nullptr;

    SDL_Surface *scaledSurface = SDL_CreateRGBSurface(surface->flags, width, height, 8, 0, 0, 0, 0);
    SDL_SetPaletteColors(scaledSurface->format->palette, gamepal, 0, 256);

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
    byte *dest;

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
    byte *dest;

    assert(x >= 0 && (unsigned) x < screenWidth && y >= 0 && (unsigned) y + height <= screenHeight &&
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
    byte *dest;

    assert(scx >= 0 && (unsigned) scx + scwidth <= screenWidth && scy >= 0 &&
           (unsigned) scy + scheight <= screenHeight && "VL_BarScaledCoord: Destination rectangle out of bounds!");

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

void VL_SurfaceToByteArray(SDL_Surface *surface, byte *byteArray) {
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

void VL_DePlaneVGA(byte *source, int width, int height) {
    int x, y, plane;
    word size, pwidth;
    byte *temp, *dest, *srcline;

    size = width * height;

    if (width & 3)
        Quit("DePlaneVGA: width not divisible by 4!");

    temp = (byte *) SafeMalloc(size);

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

void VL_MemToScreen(byte *source, int width, int height, int x, int y) {
    VL_MemToScreenScaledCoord(source, width, height, scaleFactor * x, scaleFactor * y);
}

void VL_MemToScreenScaledCoord(byte *source, int width, int height, int destx, int desty) {
    byte *dest;
    int i, j, sci, scj;
    unsigned m, n;

    assert(destx >= 0 && destx + width * scaleFactor <= screenWidth && desty >= 0 &&
           desty + height * scaleFactor <= screenHeight &&
           "VL_MemToScreenScaledCoord: Destination rectangle out of bounds!");

    dest = VL_LockSurface(screenBuffer);
    if (dest == NULL)
        return;

    for (j = 0, scj = 0; j < height; j++, scj += scaleFactor) {
        for (i = 0, sci = 0; i < width; i++, sci += scaleFactor) {
            byte col = source[(j * width) + i];
            for (m = 0; m < scaleFactor; m++) {
                for (n = 0; n < scaleFactor; n++) {
                    dest[ylookup[scj + m + desty] + sci + n + destx] = col;
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

void VL_MemToScreenScaledCoord2(byte *source, int origwidth, int origheight, int srcx, int srcy,
                                int destx, int desty, int width, int height) {
    byte *dest;
    int i, j, sci, scj;
    unsigned m, n;

    assert(destx >= 0 && destx + width * scaleFactor <= screenWidth && desty >= 0 &&
           desty + height * scaleFactor <= screenHeight &&
           "VL_MemToScreenScaledCoord: Destination rectangle out of bounds!");

    dest = VL_LockSurface(screenBuffer);
    if (dest == NULL)
        return;

    for (j = 0, scj = 0; j < height; j++, scj += scaleFactor) {
        for (i = 0, sci = 0; i < width; i++, sci += scaleFactor) {
            byte col = source[((j + srcy) * origwidth) + (i + srcx)];

            for (m = 0; m < scaleFactor; m++) {
                for (n = 0; n < scaleFactor; n++) {
                    dest[ylookup[scj + m + desty] + sci + n + destx] = col;
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

void VL_ScreenToScreen(SDL_Surface *source, SDL_Surface *dest) {
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
