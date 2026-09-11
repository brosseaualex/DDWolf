#ifndef __SDL_WRAP
#define __SDL_WRAP

#ifdef __linux__
#include<SDL2/SDL.h>
#else
#include <SDL.h>
#endif
void Present(SDL_Surface* screen);
#endif
