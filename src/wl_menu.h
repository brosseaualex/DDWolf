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

#define SENSITIVE 60
#define CENTERX ((int)screenWidth / 2)
#define CENTERY ((int)screenHeight / 2)

#define MENU_X 76
#define MENU_Y 55
#define MENU_W 178
#ifndef USE_MODERN_CONTROLS
#ifdef USE_READTHIS
#define MENU_H 13 * 8 + 6
#else
#define MENU_H 13 * 7 + 6
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

#define CTL_Y 72

#ifdef USE_MODERN_CONTROLS
#define CTL_X 38
#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)
#define CTL_H 13 * 8 + 8
#else
#define CTL_H 13 * 7 + 8
#endif
#define CTL_W 250
#else
#define CTL_X 24
#define CTL_H 13 * 4 + 8
#define CTL_W 284
#endif

#define OPT_X 90
#define OPT_Y 86
#define OPT_W 152
#ifdef SHOW_ATMOS_OPTIONS
#define OPT_H 13 * 5 + 8
#else
#define OPT_H 13 * 4 + 8
#endif

#ifdef SHOW_ATMOS_OPTIONS
#define ATMOS_X 46
#define ATMOS_Y 84
#define ATMOS_W 244
#define ATMOS_H 13 * 4 + 8
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
#define CST_SPC_Y 13

#define OPT_JOYSTICK_X 41
#define OPT_JOYSTICK_Y 72
#define OPT_JOYSTICK_W 250
#define OPT_JOYSTICK_H 63

#define CTL_MOUSE_X 180

// Resolution
#define RES_MENU_X 44
#define RES_MENU_Y 55
#define RES_MENU_W 250
#define RES_MENU_H 13 * 10 + 8

#define DISPLAY_CTL_X 34
#define DISPLAY_CTL_Y 86
#define DISPLAY_CTL_W 262
#if SDL_MAJOR_VERSION == 2
#define DISPLAY_CTL_H 13 * 6 + 8
#elif SDL_MAJOR_VERSION == 1
#define DISPLAY_CTL_H 13 * 5 + 8
#endif

#define LSM_Y 55
#define LSM_H 10 * 13 + 10
#ifndef SAVE_GAME_SCREENSHOT
#define LSM_X 85
#define LSM_W 175
#else
#define LSM_X	10
#define LSM_W	165
#define LSP_X   184
#define LSP_Y   80
#define LSP_W   128
#define LSP_H   80
#define BMP_SAVE_FILENAME "savegam?.bmp"
#endif

#ifndef USE_MODERN_CONTROLS
#define CST_START 60
#define CST_SPC 60
#else
#define CST_START 72
#define CST_SPC 95

// Mouse
#define OPT_MOUSE_X 26
#define OPT_MOUSE_Y 72
#define OPT_MOUSE_W 284
#define OPT_MOUSE_H 13 * 7 + 8

// Keyboard
#define OPT_KB_MOVE_KEYS_X 190
#define OPT_KB_MOVE_KEYS_Y 65

#define OPT_KB_MOVE_X 41
#define OPT_KB_MOVE_Y 61
#define OPT_KB_MOVE_W 250
#define OPT_KB_MOVE_H 13 * 8 + 8

#define OPT_KB_ACTION_X 41
#define OPT_KB_ACTION_Y 72
#define OPT_KB_ACTION_W 250
#define OPT_KB_ACTION_H 13 * 7 + 8

#define OPT_KB_MORE_ACTION_X 41
#define OPT_KB_MORE_ACTION_Y 60
#define OPT_KB_MORE_ACTION_W 250

#ifndef OVERHEAD_MAP
#define OPT_KB_MORE_ACTION_H 13 * 8 + 8
#else
#define OPT_KB_MORE_ACTION_H 13 * 9 + 8
#endif

#define OPT_KB_MORE_ACTION_TEXT_X 200
#define OPT_KB_MORE_ACTION_TEXT_Y 60

#define OPT_KB_MORE_ACTION_RIGHT_TEXT_X 200

// Game Controller
#define OPT_GC_MOVE_X 41
#define OPT_GC_MOVE_Y 72
#define OPT_GC_MOVE_W 250
#define OPT_GC_MOVE_H 13 * 9 + 8

#define OPT_GC_MOVE_TEXT_X 170
#define OPT_GC_MOVE_TEXT_Y 72

#define OPT_GC_ACTION_X 41
#define OPT_GC_ACTION_Y 72
#define OPT_GC_ACTION_W 250
#define OPT_GC_ACTION_H 13 * 9 + 8

