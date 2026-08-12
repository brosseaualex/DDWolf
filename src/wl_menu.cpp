////////////////////////////////////////////////////////////////////
//
// WL_MENU.C
// by John Romero (C) 1992 Id Software, Inc.
//
////////////////////////////////////////////////////////////////////

#include <sys/stat.h>
#include <sys/types.h>

#ifdef _WIN32
#include <io.h>
#include <direct.h>
#else

#include <unistd.h>

#endif

#include "wl_def.h"

#pragma hdrstop

extern int lastgamemusicoffset;
int menuExit = 0;

boolean atmosTexturedEnabled = true;
boolean atmosShadingEnabled = true;
boolean atmosSkyboxEnabled = true;
boolean atmosPrecipitationEnabled = true;

//
// PRIVATE PROTOTYPES
//
int CP_ReadThis(int);

void SetTextColor(CP_itemtype* items, int hlight);

#ifdef USE_READTHIS
#define STARTITEM       readthis
#else
#define STARTITEM       newgame
#endif

// ENDSTRx constants are defined in foreign.h
char endStrings[9][80] = {
	ENDSTR1,
	ENDSTR2,
	ENDSTR3,
	ENDSTR4,
	ENDSTR5,
	ENDSTR6,
	ENDSTR7,
	ENDSTR8,
	ENDSTR9 };

CP_itemtype MainMenu[] = {
#ifdef JAPAN
	{1, "", CP_NewGame},
	{1, "", CP_Sound},
	{1, "", CP_Control},
	{1, "", CP_LoadGame},
	{0, "", CP_SaveGame},
	{1, "", CP_ChangeView},
	{2, "", CP_ReadThis},
	{1, "", CP_ViewScores},
	{1, "", 0},
	{1, "", 0}
#else
#if (defined(USE_MODERN_CONTROLS) && !defined(SHOW_ATMOS_OPTIONS)) || (defined(USE_MODERN_CONTROLS) && defined(SHOW_ATMOS_OPTIONS))
	{ 1, STR_NG, CP_NewGame },
	{1, STR_LG, CP_LoadGame},
	{0, STR_SG, CP_SaveGame},
	{1, STR_OP, CP_Options},
#elif !defined(USE_MODERN_CONTROLS) && defined(SHOW_ATMOS_OPTIONS)
	{1, STR_NG, CP_NewGame},
	{1, STR_SD, CP_Sound},
	{1, STR_CL, CP_Control},
	{1, STR_LG, CP_LoadGame},
	{0, STR_SG, CP_SaveGame},
	{1, STR_OP, CP_Options},
#else
	{1, STR_NG, CP_NewGame},
	{1, STR_SD, CP_Sound},
	{1, STR_CL, CP_Control},
	{1, STR_LG, CP_LoadGame},
	{0, STR_SG, CP_SaveGame},
	{1, STR_CV, CP_ChangeView},
#endif

#ifdef USE_READTHIS
#ifdef SPANISH
	{2, "Ve esto!", CP_ReadThis},
#else
	{2, "Read This!", CP_ReadThis},
#endif
#endif
	{1, STR_VS, CP_ViewScores},
	{1, STR_BD, 0},
	{1, STR_QT, 0}
#endif
};

CP_itemtype SndMenu[] = {
#ifdef JAPAN
	{1, "", 0},
	{1, "", 0},
	{1, "", 0},
	{0, "", 0},
	{0, "", 0},
	{1, "", 0},
	{1, "", 0},
	{1, "", 0},
	{0, "", 0},
	{0, "", 0},
	{1, "", 0},
	{1, "", 0},
#else
#ifndef VIEASM
	{1, STR_NONE, 0},
	{1, STR_PC, 0},
	{1, STR_ALSB, 0},
	{0, "", 0},
	{0, "", 0},
	{1, STR_NONE, 0},
	{0, STR_DISNEY, 0},
	{1, STR_SB, 0},
	{0, "", 0},
	{0, "", 0},
	{1, STR_NONE, 0},
	{1, STR_ALSB, 0}
#else
	{ 1, "Off", 0 },
	{1, "On", 0},
	{0, "", 0},
	{0, "", 0},
	{1, "Off", 0},
	{1, "On", 0},
	{0, "", 0},
	{0, "", 0},
	{1, "Adjust Volume", AdjustVolume},
	{1, "Reverse Stereo", 0}
#endif // !VIEASM
#endif
};

#ifdef JAPAN
enum
{
	CTL_MOUSEENABLE,
	CTL_JOYENABLE,
	CTL_JOY2BUTTONUNKNOWN,
	CTL_GAMEPADUNKONWN,
	CTL_MOUSESENS,
	CTL_CUSTOMIZE
};
#else
#ifdef USE_MODERN_CONTROLS
enum
{
	CTL_MOUSEENABLE,
	CTL_JOYENABLE,
	CTL_ALWAYSRUN,
	CTL_OPTIONS_SPACE,
	CTL_MOUSEOPTIONS,
	CTL_KEYBOARDOPTIONS,
	CTL_JOYSTICKOPTIONS
};
enum
{
	CTL_MOUSE_RUN,
	CTL_MOUSE_OPEN,
	CTL_MOUSE_FIRE,
	CTL_MOUSE_STRAFE,
	CTL_SPACE_MOUSE,
	CTL_MOUSEMOVEMENT,
	CTL_MOUSESENS
};
enum
{
	CTL_KB_MOVE_FWRD,
	CTL_KB_MOVE_BWRD,
	CTL_KB_MOVE_LEFT,
	CTL_KB_MOVE_RIGHT,
	CTL_KB_STRAFE_LEFT,
	CTL_KB_STRAFE_RIGHT,
	CTL_SPACE_KB_MOVE,
	CTL_ACTIONKEYS
};
enum
{
	CTL_KB_ACTION_RUN,
	CTL_KB_ACTION_OPEN,
	CTL_KB_ACTION_FIRE,
	CTL_KB_ACTION_STRAFE,
	CTL_SPACE_KB_ACTION,
	CTL_MOVEMENTKEYS
};
enum
{
	CTL_KB_MORE_ACTION_WEP1,
	CTL_KB_MORE_ACTION_WEP2,
	CTL_KB_MORE_ACTION_WEP3,
	CTL_KB_MORE_ACTION_WEP4,
	CTL_KB_MORE_ACTION_PREV_WEP,
	CTL_KB_MORE_ACTION_NEXT_WEP,
	//CTL_KB_MORE_ACTION_AUTOMAP,
	CTL_SPACE_KB_MORE_ACTION,
	CTL_BACK_ACTIONKEYS
};
enum
{
	CTL_JOYSTICK_RUN,
	CTL_JOYSTICK_OPEN,
	CTL_JOYSTICK_FIRE,
	CTL_JOYSTICK_STRAFE
};
#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)
enum
{
	CTL_ADV_1,
	CTL_ADV_2,
	CTL_ADV_3,
	CTL_ADV_4,
	CTL_ADV_5,
	CTL_ADV_6,
	CTL_ADV_7,
	CTL_ADV_8,
	CTL_ADV_9,
	CTL_ADV_10
};
#endif
#else
enum
{
	CTL_MOUSEENABLE,
	CTL_MOUSESENS,
	CTL_JOYENABLE,
	CTL_CUSTOMIZE
};
#endif

#endif

enum
{
	ATMOS_USE_TEXTURED,
	ATMOS_USE_SHADING,
	ATMOS_USE_SKYBOX,
	ATMOS_USE_PRECIPITATION
};

CP_itemtype CtlMenu[] = {
#ifdef JAPAN
	{0, "", 0},
	{0, "", 0},
	{0, "", 0},
	{0, "", 0},
	{0, "", MouseSensitivity},
	{1, "", CustomControls}
#else
	{ 0, STR_MOUSEEN, 0 },
#ifndef USE_MODERN_CONTROLS
	{0, STR_SENS, MouseSensitivity},
	{0, STR_JOYEN, 0},
	{1, STR_CUSTOM, CustomControls}
#else
	{ 0, STR_JOYEN, 0 },
	{1, STR_ALWAYS_RUN, 0},
	{0, "", 0},
	{1, STR_OP_MOUSE, CP_MouseCtl},
	{1, STR_OP_KEYBOARD, CP_KeyboardMoveCtl},
	{1, STR_OP_JOYSTICK, CP_JoystickCtl},
#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)
	{1, STR_CUS_CONTROLS, CP_CustomCtl}
#endif
#endif
#endif
};

#ifndef SPEAR
CP_itemtype NewEmenu[] = {
#ifdef JAPAN
#ifdef JAPDEMO
	{1, "", 0},
	{0, "", 0},
	{0, "", 0},
	{0, "", 0},
	{0, "", 0},
	{0, "", 0},
	{0, "", 0},
	{0, "", 0},
	{0, "", 0},
	{0, "", 0},
	{0, "", 0},
	{0, "", 0},
#else
	{1, "", 0},
	{0, "", 0},
	{1, "", 0},
	{0, "", 0},
	{1, "", 0},
	{0, "", 0},
	{1, "", 0},
	{0, "", 0},
	{1, "", 0},
	{0, "", 0},
	{1, "", 0},
	{0, "", 0}
#endif
#else
#ifdef SPANISH
	{
1, "Episodio 1\n"
   "Fuga desde Wolfenstein",
0
},
{0, "", 0},
{3, "Episodio 2\n"
	"Operacion Eisenfaust",
 0},
{0, "", 0},
{3, "Episodio 3\n"
	"Muere, Fuhrer, Muere!",
 0},
{0, "", 0},
{3, "Episodio 4\n"
	"Un Negro Secreto",
 0},
{0, "", 0},
{3, "Episodio 5\n"
	"Huellas del Loco",
 0},
{0, "", 0},
{3, "Episodio 6\n"
	"Confrontacion",
 0}
#else
	{
1, "Episode 1\n"
   "Escape from Wolfenstein",
0
},
{0, "", 0},
{3, "Episode 2\n"
	"Operation: Eisenfaust",
 0},
{0, "", 0},
{3, "Episode 3\n"
	"Die, Fuhrer, Die!",
 0},
{0, "", 0},
{3, "Episode 4\n"
	"A Dark Secret",
 0},
{0, "", 0},
{3, "Episode 5\n"
	"Trail of the Madman",
 0},
{0, "", 0},
{3, "Episode 6\n"
	"Confrontation",
 0}
#endif
#endif
};
#endif

CP_itemtype NewMenu[] = {
#ifdef JAPAN
	{1, "", 0},
	{1, "", 0},
	{1, "", 0},
	{1, "", 0}
#else
	{ 1, STR_DADDY, 0 },
	{1, STR_HURTME, 0},
	{1, STR_BRINGEM, 0},
	{1, STR_DEATH, 0}
#endif
};

CP_itemtype LSMenu[] = {
	{1, "", 0},
	{1, "", 0},
	{1, "", 0},
	{1, "", 0},
	{1, "", 0},
	{1, "", 0},
	{1, "", 0},
	{1, "", 0},
	{1, "", 0},
	{1, "", 0} };

CP_itemtype CusMenu[] = {
	{1, "", 0},
	{0, "", 0},
	{0, "", 0},
	{1, "", 0},
	{0, "", 0},
	{0, "", 0},
	{1, "", 0},
	{0, "", 0},
	{1, "", 0} };

#ifdef USE_MODERN_CONTROLS
CP_itemtype CtlMouseMenu[] = {
	{1, STR_CRUN, 0},
	{1, STR_COPEN, 0},
	{1, STR_CFIRE, 0},
	{1, STR_CSTRAFE, 0},
	{0, "", 0},
	{1, STR_MOUSEMOVEMENT, 0},
	{1, STR_SENS, MouseSensitivity}
};

CP_itemtype CtlKeyboardMoveMenu[] = {
	{1, STR_FRWD, 0},
	{1, STR_BKWD, 0},
	{1, STR_LEFT, 0},
	{1, STR_RIGHT, 0},
	{1, STR_STF_LEFT, 0},
	{1, STR_STF_RIGHT, 0},
	{0, "", 0},
	{1, STR_ACTION_KEYS, CP_KeyboardActionCtl}
};

CP_itemtype CtlKeyboardActionMenu[] = {
	{1, STR_CRUN, 0},
	{1, STR_COPEN, 0},
	{1, STR_CFIRE, 0},
	{1, STR_CSTRAFE, 0},
	{0, "", 0},
	{1, "More Actions", CP_KeyboardMoreActionCtl},
	{1, STR_MOVEMENT_KEYS, CP_KeyboardMoveCtl}
};

CP_itemtype CtlKeyboardMoreActionMenu[] = {
	{1, STR_WPN_1, 0},
	{1, STR_WPN_2, 0},
	{1, STR_WPN_3, 0},
	{1, STR_WPN_4, 0},
	{1, STR_PREV_WPN, 0},
	{1, STR_NEXT_WPN, 0},
	//{1, STR_AUTOMAP, 0},
	{0, "", 0},
	{1, STR_ACTION_KEYS, CP_KeyboardActionCtl}
};

CP_itemtype CtlJoystickMenu[] = {
	{1, STR_CRUN, 0},
	{1, STR_COPEN, 0},
	{1, STR_CFIRE, 0},
	{1, STR_CSTRAFE, 0} };
#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)
CP_itemtype CusCtlMenu[] = {
	{1, STR_CUS_CTL_1, 0},
	{1, STR_CUS_CTL_2, 0},
	{1, STR_CUS_CTL_3, 0},
	{1, STR_CUS_CTL_4, 0},
	{1, STR_CUS_CTL_5, 0},
	{1, STR_CUS_CTL_6, 0},
	{1, STR_CUS_CTL_7, 0},
	{1, STR_CUS_CTL_8, 0},
	{1, STR_CUS_CTL_9, 0},
	{1, STR_CUS_CTL_10, 0} };
#endif
#else
CP_itemtype CtlMouseMenu[] = {
	{1, STR_CRUN, 0},
	{1, STR_COPEN, 0},
	{1, STR_CFIRE, 0},
	{1, STR_CSTRAFE, 0},
	{0, "", 0},
	{1, STR_SENS, MouseSensitivity} };
#endif

#ifdef SHOW_ATMOS_OPTIONS
CP_itemtype AtmosOptMenu[] = {
	{1, STR_ATMOS_TEXTURED, 0},
	{1, STR_ATMOS_SHADING, 0},
	{1, STR_ATMOS_SKYBOX, 0},
	{1, STR_ATMOS_PRECIPITATION, 0}
};
#endif

CP_itemtype OptMenu[] = {
#ifdef JAPAN
	{0, "", 0},
	{0, "", 0},
	{0, "", 0},
	{0, "", 0},
	{0, "", MouseSensitivity},
	{1, "", CustomControls}
#else
#if defined(USE_MODERN_CONTROLS)
	{ 1, STR_OP_SND, CP_Sound },
	{1, STR_OP_CTL, CP_Control},
	{1, STR_CV, CP_ChangeView},
#else
	{1, STR_CV, CP_ChangeView},
#endif
#if defined(SHOW_ATMOS_OPTIONS)
	{1, STR_ATMOS_TITLE, CP_AtmosOptions},
#endif
#endif
};

// CP_iteminfo struct format: short x, y, amount, curpos, indent;
CP_iteminfo MainItems = { MENU_X, MENU_Y, lengthof(MainMenu), STARTITEM, 24 },
OptItems = { OPT_X, OPT_Y, lengthof(OptMenu), 0, 32 },

#ifdef SHOW_ATMOS_OPTIONS
AtmosOptItems = { ATMOS_X, ATMOS_Y, lengthof(AtmosOptMenu), 0, 54 },
#endif

#ifdef USE_MODERN_CONTROLS
CusMouseItems = { OPT_MOUSE_X, OPT_MOUSE_Y, lengthof(CtlMouseMenu), 0, 54 },
CusKeyboardMoveItems = { OPT_KEYBOARD_MOVE_X, OPT_KEYBOARD_MOVE_Y + 4, lengthof(CtlKeyboardMoveMenu), 0, 54 },
CusKeyboardActionItems = { OPT_KEYBOARD_ACTION_X, OPT_KEYBOARD_ACTION_Y, lengthof(CtlKeyboardActionMenu), 0, 54 },
CusKeyboardMoreActionItems = { OPT_KEYBOARD_MORE_ACTION_X, OPT_KEYBOARD_MORE_ACTION_Y, lengthof(CtlKeyboardMoreActionMenu), 0, 54 },
CusJoystickItems = { OPT_JOYSTICK_X, OPT_JOYSTICK_Y, lengthof(CtlJoystickMenu), 0, 54 },
#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)
CusCtlItems = { CUS_CTL_TEXT_X, CUS_CTL_TEXT_Y, lengthof(CusCtlMenu), 0, 54 },
#endif
#endif
SndItems = { SM_X, SM_Y1, lengthof(SndMenu), 0, 52 },
LSItems = { LSM_X, LSM_Y, lengthof(LSMenu), 0, 24 },
CtlItems = { CTL_X, CTL_Y, lengthof(CtlMenu), -1, 56 },
CusItems = { 8, CST_Y + 13 * 2, lengthof(CusMenu), -1, 0 },
#ifndef SPEAR
NewEitems = { NE_X, NE_Y, lengthof(NewEmenu), 0, 88 },
#endif
NewItems = { NM_X, NM_Y, lengthof(NewMenu), 2, 24 };

int color_hlite[] = {
	DEACTIVE,
	HIGHLIGHT,
	READHCOLOR,
	0x67 };

int color_norml[] = {
	DEACTIVE,
	TEXTCOLOR,
	READCOLOR,
	0x6b };

int EpisodeSelect[6] = { 1 };

static int SaveGamesAvail[10];
static int StartGame;
static int SoundStatus = 1;
static int pickquick;
static char SaveGameNames[10][32];
static char SaveName[13] = "savegam?.";

////////////////////////////////////////////////////////////////////
//
// INPUT MANAGER SCANCODE TABLES
//
////////////////////////////////////////////////////////////////////

#if 0
static const char* ScanNames[] =      // Scan code names with single chars
{
	"?", "?", "1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "-", "+", "?", "?",
	"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", "[", "]", "|", "?", "A", "S",
	"D", "F", "G", "H", "J", "K", "L", ";", "\"", "?", "?", "?", "Z", "X", "C", "V",
	"B", "N", "M", ",", ".", "/", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?", "\xf", "?", "-", "\x15", "5", "\x11", "+", "?",
	"\x13", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?",
	"?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?", "?"
};                              // DEBUG - consolidate these
static ScanCode ExtScanCodes[] =        // Scan codes with >1 char names
{
	1, 0xe, 0xf, 0x1d, 0x2a, 0x39, 0x3a, 0x3b, 0x3c, 0x3d, 0x3e,
	0x3f, 0x40, 0x41, 0x42, 0x43, 0x44, 0x57, 0x59, 0x46, 0x1c, 0x36,
	0x37, 0x38, 0x47, 0x49, 0x4f, 0x51, 0x52, 0x53, 0x45, 0x48,
	0x50, 0x4b, 0x4d, 0x00
};
static const char* ExtScanNames[] =   // Names corresponding to ExtScanCodes
{
	"Esc", "BkSp", "Tab", "Ctrl", "LShft", "Space", "CapsLk", "F1", "F2", "F3", "F4",
	"F5", "F6", "F7", "F8", "F9", "F10", "F11", "F12", "ScrlLk", "Enter", "RShft",
	"PrtSc", "Alt", "Home", "PgUp", "End", "PgDn", "Ins", "Del", "NumLk", "Up",
	"Down", "Left", "Right", ""
};

/*#pragma warning 737 9
static byte
										*ScanNames[] =          // Scan code names with single chars
										{
		"?","?","1","2","3","4","5","6","7","8","9","0","-","+","?","?",
		"Q","W","E","R","T","Y","U","I","O","P","[","]","|","?","A","S",
		"D","F","G","H","J","K","L",";","\"","?","?","?","Z","X","C","V",
		"B","N","M",",",".","/","?","?","?","?","?","?","?","?","?","?",
		"?","?","?","?","?","?","?","?","\xf","?","-","\x15","5","\x11","+","?",
		"\x13","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?",
		"?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?",
		"?","?","?","?","?","?","?","?","?","?","?","?","?","?","?","?"
										};      // DEBUG - consolidate these
static byte ExtScanCodes[] =    // Scan codes with >1 char names
										{
		1,0xe,0xf,0x1d,0x2a,0x39,0x3a,0x3b,0x3c,0x3d,0x3e,
		0x3f,0x40,0x41,0x42,0x43,0x44,0x57,0x59,0x46,0x1c,0x36,
		0x37,0x38,0x47,0x49,0x4f,0x51,0x52,0x53,0x45,0x48,
		0x50,0x4b,0x4d,0x00
										};
static byte *ExtScanNames[] =   // Names corresponding to ExtScanCodes
										{
		"Esc","BkSp","Tab","Ctrl","LShft","Space","CapsLk","F1","F2","F3","F4",
		"F5","F6","F7","F8","F9","F10","F11","F12","ScrlLk","Enter","RShft",
		"PrtSc","Alt","Home","PgUp","End","PgDn","Ins","Del","NumLk","Up",
		"Down","Left","Right",""
										};*/

