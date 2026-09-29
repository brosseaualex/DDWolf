//
//	ID_DD.h - DDWolf Build Configuration
//	September 2026
//	By Alexandre Brosseau (DemolitionDerby)
// 

/* ==============================================================================
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  WARNING  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   !!!!!!!!  DO NOT MODIFY THIS FILE UNLESS YOU KNOW WHAT YOU ARE DOING  !!!!!!!!
   !!!!!!!!!!!!!!!!!!!!!!!!!  YOU WILL BREAK THE PROJECT  !!!!!!!!!!!!!!!!!!!!!!!
   !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!  USE VERSION.H  !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
   ============================================================================== */

#ifndef __ID_DD_H_
#define __ID_DD_H_

#include <SDL.h>

// ----------------------------------------

#if SDL_MAJOR_VERSION == 2
#ifndef DDWOLF
#define DDWOLF 1
#endif
#elif SDL_MAJOR_VERSION == 1
#ifndef DDWOLF_LEGACY
#define DDWOLF_LEGACY 1
#endif
#endif

// ----------------------------------------

#if defined(MODERN) && defined(VANILLA)
#define MODERN
#endif

#if !defined(MODERN) && !defined(VANILLA)
#define MODERN
#endif

// ----------------------------------------

#if defined(DDWOLF) && defined(MODERN)
#define ENABLE_GAME_CONTROLLER  1
#else
#define ENABLE_GAME_CONTROLLER  0
#endif

#if defined(MODERN)
#define USE_MODERN_CONTROLS     1
//#define USE_STICK_LOOKING       1
#else
#define USE_MODERN_CONTROLS     0
//#define USE_STICK_LOOKING       0
#endif
#endif