/*
VODKA-INDUCED ENTERTAINMENT ADVANCED SOUND MANAGER v0.9
COMPATIBILITY LAYER FOR WOLF4SDL v1.6 - SOUND NAMES REFERENCE FILE
BY GERARD 'ALUMIUN' WATSON
*/

#ifndef _ASMCREF_H_
#define _ASMCREF_H_

/*
wolfinst.txt

Now that this is set up, unless you are using the standard AUDIOWL6.H, you will need to change the tables in ASMCREF.H. These are the external file names for the sounds. The sound's position in the table reflects its position in the soundnames table in your AUDIOxxx.H file. In addition to this, there is a variable for the directory that the sounds and music are stored in. Please note that when using a backslash in either a sound filename or the directory name, you must use '\\' instead of a simple backslash.

Unfortunately, due to the inability of SDL_mixer to do some things, and due to the inability of my engine to tell whether a particular sound is playing (maybe next version?), some of the SD_ functions will no longer work as they did before.

SD_ContinueMusic - will now just start it again. The offs parameter does nothing.
SD_WaitSoundDone - does nothing at all.
SD_SoundPlaying - always returns zero.
SD_PrepareSound - does nothing at all.

I have also added two new functions for adding looping sounds -

int SD_AmbientSound(void) - signals that the next sound played will be ambient. Returns an index number that needs to be used to stop the ambient sound. The index number could be stored in the objtype structure for easy access.

void SD_StopAmbient(int ambindex) - stops an ambient sound with index number ambindex.

Implementing these is up to you, but I wouldn't imagine it would be too hard.
*/

// This table holds the file names for the sounds defined in AUDIOWL6.H.
// Change these to suit your sounds and your AUDIOHED.WL6 or SOD file.
#ifdef USE_VIEASM
const char* sounddir = "sounds\\";
const char* musicdir = "music\\";

#ifndef SPEAR
/////////////////////////////////////////////////
//
// Wolfenstein 3D
//
/////////////////////////////////////////////////
const char* ASM_Soundnames[NUMSOUNDS] = {
	"HITWALLSND.wav",              // 0
    "SELECTWPNSND.wav",            // 1
    "SELECTITEMSND.wav",           // 2
    "HEARTBEATSND.wav",            // 3
    "MOVEGUN2SND.wav",             // 4
    "MOVEGUN1SND.wav",             // 5
    "NOWAYSND.wav",                // 6
    "NAZIHITPLAYERSND.wav",        // 7
    "SCHABBSTHROWSND.wav",         // 8
    "PLAYERDEATHSND.wav",          // 9
    "DOGDEATHSND.wav",             // 10
    "ATKGATLINGSND.wav",           // 11
    "GETKEYSND.wav",               // 12
    "NOITEMSND.wav",               // 13
    "WALK1SND.wav",                // 14
    "WALK2SND.wav",                // 15
    "TAKEDAMAGESND.wav",           // 16
    "GAMEOVERSND.wav",             // 17
    "OPENDOORSND.wav",             // 18
    "CLOSEDOORSND.wav",            // 19
    "DONOTHINGSND.wav",            // 20
    "HALTSND.wav",                 // 21
    "DEATHSCREAM2SND.wav",         // 22
    "ATKKNIFESND.wav",             // 23
    "ATKPISTOLSND.wav",            // 24
    "DEATHSCREAM3SND.wav",         // 25
    "ATKMACHINEGUNSND.wav",        // 26
    "HITENEMYSND.wav",             // 27
    "SHOOTDOORSND.wav",            // 28
    "DEATHSCREAM1SND.wav",         // 29
    "GETMACHINESND.wav",           // 30
    "GETAMMOSND.wav",              // 31
    "SHOOTSND.wav",                // 32
    "HEALTH1SND.wav",              // 33
    "HEALTH2SND.wav",              // 34
    "BONUS1SND.wav",               // 35
    "BONUS2SND.wav",               // 36
    "BONUS3SND.wav",               // 37
    "GETGATLINGSND.wav",           // 38
    "ESCPRESSEDSND.wav",           // 39
    "LEVELDONESND.wav",            // 40
    "DOGBARKSND.wav",              // 41
    "ENDBONUS1SND.wav",            // 42
    "ENDBONUS2SND.wav",            // 43
    "BONUS1UPSND.wav",             // 44
    "BONUS4SND.wav",               // 45
    "PUSHWALLSND.wav",             // 46
    "NOBONUSSND.wav",              // 47
    "PERCENT100SND.wav",           // 48
    "BOSSACTIVESND.wav",           // 49
    "MUTTISND.wav",                // 50
    "SCHUTZADSND.wav",             // 51
    "AHHHGSND.wav",                // 52
    "DIESND.wav",                  // 53
    "EVASND.wav",                  // 54
    "GUTENTAGSND.wav",             // 55
    "LEBENSND.wav",                // 56
    "SCHEISTSND.wav",              // 57
    "NAZIFIRESND.wav",             // 58
    "BOSSFIRESND.wav",             // 59
    "SSFIRESND.wav",               // 60
    "SLURPIESND.wav",              // 61
    "TOT_HUNDSND.wav",             // 62
    "MEINGOTTSND.wav",             // 63
    "SCHABBSHASND.wav",            // 64
    "HITLERHASND.wav",             // 65
    "SPIONSND.wav",                // 66
    "NEINSOVASSND.wav",            // 67
    "DOGATTACKSND.wav",            // 68
    "FLAMETHROWERSND.wav",         // 69
    "MECHSTEPSND.wav",             // 70
    "GOOBSSND.wav",                // 71
    "YEAHSND.wav",                 // 72
#ifndef APOGEE_1_0
    "DEATHSCREAM4SND.wav",         // 73
    "DEATHSCREAM5SND.wav",         // 74
    "DEATHSCREAM6SND.wav",         // 75
    "DEATHSCREAM7SND.wav",         // 76
    "DEATHSCREAM8SND.wav",         // 77
    "DEATHSCREAM9SND.wav",         // 78
    "DONNERSND.wav",               // 79
    "EINESND.wav",                 // 80
    "ERLAUBENSND.wav",             // 81
    "KEINSND.wav",                 // 82
    "MEINSND.wav",                 // 83
    "ROSESND.wav",                 // 84
    "MISSILEFIRESND.wav",          // 85
    "MISSILEHITSND.wav"            // 86
#endif
};

