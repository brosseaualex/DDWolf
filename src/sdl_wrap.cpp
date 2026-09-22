#ifdef __linux__
#include <SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#include "wl_def.h"
#include "id_vl.h"

void Present(SDL_Surface* surface)
{
#if SDL_MAJOR_VERSION == 2
	// This prevents the rendering loop to run when minimized
	// Resolves recurring issue with the window keeping focus when trying to alt-tab out
	Uint32 windowFlags = SDL_GetWindowFlags(window);
	if (windowFlags & SDL_WINDOW_MINIMIZED)
	{
		SDL_Delay(16);
		return;
	}

	if (!renderer || !texture || !surface || !surface->pixels)
		return;

	if (screenBuffer && surface != screenBuffer)
		surface = screenBuffer;

	if (!surface->format || !surface->format->palette || !surface->format->palette->colors || !ylookup)
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
		byte* srcPixels = (byte*)surface->pixels;
		Uint32* dst32 = (Uint32*)texPixels;
		SDL_Color* palette = surface->format->palette->colors;

		int dstPitchPixels = texPitch / sizeof(Uint32);
		int heightToDraw = ((unsigned int)surface->h < screenHeight) ? surface->h : screenHeight;
		int widthToDraw = ((unsigned int)surface->w < screenWidth) ? surface->w : screenWidth;

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
#elif SDL_MAJOR_VERSION == 1
	// This prevents the rendering loop to run when minimized
	// Resolves recurring issue with the window keeping focus when trying to alt-tab out
	if ((SDL_GetAppState() & SDL_APPACTIVE) == 0)
	{
		SDL_Delay(16);
		return;
	}

	if (screenBuffer)
		surface = screenBuffer;

	if (!surface || !surface->pixels || !screen || !screen->pixels)
		return;

	if (!surface->format || !surface->format->palette || !surface->format->palette->colors)
	{
		SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));
		SDL_UpdateRect(screen, 0, 0, 0, 0);
		return;
	}

	int winW = screen->w;
	int winH = screen->h;
	int srcW = surface->w;
	int srcH = surface->h;

	float targetAspect = (float)srcW / (float)srcH;

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

	SDL_FillRect(screen, NULL, SDL_MapRGB(screen->format, 0, 0, 0));

	int srcBpp = surface->format->BytesPerPixel;
	int dstBpp = screen->format->BytesPerPixel;

	if (srcBpp == 1 && dstBpp == 1)
	{
		SDL_SetColors(screen, surface->format->palette->colors, 0, 256);

		if (srcW == renderDest.w && srcH == renderDest.h)
			SDL_BlitSurface(surface, NULL, screen, &renderDest);
		else
			SDL_SoftStretch(surface, NULL, screen, &renderDest);
	}
	else if (srcBpp == 1 && dstBpp == 4)
	{
		const int SCALE_BITS = 16;
		int stepX = (srcW << SCALE_BITS) / destW;
		int stepY = (srcH << SCALE_BITS) / destH;

		SDL_Color* pal = surface->format->palette->colors;
		byte* dstBase = (byte*)screen->pixels + (renderDest.y * screen->pitch) + (renderDest.x * 4);
		int dstPitch = screen->pitch;

		int currY = 0;
		for (int y = 0; y < destH; ++y)
		{
			int srcY = currY >> SCALE_BITS;
			if (srcY >= srcH) srcY = srcH - 1;

			byte* srcRow = ylookup ? ((byte*)surface->pixels + ylookup[srcY])
				: ((byte*)surface->pixels + (srcY * surface->pitch));
			Uint32* dstRow = (Uint32*)(dstBase + (y * dstPitch));

			int currX = 0;
			for (int x = 0; x < destW; ++x)
			{
				int srcX = currX >> SCALE_BITS;
				if (srcX >= srcW) srcX = srcW - 1;

				SDL_Color c = pal[srcRow[srcX]];
				dstRow[x] = SDL_MapRGB(screen->format, c.r, c.g, c.b);

				currX += stepX;
			}
			currY += stepY;
		}
	}
	else
	{
		if (srcW == renderDest.w && srcH == renderDest.h)
			SDL_BlitSurface(surface, NULL, screen, &renderDest);
		else
			SDL_SoftStretch(surface, NULL, screen, &renderDest);
	}

	SDL_UpdateRect(screen, 0, 0, 0, 0);
#endif
}
