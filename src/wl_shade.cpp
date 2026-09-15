#include "version.h"

#ifdef USE_SHADING
#include "wl_def.h"
#include "wl_shade.h"

typedef struct
{
	uint8_t destRed, destGreen, destBlue; // values between 0 and 255
	uint8_t fogStrength;
} shadedef_t;

shadedef_t shadeDefs[] = {
	{0, 0, 0, LSHADE_NOSHADING},
	{0, 0, 0, LSHADE_NORMAL},
	{0, 0, 0, LSHADE_FOG},
	{40, 40, 40, LSHADE_NORMAL},
	{60, 60, 60, LSHADE_FOG} };

uint8_t shadetable[SHADE_COUNT][256];
int LSHADE_flag;

#ifdef USE_FEATUREFLAGS

// The lower 8-bit of the upper left tile of every map determine
// the used shading definition of shadeDefs.
static inline int GetShadeDefID()
{
	int shadeID = ffDataTopLeft & 0x00ff;
	assert(shadeID >= 0 && shadeID < lengthof(shadeDefs));
	return shadeID;
}

#else

static int GetShadeDefID()
{
	int shadeID;
	switch (gamestate.episode * 10 + gamestate.mapon)
	{
	case 0:
		shadeID = 4;
		break;
	case 1:
	case 2:
	case 6:
		shadeID = 1;
		break;
	case 3:
		shadeID = 0;
		break;
	case 5:
		shadeID = 2;
		break;
	default:
		shadeID = 3;
		break;
	}
	assert(shadeID >= 0 && shadeID < lengthof(shadeDefs));
	return shadeID;
}

#endif

// Returns the palette index of the nearest matching color of the
// given RGB color in given palette
byte GetColor(byte red, byte green, byte blue, SDL_Color* palette)
{
	int col;
	byte mincol = 0;
	double mindist = 200000.0, curdist, DRed, DGreen, DBlue;

	SDL_Color* palPtr = palette;

	for (col = 0; col < 256; col++, palPtr++)
	{
		DRed = (double)((int)red - (int)palPtr->r);
		DGreen = (double)((int)green - (int)palPtr->g);
		DBlue = (double)((int)blue - (int)palPtr->b);

		curdist = (2.0 * DRed * DRed) + (4.0 * DGreen * DGreen) + (3.0 * DBlue * DBlue);

		if (curdist < mindist)
		{
			mindist = curdist;
			mincol = (byte)col;
			if (curdist == 0.0)
				break;
		}
	}
	return mincol;
}

// Fade all colors in 32 steps down to the destination-RGB
// (use gray for fogging, black for standard shading)
void GenerateShadeTable(byte destRed, byte destGreen, byte destBlue, SDL_Color* palette, int fog)
{
	int i, shade;
	double curRed, curGreen, curBlue, redStep, greenStep, blueStep;
	SDL_Color* palPtr = palette;

	// Set the fog-flag
	LSHADE_flag = fog;

	// Color loop
	for (i = 0; i < 256; i++, palPtr++)
	{
		// Get original palette color
		curRed = palPtr->r;
		curGreen = palPtr->g;
		curBlue = palPtr->b;

		// Calculate increment per step
		redStep = ((double)destRed - curRed) / (double)(SHADE_COUNT - 1);
		greenStep = ((double)destGreen - curGreen) / (double)(SHADE_COUNT - 1);
		blueStep = ((double)destBlue - curBlue) / (double)(SHADE_COUNT - 1);

		// Calc color for each shade of the current color
		for (shade = 0; shade < SHADE_COUNT; shade++)
		{
			byte r = (curRed < 0.0) ? 0 : ((curRed > 255.0) ? 255 : (byte)curRed);
			byte g = (curGreen < 0.0) ? 0 : ((curGreen > 255.0) ? 255 : (byte)curGreen);
			byte b = (curBlue < 0.0) ? 0 : ((curBlue > 255.0) ? 255 : (byte)curBlue);

			shadetable[shade][i] = GetColor(r, g, b, palette);

			// Inc to next shade
			curRed += redStep;
			curGreen += greenStep;
			curBlue += blueStep;
		}
	}
}

void NoShading()
{
	int i, shade;
	for (shade = 0; shade < SHADE_COUNT; shade++)
		for (i = 0; i < 256; i++)
			shadetable[shade][i] = i;
}

void InitLevelShadeTable()
{
	shadedef_t* shadeDef = &shadeDefs[GetShadeDefID()];
	if (shadeDef->fogStrength == LSHADE_NOSHADING)
		NoShading();
	else
		GenerateShadeTable(shadeDef->destRed, shadeDef->destGreen, shadeDef->destBlue, gamepal, shadeDef->fogStrength);
}

int GetShade(int scale)
{
	if (scale <= 0)
		return SHADE_COUNT - 1;

	int shadeFactor = (viewwidth * 8) / scale;

	int shade = (shadeFactor - 8) + LSHADE_flag;

	if (shade < 0)
		shade = 0;
	else if (shade >= SHADE_COUNT)
		shade = SHADE_COUNT - 1;

	return shade;
}

#endif
