#include <SDL.h>
#include "wl_def.h"
#include "id_vl.h"

void Present(SDL_Surface* screen)
{
	// This prevents the rendering loop to run when minimized
	// Resolves recurring issue with the window keeping focus when trying to alt-tab out
	Uint32 windowFlags = SDL_GetWindowFlags(window);
	if (windowFlags & SDL_WINDOW_MINIMIZED)
	{
		SDL_Delay(16);
		return;
	}

	if (!renderer || !texture || !screen || !screen->pixels)
		return;

	if (screenBuffer && screen != screenBuffer)
		screen = screenBuffer;

	if (!screen->format || !screen->format->palette || !screen->format->palette->colors || !ylookup)
	{
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
		SDL_RenderClear(renderer);
		SDL_RenderPresent(renderer);
		return;
	}

	void* texPixels = NULL;
	int texPitch = 0;

	if (SDL_LockTexture(texture, NULL, &texPixels, &texPitch) == 0)
	{
		byte* srcPixels = (byte*)screen->pixels;
		Uint32* dst32 = (Uint32*)texPixels;
		SDL_Color* palette = screen->format->palette->colors;

		int dstPitchPixels = texPitch / sizeof(Uint32);
		int heightToDraw = (screen->h < screenHeight) ? screen->h : screenHeight;
		int widthToDraw = (screen->w < screenWidth) ? screen->w : screenWidth;

		for (int y = 0; y < heightToDraw; y++)
		{
			byte* srcRow = srcPixels + ylookup[y];
			Uint32* dstRow = dst32 + (y * dstPitchPixels);

			for (int x = 0; x < widthToDraw; x++)
			{
				SDL_Color col = palette[srcRow[x]];
				dstRow[x] = (0xFF000000) | ((Uint32)col.r << 16) | ((Uint32)col.g << 8) | (Uint32)col.b;
			}
		}

		SDL_UnlockTexture(texture);
	}

	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
	SDL_RenderClear(renderer);

	int winW = 0, winH = 0;
	SDL_GetRendererOutputSize(renderer, &winW, &winH);

	if (winW <= 0 || winH <= 0)
	{
		winW = screenWidth;
		winH = screenHeight;
	}

	float targetAspect = (float)screenWidth / (float)screenHeight;

	int destW = winW;
	int destH = (int)((winW / targetAspect) + 0.5f);

	if (destH > winH)
	{
		destH = winH;
		destW = (int)((winH * targetAspect) + 0.5f);
	}

	SDL_Rect renderDest;
	renderDest.x = (winW - destW) / 2;
	renderDest.y = (winH - destH) / 2;
	renderDest.w = destW;
	renderDest.h = destH;

	SDL_RenderCopy(renderer, texture, NULL, &renderDest);
	SDL_RenderPresent(renderer);
}