// This table holds the file names for the music defined in AUDIOWL6.H.
// Change these to suit your music and your AUDIOHED.WL6 or SOD file.
// Make sure you put the right format on the end here
// Compatible extentions:
//  -- WAV (If you're using WAVs for music, you're mad.)
//  -- OGG
//  -- MP3
//  -- MID
//  -- MOD, XM, IT, 669, S3M, MED

const char *ASM_Musicnames[NUMMUSICS] = {
	"CORNER.ogg",              // 0
	"DUNGEON.ogg",             // 1
	"WARMARCH.ogg",            // 2
	"GETTHEM.ogg",             // 3
	"HEADACHE.ogg",            // 4
	"HITLWLTZ.ogg",            // 5
	"INTROCW3.ogg",            // 6
	"NAZI_NOR.ogg",            // 7
	"NAZI_OMI.ogg",            // 8
	"POW.ogg",                 // 9
	"SALUTE.ogg",              // 10
	"SEARCHN.ogg",             // 11
	"SUSPENSE.ogg",            // 12
	"VICTORS.ogg",             // 13
	"WONDERIN.ogg",            // 14
	"FUNKYOU.ogg",             // 15
	"ENDLEVEL.ogg",            // 16
	"GOINGAFT.ogg",            // 17
	"PREGNANT.ogg",            // 18
	"ULTIMATE.ogg",            // 19
	"NAZI_RAP.ogg",            // 20
	"ZEROHOUR.ogg",            // 21
	"TWELFTH.ogg",             // 22
	"ROSTER.ogg",              // 23
	"URAHERO.ogg",             // 24
	"VICMARCH.ogg",            // 25
	"PACMAN.ogg"               // 26
};
#else
/////////////////////////////////////////////////
//
// Spear of Destiny
//
/////////////////////////////////////////////////
const char* ASM_Soundnames[NUMSOUNDS] = {
    "HITWALLSND.wav",               // 0
    "MISSILEHITSND.wav",            // 1
    "SELECTITEMSND.wav",            // 2
    "GHOSTSIGHTSND.wav",            // 3
    "MOVEGUN2SND.wav",              // 4
    "MOVEGUN1SND.wav",              // 5
    "NOWAYSND.wav",                 // 6
    "NAZIHITPLAYERSND.wav",         // 7
    "MISSILEFIRESND.wav",           // 8
    "PLAYERDEATHSND.wav",           // 9
    "DOGDEATHSND.wav",              // 10
    "ATKGATLINGSND.wav",            // 11
    "GETKEYSND.wav",                // 12
    "NOITEMSND.wav",                // 13
    "WALK1SND.wav",                 // 14
    "WALK2SND.wav",                 // 15
    "TAKEDAMAGESND.wav",            // 16
    "GAMEOVERSND.wav",              // 17
    "OPENDOORSND.wav",              // 18
    "CLOSEDOORSND.wav",             // 19
    "DONOTHINGSND.wav",             // 20
    "HALTSND.wav",                  // 21
    "DEATHSCREAM2SND.wav",          // 22
    "ATKKNIFESND.wav",              // 23
    "ATKPISTOLSND.wav",             // 24
    "DEATHSCREAM3SND.wav",          // 25
    "ATKMACHINEGUNSND.wav",         // 26
    "HITENEMYSND.wav",              // 27
    "SHOOTDOORSND.wav",             // 28
    "DEATHSCREAM1SND.wav",          // 29
    "GETMACHINESND.wav",            // 30
    "GETAMMOSND.wav",               // 31
    "SHOOTSND.wav",                 // 32
    "HEALTH1SND.wav",               // 33
    "HEALTH2SND.wav",               // 34
    "BONUS1SND.wav",                // 35
    "BONUS2SND.wav",                // 36
    "BONUS3SND.wav",                // 37
    "GETGATLINGSND.wav",            // 38
    "ESCPRESSEDSND.wav",            // 39
    "LEVELDONESND.wav",             // 40
    "DOGBARKSND.wav",               // 41
    "ENDBONUS1SND.wav",             // 42
    "ENDBONUS2SND.wav",             // 43
    "BONUS1UPSND.wav",              // 44
    "BONUS4SND.wav",                // 45
    "PUSHWALLSND.wav",              // 46
    "NOBONUSSND.wav",               // 47
    "PERCENT100SND.wav",            // 48
    "BOSSACTIVESND.wav",            // 49
    "DEATHSCREAM4SND.wav",          // 50
    "SCHUTZADSND.wav",              // 51
    "AHHHGSND.wav",                 // 52
    "DEATHSCREAM5SND.wav",          // 53
    "DEATHSCREAM7SND.wav",          // 54
    "DEATHSCREAM8SND.wav",          // 55
    "LEBENSND.wav",                 // 56
    "DEATHSCREAM6SND.wav",          // 57
    "NAZIFIRESND.wav",              // 58
    "BOSSFIRESND.wav",              // 59
    "SSFIRESND.wav",                // 60
    "SLURPIESND.wav",               // 61
    "GHOSTFADESND.wav",             // 62
    "DEATHSCREAM9SND.wav",          // 63
    "GETAMMOBOXSND.wav",            // 64
    "ANGELSIGHTSND.wav",            // 65
    "SPIONSND.wav",                 // 66
    "NEINSOVASSND.wav",             // 67
    "DOGATTACKSND.wav",             // 68
    "ANGELFIRESND.wav",             // 69
    "TRANSSIGHTSND.wav",            // 70
    "TRANSDEATHSND.wav",            // 71
    "WILHELMSIGHTSND.wav",          // 72
    "WILHELMDEATHSND.wav",          // 73
    "UBERDEATHSND.wav",             // 74
    "KNIGHTSIGHTSND.wav",           // 75
    "KNIGHTDEATHSND.wav",           // 76
    "ANGELDEATHSND.wav",            // 77
    "KNIGHTMISSILESND.wav",         // 78
    "GETSPEARSND.wav",              // 79
    "ANGELTIREDSND.wav",            // 80
};

