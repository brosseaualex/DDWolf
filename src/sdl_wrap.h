#ifndef __SDL_WRAP
#define __SDL_WRAP

#ifdef __linux__
#include<SDL2/SDL.h>
#else
#include <SDL.h>
#endif
#if SDL_MAJOR_VERSION == 2
void Present(SDL_Surface *screen);
#endif
#endif