#else
/*static const char* const ScanNames[SDLK_LAST] =
	{
		"?","?","?","?","?","?","?","?",                                //   0
		"BkSp","Tab","?","?","?","Return","?","?",                      //   8
		"?","?","?","Pause","?","?","?","?",                            //  16
		"?","?","?","Esc","?","?","?","?",                              //  24
		"Space","!","\"","#","$","?","&","'",                           //  32
		"(",")","*","+",",","-",".","/",                                //  40
		"0","1","2","3","4","5","6","7",                                //  48
		"8","9",":",";","<","=",">","?",                                //  56
		"@","A","B","C","D","E","F","G",                                //  64
		"H","I","J","K","L","M","N","O",                                //  72
		"P","Q","R","S","T","U","V","W",                                //  80
		"X","Y","Z","[","\\","]","^","_",                               //  88
		"`","a","b","c","d","e","f","g",                                //  96
		"h","i","j","k","l","m","n","o",                                // 104
		"p","q","r","s","t","u","v","w",                                // 112
		"x","y","z","{","|","}","~","?",                                // 120
		"?","?","?","?","?","?","?","?",                                // 128
		"?","?","?","?","?","?","?","?",                                // 136
		"?","?","?","?","?","?","?","?",                                // 144
		"?","?","?","?","?","?","?","?",                                // 152
		"?","?","?","?","?","?","?","?",                                // 160
		"?","?","?","?","?","?","?","?",                                // 168
		"?","?","?","?","?","?","?","?",                                // 176
		"?","?","?","?","?","?","?","?",                                // 184
		"?","?","?","?","?","?","?","?",                                // 192
		"?","?","?","?","?","?","?","?",                                // 200
		"?","?","?","?","?","?","?","?",                                // 208
		"?","?","?","?","?","?","?","?",                                // 216
		"?","?","?","?","?","?","?","?",                                // 224
		"?","?","?","?","?","?","?","?",                                // 232
		"?","?","?","?","?","?","?","?",                                // 240
		"?","?","?","?","?","?","?","?",                                // 248
		"?","?","?","?","?","?","?","?",                                // 256
		"?","?","?","?","?","?","?","Enter",                            // 264
		"?","Up","Down","Right","Left","Ins","Home","End",              // 272
		"PgUp","PgDn","F1","F2","F3","F4","F5","F6",                    // 280
		"F7","F8","F9","F10","F11","F12","?","?",                       // 288
		"?","?","?","?","NumLk","CapsLk","ScrlLk","RShft",              // 296
		"Shift","RCtrl","Ctrl","RAlt","Alt","?","?","?",                // 304
		"?","?","?","?","PrtSc","?","?","?",                            // 312
		"?","?"                                                         // 320
	};*/

#endif

	////////////////////////////////////////////////////////////////////
	//
	// Wolfenstein Control Panel!  Ta Da!
	//
	////////////////////////////////////////////////////////////////////
void US_ControlPanel(ScanCode scancode)
{
	int which;

	if (ingame)
	{
		if (CP_CheckQuick(scancode))
			return;
		lastgamemusicoffset = StartCPMusic(MENUSONG);
	}
	else
		StartCPMusic(MENUSONG);
	SetupControlPanel();

	//
	// F-KEYS FROM WITHIN GAME
	//
	switch (scancode)
	{
	case sc_F1:
#ifdef USE_READTHIS
		HelpScreens();
#else
		BossKey();
#endif
		goto finishup;

	case sc_F2:
		CP_SaveGame(0);
		goto finishup;

	case sc_F3:
		CP_LoadGame(0);
		goto finishup;

	case sc_F4:
		CP_Sound(0);
		goto finishup;

	case sc_F5:
		CP_ChangeView(0);
		goto finishup;

	case sc_F6:
		CP_Control(0);
		goto finishup;

	finishup:
		CleanupControlPanel();
		return;
	}

	DrawMainMenu();
	MenuFadeIn();
	StartGame = 0;

	//
	// MAIN MENU LOOP
	//
	do
	{
		which = HandleMenu(&MainItems, &MainMenu[0], NULL);

#ifdef SPEAR
#ifndef SPEARDEMO
		IN_ProcessEvents();

		//
		// EASTER EGG FOR SPEAR OF DESTINY!
		//
		if (Keyboard(sc_I) && Keyboard(sc_D))
		{
			VW_FadeOut();
			StartCPMusic(XJAZNAZI_MUS);
			ClearMemory();

			VWB_DrawPic(0, 0, IDGUYS1PIC);
			VWB_DrawPic(0, 80, IDGUYS2PIC);

			VW_UpdateScreen();

			SDL_Color pal[256];
			VL_ConvertPalette(grsegs[IDGUYSPALETTE], pal, 256);
			VL_FadeIn(0, 255, pal, 30);

			while (Keyboard(sc_I) || Keyboard(sc_D))
				IN_WaitAndProcessEvents();
			IN_ClearKeysDown();
			IN_Ack();

			VW_FadeOut();

			DrawMainMenu();
			StartCPMusic(MENUSONG);
			MenuFadeIn();
		}
#endif
#endif

		switch (which)
		{
		case viewscores:
			if (MainMenu[viewscores].routine == NULL)
			{
				if (CP_EndGame(0))
					StartGame = 1;
			}
			else
			{
				DrawMainMenu();
				MenuFadeIn();
			}
			break;

		case backtodemo:
			StartGame = 1;
			if (!ingame)
				StartCPMusic(INTROSONG);
			VL_FadeOut(0, 255, 0, 0, 0, 10);
			break;

		case -1:
		case quit:
			CP_Quit(0);
			break;

		default:
			if (!StartGame)
			{
				DrawMainMenu();
				MenuFadeIn();
			}
		}

		//
		// "EXIT OPTIONS" OR "NEW GAME" EXITS
		//
	} while (!StartGame);

	//
	// DEALLOCATE EVERYTHING
	//
	CleanupControlPanel();

	//
	// CHANGE MAINMENU ITEM
	//
	if (startgame || loadedgame)
		EnableEndGameMenuItem();
}

void EnableEndGameMenuItem()
{
	MainMenu[viewscores].routine = NULL;
#ifndef JAPAN
	strcpy(MainMenu[viewscores].string, STR_EG);
#endif
}

////////////////////////
//
// DRAW MAIN MENU SCREEN
//
void DrawMainMenu(void)
{
#ifdef JAPAN
	VWB_DrawPic(0, 0, S_OPTIONSPIC);
#else
	ClearMScreen();

	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);
	DrawStripes(10);
	VWB_DrawPic(84, -scalingOffsetY, C_OPTIONSPIC);

#ifdef SPANISH
	DrawWindow(MENU_X - 8, MENU_Y - 3, MENU_W + 8, MENU_H, BKGDCOLOR);
#else
	DrawWindow(MENU_X - 8, MENU_Y - 3, MENU_W, MENU_H, BKGDCOLOR);
#endif
#endif

	//
	// CHANGE "GAME" AND "DEMO"
	//
	if (ingame)
	{
#ifndef JAPAN

#ifdef SPANISH
		strcpy(&MainMenu[backtodemo].string, STR_GAME);
#else
		strcpy(&MainMenu[backtodemo].string[8], STR_GAME);
#endif

#else
		VWB_DrawPic(12 * 8, 20 * 8, C_MRETGAMEPIC);
		VWB_DrawPic(12 * 8, 18 * 8, C_MENDGAMEPIC);
#endif
		MainMenu[backtodemo].active = 2;
	}
	else
	{
#ifndef JAPAN
#ifdef SPANISH
		strcpy(&MainMenu[backtodemo].string, STR_BD);
#else
		strcpy(&MainMenu[backtodemo].string[8], STR_DEMO);
#endif
#else
		VWB_DrawPic(12 * 8, 20 * 8, C_MRETDEMOPIC);
		VWB_DrawPic(12 * 8, 18 * 8, C_MSCORESPIC);
#endif
		MainMenu[backtodemo].active = 1;
	}

	DrawMenu(&MainItems, &MainMenu[0]);
	VW_UpdateScreen();
}

#ifdef USE_READTHIS
////////////////////////////////////////////////////////////////////
//
// READ THIS!
//
////////////////////////////////////////////////////////////////////
int CP_ReadThis(int blank)
{
#ifndef SPEAR
	StartCPMusic(CORNER_MUS);
#else
	StartCPMusic(GETTHEM_MUS);
#endif
	HelpScreens();
	StartCPMusic(MENUSONG);
	return true;
}
#endif

#if defined(SPEAR) || defined(GOODTIMES)

////////////////////////////////////////////////////////////////////
//
// BOSS KEY
//
////////////////////////////////////////////////////////////////////
void BossKey(void)
{
	int i, lastBlinkTime;
	ControlInfo ci;

	SD_MusicOff();

	VL_ClearScreen(BLACK);
	VL_FadeIn(0, 255, gamepal, 0);

	SETFONTCOLOR(7, BLACK);
	fontnumber = 0;
	PrintX = 0;
	PrintY = 1;

	US_Print("C>");
	VW_UpdateScreen();

	i = 0;
	lastBlinkTime = GetTimeCount();

	do
	{
		IN_ProcessEvents();

		if (GetTimeCount() - lastBlinkTime > 7)
		{
			if (!i)
			{
				SETFONTCOLOR(7, BLACK);
			}
			else
			{
				SETFONTCOLOR(BLACK, BLACK);
			}

			PrintX = 14;
			US_Print("_");
			VW_UpdateScreen();

			i ^= 1;
			lastBlinkTime = GetTimeCount();
		}
		else
			SDL_Delay(5);

	} while (LastScan != sc_Escape);

	VL_ClearScreen(BLACK);
}

#endif

////////////////////////////////////////////////////////////////////
//
// CHECK QUICK-KEYS & QUIT (WHILE IN A GAME)
//
////////////////////////////////////////////////////////////////////
int CP_CheckQuick(ScanCode scancode)
{
	switch (scancode)
	{
		//
		// END GAME
		//
	case sc_F7:
		WindowH = 160;
#ifdef JAPAN
		if (GetYorN(7, 8, C_JAPQUITPIC))
#else
		if (Confirm(ENDGAMESTR))
#endif
		{
			playstate = ex_died;
			killerobj = NULL;
			pickquick = gamestate.lives = 0;
		}

		WindowH = 200;
		fontnumber = 0;
		MainMenu[savegame].active = 0;
		return 1;

		//
		// QUICKSAVE
		//
	case sc_F8:
#ifdef SAVE_GAME_SCREENSHOT
		VL_SetSaveGameSlot();
#endif
		if (SaveGamesAvail[LSItems.curpos] && pickquick)
		{
			fontnumber = 1;
			Message(STR_SAVING "...");
			CP_SaveGame(1);
			fontnumber = 0;
		}
		else
		{
			VW_FadeOut();
			if (screenHeight % 200 != 0)
				VL_ClearScreen(0);

			lastgamemusicoffset = StartCPMusic(MENUSONG);
			pickquick = CP_SaveGame(0);

			SETFONTCOLOR(0, 15);
			IN_ClearKeysDown();
			VW_FadeOut();
			if (viewsize != 21)
				DrawPlayScreen();

			if (!startgame && !loadedgame)
				ContinueMusic(lastgamemusicoffset);

			if (loadedgame)
				playstate = ex_abort;
			lasttimecount = GetTimeCount();

			if (MousePresent && IN_IsInputGrabbed())
				IN_CenterMouse(); // Clear accumulated mouse movement
		}
		return 1;

		//
		// QUICKLOAD
		//
	case sc_F9:
		if (SaveGamesAvail[LSItems.curpos] && pickquick)
		{
			char string[100] = STR_LGC;

			fontnumber = 1;

			strcat(string, SaveGameNames[LSItems.curpos]);
			strcat(string, "\"?");

			if (Confirm(string))
				CP_LoadGame(1);

			fontnumber = 0;
		}
		else
		{
			VW_FadeOut();
			if (screenHeight % 200 != 0)
				VL_ClearScreen(0);

			lastgamemusicoffset = StartCPMusic(MENUSONG);
			pickquick = CP_LoadGame(0); // loads lastgamemusicoffs

			SETFONTCOLOR(0, 15);
			IN_ClearKeysDown();
			VW_FadeOut();
			if (viewsize != 21)
				DrawPlayScreen();

			if (!startgame && !loadedgame)
				ContinueMusic(lastgamemusicoffset);

			if (loadedgame)
				playstate = ex_abort;

			lasttimecount = GetTimeCount();

			if (MousePresent && IN_IsInputGrabbed())
				IN_CenterMouse(); // Clear accumulated mouse movement
		}
		return 1;

		//
		// QUIT
		//
	case sc_F10:
		WindowX = WindowY = 0;
		WindowW = 320;
		WindowH = 160;
#ifdef JAPAN
		if (GetYorN(7, 8, C_QUITMSGPIC))
#else
#ifdef SPANISH
		if (Confirm(ENDGAMESTR))
#else
		if (Confirm(endStrings[US_RndT() & 0x7 + (US_RndT() & 1)]))
#endif
#endif
		{
			VW_UpdateScreen();
			SD_MusicOff();
			SD_StopSound();
			MenuFadeOut();

			Quit(NULL);
		}

		DrawPlayBorder();
		WindowH = 200;
		fontnumber = 0;
		return 1;
	}

	return 0;
}

////////////////////////////////////////////////////////////////////
//
// END THE CURRENT GAME
//
////////////////////////////////////////////////////////////////////
int CP_EndGame(int blank)
{
	int res;
#ifdef JAPAN
	res = GetYorN(7, 8, C_JAPQUITPIC);
#else
	res = Confirm(ENDGAMESTR);
#endif
	DrawMainMenu();
	if (!res)
		return 0;

	pickquick = gamestate.lives = 0;
	playstate = ex_died;
	killerobj = NULL;

	MainMenu[savegame].active = 0;
	MainMenu[viewscores].routine = CP_ViewScores;
#ifndef JAPAN
	strcpy(MainMenu[viewscores].string, STR_VS);
#endif

	return 1;
}

////////////////////////////////////////////////////////////////////
//
// VIEW THE HIGH SCORES
//
////////////////////////////////////////////////////////////////////
int CP_ViewScores(int blank)
{
	fontnumber = 0;

#ifdef SPEAR
	StartCPMusic(XAWARD_MUS);
#else
	StartCPMusic(ROSTER_MUS);
#endif

	DrawHighScores();
	VW_UpdateScreen();
	MenuFadeIn();
	fontnumber = 1;

	IN_Ack();

	StartCPMusic(MENUSONG);
	MenuFadeOut();

	return 0;
}

////////////////////////////////////////////////////////////////////
//
// START A NEW GAME
//
////////////////////////////////////////////////////////////////////
int CP_NewGame(int blank)
{
	int which, episode;

#if !defined(SPEAR) && !defined (SEAMLESSLEVELS)
	firstpart :

	DrawNewEpisode();

	do
	{
		which = HandleMenu(&NewEitems, &NewEmenu[0], NULL);
		switch (which)
		{
		case -1:
			MenuFadeOut();
			return 0;
		default:
			if (!EpisodeSelect[which / 2])
			{
				SD_PlaySound(NOWAYSND);
				Message("Please select \"Read This!\"\n"
					"from the Options menu to\n"
					"find out how to order this\n"
					"episode from Apogee.");
				IN_ClearKeysDown();
				IN_Ack();
				DrawNewEpisode();
				which = 0;
			}
			else
			{
				episode = which / 2;
				which = 1;
			}
			break;
		}

	} while (!which);

	ShootSnd();

	//
	// ALREADY IN A GAME?
	//
	if (ingame)
#ifdef JAPAN
		if (!GetYorN(7, 8, C_JAPNEWGAMEPIC))
#else
		if (!Confirm(CURGAME))
#endif
		{
			MenuFadeOut();
			return 0;
		}

	MenuFadeOut();

#else
	episode = 0;

	//
	// ALREADY IN A GAME?
	//
	DrawNewGame();
	if (ingame)
		if (!Confirm(CURGAME))
		{
			MenuFadeOut();

			return 0;
		}

#endif

	DrawNewGame();
	which = HandleMenu(&NewItems, &NewMenu[0], DrawNewGameDiff);
	if (which < 0)
	{
		MenuFadeOut();
#if !defined(SPEAR) && !defined (SEAMLESSLEVELS)
		goto firstpart;
#else
		return 0;
#endif
	}

	ShootSnd();
	NewGame(which, episode);
	StartGame = 1;
	MenuFadeOut();

	//
	// CHANGE "READ THIS!" TO NORMAL COLOR
	//
#ifdef USE_READTHIS
	MainMenu[readthis].active = 1;
#endif

	pickquick = 0;

	return 0;
}

#if !defined(SPEAR) && !defined (SEAMLESSLEVELS)

/////////////////////
//
// DRAW NEW EPISODE MENU
//
void DrawNewEpisode(void)
{
	int i;

#ifdef JAPAN
	VWB_DrawPic(0, 0, S_EPISODEPIC);
#else
	ClearMScreen();
	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);

	DrawWindow(NE_X - 4, NE_Y - 4, NE_W + 8, NE_H + 8, BKGDCOLOR);
	SETFONTCOLOR(READHCOLOR, BKGDCOLOR);
	PrintY = 2 + scalingOffsetY;
	WindowX = scalingOffsetX;
#ifdef SPANISH
	US_CPrint("Cual episodio jugar?");
#else
	US_CPrint("Which episode to play?");
#endif
#endif

	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
	DrawMenu(&NewEitems, &NewEmenu[0]);

	for (i = 0; i < 6; i++)
		VWB_DrawPic(NE_X + 32, NE_Y + i * 26, C_EPISODE1PIC + i);

	VW_UpdateScreen();
	MenuFadeIn();
	WaitKeyUp();
}

#endif

/////////////////////
//
// DRAW NEW GAME MENU
//
void DrawNewGame(void)
{
#ifdef JAPAN
	VWB_DrawPic(0, 0, S_SKILLPIC);
#else
	ClearMScreen();
	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);

	SETFONTCOLOR(READHCOLOR, BKGDCOLOR);
	PrintX = NM_X + 20;
	PrintY = NM_Y - 32;

#ifndef SPEAR
#ifdef SPANISH
	US_Print("Eres macho?");
#else
	PrintX += scalingOffsetX;
	PrintY += scalingOffsetY;
	US_Print("How tough are you?");
#endif
#else
	VWB_DrawPic(PrintX, PrintY, C_HOWTOUGHPIC);
#endif

	DrawWindow(NM_X - 5, NM_Y - 10, NM_W, NM_H, BKGDCOLOR);
#endif

	DrawMenu(&NewItems, &NewMenu[0]);
	DrawNewGameDiff(NewItems.curpos);
	VW_UpdateScreen();
	MenuFadeIn();
	WaitKeyUp();
}

////////////////////////
//
// DRAW NEW GAME GRAPHIC
//
void DrawNewGameDiff(int w)
{
	VWB_DrawPic(NM_X + 185, NM_Y + 7, w + C_BABYMODEPIC);
}

////////////////////////////////////////////////////////////////////
//
// HANDLE SOUND MENU
//
////////////////////////////////////////////////////////////////////
#ifndef VIEASM
int CP_Sound(int blank)
{
	int which;
	menuExit = 0;

	DrawSoundMenu();
	MenuFadeIn();
	WaitKeyUp();

	do
	{
		which = HandleMenu(&SndItems, &SndMenu[0], NULL);
		//
		// HANDLE MENU CHOICES
		//
		switch (which)
		{
		case -1:
			MenuFadeOut();
			break;
			//
			// SOUND EFFECTS
			//
		case 0:
			if (SoundMode != sdm_Off)
			{
				SD_WaitSoundDone();
				SD_SetSoundMode(sdm_Off);
				DrawSoundMenu();
			}
			break;
		case 1:
			if (SoundMode != sdm_PC)
			{
				SD_WaitSoundDone();
				SD_SetSoundMode(sdm_PC);
				CA_LoadAllSounds();
				DrawSoundMenu();
				ShootSnd();
			}
			break;
		case 2:
			if (SoundMode != sdm_AdLib)
			{
				SD_WaitSoundDone();
				SD_SetSoundMode(sdm_AdLib);
				CA_LoadAllSounds();
				DrawSoundMenu();
				ShootSnd();
			}
			break;
			//
			// DIGITIZED SOUND
			//
		case 5:
			if (DigiMode != sds_Off)
			{
				SD_SetDigiDevice(sds_Off);
				DrawSoundMenu();
			}
			break;
		case 6:
			/*                if (DigiMode != sds_SoundSource)
							{
								SD_SetDigiDevice (sds_SoundSource);
								DrawSoundMenu ();
								ShootSnd ();
							}*/
			break;
		case 7:
			if (DigiMode != sds_SoundBlaster)
			{
				SD_SetDigiDevice(sds_SoundBlaster);
				DrawSoundMenu();
				ShootSnd();
			}
			break;
			//
			// MUSIC
			//
		case 10:
			if (MusicMode != smm_Off)
			{
				SD_SetMusicMode(smm_Off);
				DrawSoundMenu();
				ShootSnd();
			}
			break;
		case 11:
			if (MusicMode != smm_AdLib)
			{
				SD_SetMusicMode(smm_AdLib);
				DrawSoundMenu();
				ShootSnd();
				StartCPMusic(MENUSONG);
			}
			break;
		default:
			break;
		}
	} while (which >= 0);

	return 0;
}
#endif
//////////////////////
//
// DRAW THE SOUND MENU
//
#ifndef VIEASM
void DrawSoundMenu(void)
{
	int i, on;

#ifdef JAPAN
	VWB_DrawPic(0, 0, S_SOUNDPIC);
#else
	//
	// DRAW SOUND MENU
	//
	ClearMScreen();
	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);

	DrawWindow(SM_X - 8, SM_Y1 - 3, SM_W, SM_H1, BKGDCOLOR);
	DrawWindow(SM_X - 8, SM_Y2 - 3, SM_W, SM_H2, BKGDCOLOR);
	DrawWindow(SM_X - 8, SM_Y3 - 3, SM_W, SM_H3, BKGDCOLOR);
