//
// WL_MENU.H
//
#ifndef __WL_MENU_H_
#define __WL_MENU_H_

#ifdef SPEAR

#define BORDCOLOR 0x99
#define BORD2COLOR 0x93
#define DEACTIVE 0x9b
#define BKGDCOLOR 0x9d
//#define STRIPE                0x9c

#define MenuFadeOut() VL_FadeOut(0, 255, 0, 0, 51, 10)

#else

#define BORDCOLOR 0x29
#define BORD2COLOR 0x23
#define DEACTIVE 0x2b
#define BKGDCOLOR 0x2d
#define STRIPE 0x2c

#define MenuFadeOut() VL_FadeOut(0, 255, 43, 0, 0, 10)

#endif

#define READCOLOR 0x4a
#define READHCOLOR 0x47
#define VIEWCOLOR 0x7f
#define TEXTCOLOR 0x17
#define HIGHLIGHT 0x13
#define MenuFadeIn() VL_FadeIn(0, 255, gamepal, 10)

#define MENUSONG WONDERIN_MUS

#ifndef SPEAR
#define INTROSONG NAZI_NOR_MUS
#else
#define INTROSONG XTOWER2_MUS
#endif

#define CONTROLLER_DEAD_ZONE 8000

#define SENSITIVE 60
#define CENTERX ((int)screenWidth / 2)
#define CENTERY ((int)screenHeight / 2)

#define MENU_X 76
#define MENU_Y 55
#define MENU_W 178
#ifndef USE_MODERN_CONTROLS
#ifdef USE_READTHIS
#define MENU_H 13 * 10 + 6
#else
#define MENU_H 13 * 9 + 6
#endif
#else
#ifdef USE_READTHIS
#define MENU_H 13 * 8 + 6
#else
#define MENU_H 13 * 7 + 6
#endif
#endif

#define SM_X 48
#define SM_W 250

#ifndef VIEASM
#define SM_Y1 20
#define SM_H1 4 * 13 - 7
#define SM_Y2 SM_Y1 + 5 * 13
#define SM_H2 4 * 13 - 7
#define SM_Y3 SM_Y2 + 5 * 13
#define SM_H3 3 * 13 - 7
#else
#define SM_Y1   20
#define SM_H1   3*13-7
#define SM_Y2   SM_Y1+4*13
#define SM_H2   3*13-7
#define SM_Y3   SM_Y2+4*13
#define SM_H3   3*13-7
#endif


#ifdef JAPAN
#define CTL_Y 70
#else
#define CTL_Y 72
#endif
#ifdef USE_MODERN_CONTROLS
#define CTL_X 43
#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)
#define CTL_H 114
#else
#define CTL_H 100
#endif
#define CTL_W 250
#else
#define CTL_X 24
#define CTL_H 60
#define CTL_W 284
#endif

#define OPT_X 64
#ifdef JAPAN
#define OPT_Y 70
#else
#define OPT_Y 86
#endif
#define OPT_W 200

#if defined(USE_MODERN_CONTROLS) && defined(SHOW_GAME_OPTIONS)
#define OPT_H 62
#elif defined(USE_MODERN_CONTROLS) && !defined(SHOW_GAME_OPTIONS)
#define OPT_H 48
#elif !defined(USE_MODERN_CONTROLS) && defined(SHOW_GAME_OPTIONS)
#define OPT_H 34
//#else
//#define OPT_H 48
#endif

#ifndef SAVE_GAME_SCREENSHOT
#define LSM_X 85
#else
#define LSM_X 20
#endif
#define LSM_Y 55
#ifndef SAVE_GAME_SCREENSHOT
#define LSM_W 175
#else
#define LSM_W 165
#endif
#define LSM_H 10 * 13 + 10

#ifdef SAVE_GAME_SCREENSHOT
#define LSP_X   184
#define LSP_Y   80
#define LSP_W   128
#define LSP_H   80
#define BMP_SAVE_FILENAME "savegam?.bmp"
#endif

#define NM_X 50
#define NM_Y 100
#define NM_W 225
#define NM_H 13 * 4 + 15