#define OPT_GC_ACTION_TEXT_X 170
#define OPT_GC_ACTION_TEXT_Y 72

#define OPT_GC_MORE_ACTION_X 41
#define OPT_GC_MORE_ACTION_Y 72
#define OPT_GC_MORE_ACTION_W 250

#ifndef OVERHEAD_MAP
#define OPT_GC_MORE_ACTION_H 13 * 6 + 8
#else
#define OPT_GC_MORE_ACTION_H 13 * 7 + 8
#endif

#define OPT_GC_MORE_ACTION_TEXT_X 170
#define OPT_GC_MORE_ACTION_TEXT_Y 72

const int MORE_ACTIONS_ARRAY_START = 5;
#ifndef OVERHEAD_MAP
const int MORE_ACTIONS_ARRAY_END = 11;
#else
const int MORE_ACTIONS_ARRAY_END = 12;
#endif

#if defined(SHOW_CUSTOM_CONTROLS)
#define CUS_CTL_X 26
#define CUS_CTL_Y 50
#define CUS_CTL_W 280
#define CUS_CTL_H 13 * 10 + 8

#define CUS_CTL_TEXT_X 25
#define CUS_CTL_TEXT_Y 55

#define CUS_CTL_RIGHT_TEXT_X 200

const int MAX_CUSTOM_CONTROLS = 10;
const int CUS_CTL_ARRAY_RANGE_START = 19;
const int CUS_CTL_ARRAY_RANGE_END = 29;
#endif
#endif

// Resolution definitions
#define MAX_RESOLUTIONS 64
#define RES_LIST_MAX_VISIBLE 10

enum
{
	DISPLAY_RESOLUTION,
	DISPLAY_FULLSCREEN_EXCLUSIVE,
#if SDL_MAJOR_VERSION == 2
	DISPLAY_FULLSCREEN_BORDERLESS,
	DISPLAY_VSYNC,
#elif SDL_MAJOR_VERSION == 1
	DISPLAY_DOUBLE_BUFFERING,
#endif	
	DISPLAY_APPLY
};

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

//
// atmosphere options
//
#if defined(USE_FLOORCEILINGTEX) || defined(USE_SHADING) || defined(USE_CLOUDSKY) || defined(USE_STARSKY) || defined(USE_RAIN) || defined(USE_SNOW)
extern boolean atmosTexturedEnabled;
extern boolean atmosShadingEnabled;
extern boolean atmosSkyboxEnabled;
extern boolean atmosPrecipitationEnabled;
#endif

//
// FUNCTION PROTOTYPES
//
void ExitToControlScreen(void);

void US_ControlPanel(ScanCode);

void EnableEndGameMenuItem();

void SetupControlPanel(void);
void SetupSaveGames();
void CleanupControlPanel(void);

void ClearMScreen(void);
void WaitKeyUp(void);
void ReadAnyControl(ControlInfo* ci);
void TicDelay(int count);
int StartCPMusic(int song);
int Confirm(const char* string);
void Message(const char* string);
void CheckPause(void);
void ShootSnd(void);
void CheckSecretMissions(void);
void BossKey(void);
void PrintLSEntry(int w, int color);
void TrackWhichGame(int w);
void DrawNewGameDiff(int w);
void FixupCustom(int w);
void CheckForEpisodes(void);
void FreeMusic(void);

void DrawMenu(CP_iteminfo* item_i, CP_itemtype* items);
void DrawWindow(int x, int y, int w, int h, int wcolor);
void DrawOutline(int x, int y, int w, int h, int color1, int color2);
void DrawGun(CP_iteminfo* item_i, CP_itemtype* items, int x, int* y, int which, int basey, void (*routine)(int w));
void DrawHalfStep(int x, int y);
void DrawMenuGun(CP_iteminfo* iteminfo);
void DrawStripes(int y);
void DrawSliderBox(int, int, int, int, int, int, byte);
void EraseGun(CP_iteminfo* item_i, CP_itemtype* items, int x, int y, int which);

int HandleMenu(CP_iteminfo* item_i, CP_itemtype* items, void (*routine)(int w));
int HandleMenu(CP_iteminfo* item_i, CP_itemtype* items, void (*routine)(int w), int totalItems, int* selectedIdx, void (*buildItemsFunc)(void));