#endif

	//
	// IF NO ADLIB, NON-CHOOSENESS!
	//
	if (!AdLibPresent && !SoundBlasterPresent)
	{
		SndMenu[2].active = SndMenu[10].active = SndMenu[11].active = 0;
	}

	if (!SoundBlasterPresent)
		SndMenu[7].active = 0;

	if (!SoundBlasterPresent)
		SndMenu[5].active = 0;

	DrawMenu(&SndItems, &SndMenu[0]);
#ifndef JAPAN
	VWB_DrawPic(100, SM_Y1 - 20, C_FXTITLEPIC);
	VWB_DrawPic(100, SM_Y2 - 20, C_DIGITITLEPIC);
	VWB_DrawPic(100, SM_Y3 - 20, C_MUSICTITLEPIC);
#endif

	for (i = 0; i < SndItems.amount; i++)
#ifdef JAPAN
		if (i != 3 && i != 4 && i != 8 && i != 9)
#else
		if (SndMenu[i].string[0])
#endif
		{
			//
			// DRAW SELECTED/NOT SELECTED GRAPHIC BUTTONS
			//
			on = 0;
			switch (i)
			{
				//
				// SOUND EFFECTS
				//
			case 0:
				if (SoundMode == sdm_Off)
					on = 1;
				break;
			case 1:
				if (SoundMode == sdm_PC)
					on = 1;
				break;
			case 2:
				if (SoundMode == sdm_AdLib)
					on = 1;
				break;

				//
				// DIGITIZED SOUND
				//
			case 5:
				if (DigiMode == sds_Off)
					on = 1;
				break;
			case 6:
				//                    if (DigiMode == sds_SoundSource)
				//                        on = 1;
				break;
			case 7:
				if (DigiMode == sds_SoundBlaster)
					on = 1;
				break;

				//
				// MUSIC
				//
			case 10:
				if (MusicMode == smm_Off)
					on = 1;
				break;
			case 11:
				if (MusicMode == smm_AdLib)
					on = 1;
				break;
			}

			if (on)
				VWB_DrawPic(SM_X + 24, SM_Y1 + i * 13 + 2, C_SELECTEDPIC);
			else
				VWB_DrawPic(SM_X + 24, SM_Y1 + i * 13 + 2, C_NOTSELECTEDPIC);
		}

	DrawMenuGun(&SndItems);
	VW_UpdateScreen();
}
#else
void
DrawSliderBox(int x, int y, int val, int valinc, int width, int height, byte colour)
{
	byte usecolour;
	if (colour == READCOLOR)
		usecolour = READHCOLOR;
	else
		usecolour = HIGHLIGHT;
	DrawOutline(x + valinc * val, y, width, height, 0, colour);
	VWB_Bar(x + 1 + valinc * val + scalingOffsetX, y + 1 + scalingOffsetY, width - 1, height - 1, usecolour);
}

void
DrawSoundVols(bool curmode)
{
	ClearMScreen();
	DrawWindow(40, 25, 240, 145, BKGDCOLOR);

	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);

	WindowX = scalingOffsetX;
	WindowW = 320;

	PrintY = 30 + scalingOffsetY;
	SETFONTCOLOR(READCOLOR, BKGDCOLOR);
	US_CPrint("Adjust Volume");

	PrintY = 58 + scalingOffsetY;
	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
	US_CPrint("Sound");

	PrintY = 108 + scalingOffsetY;
	US_CPrint("Music");

	char soundstr[4], musicstr[4];

	sprintf(soundstr, "%d", soundvol);
	sprintf(musicstr, "%d", musicvol);

	if (curmode)
	{
		SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
	}
	else
	{
		SETFONTCOLOR(READCOLOR, BKGDCOLOR);
	}
	PrintX = 65 + scalingOffsetX + soundvol * 2 - strlen(soundstr) * 4;
	PrintY = 84 + scalingOffsetY;
	US_Print(soundstr);

	if (!curmode)
	{
		SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
	}
	else
	{
		SETFONTCOLOR(READCOLOR, BKGDCOLOR);
	}
	PrintX = 65 + scalingOffsetX + musicvol * 2 - strlen(musicstr) * 4;
	PrintY = 134 + scalingOffsetY;
	US_Print(musicstr);

	VWB_Bar(60 + scalingOffsetX, 72 + scalingOffsetY, 210, 10, TEXTCOLOR);
	DrawOutline(60, 72, 210, 10, 0, HIGHLIGHT);

	VWB_Bar(60 + scalingOffsetX, 122 + scalingOffsetY, 210, 10, TEXTCOLOR);
	DrawOutline(60, 122, 210, 10, 0, HIGHLIGHT);

	DrawSliderBox(60, 72, soundvol, 2, 10, 10, (curmode) ? TEXTCOLOR : READCOLOR);
	DrawSliderBox(60, 122, musicvol, 2, 10, 10, (curmode) ? READCOLOR : TEXTCOLOR);

	VW_UpdateScreen();
}

int AdjustVolume(int)
{
	ControlInfo ci;
	int exit = 0, oldSV = soundvol, oldMV = musicvol;
	bool curmode = 0;

	DrawSoundVols(curmode);
	MenuFadeIn();
	WaitKeyUp();
	do
	{
		SDL_Delay(5);
		ReadAnyControl(&ci);
		switch (ci.dir)
		{
		case dir_North:
		case dir_South:
			curmode = (curmode) ? 0 : 1;
			DrawSoundVols(curmode);
			IN_ClearKeysDown();
			break;
		case dir_West:
			if (curmode)
			{
				if (musicvol > 0)
				{
					musicvol--;
					DrawSoundVols(curmode);
					SD_ChangeVolume((byte)(soundvol * 1.28), (byte)(musicvol * 1.28));
					TicDelay(2);
				}
			}
			else
			{
				if (soundvol > 0)
				{
					soundvol--;
					DrawSoundVols(curmode);
					SD_ChangeVolume((byte)(soundvol * 1.28), (byte)(musicvol * 1.28));
					TicDelay(2);
				}
			}
			break;
		case dir_East:
			if (curmode)
			{
				if (musicvol < 100)
				{
					musicvol++;
					DrawSoundVols(curmode);
					SD_ChangeVolume((byte)(soundvol * 1.28), (byte)(musicvol * 1.28));
					TicDelay(2);
				}
			}
			else
			{
				if (soundvol < 100)
				{
					soundvol++;
					DrawSoundVols(curmode);
					SD_ChangeVolume((byte)(soundvol * 1.28), (byte)(musicvol * 1.28));
					TicDelay(2);
				}
			}
			break;
		}

		if (ci.button0 || Keyboard(sc_Space) || Keyboard(sc_Enter))
			exit = 1;
		else if (ci.button1 || Keyboard(sc_Escape))
			exit = 2;

	} while (!exit);

	if (exit == 2)
	{
		soundvol = oldSV;
		musicvol = oldMV;
		SD_PlaySound(ESCPRESSEDSND);
	}
	else
		SD_PlaySound(SHOOTSND);

	WaitKeyUp();
	MenuFadeOut();

	return 0;
}

int
CP_Sound(int)
{
	int which;

	DrawSoundMenu();
	MenuFadeIn();
	WaitKeyUp();

	do
	{
		which = HandleMenu(&SndItems, &SndMenu[0], NULL);
		//
		// HANDLE MENU CHOICES
		//
		switch (which)
		{
			//
			// SOUND EFFECTS
			//
		case 0:
			if (SoundMode != sdm_Off)
			{
				SD_WaitSoundDone();
				SD_SetSoundMode(sdm_Off);
				DrawSoundMenu();
			}
			break;
		case 1:
			if (SoundMode != sdm_AdLib)
			{
				SD_WaitSoundDone();
				SD_SetSoundMode(sdm_AdLib);
				CA_LoadAllSounds();
				DrawSoundMenu();
				ShootSnd();
			}
			break;

			//
			// MUSIC
			//
		case 4:
			if (MusicMode != smm_Off)
			{
				SD_SetMusicMode(smm_Off);
				DrawSoundMenu();
				ShootSnd();
			}
			break;
		case 5:
			if (MusicMode != smm_AdLib)
			{
				SD_SetMusicMode(smm_AdLib);
				DrawSoundMenu();
				ShootSnd();
				StartCPMusic(MENUSONG);
			}
			break;
		case 8:
			DrawSoundMenu();
			MenuFadeIn();
			WaitKeyUp();
			break;
		case 9:
			reversestereo ^= 1;
			SD_Reverse(reversestereo);
			DrawSoundMenu();
			ShootSnd();
		}
	} while (which >= 0);

	MenuFadeOut();

	return 0;
}

void
DrawSoundMenu(void)
{
	int i, on;

	//
	// DRAW SOUND MENU
	//
	ClearMScreen();
	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);

	DrawWindow(SM_X - 8, SM_Y1 - 3, SM_W, SM_H1, BKGDCOLOR);
	DrawWindow(SM_X - 8, SM_Y2 - 3, SM_W, SM_H2, BKGDCOLOR);
	DrawWindow(SM_X - 8, SM_Y3 - 3, SM_W, SM_H3, BKGDCOLOR);

	VWB_DrawPic(100, SM_Y1 - 20, C_FXTITLEPIC);
	VWB_DrawPic(100, SM_Y2 - 20, C_MUSICTITLEPIC);
	VWB_DrawPic(100, SM_Y3 - 20, C_DIGITITLEPIC);

	DrawMenu(&SndItems, &SndMenu[0]);

	for (i = 0; i < SndItems.amount; i++)
		if (SndMenu[i].string[0])
		{
			//
			// DRAW SELECTED/NOT SELECTED GRAPHIC BUTTONS
			//
			on = 0;
			switch (i)
			{
				//
				// SOUND EFFECTS
				//
			case 0:
				if (SoundMode == sdm_Off)
					on = 1;
				break;
			case 1:
				if (SoundMode == sdm_AdLib)
					on = 1;
				break;

				//
				// MUSIC
				//
			case 4:
				if (MusicMode == smm_Off)
					on = 1;
				break;
			case 5:
				if (MusicMode == smm_AdLib)
					on = 1;
				break;
			case 9:
				if (reversestereo)
					on = 1;
				break;
			}

			if (i < 6 || i > 8)
			{
				if (on)
					VWB_DrawPic(SM_X + 24, SM_Y1 + i * 13 + 2, C_SELECTEDPIC);
				else
					VWB_DrawPic(SM_X + 24, SM_Y1 + i * 13 + 2, C_NOTSELECTEDPIC);
			}
		}

	DrawMenuGun(&SndItems);
	VW_UpdateScreen();
}
#endif


//
// DRAW LOAD/SAVE IN PROGRESS
//
void DrawLSAction(int which)
{
#define LSA_X 96
#define LSA_Y 80
#define LSA_W 130
#define LSA_H 42

	DrawWindow(LSA_X, LSA_Y, LSA_W, LSA_H, TEXTCOLOR);
	DrawOutline(LSA_X, LSA_Y, LSA_W, LSA_H, 0, HIGHLIGHT);
	VWB_DrawPic(LSA_X + 8, LSA_Y + 5, C_DISKLOADING1PIC);

	fontnumber = 1;
	SETFONTCOLOR(0, TEXTCOLOR);
	PrintX = LSA_X + 46;
	PrintY = LSA_Y + 13;

	if (!which)
		US_Print(STR_LOADING "...");
	else
		US_Print(STR_SAVING "...");

	VW_UpdateScreen();
}

////////////////////////////////////////////////////////////////////
//
// LOAD SAVED GAMES
//
////////////////////////////////////////////////////////////////////
int CP_LoadGame(int quick)
{
	FILE* file;
	int which, exit = 0;
	char name[13];
	char loadpath[300];

	strcpy(name, SaveName);

	//
	// QUICKLOAD?
	//
	if (quick)
	{
		which = LSItems.curpos;

		if (SaveGamesAvail[which])
		{
			name[7] = which + '0';

			if (configdir[0])
				snprintf(loadpath, sizeof(loadpath), "%s/%s", configdir, name);
			else
				strcpy(loadpath, name);

			file = fopen(loadpath, "rb");
			fseek(file, 32, SEEK_SET);
			loadedgame = true;
			LoadTheGame(file, 0, 0);
			loadedgame = false;
			fclose(file);

			DrawFace();
			DrawHealth();
			DrawLives();
			DrawLevel();
			DrawAmmo();
			DrawKeys();
			DrawWeapon();
			DrawScore();
			ContinueMusic(lastgamemusicoffset);
			return 1;
		}
	}

	DrawLoadSaveScreen(0);

	do
	{
		which = HandleMenu(&LSItems, &LSMenu[0], TrackWhichGame);
		if (which >= 0 && SaveGamesAvail[which])
		{
			ShootSnd();
			name[7] = which + '0';

			if (configdir[0])
				snprintf(loadpath, sizeof(loadpath), "%s/%s", configdir, name);
			else
				strcpy(loadpath, name);

			file = fopen(loadpath, "rb");
			fseek(file, 32, SEEK_SET);

			DrawLSAction(0);
			loadedgame = true;

			LoadTheGame(file, LSA_X + 8, LSA_Y + 5);
			fclose(file);

			StartGame = 1;
			ShootSnd();
			//
			// CHANGE "READ THIS!" TO NORMAL COLOR
			//

#ifdef USE_READTHIS
			MainMenu[readthis].active = 1;
#endif

			exit = 1;
			break;
		}

	} while (which >= 0);

	MenuFadeOut();

	return exit;
}


///////////////////////////////////
//
// HIGHLIGHT CURRENT SELECTED ENTRY
//
void TrackWhichGame(int w)
{
	static int lastgameon = 0;

	PrintLSEntry(lastgameon, TEXTCOLOR);
	PrintLSEntry(w, HIGHLIGHT);

#ifdef SAVE_GAME_SCREENSHOT
	char loadpath[300];
	char bmpName[13] = BMP_SAVE_FILENAME;
	bmpName[7] = w + '0';

	if (configdir[0]) {
		snprintf(loadpath, sizeof(loadpath), "%s/%s", configdir, bmpName);
	}
	else {
		strcpy(loadpath, bmpName);
	}

	DrawWindow(LSP_X - 1, LSP_Y - 1, LSP_W + 1, LSP_H + 1, 0x00);
	SDL_Surface* bmpSurface = SDL_LoadBMP(bmpName);

	if (bmpSurface != NULL) {
		byte* surfaceBytes = new byte[bmpSurface->w * bmpSurface->h * 4];

		VL_SurfaceToByteArray(bmpSurface, surfaceBytes);
		VL_MemToScreen(surfaceBytes, LSP_W, LSP_H, LSP_X + scalingOffsetX, LSP_Y + scalingOffsetY);

		delete[] surfaceBytes;
		SDL_FreeSurface(bmpSurface);
	}
#endif

	lastgameon = w;
}

////////////////////////////
//
// DRAW THE LOAD/SAVE SCREEN
//
void DrawLoadSaveScreen(int loadsave)
{
#define DISKX 100 + scalingOffsetX
#define DISKY scalingOffsetY

	int i;

	ClearMScreen();
	fontnumber = 1;
	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);
	DrawWindow(LSM_X - 10, LSM_Y - 5, LSM_W, LSM_H, BKGDCOLOR);

#ifdef SAVE_GAME_SCREENSHOT
	DrawWindow(LSP_X - 1, LSP_Y - 1, LSP_W + 1, LSP_H + 1, 0x00);
#endif

	DrawStripes(10);

	if (!loadsave)
		VWB_DrawPic(60, -scalingOffsetY, C_LOADGAMEPIC);
	else
		VWB_DrawPic(60, -scalingOffsetY, C_SAVEGAMEPIC);

	for (i = 0; i < 10; i++)
		PrintLSEntry(i, TEXTCOLOR);

	DrawMenu(&LSItems, &LSMenu[0]);
	VW_UpdateScreen();
	MenuFadeIn();
	WaitKeyUp();
}

///////////////////////////////////////////
//
// PRINT LOAD/SAVE GAME ENTRY W/BOX OUTLINE
//
void PrintLSEntry(int w, int color)
{
	SETFONTCOLOR(color, BKGDCOLOR);
	DrawOutline(LSM_X + LSItems.indent, LSM_Y + w * 13, LSM_W - LSItems.indent - 15, 11, color,
		color);
	PrintX = LSM_X + LSItems.indent + 2 + scalingOffsetX;
	PrintY = LSM_Y + w * 13 + 1 + scalingOffsetY;
	fontnumber = 0;

	if (SaveGamesAvail[w])
		US_Print(SaveGameNames[w]);
	else
		US_Print("      - " STR_EMPTY " -");

	fontnumber = 1;
}

////////////////////////////////////////////////////////////////////
//
// SAVE CURRENT GAME
//
////////////////////////////////////////////////////////////////////
int CP_SaveGame(int quick)
{
	int which, exit = 0;
	FILE* file;
	char name[13];
	char savepath[300];
	char input[32];

#ifdef SAVE_GAME_SCREENSHOT
	char picpath[300];
	char bmpName[13] = BMP_SAVE_FILENAME;
#endif

	strcpy(name, SaveName);

	//
	// QUICKSAVE?
	//
	if (quick)
	{
		which = LSItems.curpos;

		if (SaveGamesAvail[which])
		{
			name[7] = which + '0';

			if (configdir[0])
				snprintf(savepath, sizeof(savepath), "%s/%s", configdir, name);
			else
				strcpy(savepath, name);

			unlink(savepath);
			file = fopen(savepath, "wb");

			strcpy(input, &SaveGameNames[which][0]);

			fwrite(input, 1, 32, file);
			fseek(file, 32, SEEK_SET);
			SaveTheGame(file, 0, 0);

#ifdef SAVE_GAME_SCREENSHOT
			bmpName[7] = which + '0';
			if (configdir[0]) {
				snprintf(picpath, sizeof(picpath), "%s/%s", configdir, bmpName);
			}
			else {
				strcpy(picpath, bmpName);
			}

			unlink(picpath);
			SDL_SaveBMP(lastGameSurface, picpath);
#endif

			fclose(file);

			return 1;
		}
	}

	DrawLoadSaveScreen(1);

	do
	{
		which = HandleMenu(&LSItems, &LSMenu[0], TrackWhichGame);
		if (which >= 0)
		{
			//
			// OVERWRITE EXISTING SAVEGAME?
			//
			if (SaveGamesAvail[which])
			{
#ifdef JAPAN
				if (!GetYorN(7, 8, C_JAPSAVEOVERPIC))
#else
				if (!Confirm(GAMESVD))
#endif
				{
					DrawLoadSaveScreen(1);
					continue;
				}
				else
				{
					DrawLoadSaveScreen(1);
					PrintLSEntry(which, HIGHLIGHT);
					VW_UpdateScreen();
				}
			}

			ShootSnd();

			strcpy(input, &SaveGameNames[which][0]);
			name[7] = which + '0';

			fontnumber = 0;
			if (!SaveGamesAvail[which])
				VWB_Bar(LSM_X + LSItems.indent + 1 + scalingOffsetX, LSM_Y + scalingOffsetY + which * 13 + 1,
					LSM_W - LSItems.indent - 16, 10, BKGDCOLOR);
			VW_UpdateScreen();

			if (US_LineInput(LSM_X + LSItems.indent + 2 + scalingOffsetX, LSM_Y + scalingOffsetY + which * 13 + 1, input, input, true, 31,
				LSM_W - LSItems.indent - 30))
			{
				SaveGamesAvail[which] = 1;
				strcpy(&SaveGameNames[which][0], input);

				if (configdir[0])
					snprintf(savepath, sizeof(savepath), "%s/%s", configdir, name);
				else
					strcpy(savepath, name);

				unlink(savepath);
				file = fopen(savepath, "wb");
				fwrite(input, 32, 1, file);
				fseek(file, 32, SEEK_SET);

				DrawLSAction(1);
				SaveTheGame(file, LSA_X + 8, LSA_Y + 5);

#ifdef SAVE_GAME_SCREENSHOT
				bmpName[7] = which + '0';
				if (configdir[0]) {
					snprintf(picpath, sizeof(picpath), "%s/%s", configdir, bmpName);
				}
				else {
					strcpy(picpath, bmpName);
				}

				unlink(picpath);
				SDL_SaveBMP(lastGameSurface, picpath);
#endif

				fclose(file);
				ShootSnd();
				exit = 1;
			}
			else
			{
				VWB_Bar(LSM_X + LSItems.indent + 1 + scalingOffsetX, LSM_Y + scalingOffsetY + which * 13 + 1,
					LSM_W - LSItems.indent - 16, 10, BKGDCOLOR);
				PrintLSEntry(which, HIGHLIGHT);
				VW_UpdateScreen();
				SD_PlaySound(ESCPRESSEDSND);
				continue;
			}

			fontnumber = 1;
			break;
		}

	} while (which >= 0);

	MenuFadeOut();

	return exit;
}