#define NE_X 10
#define NE_Y 23
#define NE_W 320 - NE_X * 2
#define NE_H 200 - NE_Y * 2

#define CST_X 20
#define CST_Y 48

#ifndef USE_MODERN_CONTROLS
#define CST_START 60
#define CST_SPC 60
#else
#define CST_START 72
#define CST_SPC 95
#endif

#ifdef USE_MODERN_CONTROLS
#define OPT_MOUSE_X 26
#define OPT_MOUSE_Y 72
#define OPT_MOUSE_W 284
#define OPT_MOUSE_H 100

#define OPT_KB_MOVE_KEYS_X 190
#define OPT_KB_MOVE_KEYS_Y 65

#define OPT_KEYBOARD_MOVE_X 41
#define OPT_KEYBOARD_MOVE_Y 61
#define OPT_KEYBOARD_MOVE_W 250
#define OPT_KEYBOARD_MOVE_H 112

#define OPT_KEYBOARD_ACTION_X 41
#define OPT_KEYBOARD_ACTION_Y 72
#define OPT_KEYBOARD_ACTION_W 250
#define OPT_KEYBOARD_ACTION_H 112

#define OPT_KEYBOARD_MORE_ACTION_X 41
#define OPT_KEYBOARD_MORE_ACTION_Y 60
#define OPT_KEYBOARD_MORE_ACTION_W 250
#define OPT_KEYBOARD_MORE_ACTION_H 126

#define OPT_KEYBOARD_MORE_ACTION_TEXT_X 200
#define OPT_KEYBOARD_MORE_ACTION_TEXT_Y 60

#define OPT_KEYBOARD_MORE_ACTION_RIGHT_TEXT_X 200

#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)
#define CUS_CTL_X 26
#define CUS_CTL_Y 50
#define CUS_CTL_W 280
#define CUS_CTL_H 138

#define CUS_CTL_TEXT_X 25
#define CUS_CTL_TEXT_Y 55

#define CUS_CTL_RIGHT_TEXT_X 200
#endif

#define OPT_JOYSTICK_X 41
#define OPT_JOYSTICK_Y 72
#define OPT_JOYSTICK_W 250
#define OPT_JOYSTICK_H 63

#define CST_SPC_Y 13
#define CTL_MOUSE_X 180
#endif

//
// TYPEDEFS
//
typedef struct
{
	short x, y, amount, curpos, indent;
} CP_iteminfo;

typedef struct
{
	short active;
	char string[36];
	int (*routine)(int temp1);
} CP_itemtype;

typedef struct
{
#ifdef USE_MODERN_CONTROLS
	short allowed[14];
#else
	short allowed[4];
#endif
} CustomCtrls;

extern CP_itemtype MainMenu[];
extern CP_iteminfo MainItems;

void ExitToControlScreen(void);

#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)
extern const int MAX_CUSTOM_CONTROLS;
extern const int CUS_CTL_ARRAY_RANGE_START;
extern const int CUS_CTL_ARRAY_RANGE_END;
extern int menuExit;


#endif

//
// FUNCTION PROTOTYPES
//

void US_ControlPanel(ScanCode);

void EnableEndGameMenuItem();

void SetupControlPanel(void);
void SetupSaveGames();
void CleanupControlPanel(void);

void DrawMenu(CP_iteminfo *item_i, CP_itemtype *items);
int HandleMenu(CP_iteminfo *item_i,
			   CP_itemtype *items,
			   void (*routine)(int w));
void ClearMScreen(void);
void DrawWindow(int x, int y, int w, int h, int wcolor);
void DrawOutline(int x, int y, int w, int h, int color1, int color2);
void WaitKeyUp(void);
void ReadAnyControl(ControlInfo *ci);
void TicDelay(int count);
int StartCPMusic(int song);
int Confirm(const char *string);
void Message(const char *string);
void CheckPause(void);
void ShootSnd(void);
void CheckSecretMissions(void);
void BossKey(void);