void EnterCtrlData(int index, CustomCtrls* cust, void (*DrawRtn)(int), void (*PrintRtn)(int), int type);
void DrawMainMenu(void);
void DrawSoundMenu(void);
void DrawLoadSaveScreen(int loadsave);

void DrawNewEpisode(void);
void DrawNewGame(void);
void DrawChangeView(int view);
void DrawMouseSens(void);
void DrawCtlScreen(void);
void DrawOptScreen(void);
void DrawResolutionMenu(void);
void DrawDisplayOptScreen(void);
void DrawCustomScreen(void);
void DrawLSAction(int which);
void DrawCustMouse(int hilight);
void DrawCustKeybd(int hilight);
void DrawCustKeys(int hilight);
void PrintCustMouse(int i);
void PrintCustKeybd(int i);
void PrintCustKeys(int i);

#ifndef USE_MODERN_CONTROLS
void DefineMouseBtns(void);
void DefineKeyMove(void);
void DefineKeyBtns(void);
void DefineJoyBtns(void);
void PrintCustJoy(int i);
void DrawCustJoy(int hilight);
#endif

int CP_NewGame(int);
int CP_Sound(int);
int CP_LoadGame(int quick);
int CP_SaveGame(int quick);
int CP_Options(int);
int CP_Control(int);

int CP_Resolution(int);
int CP_ChangeView(int);
int CP_Quit(int);
int CP_ViewScores(int);
int CP_EndGame(int);
int CP_CheckQuick(ScanCode scancode);
int MouseSensitivity(int);
int CustomControls(int);

// Resolution helper functions
void Res_AddIfMissing(int w, int h);
void Res_Init(int);
void Res_BuildMenuItems(void);
bool Res_IsDisplayChanged(void);
void Res_RevertDisplay(void);

// Resolution screens
int CP_Resolution(int);
int CP_Display(int);

#if defined(SHOW_ATMOS_OPTIONS) && (defined(USE_FLOORCEILINGTEX) || defined(USE_SHADING) || defined(USE_CLOUDSKY) || defined(USE_STARSKY) || defined(USE_RAIN) || defined(USE_SNOW))
void DrawAtmosOptScreen(void);
int CP_Atmos(int);
#endif

#ifdef USE_MODERN_CONTROLS
void DrawMouseCtlScreen(void);

int CP_MouseCtl(int);
int CP_KbMoveCtl(int);
int CP_KbActionCtl(int);
int CP_KbMoreActionCtl(int);

void CheckKeyConflict(void);

void DrawKbMoveCtlScreen(void);
void DrawKbActionCtlScreen(void);
void DrawKbMoreActionCtlScreen(void);
void DrawKbMoreActionsKeys(int hilight);
void PrintKbMoreActionsKeys(int i);

void DefineMouseBtns(int);
void DefineKbMoveBtns(int);
void DefineKbActionBtns(int);
void DefineJoyBtns(int);

#if SDL_MAJOR_VERSION == 2 && defined(USE_MODERN_CONTROLS)
int CP_GcMoveCtl(int);
int CP_GcActionCtl(int);
int CP_GcMoreActionCtl(int);
int CP_GcTurnSens(int);

void DrawGcMoveCtlScreen(void);
void DrawGcActionCtlScreen(void);
void DrawGcMoreActionCtlScreen(void);
void DrawGcMoveBtns(int);
void DrawGcActionsBtns(int);
void DrawGcMoreActionsBtns(int);
void DrawGcTurnSensScreen(void);

//void GC_PollMenuInputs(ControlInfo* ci);

#ifdef SHOW_CUSTOM_CONTROLS
void DrawCustomCtlScreen(void);
void DrawCustomCtlKeys(int hilight);
void PrintCustomCtlKeys(int i);
int CP_CustomCtl(int);
#endif
#endif

#if SDL_MAJOR_VERSION == 1
void DrawJoystickScreen(void);
int CP_JoystickCtl(int);
void PrintCustJoy(int);
void DrawCustJoy(int);
void DefineJoyBtns(int value);
#endif

#endif

enum
{
	MOUSE,
	JOYSTICK,
	KB_ACTIONS,
	KB_MOVE,
	KB_MORE_ACTIONS,
	KB_CUSTOM_CONTROLS
}; // FOR INPUT TYPES

enum menuitems
{
	newgame,
	loadgame,
	savegame,
	options,
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

void Write(int x, int y, const char* string);
void NonShareware(void);
int GetYorN(int x, int y, int pic);
#endif

#ifdef VIEASM
void DrawSoundVols(bool);
int AdjustVolume(int);
#endif