////////////////////////////////////////////////////////////////////
//
// DEFINE CONTROLS
//
////////////////////////////////////////////////////////////////////
int CP_Control(int blank)
{
	int which;

	DrawCtlScreen();
	MenuFadeIn();
	WaitKeyUp();

	do
	{
		which = HandleMenu(&CtlItems, CtlMenu, NULL);
		switch (which)
		{
		case -1:
			MenuFadeOut();
			return 0;
			break;
		case CTL_MOUSEENABLE:
			mouseenabled ^= 1;
			if (IN_IsInputGrabbed())
				IN_CenterMouse();
			DrawCtlScreen();
			CusItems.curpos = -1;
			ShootSnd();
			break;
#ifdef USE_MODERN_CONTROLS
		case CTL_JOYENABLE:
			controllerEnabled ^= 1;
			DrawCtlScreen();
			CusItems.curpos = -1;
			ShootSnd();
			break;
#else
		case CTL_JOYENABLE:
			joystickenabled ^= 1;
			DrawCtlScreen();
			CusItems.curpos = -1;
			ShootSnd();
			break;
#endif

#ifdef USE_MODERN_CONTROLS

		case CTL_ALWAYSRUN:
			alwaysRun ^= 1;
			DrawCtlScreen();
			CusItems.curpos = -1;
			ShootSnd();
			break;
#else
		case CTL_MOUSESENS:
		case CTL_CUSTOMIZE:
			DrawCtlScreen();
			MenuFadeIn();
			WaitKeyUp();
			break;
#endif
		}
	} while (which >= 0);

	return 0;
}

#if defined(USE_MODERN_CONTROLS) || defined(SHOW_ATMOS_OPTIONS)
////////////////////////////////////////////////////////////////////
//
// DEFINE OPTIONS
//
////////////////////////////////////////////////////////////////////
int CP_Options(int blank)
{
	int which;
	menuExit = 0;

	DrawOptScreen();
	MenuFadeIn();
	WaitKeyUp();

	do
	{
		which = HandleMenu(&OptItems, OptMenu, NULL);

		switch (which)
		{
		case -1:
			MenuFadeOut();
			return 0;
			break;
		default:
			DrawOptScreen();
			MenuFadeIn();
			WaitKeyUp();
			break;
		}
	} while (which >= 0);

	return 0;
}

#ifdef SHOW_ATMOS_OPTIONS
////////////////////////////////////////////////////////////////////
//
// DEFINE OPTIONS
//
////////////////////////////////////////////////////////////////////
int CP_AtmosOptions(int blank)
{
	int which;

	DrawAtmosScreen();
	MenuFadeIn();
	WaitKeyUp();

	do
	{
		which = HandleMenu(&AtmosOptItems, AtmosOptMenu, NULL);
		switch (which)
		{
		case -1:
			MenuFadeOut();
			return 0;
			break;
		case ATMOS_USE_TEXTURED:
			atmosTexturedEnabled ^= 1;
			DrawAtmosScreen();
			CusItems.curpos = -1;
			ShootSnd();
			break;
		case ATMOS_USE_SHADING:
			atmosShadingEnabled ^= 1;
			DrawAtmosScreen();
			CusItems.curpos = -1;
			ShootSnd();
			break;
		case ATMOS_USE_SKYBOX:
			atmosSkyboxEnabled ^= 1;
			DrawAtmosScreen();
			CusItems.curpos = -1;
			ShootSnd();
			break;
		case ATMOS_USE_PRECIPITATION:
			atmosPrecipitationEnabled ^= 1;
			DrawAtmosScreen();
			MenuFadeIn();
			WaitKeyUp();
			break;
		}
	} while (which >= 0);

	return 0;
}
#endif
#endif

////////////////////////////////
//
// DRAW MOUSE SENSITIVITY SCREEN
//
void DrawMouseSens(void)
{
#ifdef JAPAN
	VWB_DrawPic(0, 0, S_MOUSESENSPIC);
#else
	ClearMScreen();
	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);
#ifdef SPANISH
	DrawWindow(10, 80, 300, 43, BKGDCOLOR);
#else
	DrawWindow(10, 80, 300, 30, BKGDCOLOR);
#endif

	WindowX = scalingOffsetX;
	WindowW = 320;
	PrintY = 82 + scalingOffsetY;
	SETFONTCOLOR(READCOLOR, BKGDCOLOR);
	US_CPrint(STR_MOUSEADJ);

	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
#ifdef SPANISH
	PrintX = 14;
	PrintY = 95 + 13;
	US_Print(STR_SLOW);
	PrintX = 252;
	US_Print(STR_FAST);
#else
	PrintX = 14 + scalingOffsetX;
	PrintY = 95 + scalingOffsetY;
	US_Print(STR_SLOW);
	PrintX = 269 + scalingOffsetX;
	US_Print(STR_FAST);
#endif
#endif

	VWB_Bar(60 + scalingOffsetX, 97 + scalingOffsetY, 200, 10, TEXTCOLOR);
	DrawOutline(60, 97, 200, 10, 0, HIGHLIGHT);
	DrawOutline(60 + 20 * mouseadjustment, 97, 20, 10, 0, READCOLOR);
	VWB_Bar(61 + 20 * mouseadjustment + scalingOffsetX, 98 + scalingOffsetY, 19, 9, READHCOLOR);

	VW_UpdateScreen();
	MenuFadeIn();
}

///////////////////////////
//
// ADJUST MOUSE SENSITIVITY
//
int MouseSensitivity(int blank)
{
	ControlInfo ci;
	int exit = 0, oldMA;

	oldMA = mouseadjustment;
	DrawMouseSens();
	do
	{
		SDL_Delay(5);
		ReadAnyControl(&ci);
		switch (ci.dir)
		{
		case dir_North:
		case dir_West:
			if (mouseadjustment)
			{
				mouseadjustment--;
				VWB_Bar(60 + scalingOffsetX, 97 + scalingOffsetY, 200, 10, TEXTCOLOR);
				DrawOutline(60, 97, 200, 10, 0, HIGHLIGHT);
				DrawOutline(60 + 20 * mouseadjustment, 97, 20, 10, 0, READCOLOR);
				VWB_Bar(61 + 20 * mouseadjustment + scalingOffsetX, 98 + scalingOffsetY, 19, 9, READHCOLOR);
				VW_UpdateScreen();
				SD_PlaySound(MOVEGUN1SND);
				TicDelay(20);
			}
			break;

		case dir_South:
		case dir_East:
			if (mouseadjustment < 9)
			{
				mouseadjustment++;
				VWB_Bar(60 + scalingOffsetX, 97 + scalingOffsetY, 200, 10, TEXTCOLOR);
				DrawOutline(60, 97, 200, 10, 0, HIGHLIGHT);
				DrawOutline(60 + 20 * mouseadjustment, 97, 20, 10, 0, READCOLOR);
				VWB_Bar(61 + 20 * mouseadjustment + scalingOffsetX, 98 + scalingOffsetY, 19, 9, READHCOLOR);
				VW_UpdateScreen();
				SD_PlaySound(MOVEGUN1SND);
				TicDelay(20);
			}
			break;
		}

		if (ci.button0 || Keyboard(sc_Space) || Keyboard(sc_Enter))
			exit = 1;
		else if (ci.button1 || Keyboard(sc_Escape))
			exit = 2;

	} while (!exit);

	if (exit == 2)
	{
		mouseadjustment = oldMA;
		SD_PlaySound(ESCPRESSEDSND);

	}
	else
		SD_PlaySound(SHOOTSND);

	WaitKeyUp();

	return 0;
}

///////////////////////////
//
// DRAW CONTROL MENU SCREEN
//
void DrawCtlScreen(void)
{
	int i, x, y;

#ifdef JAPAN
	VWB_DrawPic(0, 0, S_CONTROLPIC);
#else
	ClearMScreen();
	DrawStripes(10);
	VWB_DrawPic(80, -scalingOffsetY, C_CONTROLPIC);
	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);
	DrawWindow(CTL_X - 8, CTL_Y - 5, CTL_W, CTL_H, BKGDCOLOR);
#endif
	WindowX = 0;
	WindowW = 320;
	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);

#ifdef USE_MODERN_CONTROLS
	if (IN_ControllerPresent())
		CtlMenu[CTL_JOYENABLE].active = 1;
#else
	if (IN_JoyPresent())
		CtlMenu[CTL_JOYENABLE].active = 1;
#endif

	if (MousePresent)
	{
#ifndef USE_MODERN_CONTROLS
		CtlMenu[CTL_MOUSESENS].active = CtlMenu[CTL_MOUSEENABLE].active = 1;
#else
		CtlMenu[CTL_MOUSEENABLE].active = CtlMenu[CTL_MOUSEENABLE].active = 1;
#endif
	}
#ifndef USE_MODERN_CONTROLS
	CtlMenu[CTL_MOUSESENS].active = mouseenabled;
#else
	CtlMenu[CTL_MOUSEOPTIONS].active = mouseenabled;
	CtlMenu[CTL_JOYSTICKOPTIONS].active = controllerEnabled;
#endif
	DrawMenu(&CtlItems, CtlMenu);

	x = CTL_X + CtlItems.indent - 24;
	y = CTL_Y + 3;

	if (mouseenabled)
		VWB_DrawPic(x, y, C_SELECTEDPIC);
	else
		VWB_DrawPic(x, y, C_NOTSELECTEDPIC);

#ifdef USE_MODERN_CONTROLS
	y = CTL_Y + 16;
#else
	y = CTL_Y + 29;
#endif	

#ifdef USE_MODERN_CONTROLS
	if (controllerEnabled)
		VWB_DrawPic(x, y, C_SELECTEDPIC);
	else
		VWB_DrawPic(x, y, C_NOTSELECTEDPIC);

	y = CTL_Y + 29;

	if (alwaysRun)
		VWB_DrawPic(x, y, C_SELECTEDPIC);
	else
		VWB_DrawPic(x, y, C_NOTSELECTEDPIC);
#else
	if (joystickenabled)
		VWB_DrawPic(x, y, C_SELECTEDPIC);
	else
		VWB_DrawPic(x, y, C_NOTSELECTEDPIC);
#endif

	//
	// PICK FIRST AVAILABLE SPOT
	//
	if (CtlItems.curpos < 0 || !CtlMenu[CtlItems.curpos].active)
	{
		for (i = 0; i < CtlItems.amount; i++)
		{
			if (CtlMenu[i].active)
			{
				CtlItems.curpos = i;
				break;
			}
		}
	}

	DrawMenuGun(&CtlItems);
	VW_UpdateScreen();
}

#if defined(USE_MODERN_CONTROLS) || defined(SHOW_ATMOS_OPTIONS)
///////////////////////////
//
// DRAW OPTIONS MENU SCREEN
//
void DrawOptScreen(void)
{
	int i = 0, x = 0, y = 0;

#ifdef JAPAN
	VWB_DrawPic(0, 0, S_CONTROLPIC);
#else
	ClearMScreen();
	DrawStripes(10);
	VWB_DrawPic(80, -scalingOffsetY, C_OPTIONSPIC);
	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);
	DrawWindow(OPT_X - 8, OPT_Y - 5, OPT_W, OPT_H, BKGDCOLOR);
#endif
	WindowX = 0;
	WindowW = 320;
	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);

	DrawMenu(&OptItems, OptMenu);

	//
	// PICK FIRST AVAILABLE SPOT
	//
	if (OptItems.curpos < 0 || !OptMenu[OptItems.curpos].active)
	{
		for (i = 0; i < OptItems.amount; i++)
		{
			if (OptMenu[i].active)
			{
				OptItems.curpos = i;
				break;
			}
		}
	}

	DrawMenuGun(&OptItems);
	VW_UpdateScreen();
}

///////////////////////////
//
// DRAW GAME OPTIONS MENU SCREEN
//
void DrawAtmosScreen(void)
{
	int i, x, y;

#ifdef JAPAN
	VWB_DrawPic(0, 0, S_CONTROLPIC);
#else
	ClearMScreen();
	DrawStripes(10);
	VWB_DrawPic(80, -scalingOffsetY, C_OPTIONSPIC);
	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);
	DrawWindow(ATMOS_X - 8, ATMOS_Y - 5, ATMOS_W, ATMOS_H, BKGDCOLOR);
#endif
	WindowX = 0;
	WindowW = 320;
	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);

#if !defined(USE_FLOORCEILINGTEX)
	AtmosOptMenu[ATMOS_USE_TEXTURED].active = 0;
#endif

#if !defined(ATMOS_USE_SHADING)
	AtmosOptMenu[ATMOS_USE_SHADING].active = 0;
#endif

#if !defined(ATMOS_USE_CLOUD_STAR)
	AtmosOptMenu[ATMOS_USE_SKYBOX].active = 0;
#endif

#if !defined(ATMOS_USE_RAIN_SNOW)
	AtmosOptMenu[ATMOS_USE_PRECIPITATION].active = 0;
#endif

	DrawMenu(&AtmosOptItems, AtmosOptMenu);

	x = ATMOS_X + AtmosOptItems.indent - 23;
	y = ATMOS_Y + 2;

	if (atmosTexturedEnabled)
		VWB_DrawPic(x, y, C_SELECTEDPIC);
	else
		VWB_DrawPic(x, y, C_NOTSELECTEDPIC);

	y = ATMOS_Y + 15;

	if (atmosShadingEnabled)
		VWB_DrawPic(x, y, C_SELECTEDPIC);
	else
		VWB_DrawPic(x, y, C_NOTSELECTEDPIC);

	y = ATMOS_Y + 28;

	if (atmosSkyboxEnabled)
		VWB_DrawPic(x, y, C_SELECTEDPIC);
	else
		VWB_DrawPic(x, y, C_NOTSELECTEDPIC);

	y = ATMOS_Y + 41;

	if (atmosPrecipitationEnabled)
		VWB_DrawPic(x, y, C_SELECTEDPIC);
	else
		VWB_DrawPic(x, y, C_NOTSELECTEDPIC);

	//
	// PICK FIRST AVAILABLE SPOT
	//
	if (AtmosOptItems.curpos < 0 || !AtmosOptMenu[AtmosOptItems.curpos].active)
	{
		for (i = 0; i < AtmosOptItems.amount; i++)
		{
			if (AtmosOptMenu[i].active)
			{
				AtmosOptItems.curpos = i;
				break;
			}
		}
	}

	DrawMenuGun(&AtmosOptItems);
	VW_UpdateScreen();
}
#endif

#ifdef USE_MODERN_CONTROLS

////////////////////////////////////////////////////////////////////
//
// CUSTOMIZE MOUSE CONTROLS
//
////////////////////////////////////////////////////////////////////
enum
{
	FIRE,
	STRAFE,
	RUN,
	OPEN,
	PREV_WEP,
	NEXT_WEP
};
char mbarray[4][11] = { "Left Bt", "Right Bt", "Middle Bt", "Top Bt" };
int8_t order[6] = { RUN, OPEN, FIRE, STRAFE, PREV_WEP, NEXT_WEP };

int CP_MouseCtl(int blank)
{
	int which;
	menuExit = 0;

	DrawMouseCtlScreen();
	WaitKeyUp();

	do
	{
		which = HandleMenu(&CusMouseItems, &CtlMouseMenu[0], NULL);

		switch (which)
		{
		case CTL_MOUSE_RUN:
			DefineMouseBtns(1);
			DrawCustMouse(1);
			break;
		case CTL_MOUSE_OPEN:
			DefineMouseBtns(2);
			DrawCustMouse(2);
			break;
		case CTL_MOUSE_FIRE:
			DefineMouseBtns(3);
			DrawCustMouse(3);
			break;
		case CTL_MOUSE_STRAFE:
			DefineMouseBtns(4);
			DrawCustMouse(4);
			break;
		case CTL_MOUSEMOVEMENT:
			mouseYAxis ^= 1;
			ShootSnd();
			break;
		default:
			which = -1;
			menuExit++;
			break;
		}

		if (which != -1)
			DrawMouseCtlScreen();
	} while (which >= 0);

	ExitToControlScreen();

	return 0;
}

#else
////////////////////////////////////////////////////////////////////
//
// CUSTOMIZE CONTROLS
//
////////////////////////////////////////////////////////////////////
enum
{
	FIRE,
	STRAFE,
	RUN,
	OPEN
};
char mbarray[4][3] = { "b0", "b1", "b2", "b3" };
int8_t order[4] = { RUN, OPEN, FIRE, STRAFE };

int CustomControls(int blank)
{
	int which;

	DrawCustomScreen();
	do
	{
		which = HandleMenu(&CusItems, &CusMenu[0], FixupCustom);
		switch (which)
		{
		case 0:
			DefineMouseBtns();
			DrawCustMouse(1);
			break;
		case 3:
			DefineJoyBtns();
			DrawCustJoy(0);
			break;
		case 6:
			DefineKeyBtns();
			DrawCustKeybd(0);
			break;
		case 8:
			DefineKeyMove();
			DrawCustKeys(0);
		}
	} while (which >= 0);

	MenuFadeOut();

	return 0;
}

#endif

#ifdef USE_MODERN_CONTROLS

////////////////////////
//
// DEFINE THE MOUSE BUTTONS
//
void DefineMouseBtns(int value)
{
	CustomCtrls mouseallowed;
	int i;

	--value;

	for (i = 0; i < 4; i++)
	{
		if (i == value)
		{
			mouseallowed.allowed[i] = 1;
		}
		else
		{
			mouseallowed.allowed[i] = 0;
		}
	}

	++value;

	EnterCtrlData(value, &mouseallowed, DrawCustMouse, PrintCustMouse, MOUSE);
}

#else
void DefineMouseBtns(void)
{
	CustomCtrls mouseallowed = { 0, 1, 1, 1 };
	EnterCtrlData(2, &mouseallowed, DrawCustMouse, PrintCustMouse, MOUSE);
}
#endif

////////////////////////
//
// DEFINE THE JOYSTICK BUTTONS
//
#ifdef USE_MODERN_CONTROLS

void DefineJoyBtns(int value)
{
	CustomCtrls joyallowed;
	int i;

	--value;

	for (i = 0; i < 4; i++)
	{
		if (i == value)
		{
			joyallowed.allowed[i] = 1;
		}
		else
		{
			joyallowed.allowed[i] = 0;
		}
	}

	++value;

	EnterCtrlData(value, &joyallowed, DrawCustJoy, PrintCustJoy, CONTROLLER);
}

#else
void DefineJoyBtns(void)
{
	CustomCtrls joyallowed = { 1, 1, 1, 1 };
	EnterCtrlData(5, &joyallowed, DrawCustJoy, PrintCustJoy, JOYSTICK);
}
#endif

////////////////////////
//
// DEFINE THE KEYBOARD BUTTONS
//
#ifdef USE_MODERN_CONTROLS
void DefineKeyBtns(int value)
{
	CustomCtrls keyallowed;
	int i;

	--value;

	for (i = 0; i < 6; i++)
	{
		if (i == value)
		{
			keyallowed.allowed[i] = 1;
		}
		else
		{
			keyallowed.allowed[i] = 0;
		}
	}

	++value;
	EnterCtrlData(value, &keyallowed, DrawCustKeybd, PrintCustKeybd, KEYBOARDBTNS);
}
#else
void DefineKeyBtns(void)
{
	CustomCtrls keyallowed = { 1, 1, 1, 1 };
	EnterCtrlData(8, &keyallowed, DrawCustKeybd, PrintCustKeybd, KEYBOARDBTNS);
}
#endif

////////////////////////
//
// DEFINE THE KEYBOARD BUTTONS
//
#ifdef USE_MODERN_CONTROLS

void DefineKeyMove(int value)
{
	CustomCtrls keyallowed;
	int i;

	--value;

	for (i = 0; i < 6; i++)
	{
		if (i == value)
		{
			keyallowed.allowed[i] = 1;
		}
		else
		{
			keyallowed.allowed[i] = 0;
		}
	}

	++value;
	EnterCtrlData(value, &keyallowed, DrawCustKeys, PrintCustKeys, KEYBOARDMOVE);
}

#else
void DefineKeyMove(void)
{
	CustomCtrls keyallowed = { 1, 1, 1, 1 };
	EnterCtrlData(10, &keyallowed, DrawCustKeys, PrintCustKeys, KEYBOARDMOVE);
}
#endif

////////////////////////
//
// DEFINE THE KEYBOARD MORE ACTIONS BUTTONS
//
#ifdef USE_MODERN_CONTROLS
void DefineKeyMoreActionsBtns(int value)
{
	CustomCtrls keyallowed;
	int i;

	--value;

	for (i = 0; i < 9; i++)
	{
		if (i == value)
		{
			keyallowed.allowed[i] = 1;
		}
		else
		{
			keyallowed.allowed[i] = 0;
		}
	}

	++value;
	EnterCtrlData(value, &keyallowed, DrawMoreActionsKeys, PrintMoreActionsKeys, KEYBOARDMOREACTIONS);
}
#endif

////////////////////////
//
// ENTER CONTROL DATA FOR ANY TYPE OF CONTROL
//

#ifdef USE_MODERN_CONTROLS
enum
{
	FWRD,
	RIGHT,
	BKWD,
	LEFT,
	STF_LEFT,
	STF_RIGHT
};
int moveorder[6] = { FWRD, BKWD, LEFT, RIGHT, STF_LEFT, STF_RIGHT };
#else
enum
{
	FWRD,
	RIGHT,
	BKWD,
	LEFT
};
int moveorder[4] = { LEFT, RIGHT, FWRD, BKWD }; //Move order is not the same as everywhere else here, keep it as is												
#endif

#ifdef USE_MODERN_CONTROLS

int CP_KeyboardMoveCtl(int blank)
{
	int which;
	menuExit = 0;

	DrawKeyboardMoveCtlScreen();
	WaitKeyUp();

	do
	{
		which = HandleMenu(&CusKeyboardMoveItems, &CtlKeyboardMoveMenu[0], NULL);

		switch (which)
		{
		case CTL_KB_MOVE_FWRD:
			DefineKeyMove(1);
			DrawCustKeys(1);
			break;
		case CTL_KB_MOVE_BWRD:
			DefineKeyMove(2);
			DrawCustKeys(2);
			break;
		case CTL_KB_MOVE_LEFT:
			DefineKeyMove(3);
			DrawCustKeys(3);
			break;
		case CTL_KB_MOVE_RIGHT:
			DefineKeyMove(4);
			DrawCustKeys(4);
			break;
		case CTL_KB_STRAFE_LEFT:
			DefineKeyMove(5);
			DrawCustKeys(5);
			break;
		case CTL_KB_STRAFE_RIGHT:
			DefineKeyMove(6);
			DrawCustKeys(6);
			break;
		default:
			which = -1;
			menuExit++;
			break;
		}

		if (which != -1)
			DrawKeyboardMoveCtlScreen();

	} while (which >= 0);

	ExitToControlScreen();

	return 0;
}