void DrawGun(CP_iteminfo *item_i, CP_itemtype *items, int x, int *y, int which, int basey, void (*routine)(int w));
void DrawHalfStep(int x, int y);
void EraseGun(CP_iteminfo *item_i, CP_itemtype *items, int x, int y, int which);
void DrawMenuGun(CP_iteminfo *iteminfo);
void DrawStripes(int y);
#ifndef USE_MODERN_CONTROLS
void DefineMouseBtns(void);
void DefineKeyMove(void);
void DefineKeyBtns(void);
void DefineJoyBtns(void);
#else
void DefineMouseBtns(int);
void DefineKeyMove(int);
void DefineKeyBtns(int);
void DefineJoyBtns(int);
#endif

void EnterCtrlData(int index, CustomCtrls *cust, void (*DrawRtn)(int), void (*PrintRtn)(int), int type);

void DrawMainMenu(void);
void DrawSoundMenu(void);
void DrawLoadSaveScreen(int loadsave);
void DrawNewEpisode(void);
void DrawNewGame(void);
void DrawChangeView(int view);
void DrawMouseSens(void);
void DrawCtlScreen(void);
void DrawCustomScreen(void);
void DrawMouseCtlScreen(void);
#ifdef USE_MODERN_CONTROLS
void DrawKeyboardMoveCtlScreen(void);
void DrawKeyboardActionCtlScreen(void);
void DrawKeyboardMoreActionCtlScreen(void);

void CheckKeyConflict(void);

#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)
void DrawCustomCtlScreen(void);
#endif
#endif

void DrawLSAction(int which);
void DrawCustMouse(int hilight);
void DrawCustJoy(int hilight);
void DrawCustKeybd(int hilight);
void PrintCustMouse(int i);
void PrintCustJoy(int i);
void PrintCustKeybd(int i);

void DrawMoreActionsKeys(int hilight);
void PrintMoreActionsKeys(int i);

void DrawCustomCtlKeys(int hilight);
void PrintCustomCtlKeys(int i);

void DrawCustKeys(int hilight);
void PrintCustKeys(int i);

#if defined(USE_MODERN_CONTROLS) || defined(SHOW_GAME_OPTIONS)
void DrawOptScreen(void);
#endif
void DrawJoystickScreen(void);

void PrintLSEntry(int w, int color);
void TrackWhichGame(int w);
void DrawNewGameDiff(int w);
void FixupCustom(int w);

int CP_NewGame(int);
int CP_Sound(int);
int CP_LoadGame(int quick);
int CP_SaveGame(int quick);
int CP_Options(int);
int CP_Control(int);
int CP_ChangeView(int);
int CP_ExitOptions(int);
int CP_Quit(int);
int CP_ViewScores(int);
int CP_EndGame(int);
int CP_CheckQuick(ScanCode scancode);
int Controls(int);
int MouseSensitivity(int);
#ifdef USE_MODERN_CONTROLS
int CP_MouseCtl(int);
int CP_KeyboardMoveCtl(int);
int CP_KeyboardActionCtl(int);
int CP_KeyboardMoreActionCtl(int);
int CP_JoystickCtl(int);
#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)
int CP_CustomCtl(int);
#endif
#else
int CustomControls(int);
#endif

void CheckForEpisodes(void);

void FreeMusic(void);

enum
{
	MOUSE,
	JOYSTICK,
	CONTROLLER,
	KEYBOARDBTNS,
	KEYBOARDMOVE,
	KEYBOARDMOREACTIONS,
	CUSTOMCONTROLS
}; // FOR INPUT TYPES

enum menuitems
{
	newgame,
#ifndef USE_MODERN_CONTROLS
	soundmenu,
	control,
#endif
	loadgame,
	savegame,
#ifndef USE_MODERN_CONTROLS
	changeview,
#else
	options,
#endif

#ifdef USE_READTHIS
	readthis,
#endif
	viewscores,
	backtodemo,
	quit
};

//
// WL_INTER
//
typedef struct
{
	int kill, secret, treasure;
	int32_t time;
} LRstruct;

extern LRstruct LevelRatios[];

void Write(int x, int y, const char *string);
void NonShareware(void);
int GetYorN(int x, int y, int pic);

#endif

#ifdef VIEASM
void DrawSoundVols(bool);
int AdjustVolume(int);
#endif