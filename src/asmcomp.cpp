/*
VODKA-INDUCED ENTERTAINMENT ADVANCED SOUND MANAGER v0.9.1
COMPATIBILITY LAYER FOR WOLF4SDL v1.6
BY GERARD 'ALUMIUN' WATSON

This file replaces all external ID_SD functions and audio-based ID_CA
functions in Wolf4SDL v1.6 for use with the Advanced Sound Manager.

Please read vieasm.txt before using.

PLEASE ALSO NOTE THAT THIS IS STILL A BETA. IT IS LIKELY TO HAVE LOTS OF BUGS.
IF YOU FIND ANY, PLEASE TELL ME SO I CAN FIX THEM!
*/

#include "wl_def.h"
#include "asmcref.h"

#ifdef USE_VIEASM
SDMode          SoundMode;
SMMode          MusicMode;

static  int     LeftPosition;
static  int     RightPosition;
static  bool    ambience;

sample ASM_Audiosegs[NUMSOUNDS];
int ambientsnds[NUMAMBIENTS];

globalsoundpos channelSoundPos[ASM_ABSMAXCHANNELS];

// SD_MusIsOn
// Returns true if music is on, false otherwise

bool SD_MusIsOn(void)
{
    return (MusicMode == smm_AdLib) ? true : false;
}

bool SD_SndIsOn(void)
{
    return (SoundMode == sdm_AdLib) ? true : false;
}

void SD_ChannelDone(int channel)
{
    ASM_ChannelDone(channel);
    channelSoundPos[channel].valid = 0;
}

void SD_Startup(void)
{
    if (noSound)
        return;

    if (!ASM_Open(param_samplerate, param_8bitsound, ASM_ABSMAXCHANNELS/2, param_audiobuffer, 128, 128, false))
    {        
        printf ("SD_Startup: Unable to open audio device. Trying defaults.\n"); 
        if (!ASM_Open(22050, true, 128, 2048, 128, 128, false))
        {    
            printf ("SD_Startup: Unable to open audio device with defaults.\n"
                    "            Try closing any other applications that might\n"
                    "            be using the audio driver and try again, or\n"
                    "            start the program with the '--nosound' switch.\n");
            exit(1);
        }
    }
    
    Mix_ChannelFinished(SD_ChannelDone);
    memset (ASM_Audiosegs, 0, sizeof(ASM_Audiosegs));
}

void SD_Shutdown(void)
{
    for (int i = 0; i < NUMSOUNDS; i++)
        if (ASM_Audiosegs[i].chunk != NULL)
            UNCACHEAUDIOCHUNK(i);

    ASM_Close();
}

bool SD_MusicPlaying(void)
{
    ASM_AbortIfClosed false;
    SD_AbortIfMusOff false;

    return Mix_PlayingMusic();
}

// This has changed slightly - it returns 0 on an error, 1 on a successful load

int32_t CA_CacheAudioChunk(int chunk)
{
    if (chunk >= NUMSOUNDS)
    {
#ifdef VERBOSE
        printf("CA_CacheAudioChunk: Invalid sample number %d!", chunk);
#endif
        return 0;
    }

    char *file = (char *) malloc((size_t)(strlen(sounddir)+strlen(ASM_Soundnames[chunk]))+1);
    strcpy (file, sounddir);
    strcat (file, ASM_Soundnames[chunk]);

    ASM_Audiosegs[chunk] = ASM_Cache(file, ASM_Soundnames[chunk]);

    free (file);
    return 1;
}

void CA_LoadAllSounds(void)
{
    for (int i = 0; i < NUMSOUNDS; i++)
    {
        if (ASM_Audiosegs[i].chunk != NULL)
            continue;
        CA_CacheAudioChunk(i);
    }
}

void SD_MusicOn(void)
{
    ASM_AbortIfClosed;

    SD_AbortIfMusOff;

    ASM_ResumeMusic();
}

int SD_MusicOff(void)
{
    ASM_AbortIfClosed 0;
    
    ASM_PauseMusic();
    
    return 0;
}

void SD_StartMusic (int chunk)
{
    ASM_AbortIfClosed;

    SD_AbortIfMusOff;

    if (chunk >= NUMMUSICS)
    {
#ifdef VERBOSE
        printf("SD_StartMusic: Invalid chunk number %d!", chunk);
#endif
        return;
    }

    char *file = (char *) malloc((size_t)(strlen(musicdir)+strlen(ASM_Musicnames[chunk]))+1);
    strcpy (file, musicdir);
    strcat (file, ASM_Musicnames[chunk]);

    ASM_PlayMusic(file);
}

bool SD_SetMusicMode(SMMode mode)
{
    if (mode == MusicMode)
        return true;

    if (mode == smm_Off)
    {
        MusicMode = smm_Off;
        SD_MusicOff();
    }
    else
    {
        MusicMode = smm_AdLib;
        SD_MusicOn();
    }

    return true;
}

bool SD_SetSoundMode(SDMode mode)
{
    if (mode == SoundMode)
        return true;

    ASM_HaltSound();

    if (mode == sdm_Off)
        SoundMode = sdm_Off;
    else
        SoundMode = sdm_AdLib;
    return true;
}

void SD_StopSound(void)
{
    ASM_AbortIfClosed;

    ASM_HaltSound();
}

void SD_PositionSound(int leftvol, int rightvol)
{
    LeftPosition = leftvol;
    RightPosition = rightvol;
}

void SD_SetPosition(int channel, int leftpos, int rightpos)
{
    if((leftpos < 0) || (leftpos > 15) || (rightpos < 0) || (rightpos > 15) || ((leftpos == 15) && (rightpos == 15)))
    {
#ifdef VERBOSE
        printf("SD_SetPosition: Illegal position: leftpos - %d, rightpos - %d", leftpos, rightpos);
#endif
        return;
    }
    Mix_SetPanning(channel, ((15 - leftpos) << 4) + 15, ((15 - rightpos) << 4) + 15);
}

int SD_GetAmbIndex(void)
{
    for (int i = 0; i < NUMAMBIENTS; i++)
        if (ambientsnds[i] != 0)
            return i;
    return -1;
}

byte SD_PlaySound(soundnames sound)
{
    SD_AbortIfSndOff 0;

    int lp = LeftPosition;
    int rp = RightPosition;
    bool amb = ambience;

    LeftPosition = RightPosition = ambience = 0;

    if (sound >= NUMSOUNDS)
    {
#ifdef VERBOSE
        printf ("SD_PlaySound: Invalid sound number %d!\n", sound);
#endif
        return 0;
    }

    if (ASM_Audiosegs[sound].chunk == NULL)
    {
#ifdef VERBOSE
        printf ("SD_PlaySound: Sound %d not cached!\n", sound);
#endif
        return 0;
    }


    int channel = SD_PlayDigitized(sound, lp, rp, amb);
    return channel;
}

int SD_PlayDigitized(word which, int leftpos, int rightpos, bool ambient)
{
    SD_AbortIfSndOff 0;

    int channel = ASM_PlayDirect(ASM_Audiosegs[which], ambient);
    if (ambient)
        ambientsnds[SD_GetAmbIndex()] = channel;
    ambient = false;
    SD_SetPosition(channel, leftpos, rightpos);
    return channel;
}

int SD_AmbientSound(void)
{
    int ambindex = SD_GetAmbIndex();
    if (ambindex > -1)
        ambience = true;
    return ambindex;
}

void SD_StopAmbient(int ambindex)
{
    if (ambindex >= NUMAMBIENTS)
        return;

    ASM_StopChannel(ambientsnds[ambindex]);
}
#endif