int CP_KeyboardActionCtl(int blank)
{
	int which;

	DrawKeyboardActionCtlScreen();
	WaitKeyUp();

	do
	{
		which = HandleMenu(&CusKeyboardActionItems, &CtlKeyboardActionMenu[0], NULL);

		switch (which)
		{
		case CTL_KB_ACTION_RUN:
			DefineKeyBtns(1);
			DrawCustKeybd(1);
			break;
		case CTL_KB_ACTION_OPEN:
			DefineKeyBtns(2);
			DrawCustKeybd(2);
			break;
		case CTL_KB_ACTION_FIRE:
			DefineKeyBtns(3);
			DrawCustKeybd(3);
			break;
		case CTL_KB_ACTION_STRAFE:
			DefineKeyBtns(4);
			DrawCustKeybd(4);
			break;
		default:
			which = -1;
			menuExit++;
			break;
		}

		if (which != -1)
			DrawKeyboardActionCtlScreen();

	} while (which >= 0);

	ExitToControlScreen();

	return 0;
}

enum
{
	WEP1,
	WEP2,
	WEP3,
	WEP4,
	PREVWEP,
	NEXTWEP//,
	//AUTOMAP
};
int actionorder[6] = { WEP1, WEP2, WEP3, WEP4, PREVWEP, NEXTWEP/*, AUTOMAP*/ };

int CP_KeyboardMoreActionCtl(int blank)
{
	int which;

	DrawKeyboardMoreActionCtlScreen();
	WaitKeyUp();

	do
	{
		which = HandleMenu(&CusKeyboardMoreActionItems, &CtlKeyboardMoreActionMenu[0], NULL);

		switch (which)
		{
		case CTL_KB_MORE_ACTION_WEP1:
			DefineKeyMoreActionsBtns(1);
			DrawMoreActionsKeys(1);
			break;
		case CTL_KB_MORE_ACTION_WEP2:
			DefineKeyMoreActionsBtns(2);
			DrawMoreActionsKeys(2);
			break;
		case CTL_KB_MORE_ACTION_WEP3:
			DefineKeyMoreActionsBtns(3);
			DrawMoreActionsKeys(3);
			break;
		case CTL_KB_MORE_ACTION_WEP4:
			DefineKeyMoreActionsBtns(4);
			DrawMoreActionsKeys(4);
			break;
		case CTL_KB_MORE_ACTION_PREV_WEP:
			DefineKeyMoreActionsBtns(5);
			DrawMoreActionsKeys(5);
			break;
		case CTL_KB_MORE_ACTION_NEXT_WEP:
			DefineKeyMoreActionsBtns(6);
			DrawMoreActionsKeys(6);
			break;
			//case CTL_KB_MORE_ACTION_AUTOMAP:
			//	DefineKeyMoreActionsBtns(7);
			//	DrawMoreActionsKeys(7);
			//	break;
		default:
			which = -1;
			menuExit++;
			break;
		}

		if (which != -1)
			DrawKeyboardMoreActionCtlScreen();

	} while (which >= 0);

	ExitToControlScreen();

	return 0;
}

void ExitToControlScreen(void) {
	if (menuExit <= 2) {
		if (menuExit == 1) {
			menuExit++;
			MenuFadeOut();
		}

		if (menuExit == 2) {
			DrawCtlScreen();
			MenuFadeIn();
		}
	}
}

////////////////////////////////////////////////////////////////////
//
// CUSTOMIZE JOYSTICK CONTROLS
//
////////////////////////////////////////////////////////////////////

char jbarray[4][11] = { "AB Down", "AB Right", "AB Left", "AB Top" };

int CP_JoystickCtl(int blank)
{
	int which;

	DrawJoystickScreen();
	WaitKeyUp();

	do
	{
		which = HandleMenu(&CusJoystickItems, &CtlJoystickMenu[0], NULL);

		switch (which)
		{
		case CTL_JOYSTICK_RUN:
			DefineJoyBtns(1);
			DrawCustJoy(1);
			break;
		case CTL_JOYSTICK_OPEN:
			DefineJoyBtns(2);
			DrawCustJoy(2);
			break;
		case CTL_JOYSTICK_FIRE:
			DefineJoyBtns(3);
			DrawCustJoy(3);
			break;
		case CTL_JOYSTICK_STRAFE:
			DefineJoyBtns(4);
			DrawCustJoy(4);
			break;
		case -1:
			MenuFadeOut();
			break;
		default:
			break;
		}

		if (which != -1)
			DrawJoystickScreen();
	} while (which >= 0);

	DrawCtlScreen();
	MenuFadeIn();

	return 0;
}

#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)
enum
{
	ADV_CTL_1,
	ADV_CTL_2,
	ADV_CTL_3,
	ADV_CTL_4,
	ADV_CTL_5,
	ADV_CTL_6,
	ADV_CTL_7,
	ADV_CTL_8,
	ADV_CTL_9,
	ADV_CTL_10
};
int8_t advorder[10] = { ADV_CTL_1, ADV_CTL_2, ADV_CTL_3, ADV_CTL_4, ADV_CTL_5, ADV_CTL_6, ADV_CTL_7, ADV_CTL_8,
					   ADV_CTL_9, ADV_CTL_10 };

void DefineCustomCtl(int value)
{
	CustomCtrls keyallowed;
	int i;

	--value;

	for (i = 0; i < MAX_CUSTOM_CONTROLS; i++)
	{
		if (i == value)
		{
			keyallowed.allowed[i] = 1;
		}
		else
		{
			keyallowed.allowed[i] = 0;
		}
	}

	++value;
	EnterCtrlData(value, &keyallowed, DrawCustomCtlKeys, PrintCustomCtlKeys, CUSTOMCONTROLS);
}

int CP_CustomCtl(int blank)
{
	int which;
	menuExit = 0;

	DrawCustomCtlScreen();
	WaitKeyUp();

	do
	{
		which = HandleMenu(&CusCtlItems, &CusCtlMenu[0], NULL);

		switch (which)
		{
		case CTL_ADV_1:
			DefineCustomCtl(1);
			DrawCustomCtlKeys(1);
			break;
		case CTL_ADV_2:
			DefineCustomCtl(2);
			DrawCustomCtlKeys(2);
			break;
		case CTL_ADV_3:
			DefineCustomCtl(3);
			DrawCustomCtlKeys(3);
			break;
		case CTL_ADV_4:
			DefineCustomCtl(4);
			DrawCustomCtlKeys(4);
			break;
		case CTL_ADV_5:
			DefineCustomCtl(5);
			DrawCustomCtlKeys(5);
			break;
		case CTL_ADV_6:
			DefineCustomCtl(6);
			DrawCustomCtlKeys(6);
			break;
		case CTL_ADV_7:
			DefineCustomCtl(7);
			DrawCustomCtlKeys(7);
			break;
		case CTL_ADV_8:
			DefineCustomCtl(8);
			DrawCustomCtlKeys(8);
			break;
		case CTL_ADV_9:
			DefineCustomCtl(9);
			DrawCustomCtlKeys(9);
			break;
		case CTL_ADV_10:
			DefineCustomCtl(10);
			DrawCustomCtlKeys(10);
			break;
		default:
			which = -1;
			menuExit++;
			break;
		}

		if (which != -1)
			DrawCustomCtlScreen();

	} while (which >= 0);

	ExitToControlScreen();

	return 0;
}

#endif
#endif

void EnterCtrlData(int index, CustomCtrls* cust, void (*DrawRtn)(int), void (*PrintRtn)(int), int type)
{
	int j, z, exit, tick, redraw, which, x, y, picked, lastFlashTime;
	ControlInfo ci;

	int start = 0;
	int amount = 6;
	int arrayIndex = 0;

	int w = 0;

	ShootSnd();

#ifndef USE_MODERN_CONTROLS
	amount = 4;
	PrintY = CST_Y + scalingOffsetY + 13 * index;
#else
	switch (type) {
	case JOYSTICK:
		start = 0;
		amount = 4;
		break;
	case CONTROLLER:
		start = 0;
		amount = 4;
		break;
	case KEYBOARDMOVE:
		start = 0;
		amount = 8;
		break;
	case KEYBOARDMOREACTIONS:
		start = MORE_ACTIONS_ARRAY_START;
		amount = MORE_ACTIONS_ARRAY_END;
		break;
#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)
	case CUSTOMCONTROLS:
		start = CUS_CTL_ARRAY_RANGE_START;
		amount = CUS_CTL_ARRAY_RANGE_END;
		break;
#endif
	default:
		start = 0;
		amount = 6;
		break;
	}
#endif
	IN_ClearKeysDown();
	exit = 0;
	redraw = 1;
	//
	// FIND FIRST SPOT IN ALLOWED ARRAY
	//
	for (j = start; j < amount; j++)
	{
		if (cust->allowed[arrayIndex])
		{
			which = arrayIndex;
			break;
		}
		arrayIndex++;
	}

	do
	{
		if (redraw)
		{
#ifndef USE_MODERN_CONTROLS
			x = CST_START + CST_SPC * which;
			DrawWindow(5, PrintY - 1 - scalingOffsetY, 310, 13, BKGDCOLOR);
#else
			switch (type) {
			case KEYBOARDMOVE:
				x = CTL_MOUSE_X + 10;
				y = OPT_KB_MOVE_KEYS_Y;
				w = 80;
				break;
			case KEYBOARDMOREACTIONS:
				x = OPT_KEYBOARD_MORE_ACTION_TEXT_X;
				y = OPT_KEYBOARD_MORE_ACTION_TEXT_Y;
				w = 75;
				break;
#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)
			case CUSTOMCONTROLS:
				x = CUS_CTL_RIGHT_TEXT_X;
				y = CUS_CTL_TEXT_Y;
				w = 75;
				break;
#endif
			default:
				x = CTL_MOUSE_X;
				y = CST_START;
				w = CST_SPC;
				break;
			}
#endif
			DrawRtn(1);

#ifndef USE_MODERN_CONTROLS
			DrawWindow(x - 2, PrintY - scalingOffsetY, CST_SPC, 11, TEXTCOLOR);
			DrawOutline(x - 2, PrintY - scalingOffsetY, CST_SPC, 11, 0, HIGHLIGHT);
#else
			DrawWindow(x - 2, y + (CST_SPC_Y * (index - 1)), w, 11, TEXTCOLOR);
			DrawOutline(x - 2, y + (CST_SPC_Y * (index - 1)), w, 11, 0, HIGHLIGHT);
#endif
			SETFONTCOLOR(0, TEXTCOLOR);
			PrintRtn(which);
			PrintX = x + scalingOffsetX;
			SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
			VW_UpdateScreen();
			WaitKeyUp();
			redraw = 0;
		}

		SDL_Delay(5);
		ReadAnyControl(&ci);

		if (type == MOUSE || type == JOYSTICK || type == CONTROLLER)
			if (IN_KeyDown(sc_Enter) || IN_KeyDown(sc_Control) || IN_KeyDown(sc_Alt))
			{
				IN_ClearKeysDown();
				ci.button0 = ci.button1 = false;
			}

		//
		// CHANGE BUTTON VALUE?
		//
		if ((type != KEYBOARDBTNS && type != KEYBOARDMOVE && type != CUSTOMCONTROLS && type != KEYBOARDMOREACTIONS) &&
			(ci.button0 | ci.button1 | ci.button2 | ci.button3) ||
			((type == KEYBOARDBTNS || type == KEYBOARDMOVE || type == CUSTOMCONTROLS || type == KEYBOARDMOREACTIONS) && LastScan == sc_Enter))
		{
			lastFlashTime = GetTimeCount();
			tick = picked = 0;
			SETFONTCOLOR(0, TEXTCOLOR);

			if (type == KEYBOARDBTNS || type == KEYBOARDMOVE || type == CUSTOMCONTROLS || type == KEYBOARDMOREACTIONS)
				IN_ClearKeysDown();

			while (1)
			{
				int button, result = 0;

				//
				// FLASH CURSOR
				//
				if (GetTimeCount() - lastFlashTime > 10)
				{
					switch (tick)
					{
					case 0:
#ifndef USE_MODERN_CONTROLS
						w = CST_SPC;
#endif
						VWB_Bar(x + scalingOffsetX, PrintY + 1, w - 2, 10, TEXTCOLOR);
						break;
					case 1:
						PrintX = x + scalingOffsetX;
						US_Print("?");
						SD_PlaySound(HITWALLSND);
					}
					tick ^= 1;
					lastFlashTime = GetTimeCount();
					VW_UpdateScreen();
				}
				else
					SDL_Delay(5);

				//
				// WHICH TYPE OF INPUT DO WE PROCESS?
				//
				switch (type)
				{
				case MOUSE:
					button = IN_MouseButtons();

					switch (button)
					{
					case 1:
						result = 1;
						break;
					case 2:
						result = 2;
						break;
					case 4:
						result = 3;
						break;
					}

					if (result)
					{
						for (z = 0; z < 4; z++)
							if (order[which] == buttonmouse[z])
							{
								buttonmouse[z] = bt_nobutton;
								break;
							}

						buttonmouse[result - 1] = order[which];
						picked = 1;
						SD_PlaySound(SHOOTDOORSND);
					}
					break;
#ifdef USE_MODERN_CONTROLS
				case CONTROLLER:
					if (ci.button0)
						result = 1;
					else if (ci.button1)
						result = 2;
					else if (ci.button2)
						result = 3;
					else if (ci.button3)
						result = 4;

					if (result)
					{
						for (z = 0; z < 4; z++)
						{
							if (order[which] == buttoncontroller[z])
							{
								buttoncontroller[z] = bt_nobutton;
								break;
							}
						}

						buttoncontroller[result - 1] = order[which];
						picked = 1;
						SD_PlaySound(SHOOTDOORSND);
					}
					break;
#else
				case JOYSTICK:
					if (ci.button0)
						result = 1;
					else if (ci.button1)
						result = 2;
					else if (ci.button2)
						result = 3;
					else if (ci.button3)
						result = 4;

					if (result)
					{
						for (z = 0; z < 4; z++)
						{
							if (order[which] == buttonjoy[z])
							{
								buttonjoy[z] = bt_nobutton;
								break;
							}
						}

						buttonjoy[result - 1] = order[which];
						picked = 1;
						SD_PlaySound(SHOOTDOORSND);
					}
					break;
#endif
				case KEYBOARDBTNS:
					if (LastScan && LastScan != sc_Escape)
					{
#ifdef USE_MODERN_CONTROLS
						CheckKeyConflict();
#endif
						buttonscan[order[which]] = LastScan;

						picked = 1;
						SD_PlaySound(SHOOTDOORSND);
						IN_ClearKeysDown();
#ifdef USE_MODERN_CONTROLS
						exit = 1;
#endif
					}
					break;

				case KEYBOARDMOVE:
					if (LastScan && LastScan != sc_Escape)
					{
#ifdef USE_MODERN_CONTROLS
						CheckKeyConflict();
#endif
						dirscan[moveorder[which]] = LastScan;

						picked = 1;
						SD_PlaySound(SHOOTDOORSND);
						IN_ClearKeysDown();
#ifdef USE_MODERN_CONTROLS
						exit = 1;
#endif
					}
					break;

#ifdef USE_MODERN_CONTROLS
				case KEYBOARDMOREACTIONS:
					if (LastScan && LastScan != sc_Escape)
					{
						CheckKeyConflict();

						buttonscan[4 + actionorder[which]] = LastScan;

						picked = 1;
						SD_PlaySound(SHOOTDOORSND);
						IN_ClearKeysDown();
						exit = 1;
					}
					break;
#endif

#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)
				case CUSTOMCONTROLS:
					if (LastScan && LastScan != sc_Escape)
					{
						CheckKeyConflict();

						buttonscan[19 + advorder[which]] = LastScan;

						picked = 1;
						SD_PlaySound(SHOOTDOORSND);
						IN_ClearKeysDown();

						exit = 1;
					}
					break;
#endif
				}

				//
				// EXIT INPUT?
				//
				if (IN_KeyDown(sc_Escape) || type != JOYSTICK && ci.button1 || type != CONTROLLER && ci.button1)
				{
					picked = 1;
					SD_PlaySound(ESCPRESSEDSND);
				}

				if (picked)
					break;

				ReadAnyControl(&ci);
			}

			SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
			redraw = 1;
			WaitKeyUp();
			continue;
		}

		if (ci.button1 || IN_KeyDown(sc_Escape))
			exit = 1;

		//
		// MOVE TO ANOTHER SPOT?
		//
		switch (ci.dir)
		{
		case dir_West:
			do
			{
				which--;
				if (which < 0)
					which = 3;
			} while (!cust->allowed[which]);
			redraw = 1;
			SD_PlaySound(MOVEGUN1SND);
			while (ReadAnyControl(&ci), ci.dir != dir_None)
				SDL_Delay(5);
			IN_ClearKeysDown();
			break;

		case dir_East:
			do
			{
				which++;
				if (which > 3)
					which = 0;
			} while (!cust->allowed[which]);
			redraw = 1;
			SD_PlaySound(MOVEGUN1SND);
			while (ReadAnyControl(&ci), ci.dir != dir_None)
				SDL_Delay(5);
			IN_ClearKeysDown();
			break;
		case dir_North:
		case dir_South:
			exit = 1;
		}
	} while (!exit);

	SD_PlaySound(ESCPRESSEDSND);
	WaitKeyUp();

#ifndef USE_MODERN_CONTROLS
	DrawWindow(5, PrintY - 1 - scalingOffsetY, 310, 13, BKGDCOLOR);
#endif
}

////////////////////////
//
// FIXUP GUN CURSOR OVERDRAW SHIT
//
void FixupCustom(int w)
{
	static int lastwhich = -1;
	int x1 = 7 + scalingOffsetX;
	int x2 = 32 + scalingOffsetX;
	int y = CST_Y + scalingOffsetY + 26 + w * 13;

	VWB_Hlin(x1, x2, y - 1, DEACTIVE);
	VWB_Hlin(x1, x2, y + 12, BORD2COLOR);
#ifndef SPEAR
	VWB_Hlin(x1, x2, y - 2, BORDCOLOR);
	VWB_Hlin(x1, x2, y + 13, BORDCOLOR);
#else
	VWB_Hlin(x1, x2, y - 2, BORD2COLOR);
	VWB_Hlin(x1, x2, y + 13, BORD2COLOR);
#endif

	switch (w)
	{
	case 0:
		DrawCustMouse(1);
		break;
	case 3:
		DrawCustJoy(1);
		break;
	case 6:
		DrawCustKeybd(1);
		break;
	case 8:
		DrawCustKeys(1);
	}

	if (lastwhich >= 0)
	{
		y = CST_Y + scalingOffsetY + 26 + lastwhich * 13;
		VWB_Hlin(x1, x2, y - 1, DEACTIVE);
		VWB_Hlin(x1, x2, y + 12, BORD2COLOR);
#ifndef SPEAR
		VWB_Hlin(x1, x2, y - 2, BORDCOLOR);
		VWB_Hlin(x1, x2, y + 13, BORDCOLOR);
#else
		VWB_Hlin(x1, x2, y - 2, BORDCOLOR);
		VWB_Hlin(x1, x2, y + 13, BORDCOLOR);
#endif

		if (lastwhich != w)
			switch (lastwhich)
			{
			case 0:
				DrawCustMouse(0);
				break;
			case 3:
				DrawCustJoy(0);
				break;
			case 6:
				DrawCustKeybd(0);
				break;
			case 8:
				DrawCustKeys(0);
			}
	}

	lastwhich = w;
}

#ifdef USE_MODERN_CONTROLS

////////////////////////
//
// DRAW MOUSE CONTROLS SCREEN
//
void DrawMouseCtlScreen(void)
{
	int i;
	int which = 0;

	ClearMScreen();
	WindowX = 0;
	WindowW = 320;
	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);
	DrawStripes(10);
	VWB_DrawPic(80, -scalingOffsetY, C_CUSTOMIZEPIC);

	//
	// MOUSE
	//
	SETFONTCOLOR(READCOLOR, BKGDCOLOR);
	WindowX = 0;
	WindowW = 320;

#ifndef SPEAR
	//PrintY = OPT_MOUSE_Y;
	//US_CPrint("Mouse\n");
#else
	PrintY = CST_Y + 13;
	VWB_DrawPic(128, 48, C_MOUSEPIC);
#endif

	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);

	DrawWindow(OPT_MOUSE_X - 8, OPT_MOUSE_Y - 5, OPT_MOUSE_W, OPT_MOUSE_H, BKGDCOLOR);
	DrawMenuGun(&CusMouseItems);

	if (mouseYAxis)
		VWB_DrawPic(56, OPT_MOUSE_Y + 68, C_SELECTEDPIC);
	else
		VWB_DrawPic(56, OPT_MOUSE_Y + 68, C_NOTSELECTEDPIC);

	DrawMenu(&CusMouseItems, CtlMouseMenu);
	DrawCustMouse(0);

	WaitKeyUp();
	US_Print("\n");

	//
	// PICK STARTING POINT IN MENU
	//
	if (CusMouseItems.curpos < 0)
		for (i = 0; i < CusMouseItems.amount; i++)
			if (CtlMouseMenu[i].active)
			{
				CusMouseItems.curpos = i;
				break;
			}

	VW_UpdateScreen();
	MenuFadeIn();
}

