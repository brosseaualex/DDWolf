/*
VODKA-INDUCED ENTERTAINMENT ADVANCED SOUND MANAGER v0.9.1 - HEADER FILE
BY GERARD 'ALUMIUN' WATSON
*/

#ifndef _VIEASM_H_
#define _VIEASM_H_

#ifdef __linux__
#include <SDL2/SDL_mixer.h>
#else
#include <SDL_mixer.h> // Duh.
#endif

#include <string.h>    // For memset
#include <stdio.h>     // For printf

#define VERBOSE        // Verbose mode?

typedef struct
{
    Mix_Chunk *chunk;
    char *name;
} sample;

extern bool switching;

/* defines for management */

#define ASM_ABSMAXCHANNELS 256              // Absolute maximum amount for maxchan in ASM_Open

/* #defined functions */

// ASM_Start
// Calls ASM_Open with parameters and good defaults
// Has a max channel number of half of ASM_ABSMAXCHANNELS (128 by default)

#define ASM_Start(vol_sound, vol_music) ASM_Open(22050, 16, ASM_ABSMAXCHANNELS/2, 2048, vol_sound, vol_music)

// ASM_HaltMusic
// Ends music and frees it

#define ASM_HaltMusic() Mix_HaltMusic()

// ASM_PauseSound
// Pauses all sound channels

#define ASM_PauseSound() Mix_Pause(-1)

// ASM_ResumeSound
// Resumes all sound channels

#define ASM_ResumeSound() Mix_Resume(-1)

// ASM_PauseMusic
// Pauses music

#define ASM_PauseMusic() Mix_PauseMusic()

// ASM_ResumeMusic
// Resumes music

#define ASM_ResumeMusic() Mix_ResumeMusic()

// ASM_PlayDirect
// Plays a sound directly at player location

#define ASM_PlayDirect(sample, ambient) ASM_PlaySound(sample, 0, 0, ambient)

// ASM_AbortIfClosed
// Checks if the device is open, returns if it isn't
// Cunning lack of semi-colon means return values can be used appropriately :)

#ifdef VERBOSE
#define ASM_AbortIfClosed if(!ASM_IsOpen()) printf("Not open.\n"); if(!ASM_IsOpen()) return
#else
#define ASM_AbortIfClosed if(!ASM_IsOpen()) return
#endif

// ASM_Step
// Check if the music switching needs to be checked

#define ASM_Step() ASM_AbortIfClosed; if (switching) ASM_SwitchStep()

/* Other, 'real' functions */

extern bool ASM_Open(int frequency, bool use8Bit, int maxchan, int buffersize, Uint8 sndvolume, Uint8 musvolume, bool reverse);
extern bool ASM_IsOpen(void);
extern bool ASM_PlayMusic(char *musfile);
extern bool ASM_SwitchMus(char *loadmus, int fadems, bool fade);
extern bool ASM_FadeInMus(char *loadmus, int fadems);
extern sample ASM_Cache(char *sndfile, const char* name);
extern sample ASM_CacheFromMem(void *ptr, int size, const char *name);
extern int ASM_PlaySound(sample &sound, int angle, Uint8 distance, bool ambient);
extern void ASM_Uncache(sample &chunk);
extern void ASM_FadeOutMus(int fadems);
extern void ASM_HaltSound(void);
extern void ASM_Pause(void);
extern void ASM_Resume(void);
extern void ASM_Halt(void);
extern void ASM_ChangeVolume(Uint8 sndvolume, Uint8 musvolume);
extern void ASM_ReturnVolume(Uint8* retsnd, Uint8* retmus);
extern void ASM_Close(void);
extern void ASM_StopChannel(int channel);
extern void ASM_SwitchStep(void);
extern void ASM_ReverseStereo(bool reverse);
extern void ASM_ChannelDone(int channel);
extern int ASM_CurChannels(void);

#endif  // _VIEASM_H_
