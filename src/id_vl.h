// ID_VL.H

#ifndef __ID_VL_H_
#define __ID_VL_H_

// wolf compatability

void Quit(const char *error, ...);

//===========================================================================

#define CHARWIDTH 2
#define TILEWIDTH 4

//===========================================================================

extern SDL_Surface* screen, * screenBuffer;
#ifdef SAVE_GAME_SCREENSHOT
extern SDL_Surface *lastGameSurface;
#endif

#if SDL_MAJOR_VERSION == 2
extern SDL_Window *window;
extern SDL_Renderer *renderer;
extern SDL_Texture *texture;
extern SDL_Rect *displayBounds;
#endif

extern boolean fullscreen, usedoublebuffering, disableresscaling, stretchtoscreen;
extern unsigned screenWidth, screenHeight, screenPitch, bufferPitch;
extern unsigned originalScreenWidth, originalScreenHeight, defaultScreenWidth, defaultScreenHeight;
extern int screenBits;
extern int scaleFactor;

extern boolean screenfaded;
extern unsigned bordercolor;

extern uint32_t *ylookup;

extern SDL_Color gamepal[256];

//===========================================================================

//
// VGA hardware routines
//

#define VL_WaitVBL(a) SDL_Delay((a)*8)
#define VL_ClearScreen(c) SDL_FillRect(screenBuffer, NULL, (c))

void VL_DePlaneVGA(byte *source, int width, int height);
void VL_SetVGAPlaneMode(void);
void VL_SetTextMode(void);
void VL_Shutdown(void);

void VL_ConvertPalette(byte *srcpal, SDL_Color *destpal, int numColors);
void VL_FillPalette(int red, int green, int blue);
void VL_SetColor(int color, int red, int green, int blue);
void VL_GetColor(int color, int *red, int *green, int *blue);
void VL_SetPalette(SDL_Color *palette, bool forceupdate);
void VL_GetPalette(SDL_Color *palette);
void VL_FadeOut(int start, int end, int red, int green, int blue, int steps);
void VL_FadeIn(int start, int end, SDL_Color *palette, int steps);

byte *VL_LockSurface(SDL_Surface *surface);
void VL_UnlockSurface(SDL_Surface *surface);

#ifndef SAVE_GAME_SCREENSHOT
byte VL_GetPixel(int x, int y);
#else
byte VL_GetPixel(SDL_Surface *surface, int x, int y);
void VL_SetSaveGameSlot();
#endif

void VL_Plot(int x, int y, int color);
void VL_Hlin(unsigned x, unsigned y, unsigned width, int color);
void VL_Vlin(int x, int y, int height, int color);
void VL_BarScaledCoord(int scx, int scy, int scwidth, int scheight, int color);
void VL_Bar(int x, int y, int width, int height, int color);

void VL_DrawPicBare(int x, int y, byte *pic, int width, int height);
void VL_ScreenToScreen(SDL_Surface *source, SDL_Surface *dest);
void VL_MemToScreenScaledCoord(byte *source, int width, int height, int scx, int scy);
void VL_MemToScreenScaledCoord2(byte *source, int origwidth, int origheight, int srcx, int srcy,
                                int destx, int desty, int width, int height);

void VL_MemToScreen(byte *source, int width, int height, int x, int y);
void VL_SurfaceToByteArray(SDL_Surface* surface, byte* byteArray);

#endif

extern int picHorizAdjust;
extern int picVertAdjust;
extern int printHorizAdjust;
extern int printVertAdjust;