////////////////////////
//
// DRAW KEYBOARD MOVE CONTROLS SCREEN
//
void DrawKeyboardMoveCtlScreen(void)
{
	int i;
	int which = 0;

	ClearMScreen();
	WindowX = 0;
	WindowW = 320;
	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);
	DrawStripes(10);
	VWB_DrawPic(80, -scalingOffsetY, C_CUSTOMIZEPIC);

	//
	// MOUSE
	//
	SETFONTCOLOR(READCOLOR, BKGDCOLOR);
	WindowX = scalingOffsetX;
	WindowW = 320;

#ifdef SPEAR
	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
	PrintY = OPT_MOUSE_Y;
	US_CPrint("Movement keys");
#endif

	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);

	DrawWindow(OPT_KEYBOARD_MOVE_X - 8, OPT_KEYBOARD_MOVE_Y, OPT_KEYBOARD_MOVE_W, OPT_KEYBOARD_MOVE_H, BKGDCOLOR);
	DrawMenuGun(&CusKeyboardMoveItems);

	DrawMenu(&CusKeyboardMoveItems, CtlKeyboardMoveMenu);
	DrawCustKeys(0);

	WaitKeyUp();
	US_Print("\n");

	//
	// PICK STARTING POINT IN MENU
	//
	if (CusKeyboardMoveItems.curpos < 0)
		for (i = 0; i < CusKeyboardMoveItems.amount; i++)
			if (CtlKeyboardMoveMenu[i].active)
			{
				CusKeyboardMoveItems.curpos = i;
				break;
			}

	VW_UpdateScreen();
	MenuFadeIn();
}

////////////////////////
//
// DRAW KEYBOARD ACTION CONTROLS SCREEN
//
void DrawKeyboardActionCtlScreen(void)
{
	int i;
	int which = 0;

	ClearMScreen();
	WindowX = 0;
	WindowW = 320;
	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);
	DrawStripes(10);
	VWB_DrawPic(80, -scalingOffsetY, C_CUSTOMIZEPIC);

	//
	// MOUSE
	//
	SETFONTCOLOR(READCOLOR, BKGDCOLOR);
	WindowX = scalingOffsetX;
	WindowW = 320;

#ifdef SPEAR
	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
	PrintY = OPT_MOUSE_Y;
	US_CPrint("Action keys 1/2");
#endif

	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);

	DrawWindow(OPT_KEYBOARD_ACTION_X - 8, OPT_KEYBOARD_ACTION_Y - 5, OPT_KEYBOARD_ACTION_W, OPT_KEYBOARD_ACTION_H - 13, BKGDCOLOR);
	DrawMenuGun(&CusKeyboardActionItems);

	DrawMenu(&CusKeyboardActionItems, CtlKeyboardActionMenu);
	DrawCustKeybd(0);

	WaitKeyUp();
	US_Print("\n");

	//
	// PICK STARTING POINT IN MENU
	//
	if (CusKeyboardActionItems.curpos < 0)
		for (i = 0; i < CusKeyboardActionItems.amount; i++)
			if (CtlKeyboardActionMenu[i].active)
			{
				CusKeyboardActionItems.curpos = i;
				break;
			}

	VW_UpdateScreen();
	MenuFadeIn();
}

////////////////////////
//
// DRAW KEYBOARD ACTION CONTROLS SCREEN
//
void DrawKeyboardMoreActionCtlScreen(void)
{
	int i;
	int which = 0;

	ClearMScreen();
	WindowX = 0;
	WindowW = 320;
	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);
	DrawStripes(10);
	VWB_DrawPic(80, -scalingOffsetY, C_CUSTOMIZEPIC);

	//
	// MOUSE
	//
	SETFONTCOLOR(READCOLOR, BKGDCOLOR);
	WindowX = scalingOffsetX;
	WindowW = 320;

#ifdef SPEAR
	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
	PrintY = OPT_MOUSE_Y;
	US_CPrint("Action keyr 2/2");
#endif

	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);

	DrawWindow(OPT_KEYBOARD_MORE_ACTION_X - 8, OPT_KEYBOARD_MORE_ACTION_Y - 5, OPT_KEYBOARD_MORE_ACTION_W, OPT_KEYBOARD_MORE_ACTION_H - 13, BKGDCOLOR);
	DrawMenuGun(&CusKeyboardMoreActionItems);

	DrawMenu(&CusKeyboardMoreActionItems, CtlKeyboardMoreActionMenu);
	DrawMoreActionsKeys(0);

	WaitKeyUp();
	US_Print("\n");

	//
	// PICK STARTING POINT IN MENU
	//
	if (CusKeyboardMoreActionItems.curpos < 0)
		for (i = 0; i < CusKeyboardMoreActionItems.amount; i++)
			if (CtlKeyboardMoreActionMenu[i].active)
			{
				CusKeyboardMoreActionItems.curpos = i;
				break;
			}

	VW_UpdateScreen();
	MenuFadeIn();
}

#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)

////////////////////////
//
// DRAW CUSTOM CONTROLS SCREEN
//
void DrawCustomCtlScreen(void)
{
	int i;
	int which = 0;

	ClearMScreen();
	WindowX = 0;
	WindowW = 320;
	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);
	DrawStripes(10);
	VWB_DrawPic(80, -scalingOffsetY, C_CUSTOMIZEPIC);

	//
	// MOUSE
	//
	SETFONTCOLOR(READCOLOR, BKGDCOLOR);
	WindowX = 0;
	WindowW = 320;

#ifndef SPEAR
	//PrintY = OPT_MOUSE_Y;
	//US_CPrint("Mouse\n");
#else
	PrintY = CST_Y + 13;
	VWB_DrawPic(128, 48, C_KEYBOARDPIC);
#endif

	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);

	DrawWindow(CUS_CTL_X - 8, CUS_CTL_Y, CUS_CTL_W, CUS_CTL_H, BKGDCOLOR);
	DrawMenuGun(&CusCtlItems);

	DrawMenu(&CusCtlItems, CusCtlMenu);
	DrawCustomCtlKeys(0);

	WaitKeyUp();
	US_Print("\n");

	//
	// PICK STARTING POINT IN MENU
	//
	if (CusCtlItems.curpos < 0)
		for (i = 0; i < CusCtlItems.amount; i++)
			if (CusCtlMenu[i].active)
			{
				CusCtlItems.curpos = i;
				break;
			}

	VW_UpdateScreen();
	MenuFadeIn();
}

#endif

////////////////////////
//
// DRAW JOYSTICK SCREEN
//
void DrawJoystickScreen(void)
{
	int i;
	int which = 0;

	ClearMScreen();
	WindowX = 0;
	WindowW = 320;
	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);
	DrawStripes(10);
	VWB_DrawPic(80, -scalingOffsetY, C_CUSTOMIZEPIC);

	//
	// MOUSE
	//
	SETFONTCOLOR(READCOLOR, BKGDCOLOR);
	WindowX = 0;
	WindowW = 320;

#ifndef SPEAR
	//PrintY = OPT_MOUSE_Y;
	//US_CPrint("Mouse\n");
#else
	PrintY = CST_Y + 13;
	VWB_DrawPic(40, 48, C_JOYSTICKPIC);
#endif

	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);

	DrawWindow(OPT_JOYSTICK_X - 8, OPT_JOYSTICK_Y - 5, OPT_JOYSTICK_W, OPT_JOYSTICK_H, BKGDCOLOR);
	DrawMenuGun(&CusJoystickItems);

	DrawMenu(&CusJoystickItems, CtlJoystickMenu);
	DrawCustJoy(0);

	WaitKeyUp();
	US_Print("\n");

	//
	// PICK STARTING POINT IN MENU
	//
	if (CusJoystickItems.curpos < 0)
		for (i = 0; i < CusJoystickItems.amount; i++)
			if (CtlJoystickMenu[i].active)
			{
				CusJoystickItems.curpos = i;
				break;
			}

	VW_UpdateScreen();
	MenuFadeIn();
}

#else

////////////////////////
//
// DRAW CUSTOMIZE SCREEN
//
void DrawCustomScreen(void)
{
	int i;
	int cstStart = CST_START + scalingOffsetX;

#ifdef JAPAN
	VWB_DrawPic(0, 0, S_CUSTOMPIC);
	fontnumber = 1;

	PrintX = CST_START;
	PrintY = CST_Y + 26;
	DrawCustMouse(0);

	PrintX = CST_START;
	US_Print("\n\n\n");
	DrawCustJoy(0);

	PrintX = CST_START;
	US_Print("\n\n\n");
	DrawCustKeybd(0);

	PrintX = CST_START;
	US_Print("\n\n\n");
	DrawCustKeys(0);
#else
	ClearMScreen();
	WindowX = scalingOffsetX;
	WindowW = 320;
	VWB_DrawPic(112, 184 + scalingOffsetY, C_MOUSELBACKPIC);
	DrawStripes(10);
	VWB_DrawPic(80, -scalingOffsetY, C_CUSTOMIZEPIC);

	//
	// MOUSE
	//
	SETFONTCOLOR(READCOLOR, BKGDCOLOR);
	WindowX = scalingOffsetX;
	WindowW = 320;

#ifndef SPEAR
	PrintY = CST_Y + scalingOffsetY;
	US_CPrint("Mouse\n");
#else
	PrintY = CST_Y + scalingOffsetY + 13;
	VWB_DrawPic(128, 48, C_MOUSEPIC);
#endif

	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
#ifdef SPANISH
	PrintX = CST_START - 16;
	US_Print(STR_CRUN);
	PrintX = CST_START - 16 + CST_SPC * 1;
	US_Print(STR_COPEN);
	PrintX = CST_START - 16 + CST_SPC * 2;
	US_Print(STR_CFIRE);
	PrintX = CST_START - 16 + CST_SPC * 3;
	US_Print(STR_CSTRAFE "\n");
#else
	PrintX = cstStart;
	US_Print(STR_CRUN);
	PrintX = cstStart + CST_SPC * 1;
	US_Print(STR_COPEN);
	PrintX = cstStart + CST_SPC * 2;
	US_Print(STR_CFIRE);
	PrintX = cstStart + CST_SPC * 3;
	US_Print(STR_CSTRAFE "\n");
#endif
	DrawWindow(5, PrintY - 1 - scalingOffsetY, 310, 13, BKGDCOLOR);
	DrawCustMouse(0);
	US_Print("\n");

	//
	// JOYSTICK/PAD
	//
#ifndef SPEAR
	SETFONTCOLOR(READCOLOR, BKGDCOLOR);
	US_CPrint("Joystick/Gravis GamePad\n");
#else
	PrintY += 13;
	VWB_DrawPic(40, 88, C_JOYSTICKPIC);
#endif

#ifdef SPEAR
	VWB_DrawPic(112, 120, C_KEYBOARDPIC);
#endif

	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
#ifdef SPANISH
	PrintX = CST_START - 16;
	US_Print(STR_CRUN);
	PrintX = CST_START - 16 + CST_SPC * 1;
	US_Print(STR_COPEN);
	PrintX = CST_START - 16 + CST_SPC * 2;
	US_Print(STR_CFIRE);
	PrintX = CST_START - 16 + CST_SPC * 3;
	US_Print(STR_CSTRAFE "\n");
#else
	PrintX = cstStart;
	US_Print(STR_CRUN);
	PrintX = cstStart + CST_SPC * 1;
	US_Print(STR_COPEN);
	PrintX = cstStart + CST_SPC * 2;
	US_Print(STR_CFIRE);
	PrintX = cstStart + CST_SPC * 3;
	US_Print(STR_CSTRAFE "\n");
#endif
	DrawWindow(5, PrintY - 1 - scalingOffsetY, 310, 13, BKGDCOLOR);
	DrawCustJoy(0);
	US_Print("\n");

	//
	// KEYBOARD
	//
#ifndef SPEAR
	SETFONTCOLOR(READCOLOR, BKGDCOLOR);
	US_CPrint("Keyboard\n");
#else
	PrintY += 13;
#endif
	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
#ifdef SPANISH
	PrintX = CST_START - 16;
	US_Print(STR_CRUN);
	PrintX = CST_START - 16 + CST_SPC * 1;
	US_Print(STR_COPEN);
	PrintX = CST_START - 16 + CST_SPC * 2;
	US_Print(STR_CFIRE);
	PrintX = CST_START - 16 + CST_SPC * 3;
	US_Print(STR_CSTRAFE "\n");
#else
	PrintX = cstStart;
	US_Print(STR_CRUN);
	PrintX = cstStart + CST_SPC * 1;
	US_Print(STR_COPEN);
	PrintX = cstStart + CST_SPC * 2;
	US_Print(STR_CFIRE);
	PrintX = cstStart + CST_SPC * 3;
	US_Print(STR_CSTRAFE "\n");
#endif
	DrawWindow(5, PrintY - 1 - scalingOffsetY, 310, 13, BKGDCOLOR);
	DrawCustKeybd(0);
	US_Print("\n");

	//
	// KEYBOARD MOVE KEYS
	//
	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
#ifdef SPANISH
	PrintX = 4;
	US_Print(STR_LEFT);
	US_Print("/");
	US_Print(STR_RIGHT);
	US_Print("/");
	US_Print(STR_FRWD);
	US_Print("/");
	US_Print(STR_BKWD "\n");
#else
	PrintX = cstStart;
	US_Print(STR_LEFT);
	PrintX = cstStart + CST_SPC * 1;
	US_Print(STR_RIGHT);
	PrintX = cstStart + CST_SPC * 2;
	US_Print(STR_FRWD);
	PrintX = cstStart + CST_SPC * 3;
	US_Print(STR_BKWD "\n");
#endif
	DrawWindow(5, PrintY - 1 - scalingOffsetY, 310, 13, BKGDCOLOR);
	DrawCustKeys(0);
#endif
	//
	// PICK STARTING POINT IN MENU
	//
	if (CusItems.curpos < 0)
		for (i = 0; i < CusItems.amount; i++)
			if (CusMenu[i].active)
			{
				CusItems.curpos = i;
				break;
			}

	VW_UpdateScreen();
	MenuFadeIn();
}

#endif

void PrintCustMouse(int i)
{
	int j;

	for (j = 0; j < 4; j++)
		if (order[i] == buttonmouse[j])
		{
#ifndef USE_MODERN_CONTROLS
			PrintX = CST_START + scalingOffsetX + CST_SPC * i;
#else
			PrintX = CTL_MOUSE_X + scalingOffsetX;
			PrintY = CST_START + scalingOffsetY + (CST_SPC_Y * i);
#endif
			US_Print(mbarray[j]);
			break;
		}
}

void DrawCustMouse(int highlight)
{
	int i, color;

	color = TEXTCOLOR;
	if (highlight)
		color = HIGHLIGHT;
	SETFONTCOLOR(color, BKGDCOLOR);

	if (!mouseenabled)
	{
		SETFONTCOLOR(DEACTIVE, BKGDCOLOR);
		CtlMouseMenu[0].active = 0;
	}
	else
		CtlMouseMenu[0].active = 1;

#ifndef USE_MODERN_CONTROLS
	PrintY = CST_Y + scalingOffsetY + 13 * 2;
	PrintX = CST_START + scalingOffsetX;
#else
	PrintX = CTL_MOUSE_X + scalingOffsetX;
#endif
	for (i = 0; i < 4; i++)
		PrintCustMouse(i);
}

#ifdef USE_MODERN_CONTROLS
void PrintCustJoy(int i)
{
	int j;

	for (j = 0; j < 4; j++)
	{
		if (order[i] == buttoncontroller[j])
		{
#ifndef USE_MODERN_CONTROLS
			PrintX = CST_START + CST_SPC * i;
			US_Print(mbarray[j]);
#else
			PrintX = CTL_MOUSE_X + scalingOffsetX;
			PrintY = CST_START + scalingOffsetY + (CST_SPC_Y * i);
			US_Print(jbarray[j]);
#endif
			break;
		}
	}
}
void DrawCustJoy(int hilight)
{
	int i, color;

	color = TEXTCOLOR;
	if (hilight)
		color = HIGHLIGHT;
	SETFONTCOLOR(color, BKGDCOLOR);

#ifdef USE_MODERN_CONTROLS
	if (!controllerEnabled)
	{
		SETFONTCOLOR(DEACTIVE, BKGDCOLOR);
		CusMenu[3].active = 0;
	}
	else
		CusMenu[3].active = 1;
#else
	if (!joystickenabled)
	{
		SETFONTCOLOR(DEACTIVE, BKGDCOLOR);
		CusMenu[3].active = 0;
	}
	else
		CusMenu[3].active = 1;
#endif	

#ifndef USE_MODERN_CONTROLS
	PrintY = CST_Y + 13 * 5 + scalingOffsetY;
#else
	PrintX = CTL_MOUSE_X + scalingOffsetX;
#endif

	for (i = 0; i < 4; i++)
		PrintCustJoy(i);
}
#else
void PrintCustJoy(int i)
{
	int j;

	for (j = 0; j < 4; j++)
	{
		if (order[i] == buttonjoy[j])
		{
#ifndef USE_MODERN_CONTROLS
			PrintX = CST_START + scalingOffsetX + CST_SPC * i;
			US_Print(mbarray[j]);
#else
			PrintX = CTL_MOUSE_X;
			PrintY = CST_START + (CST_SPC_Y * i);
			US_Print(jbarray[j]);
#endif
			break;
		}
	}
}

void DrawCustJoy(int hilight)
{
	int i, color;

	color = TEXTCOLOR;
	if (hilight)
		color = HIGHLIGHT;
	SETFONTCOLOR(color, BKGDCOLOR);

#ifdef USE_MODERN_CONTROLS
	if (!controllerEnabled)
	{
		SETFONTCOLOR(DEACTIVE, BKGDCOLOR);
		CusMenu[3].active = 0;
	}
	else
		CusMenu[3].active = 1;
#else
	if (!joystickenabled)
	{
		SETFONTCOLOR(DEACTIVE, BKGDCOLOR);
		CusMenu[3].active = 0;
	}
	else
		CusMenu[3].active = 1;
#endif	

#ifndef USE_MODERN_CONTROLS
	PrintY = CST_Y + scalingOffsetY + 13 * 5;
	PrintX = CST_START + scalingOffsetX;
#else
	PrintX = CTL_MOUSE_X;
#endif

	for (i = 0; i < 4; i++)
		PrintCustJoy(i);
}
#endif

void PrintCustKeybd(int i)
{
#ifndef USE_MODERN_CONTROLS
	PrintX = CST_START + scalingOffsetX + CST_SPC * i;
#else
	PrintX = CTL_MOUSE_X + scalingOffsetX;
	PrintY = CST_START + scalingOffsetY + (CST_SPC_Y * i);
#endif
	US_Print((const char*)IN_GetScanName(buttonscan[order[i]]));
}

void DrawCustKeybd(int hilight)
{
	int i, color;

	color = TEXTCOLOR;
	if (hilight)
		color = HIGHLIGHT;
	SETFONTCOLOR(color, BKGDCOLOR);
#ifndef USE_MODERN_CONTROLS
	PrintY = CST_Y + scalingOffsetY + 13 * 8;
	PrintX = CST_START + scalingOffsetX;
#else
	PrintX = CTL_MOUSE_X + scalingOffsetX;
#endif
	for (i = 0; i < 4; i++)
		PrintCustKeybd(i);
}

void PrintCustKeys(int i)
{
#ifndef USE_MODERN_CONTROLS
	PrintX = CST_START + scalingOffsetX + CST_SPC * i;
#else
	PrintX = OPT_KB_MOVE_KEYS_X + scalingOffsetX;
	PrintY = OPT_KB_MOVE_KEYS_Y + scalingOffsetY + (CST_SPC_Y * i);
#endif
	US_Print((const char*)IN_GetScanName(dirscan[moveorder[i]]));
}

void DrawCustKeys(int hilight)
{
	int i, color;

	color = TEXTCOLOR;
	if (hilight)
		color = HIGHLIGHT;
	SETFONTCOLOR(color, BKGDCOLOR);

	int amount = 4;

#ifdef USE_MODERN_CONTROLS
	PrintX = CTL_MOUSE_X + scalingOffsetX;
	amount = 6;
#else
	PrintY = CST_Y + scalingOffsetY + 13 * 10;
	PrintX = CST_START + scalingOffsetX;
#endif
	for (i = 0; i < amount; i++)
		PrintCustKeys(i);
}

#ifdef USE_MODERN_CONTROLS
void PrintMoreActionsKeys(int i)
{
	PrintX = OPT_KEYBOARD_MORE_ACTION_RIGHT_TEXT_X + scalingOffsetX;
	//PrintY = CST_START + (CST_SPC_Y * i);
	PrintY = OPT_KEYBOARD_MORE_ACTION_TEXT_Y + scalingOffsetY + (CST_SPC_Y * i);
	US_Print((const char*)IN_GetScanName(buttonscan[4 + actionorder[i]]));
}

void DrawMoreActionsKeys(int hilight)
{
	int i, color;

	color = TEXTCOLOR;
	if (hilight)
		color = HIGHLIGHT;
	SETFONTCOLOR(color, BKGDCOLOR);

	PrintX = CTL_MOUSE_X + scalingOffsetX;

	for (i = 0; i < 6; i++)
		PrintMoreActionsKeys(i);
}

#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)