const char* ASM_Musicnames[NUMMUSICS] = {
    "XFUNKIE_MUS.ogg",              // 0
    "DUNGEON_MUS.ogg",              // 1
    "XDEATH_MUS.ogg",               // 2
    "GETTHEM_MUS.ogg",              // 3
    "XTIPTOE_MUS.ogg",              // 4
    "GOINGAFT_MUS.ogg",             // 5
    "URAHERO_MUS.ogg",              // 6
    "XTHEEND_MUS.ogg",              // 7
    "NAZI_OMI_MUS.ogg",             // 8
    "POW_MUS.ogg",                  // 9
    "TWELFTH_MUS.ogg",              // 10
    "SEARCHN_MUS.ogg",              // 11
    "SUSPENSE_MUS.ogg",             // 12
    "ZEROHOUR_MUS.ogg",             // 13
    "WONDERIN_MUS.ogg",             // 14
    "ULTIMATE_MUS.ogg",             // 15
    "ENDLEVEL_MUS.ogg",             // 16
    "XEVIL_MUS.ogg",                // 17
    "XJAZNAZI_MUS.ogg",             // 18
    "COPYPRO_MUS.ogg",              // 19
    "XAWARD_MUS.ogg",               // 20
    "XPUTIT_MUS.ogg",               // 21
    "XGETYOU_MUS.ogg",              // 22
    "XTOWER2_MUS.ogg",              // 23
};
#endif
#endif  // _ASMCREF_H_
#endif