void PrintCustomCtlKeys(int i)
{
	PrintX = CUS_CTL_RIGHT_TEXT_X + scalingOffsetX;
	//PrintY = CST_START + (CST_SPC_Y * i);
	PrintY = CUS_CTL_TEXT_Y + scalingOffsetY + (CST_SPC_Y * i);
	US_Print((const char*)IN_GetScanName(buttonscan[19 + advorder[i]]));
}

void DrawCustomCtlKeys(int hilight)
{
	int i, color;

	color = TEXTCOLOR;
	if (hilight)
		color = HIGHLIGHT;
	SETFONTCOLOR(color, BKGDCOLOR);

	PrintX = CTL_MOUSE_X + scalingOffsetX;

	for (i = 0; i < MAX_CUSTOM_CONTROLS; i++)
		PrintCustomCtlKeys(i);
}

#endif
#endif

////////////////////////////////////////////////////////////////////
//
// CHANGE SCREEN VIEWING SIZE
//
////////////////////////////////////////////////////////////////////
int CP_ChangeView(int blank)
{
	int exit = 0, oldview, newview;
	ControlInfo ci;

	WindowX = WindowY = 0;
	WindowW = 320;
	WindowH = 200;
	newview = oldview = viewsize;
	DrawChangeView(oldview);
	MenuFadeIn();

	do
	{
		CheckPause();
		SDL_Delay(5);
		ReadAnyControl(&ci);
		switch (ci.dir)
		{
		case dir_South:
		case dir_West:
			newview--;
			if (newview < 4)
				newview = 4;
			if (newview >= 19)
				DrawChangeView(newview);
			else
				ShowViewSize(newview);
			VW_UpdateScreen();
			SD_PlaySound(HITWALLSND);
			TicDelay(10);
			break;

		case dir_North:
		case dir_East:
			newview++;
			if (newview >= 21)
			{
				newview = 21;
				DrawChangeView(newview);
			}
			else
				ShowViewSize(newview);
			VW_UpdateScreen();
			SD_PlaySound(HITWALLSND);
			TicDelay(10);
			break;
		}

		if (ci.button0 || Keyboard(sc_Enter))
			exit = 1;
		else if (ci.button1 || Keyboard(sc_Escape))
		{
			SD_PlaySound(ESCPRESSEDSND);
			MenuFadeOut();
			if (screenHeight % 200 != 0)
				VL_ClearScreen(0);
			return 0;
		}
	} while (!exit);

	if (oldview != newview)
	{
		SD_PlaySound(SHOOTSND);
		Message(STR_THINK "...");
		NewViewSize(newview);
	}

	ShootSnd();
	MenuFadeOut();
	if (screenHeight % 200 != 0)
		VL_ClearScreen(0);

	return 0;
}

/////////////////////////////
//
// DRAW THE CHANGEVIEW SCREEN
//
void DrawChangeView(int view)
{
	if (view != 21)
		VWB_Bar(0, rescaledHeight - 40, rescaledWidth, 40, bordercol);

#ifdef JAPAN
	VWB_DrawPic(0, 0, S_CHANGEPIC);

	ShowViewSize(view);
#else
	ShowViewSize(view);

	PrintY = (screenHeight / scaleFactor) - 39;
	WindowX = scalingOffsetX;
	WindowY = 320; // TODO: Check this!
	SETFONTCOLOR(HIGHLIGHT, BKGDCOLOR);

	US_CPrint(STR_SIZE1 "\n");
	US_CPrint(STR_SIZE2 "\n");
	US_CPrint(STR_SIZE3);
#endif
	VW_UpdateScreen();
}

////////////////////////////////////////////////////////////////////
//
// QUIT THIS INFERNAL GAME!
//
////////////////////////////////////////////////////////////////////
int CP_Quit(int blank)
{
#ifdef JAPAN
	if (GetYorN(7, 11, C_QUITMSGPIC))
#else

#ifdef SPANISH
	if (Confirm(ENDGAMESTR))
#else
	if (Confirm(endStrings[US_RndT() & 0x7 + (US_RndT() & 1)]))
#endif

#endif
	{
		VW_UpdateScreen();
		SD_MusicOff();
		SD_StopSound();
		MenuFadeOut();
		Quit(NULL);
	}

	DrawMainMenu();
	return 0;
}

////////////////////////////////////////////////////////////////////
//
// HANDLE INTRO SCREEN (SYSTEM CONFIG)
//
////////////////////////////////////////////////////////////////////
void IntroScreen(void)
{
#ifdef SPEAR

#define MAINCOLOR 0x4f
#define EMSCOLOR 0x4f
#define XMSCOLOR 0x4f

#else

#define MAINCOLOR 0x6c
#define EMSCOLOR 0x6c // 0x4f
#define XMSCOLOR 0x6c // 0x7f

#endif
#define FILLCOLOR 14

	//      long memory;
	//      long emshere,xmshere;
	int i;
	/*      int ems[10]={100,200,300,400,500,600,700,800,900,1000},
					xms[10]={100,200,300,400,500,600,700,800,900,1000};
			int main[10]={32,64,96,128,160,192,224,256,288,320};*/

			//
			// DRAW MAIN MEMORY
			//
#ifdef ABCAUS
	memory = (1023l + mminfo.nearheap + mminfo.farheap) / 1024l;
	for (i = 0; i < 10; i++)
		if (memory >= main[i])
			VWB_Bar(49, 163 - 8 * i, 6, 5, MAINCOLOR - i);

	//
	// DRAW EMS MEMORY
	//
	if (EMSPresent)
	{
		emshere = 4l * EMSPagesAvail;
		for (i = 0; i < 10; i++)
			if (emshere >= ems[i])
				VWB_Bar(89, 163 - 8 * i, 6, 5, EMSCOLOR - i);
	}

	//
	// DRAW XMS MEMORY
	//
	if (XMSPresent)
	{
		xmshere = 4l * XMSPagesAvail;
		for (i = 0; i < 10; i++)
			if (xmshere >= xms[i])
				VWB_Bar(129, 163 - 8 * i, 6, 5, XMSCOLOR - i);
	}
#else
	for (i = 0; i < 10; i++)
		VWB_Bar(49 + scalingOffsetX, (163 - 8 * i) + scalingOffsetY, 6, 5, MAINCOLOR - i);
	for (i = 0; i < 10; i++)
		VWB_Bar(89 + scalingOffsetX, (163 - 8 * i) + scalingOffsetY, 6, 5, EMSCOLOR - i);
	for (i = 0; i < 10; i++)
		VWB_Bar(129 + scalingOffsetX, (163 - 8 * i) + scalingOffsetY, 6, 5, XMSCOLOR - i);
#endif

	//
	// FILL BOXES
	//
	if (MousePresent)
		VWB_Bar(164 + scalingOffsetX, 82 + scalingOffsetY, 12, 2, FILLCOLOR);

#ifdef USE_MODERN_CONTROLS
	if (IN_ControllerPresent())
		VWB_Bar(164 + scalingOffsetX, 105 + scalingOffsetY, 12, 2, FILLCOLOR);
#else
	if (IN_JoyPresent())
		VWB_Bar(164 + scalingOffsetX, 105 + scalingOffsetY, 12, 2, FILLCOLOR);
#endif

#ifndef VIEASM
	if (AdLibPresent && !SoundBlasterPresent)
#endif	
		VWB_Bar(164 + scalingOffsetX, 128 + scalingOffsetY, 12, 2, FILLCOLOR);

#ifndef VIEASM
	if (SoundBlasterPresent)
#endif	
		VWB_Bar(164 + scalingOffsetX, 151 + scalingOffsetY, 12, 2, FILLCOLOR);

	//    if (SoundSourcePresent)
	//        VWB_Bar (164, 174, 12, 2, FILLCOLOR);
}

////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////
//
// SUPPORT ROUTINES
//
////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////
//
// Clear Menu screens to dark red
//
////////////////////////////////////////////////////////////////////
void ClearMScreen(void)
{
#ifndef SPEAR
	VWB_BarScaledCoord(0, 0, screenWidth, screenHeight, BORDCOLOR);
#else
	VWB_BarScaledCoord(0, 0, screenWidth, screenHeight, 0);
	VWB_DrawPic(0, 0, C_BACKDROPPIC);
#endif
}

////////////////////////////////////////////////////////////////////
//
// Draw a window for a menu
//
////////////////////////////////////////////////////////////////////
void DrawWindow(int x, int y, int w, int h, int wcolor)
{
	VWB_Bar(x + scalingOffsetX, y + scalingOffsetY, w, h, wcolor);
	DrawOutline(x, y, w, h, BORD2COLOR, DEACTIVE);
}

void DrawOutline(int x, int y, int w, int h, int color1, int color2)
{
	VWB_Hlin(x + scalingOffsetX, x + w + scalingOffsetX, y + scalingOffsetY, color2);
	VWB_Vlin(y + scalingOffsetY, y + h + scalingOffsetY, x + scalingOffsetX, color2);
	VWB_Hlin(x + scalingOffsetX, x + w + scalingOffsetX, y + h + scalingOffsetY, color1);
	VWB_Vlin(y + scalingOffsetY, y + h + scalingOffsetY, x + w + scalingOffsetX, color1);
}

////////////////////////////////////////////////////////////////////
//
// Setup Control Panel stuff - graphics, etc.
//
////////////////////////////////////////////////////////////////////
void SetupControlPanel(void)
{
	//
	// CACHE SOUNDS
	//
	SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
	fontnumber = 1;
	WindowH = 200;
	if (screenHeight % 200 != 0)
		VL_ClearScreen(0);

	if (!ingame)
		CA_LoadAllSounds();
	else
		MainMenu[savegame].active = 1;

	//
	// CENTER MOUSE
	//
	if (IN_IsInputGrabbed())
		IN_CenterMouse();
}

////////////////////////////////////////////////////////////////////
//
// SEE WHICH SAVE GAME FILES ARE AVAILABLE & READ STRING IN
//
////////////////////////////////////////////////////////////////////
void SetupSaveGames()
{
	int i;
	char name[13];
	char savepath[300];

	strcpy(name, SaveName);
	for (i = 0; i < 10; i++)
	{
		name[7] = '0' + i;

		if (configdir[0])
			snprintf(savepath, sizeof(savepath), "%s/%s", configdir, name);
		else
			strcpy(savepath, name);

		const int handle = open(savepath, O_RDONLY | O_BINARY);
		if (handle >= 0)
		{
			char temp[32];

			SaveGamesAvail[i] = 1;
			read(handle, temp, 32);
			close(handle);
			strcpy(&SaveGameNames[i][0], temp);
		}
	}
}

////////////////////////////////////////////////////////////////////
//
// Clean up all the Control Panel stuff
//
////////////////////////////////////////////////////////////////////
void CleanupControlPanel(void)
{
	fontnumber = 0;
}

////////////////////////////////////////////////////////////////////
//
// Handle moving gun around a menu
//
////////////////////////////////////////////////////////////////////
int HandleMenu(CP_iteminfo* item_i, CP_itemtype* items, void (*routine)(int w))
{
	char key;
	static int redrawitem = 1, lastitem = -1;
	int i, x, y, basey, exit, which, shape;
	int32_t lastBlinkTime, timer;
	ControlInfo ci;

	which = item_i->curpos;
	x = item_i->x & -8;
	basey = item_i->y - 2;
	y = basey + which * 13;

	VWB_DrawPic(x, y, C_CURSOR1PIC);
	SetTextColor(items + which, 1);
	if (redrawitem)
	{
		PrintX = item_i->x + item_i->indent + scalingOffsetX;
		PrintY = item_i->y + which * 13 + scalingOffsetY;
		US_Print((items + which)->string);
	}
	//
	// CALL CUSTOM ROUTINE IF IT IS NEEDED
	//
	if (routine)
		routine(which);
	VW_UpdateScreen();

	shape = C_CURSOR1PIC;
	timer = 8;
	exit = 0;
	lastBlinkTime = GetTimeCount();
	IN_ClearKeysDown();

	do
	{
		//
		// CHANGE GUN SHAPE
		//
		if ((int32_t)GetTimeCount() - lastBlinkTime > timer)
		{
			lastBlinkTime = GetTimeCount();
			if (shape == C_CURSOR1PIC)
			{
				shape = C_CURSOR2PIC;
				timer = 8;
			}
			else
			{
				shape = C_CURSOR1PIC;
				timer = 70;
			}
			VWB_DrawPic(x, y, shape);
			if (routine)
				routine(which);
			VW_UpdateScreen();
		}
		else
			SDL_Delay(5);

		CheckPause();

		//
		// SEE IF ANY KEYS ARE PRESSED FOR INITIAL CHAR FINDING
		//
		key = LastASCII;
		if (key)
		{
			int ok = 0;

			if (key >= 'a')
				key -= 'a' - 'A';

			for (i = which + 1; i < item_i->amount; i++)
				if ((items + i)->active && (items + i)->string[0] == key)
				{
					EraseGun(item_i, items, x, y, which);
					which = i;
					DrawGun(item_i, items, x, &y, which, basey, routine);
					ok = 1;
					IN_ClearKeysDown();
					break;
				}

			//
			// DIDN'T FIND A MATCH FIRST TIME THRU. CHECK AGAIN.
			//
			if (!ok)
			{
				for (i = 0; i < which; i++)
					if ((items + i)->active && (items + i)->string[0] == key)
					{
						EraseGun(item_i, items, x, y, which);
						which = i;
						DrawGun(item_i, items, x, &y, which, basey, routine);
						IN_ClearKeysDown();
						break;
					}
			}
		}

		//
		// GET INPUT
		//
		ReadAnyControl(&ci);
		switch (ci.dir)
		{
			////////////////////////////////////////////////
			//
			// MOVE UP
			//
		case dir_North:

			EraseGun(item_i, items, x, y, which);

			//
			// ANIMATE HALF-STEP
			//
			if (which && (items + which - 1)->active)
			{
				y -= 6;
				DrawHalfStep(x, y);
			}

			//
			// MOVE TO NEXT AVAILABLE SPOT
			//
			do
			{
				if (!which)
					which = item_i->amount - 1;
				else
					which--;
			} while (!(items + which)->active);

			DrawGun(item_i, items, x, &y, which, basey, routine);
			//
			// WAIT FOR BUTTON-UP OR DELAY NEXT MOVE
			//
			TicDelay(20);
			break;

			////////////////////////////////////////////////
			//
			// MOVE DOWN
			//
		case dir_South:

			EraseGun(item_i, items, x, y, which);
			//
			// ANIMATE HALF-STEP
			//
			if (which != item_i->amount - 1 && (items + which + 1)->active)
			{
				y += 6;
				DrawHalfStep(x, y);
			}

			do
			{
				if (which == item_i->amount - 1)
					which = 0;
				else
					which++;
			} while (!(items + which)->active);

			DrawGun(item_i, items, x, &y, which, basey, routine);

			//
			// WAIT FOR BUTTON-UP OR DELAY NEXT MOVE
			//
			TicDelay(20);
			break;
		}

		if (ci.button0 || Keyboard(sc_Space) || Keyboard(sc_Enter))
			exit = 1;

		if (ci.button1 && !Keyboard(sc_Alt) || Keyboard(sc_Escape))
			exit = 2;

	} while (!exit);

	IN_ClearKeysDown();

	//
	// ERASE EVERYTHING
	//
	if (lastitem != which)
	{
		VWB_Bar(x - 1 + scalingOffsetX, y + scalingOffsetY, 25, 16, BKGDCOLOR);
		PrintX = item_i->x + item_i->indent + scalingOffsetX;
		PrintY = item_i->y + which * 13 + scalingOffsetY;
		US_Print((items + which)->string);
		redrawitem = 1;
	}
	else
		redrawitem = 0;

	if (routine)
		routine(which);
	VW_UpdateScreen();

	item_i->curpos = which;

	lastitem = which;
	switch (exit)
	{
	case 1:
		//
		// CALL THE ROUTINE
		//
		if ((items + which)->routine != NULL)
		{
			ShootSnd();
			MenuFadeOut();
			(items + which)->routine(0);
		}
		return which;

	case 2:
		SD_PlaySound(ESCPRESSEDSND);
		return -1;
	}

	return 0; // JUST TO SHUT UP THE ERROR MESSAGES!
}

//
// ERASE GUN & DE-HIGHLIGHT STRING
//
void EraseGun(CP_iteminfo* item_i, CP_itemtype* items, int x, int y, int which)
{
	VWB_Bar(x - 1 + scalingOffsetX, y + scalingOffsetY, 25, 16, BKGDCOLOR);
	SetTextColor(items + which, 0);

	PrintX = item_i->x + item_i->indent + scalingOffsetX;
	PrintY = item_i->y + which * 13 + scalingOffsetY;
	US_Print((items + which)->string);
	VW_UpdateScreen();
}

//
// DRAW HALF STEP OF GUN TO NEXT POSITION
//
void DrawHalfStep(int x, int y)
{
	VWB_DrawPic(x, y, C_CURSOR1PIC);
	VW_UpdateScreen();
	SD_PlaySound(MOVEGUN1SND);
	SDL_Delay(8 * 100 / 7);
}

//
// DRAW GUN AT NEW POSITION
//
void DrawGun(CP_iteminfo* item_i, CP_itemtype* items, int x, int* y, int which, int basey,
	void (*routine)(int w))
{
	VWB_Bar(x - 1 + scalingOffsetX, *y + scalingOffsetY, 25, 16, BKGDCOLOR);
	*y = basey + which * 13;
	VWB_DrawPic(x, *y, C_CURSOR1PIC);
	SetTextColor(items + which, 1);

	PrintX = item_i->x + item_i->indent + scalingOffsetX;
	PrintY = item_i->y + which * 13 + scalingOffsetY;
	US_Print((items + which)->string);

	//
	// CALL CUSTOM ROUTINE IF IT IS NEEDED
	//
	if (routine)
		routine(which);
	VW_UpdateScreen();
	SD_PlaySound(MOVEGUN2SND);
}

////////////////////////////////////////////////////////////////////
//
// DELAY FOR AN AMOUNT OF TICS OR UNTIL CONTROLS ARE INACTIVE
//
////////////////////////////////////////////////////////////////////
void TicDelay(int count)
{
	ControlInfo ci;

	int32_t startTime = GetTimeCount();
	do
	{
		SDL_Delay(5);
		ReadAnyControl(&ci);
	} while ((int32_t)GetTimeCount() - startTime < count && ci.dir != dir_None);
}

////////////////////////////////////////////////////////////////////
//
// Draw a menu
//
////////////////////////////////////////////////////////////////////
void DrawMenu(CP_iteminfo* item_i, CP_itemtype* items)
{
	int i, which = item_i->curpos;

	WindowX = PrintX = item_i->x + item_i->indent + scalingOffsetX;
	WindowY = PrintY = item_i->y + scalingOffsetY;
	WindowW = 320;
	WindowH = 200;

	for (i = 0; i < item_i->amount; i++)
	{
		SetTextColor(items + i, which == i);

		PrintY = item_i->y + i * 13 + scalingOffsetY;
		if ((items + i)->active)
			US_Print((items + i)->string);
		else
		{
			SETFONTCOLOR(DEACTIVE, BKGDCOLOR);
			US_Print((items + i)->string);
			SETFONTCOLOR(TEXTCOLOR, BKGDCOLOR);
		}

		US_Print("\n");
	}
}

////////////////////////////////////////////////////////////////////
//
// SET TEXT COLOR (HIGHLIGHT OR NO)
//
////////////////////////////////////////////////////////////////////
void SetTextColor(CP_itemtype* items, int hlight)
{
	if (hlight)
	{
		SETFONTCOLOR(color_hlite[items->active], BKGDCOLOR);
	}
	else
	{
		SETFONTCOLOR(color_norml[items->active], BKGDCOLOR);
	}
}

////////////////////////////////////////////////////////////////////
//
// WAIT FOR CTRLKEY-UP OR BUTTON-UP
//
////////////////////////////////////////////////////////////////////
void WaitKeyUp(void)
{
	ControlInfo ci;
	while (ReadAnyControl(&ci), ci.button0 |
		ci.button1 |
		ci.button2 | ci.button3 | Keyboard(sc_Space) | Keyboard(sc_Enter) |
		Keyboard(sc_Escape))
	{
		IN_WaitAndProcessEvents();
	}
}

////////////////////////////////////////////////////////////////////
//
// READ KEYBOARD, JOYSTICK AND MOUSE FOR INPUT
//
////////////////////////////////////////////////////////////////////
static int totalMousex = 0, totalMousey = 0;

void ReadAnyControl(ControlInfo* ci)
{
	int mouseactive = 0;

	IN_ReadControl(0, ci);

	if (mouseenabled && IN_IsInputGrabbed())
	{
		int mousex, mousey, buttons;
#if SDL_MAJOR_VERSION == 1
		buttons = SDL_GetMouseState(&mousex, &mousey);
		mousex -= screenWidth / 2;
		mousey -= screenHeight / 2;
		IN_CenterMouse();
#else
		buttons = SDL_GetRelativeMouseState(&mousex, &mousey);
#endif
		int middlePressed = buttons & SDL_BUTTON(SDL_BUTTON_MIDDLE);
		int rightPressed = buttons & SDL_BUTTON(SDL_BUTTON_RIGHT);
		buttons &= ~(SDL_BUTTON(SDL_BUTTON_MIDDLE) | SDL_BUTTON(SDL_BUTTON_RIGHT));
		if (middlePressed)
			buttons |= 1 << 2;
		if (rightPressed)
			buttons |= 1 << 1;

		totalMousex += mousex;
		totalMousey += mousey;

		if (totalMousey < -SENSITIVE)
		{
			ci->dir = dir_North;
			mouseactive = 1;
		}
		else if (totalMousey > SENSITIVE)
		{
			ci->dir = dir_South;
			mouseactive = 1;
		}

		if (totalMousex < -SENSITIVE)
		{
			ci->dir = dir_West;
			mouseactive = 1;
		}
		else if (totalMousex > SENSITIVE)
		{
			ci->dir = dir_East;
			mouseactive = 1;
		}

		if (mouseactive)
		{
			totalMousex = 0;
			totalMousey = 0;
		}

		if (buttons)
		{
			ci->button0 = buttons & 1;
			ci->button1 = buttons & 2;
			ci->button2 = buttons & 4;
			ci->button3 = false;
			mouseactive = 1;
		}
	}

#ifdef USE_MODERN_CONTROLS
	if (controllerEnabled && !mouseactive)
	{
		int a0x, a0y;
		int a1x, a1y;

		int a, b, c, d;
		int gcb;

		IN_GetGameControllerDelta(&a0x, &a0y, &a1x, &a1y);

		if (a0y < -CONTROLLER_DEAD_ZONE)
			ci->dir = dir_North;
		else if (a0y > CONTROLLER_DEAD_ZONE)
			ci->dir = dir_South;
		if (a0x < -CONTROLLER_DEAD_ZONE)
			ci->dir = dir_West;
		else if (a0x > CONTROLLER_DEAD_ZONE)
			ci->dir = dir_East;

		gcb = IN_GameControllerButtons();
		if (gcb)
		{
			ci->button0 = gcb & 1;
			ci->button1 = gcb & 2;
			ci->button2 = gcb & 4;
			ci->button3 = gcb & 8;
		}
	}
#else
	if (joystickenabled && !mouseactive)
	{
		int jx, jy, jb;

		IN_GetJoyDelta(&jx, &jy);
		if (jy < -SENSITIVE)
			ci->dir = dir_North;
		else if (jy > SENSITIVE)
			ci->dir = dir_South;

		if (jx < -SENSITIVE)
			ci->dir = dir_West;
		else if (jx > SENSITIVE)
			ci->dir = dir_East;

		jb = IN_JoyButtons();
		if (jb)
		{
			ci->button0 = jb & 1;
			ci->button1 = jb & 2;
			ci->button2 = jb & 4;
			ci->button3 = jb & 8;
		}
	}
#endif
}

////////////////////////////////////////////////////////////////////
//
// DRAW DIALOG AND CONFIRM YES OR NO TO QUESTION
//
////////////////////////////////////////////////////////////////////
int Confirm(const char* string)
{
	int xit = 0, x, y, tick = 0, lastBlinkTime;
	int whichsnd[2] = { ESCPRESSEDSND, SHOOTSND };
	ControlInfo ci;

	Message(string);
	IN_ClearKeysDown();
	WaitKeyUp();

	//
	// BLINK CURSOR
	//
	x = PrintX;
	y = PrintY;
	lastBlinkTime = GetTimeCount();

	do
	{
		ReadAnyControl(&ci);

		if (GetTimeCount() - lastBlinkTime >= 10)
		{
			switch (tick)
			{
			case 0:
				VWB_Bar(x, y, 8, 13, TEXTCOLOR);
				break;
			case 1:
				PrintX = x;
				PrintY = y;
				US_Print("_");
			}
			VW_UpdateScreen();
			tick ^= 1;
			lastBlinkTime = GetTimeCount();
		}
		else
			SDL_Delay(5);

#ifdef SPANISH
	} while (!Keyboard(sc_S) && !Keyboard(sc_N) && !Keyboard(sc_Escape));
#else
	} while (!Keyboard(sc_Y) && !Keyboard(sc_N) && !Keyboard(sc_Escape) && !ci.button0 && !ci.button1);
#endif

#ifdef SPANISH
	if (Keyboard(sc_S) || ci.button0)
	{
		xit = 1;
		ShootSnd();
	}
#else
	if (Keyboard(sc_Y) || ci.button0)
	{
		xit = 1;
		ShootSnd();
	}
#endif

	IN_ClearKeysDown();
	WaitKeyUp();

	SD_PlaySound((soundnames)whichsnd[xit]);

	return xit;
}

#ifdef JAPAN
////////////////////////////////////////////////////////////////////
//
// DRAW MESSAGE & GET Y OR N
//
////////////////////////////////////////////////////////////////////
int GetYorN(int x, int y, int pic)
{
	int xit = 0;
	soundnames whichsnd[2] = { ESCPRESSEDSND, SHOOTSND };

	VWB_DrawPic(x * 8, y * 8, pic);
	VW_UpdateScreen();
	IN_ClearKeysDown();

	do
	{
		IN_WaitAndProcessEvents();
	}
#ifdef SPANISH
	while (!Keyboard(sc_S) && !Keyboard(sc_N) && !Keyboard(sc_Escape));
#else
	while (!Keyboard(sc_Y) && !Keyboard(sc_N) && !Keyboard(sc_Escape));
#endif

#ifdef SPANISH
	if (Keyboard(sc_S))
	{
		xit = 1;
		ShootSnd();
	}

	while (Keyboard(sc_S) || Keyboard(sc_N) || Keyboard(sc_Escape))
		IN_WaitAndProcessEvents();

#else

	if (Keyboard(sc_Y))
	{
		xit = 1;
		ShootSnd();
	}

	while (Keyboard(sc_Y) || Keyboard(sc_N) || Keyboard(sc_Escape))
		IN_WaitAndProcessEvents();
#endif

	IN_ClearKeysDown();
	SD_PlaySound(whichsnd[xit]);
	return xit;
}
#endif

////////////////////////////////////////////////////////////////////
//
// PRINT A MESSAGE IN A WINDOW
//
////////////////////////////////////////////////////////////////////
void Message(const char* string)
{
	int h = 0, w = 0, mw = 0, i, len = (int)strlen(string);
	fontstruct* font;

	fontnumber = 1;
	font = (fontstruct*)grsegs[STARTFONT + fontnumber];
	h = font->height;
	for (i = 0; i < len; i++)
	{
		if (string[i] == '\n')
		{
			if (w > mw)
				mw = w;
			w = 0;
			h += font->height;
		}
		else
			w += font->width[string[i]];
	}

	if (w + 10 > mw)
		mw = w + 10;

	PrintY = (WindowH / 2) - h / 2 + scalingOffsetY;
	PrintX = WindowX = 160 - mw / 2 + scalingOffsetX;

	DrawWindow(WindowX - 5 - scalingOffsetX, PrintY - 5 - scalingOffsetY, mw + 10, h + 10, TEXTCOLOR);
	DrawOutline(WindowX - 5 - scalingOffsetX, PrintY - 5 - scalingOffsetY, mw + 10, h + 10, 0, HIGHLIGHT);
	SETFONTCOLOR(0, TEXTCOLOR);
	US_Print(string);
	VW_UpdateScreen();
}

////////////////////////////////////////////////////////////////////
//
// THIS MAY BE FIXED A LITTLE LATER...
//
////////////////////////////////////////////////////////////////////
static int lastmusic;

int StartCPMusic(int song)
{
	int lastoffs;

	lastmusic = song;
	lastoffs = SD_MusicOff();

#ifndef VIEASM
	UNCACHEAUDIOCHUNK(STARTMUSIC + lastmusic);
#endif	

	SD_StartMusic(STARTMUSIC + song);
	return lastoffs;
}

void FreeMusic(void)
{
#ifndef VIEASM
	UNCACHEAUDIOCHUNK(STARTMUSIC + lastmusic);
#endif	
}

///////////////////////////////////////////////////////////////////////////
//
//      IN_GetScanName() - Returns a string containing the name of the
//              specified scan code
//
///////////////////////////////////////////////////////////////////////////
const char*
IN_GetScanName(ScanCode scan)
{
	/*    const char **p;
		ScanCode *s;

		for (s = ExtScanCodes, p = ExtScanNames; *s; p++, s++)
			if (*s == scan)
				return (*p);*/

	switch (scan)
	{
	case (SDLK_BACKSPACE):
		return "BkSp";
	case (SDLK_TAB):
		return "Tab";
	case (SDLK_RETURN):
		return "Enter";
	case (SDLK_PAUSE):
		return "Pause";
	case (SDLK_ESCAPE):
		return "Esc";
	case (SDLK_SPACE):
		return "Space";
	case (SDLK_EXCLAIM):
		return "!";
	case (SDLK_QUOTEDBL):
		return "\"";
	case (SDLK_HASH):
		return "#";
	case (SDLK_DOLLAR):
		return "$";
	case (SDLK_AMPERSAND):
		return "&";
	case (SDLK_QUOTE):
		return "'";
	case (SDLK_LEFTPAREN):
		return "(";
	case (SDLK_RIGHTPAREN):
		return ")";
	case (SDLK_ASTERISK):
		return "*";
	case (SDLK_PLUS):
		return "+";
	case (SDLK_COMMA):
		return ",";
	case (SDLK_MINUS):
		return "-";
	case (SDLK_PERIOD):
		return ".";
	case (SDLK_SLASH):
		return "/";
	case (SDLK_0):
		return "0";
	case (SDLK_1):
		return "1";
	case (SDLK_2):
		return "2";
	case (SDLK_3):
		return "3";
	case (SDLK_4):
		return "4";
	case (SDLK_5):
		return "5";
	case (SDLK_6):
		return "6";
	case (SDLK_7):
		return "7";
	case (SDLK_8):
		return "8";
	case (SDLK_9):
		return "9";
	case (SDLK_COLON):
		return ":";
	case (SDLK_SEMICOLON):
		return ";";
	case (SDLK_LESS):
		return "<";
	case (SDLK_EQUALS):
		return "=";
	case (SDLK_GREATER):
		return ">";
	case (SDLK_QUESTION):
		return "?";
	case (SDLK_AT):
		return "@";
	case (SDLK_a):
		return "A";
	case (SDLK_b):
		return "B";
	case (SDLK_c):
		return "C";
	case (SDLK_d):
		return "D";
	case (SDLK_e):
		return "E";
	case (SDLK_f):
		return "F";
	case (SDLK_g):
		return "G";
	case (SDLK_h):
		return "H";
	case (SDLK_i):
		return "I";
	case (SDLK_j):
		return "J";
	case (SDLK_k):
		return "K";
	case (SDLK_l):
		return "L";
	case (SDLK_m):
		return "M";
	case (SDLK_n):
		return "N";
	case (SDLK_o):
		return "O";
	case (SDLK_p):
		return "P";
	case (SDLK_q):
		return "Q";
	case (SDLK_r):
		return "R";
	case (SDLK_s):
		return "S";
	case (SDLK_t):
		return "T";
	case (SDLK_u):
		return "U";
	case (SDLK_v):
		return "V";
	case (SDLK_w):
		return "W";
	case (SDLK_x):
		return "X";
	case (SDLK_y):
		return "Y";
	case (SDLK_z):
		return "Z";
	case (SDLK_LEFTBRACKET):
		return "[";
	case (SDLK_BACKSLASH):
		return "\\";
	case (SDLK_RIGHTBRACKET):
		return "]";
	case (SDLK_CARET):
		return "^";
	case (SDLK_UNDERSCORE):
		return "_";
	case (SDLK_BACKQUOTE):
		return "`";
	case (SDLK_UP):
		return "Up";
	case (SDLK_DOWN):
		return "Down";
	case (SDLK_RIGHT):
		return "Right";
	case (SDLK_LEFT):
		return "Left";
	case (SDLK_INSERT):
		return "Ins";
	case (SDLK_HOME):
		return "Home";
	case (SDLK_END):
		return "End";
	case (SDLK_PAGEUP):
		return "PgUp";
	case (SDLK_PAGEDOWN):
		return "PgDn";
	case (SDLK_DELETE):
		return "Del";
	case (SDLK_F1):
		return "F1";
	case (SDLK_F2):
		return "F2";
	case (SDLK_F3):
		return "F3";
	case (SDLK_F4):
		return "F4";
	case (SDLK_F5):
		return "F5";
	case (SDLK_F6):
		return "F6";
	case (SDLK_F7):
		return "F7";
	case (SDLK_F8):
		return "F8";
	case (SDLK_F9):
		return "F9";
	case (SDLK_F10):
		return "F10";
	case (SDLK_F11):
		return "F11";
	case (SDLK_F12):
		return "F12";
	case (SDLK_NUMLOCKCLEAR):
		return "NumLk";
	case (SDLK_CAPSLOCK):
		return "CapsLk";
	case (SDLK_SCROLLLOCK):
		return "ScrlLk";
	case (SDLK_RSHIFT):
		return "RShft";
	case (SDLK_LSHIFT):
		return "Shift";
	case (SDLK_RCTRL):
		return "RCtrl";
	case (SDLK_LCTRL):
		return "Ctrl";
	case (SDLK_RALT):
		return "RAlt";
	case (SDLK_LALT):
		return "Alt";
	case (SDLK_PRINTSCREEN):
		return "PrtSc";
	case (SDLK_KP_0):
		return "KP 0";
	case (SDLK_KP_1):
		return "KP 1";
	case (SDLK_KP_2):
		return "KP 2";
	case (SDLK_KP_3):
		return "KP 3";
	case (SDLK_KP_4):
		return "KP 4";
	case (SDLK_KP_5):
		return "KP 5";
	case (SDLK_KP_6):
		return "KP 6";
	case (SDLK_KP_7):
		return "KP 7";
	case (SDLK_KP_8):
		return "KP 8";
	case (SDLK_KP_9):
		return "KP 9";
	case (SDLK_KP_DIVIDE):
		return "KP /";
	case (SDLK_KP_MULTIPLY):
		return "KP *";
	case (SDLK_KP_MINUS):
		return "KP -";
	case (SDLK_KP_PLUS):
		return "KP +";
	case (SDLK_KP_PERIOD):
		return "KP .";
	default:
		return "?";
	}
}

///////////////////////////////////////////////////////////////////////////
//
// CHECK FOR PAUSE KEY (FOR MUSIC ONLY)
//
///////////////////////////////////////////////////////////////////////////
void CheckPause(void)
{
	if (Paused)
	{
		switch (SoundStatus)
		{
		case 0:
			SD_MusicOn();
			break;
		case 1:
			SD_MusicOff();
			break;
		}

		SoundStatus ^= 1;
		VW_WaitVBL(3);
		IN_ClearKeysDown();
		Paused = false;
	}
}

///////////////////////////////////////////////////////////////////////////
//
// DRAW GUN CURSOR AT CORRECT POSITION IN MENU
//
///////////////////////////////////////////////////////////////////////////
void DrawMenuGun(CP_iteminfo* iteminfo)
{
	int x, y;

	x = iteminfo->x;
	y = iteminfo->y + iteminfo->curpos * 13 - 2;
	VWB_DrawPic(x, y, C_CURSOR1PIC);
}

///////////////////////////////////////////////////////////////////////////
//
// DRAW SCREEN TITLE STRIPES
//
///////////////////////////////////////////////////////////////////////////
void DrawStripes(int y)
{
#ifndef SPEAR
	VWB_Bar(0, y, rescaledWidth, 24, 0);
	VWB_Hlin(0, rescaledWidth - 1, y + 22, STRIPE);
#else
	VWB_Bar(0, y, 320, 22, 0);
	VWB_Hlin(0, 319, y + 23, 0);
#endif
}

void ShootSnd(void)
{
	SD_PlaySound(SHOOTSND);
}

#ifdef USE_MODERN_CONTROLS
void CheckKeyConflict(void)
{
	int i = 0;
	int arrayEnd = 0;

	for (i = 0; i < 8; i++)
		if (buttonscan[order[i]] == LastScan)
			buttonscan[order[i]] = bt_nobutton;

	arrayEnd = 0;

	for (i = 0; i < 6; i++)
		if (dirscan[moveorder[i]] == LastScan)
			dirscan[moveorder[i]] = bt_nobutton;

	arrayEnd = 0;

	//TODO DemolitionDerby - Finish
	arrayEnd = MORE_ACTIONS_ARRAY_END - MORE_ACTIONS_ARRAY_START;

	for (i = 0; i < arrayEnd; i++)
		if (buttonscan[4 + actionorder[i]] == LastScan)
			buttonscan[4 + actionorder[i]] = bt_nobutton;

	arrayEnd = 0;

#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)
	arrayEnd = CUS_CTL_ARRAY_RANGE_END - CUS_CTL_ARRAY_RANGE_START;

	for (i = 0; i < arrayEnd; i++)
		if (buttonscan[19 + advorder[i]] == LastScan)
			buttonscan[19 + advorder[i]] = bt_nobutton;

	arrayEnd = 0;
#endif
}
#endif

///////////////////////////////////////////////////////////////////////////
//
// CHECK FOR EPISODES
//
///////////////////////////////////////////////////////////////////////////
void CheckForEpisodes(void)
{
	struct stat statbuf;

	// On Linux like systems, the configdir defaults to $HOME/.wolf4sdl
#if !defined(_WIN32)
	if (configdir[0] == 0)
	{
		// Set config location to home directory for multi-user support
		char* homedir = getenv("HOME");
		if (homedir == NULL)
		{
			Quit("Your $HOME directory is not defined. You must set this before playing.");
		}
#define WOLFDIR "/.wolf4sdl"
		if (strlen(homedir) + sizeof(WOLFDIR) > sizeof(configdir))
		{
			Quit("Your $HOME directory path is too long. It cannot be used for saving games.");
		}
		snprintf(configdir, sizeof(configdir), "%s" WOLFDIR, homedir);
	}
#endif

	if (configdir[0] != 0)
	{
		// Ensure config directory exists and create if necessary
		if (stat(configdir, &statbuf) != 0)
		{
#ifdef _WIN32
			if (_mkdir(configdir) != 0)
#else
			if (mkdir(configdir, 0755) != 0)
#endif
			{
				Quit("The configuration directory \"%s\" could not be created.", configdir);
			}
		}
	}

	//
	// JAPANESE VERSION
	//
#ifdef JAPAN
#ifdef JAPDEMO
	if (!stat("vswap.wj1", &statbuf))
	{
		strcpy(extension, "wj1");
#else
	if (!stat("vswap.wj6", &statbuf))
	{
		strcpy(extension, "wj6");
#endif
		strcat(configname, extension);
		strcat(SaveName, extension);
		strcat(demoname, extension);
		EpisodeSelect[1] =
			EpisodeSelect[2] = EpisodeSelect[3] = EpisodeSelect[4] = EpisodeSelect[5] = 1;
	}
	else
		Quit("NO JAPANESE WOLFENSTEIN 3-D DATA FILES to be found!");
	strcpy(graphext, extension);
	strcpy(audioext, extension);
#else

//
// ENGLISH
//
#ifdef UPLOAD
	if (!stat("vswap.wl1", &statbuf))
		strcpy(extension, "wl1");
	else
		Quit("NO WOLFENSTEIN 3-D DATA FILES to be found!");
#else
#ifndef SPEAR
#ifndef SEAMLESSLEVELS
	if (!stat("vswap.wl6", &statbuf))
	{
		strcpy(extension, "wl6");
		NewEmenu[2].active =
			NewEmenu[4].active =
			NewEmenu[6].active =
			NewEmenu[8].active =
			NewEmenu[10].active =
			EpisodeSelect[1] =
			EpisodeSelect[2] = EpisodeSelect[3] = EpisodeSelect[4] = EpisodeSelect[5] = 1;

	}
	else
	{
		if (!stat("vswap.wl3", &statbuf))
		{
			strcpy(extension, "wl3");
			NewEmenu[2].active = NewEmenu[4].active = EpisodeSelect[1] = EpisodeSelect[2] = 1;
		}
		else
		{
			if (!stat("vswap.wl1", &statbuf))
			{
				strcpy(extension, "wl1");
			}
			else
				Quit("NO WOLFENSTEIN 3-D DATA FILES to be found!");
		}
	}
#else
	if (!stat("vswap.wl6", &statbuf))
	{
		strcpy(extension, "wl6");
	}
	else
		Quit("NO WOLFENSTEIN 3-D DATA FILES to be found!");
#endif
#endif
#endif

#ifdef SPEAR
#ifndef SPEARDEMO
	if (param_mission == 0)
	{
		if (!stat("vswap.sod", &statbuf))
			strcpy(extension, "sod");
		else
			Quit("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
	}
	else if (param_mission == 1)
	{
		if (!stat("vswap.sd1", &statbuf))
			strcpy(extension, "sd1");
		else
			Quit("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
	}
	else if (param_mission == 2)
	{
		if (!stat("vswap.sd2", &statbuf))
			strcpy(extension, "sd2");
		else
			Quit("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
	}
	else if (param_mission == 3)
	{
		if (!stat("vswap.sd3", &statbuf))
			strcpy(extension, "sd3");
		else
			Quit("NO SPEAR OF DESTINY DATA FILES TO BE FOUND!");
	}
	else
		Quit("UNSUPPORTED MISSION!");
	strcpy(graphext, "sod");
	strcpy(audioext, "sod");
#else
	if (!stat("vswap.sdm", &statbuf))
	{
		strcpy(extension, "sdm");
	}
	else
		Quit("NO SPEAR OF DESTINY DEMO DATA FILES TO BE FOUND!");
	strcpy(graphext, "sdm");
	strcpy(audioext, "sdm");
#endif
#else
	strcpy(graphext, extension);
#ifndef VIEASM
	strcpy(audioext, extension);
#endif
#endif

	strcat(configname, extension);
	strcat(SaveName, extension);
	strcat(demoname, extension);

#ifdef USE_READTHIS
	strcat(helpfilename, extension);
#endif
	strcat(endfilename, extension);
#endif
}
