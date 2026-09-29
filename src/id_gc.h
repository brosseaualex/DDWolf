//
//	ID Tech 0
//	ID_GC.h - Header file for Game Controller Input Manager
//	v1.0 - September 2026
//	By Alexandre Brosseau (DemolitionDerby)
//	Developed for DDWolf
// 

// 
//	Feel free to use this, in part or in its entirety,
//	however you want : )
//
//	The only thing that I ask is that you leave this
//	header here or give proper credits if you only
//	use parts of it.
//
//	This system should be fairly easy to implement in
//	your own project, I tried to make it as easy
//	to copy / paste as possible.
// 
//	It should also be generic enough to be easily
//	ported to any ID Tech 0 variants.
//
//	Everything is contained mostly inside the
//	ENABLE_GAME_CONTROLLER definition.
//
//	This manager is only compatible with SDL2.
//	
//	-- EOF o7
//

#ifndef __ID_GC_H_
#define __ID_GC_H_

#include "wl_def.h"

#if ENABLE_GAME_CONTROLLER

#define TRIGGER_THRESHOLD	16000		// Analog trigger press depth (~50% on a XBox One Controller)
#define STICK_THRESHOLD		16000		// Stick deflection threshold for discrete action checks
#define DPAD_MAX_DELTA		32767

//===========================================================================

// Do not change anything here, in order to change the default mapping, edit gcDefaultBindings
#define sc_gc_NoButton					-1
#define sc_gc_Btn_A						SDL_CONTROLLER_BUTTON_A					// PS = Cross
#define sc_gc_Btn_B						SDL_CONTROLLER_BUTTON_B					// PS = Circle
#define sc_gc_Btn_X						SDL_CONTROLLER_BUTTON_X					// PS = Square
#define sc_gc_Btn_Y						SDL_CONTROLLER_BUTTON_Y					// PS = Triangle
#define sc_gc_Btn_Back					SDL_CONTROLLER_BUTTON_BACK
#define sc_gc_Btn_Guide					SDL_CONTROLLER_BUTTON_GUIDE
#define sc_gc_Btn_Start					SDL_CONTROLLER_BUTTON_START
#define sc_gc_Left_Stick				SDL_CONTROLLER_BUTTON_LEFTSTICK			// Stick Click
#define sc_gc_Right_Stick				SDL_CONTROLLER_BUTTON_RIGHTSTICK		// Stick Click
#define sc_gc_Left_Shoulder				SDL_CONTROLLER_BUTTON_LEFTSHOULDER
#define sc_gc_Right_Shoulder			SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
#define sc_gc_DPad_Up					SDL_CONTROLLER_BUTTON_DPAD_UP
#define sc_gc_DPad_Down					SDL_CONTROLLER_BUTTON_DPAD_DOWN
#define sc_gc_DPad_Left					SDL_CONTROLLER_BUTTON_DPAD_LEFT
#define sc_gc_DPad_Right				SDL_CONTROLLER_BUTTON_DPAD_RIGHT
#define sc_gc_Share_Micro				SDL_CONTROLLER_BUTTON_MISC1				// Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button
#define sc_gc_Btn_Paddle_1				SDL_CONTROLLER_BUTTON_PADDLE1			// Xbox Elite paddle P1 (upper left, facing the back)
#define sc_gc_Btn_Paddle_2				SDL_CONTROLLER_BUTTON_PADDLE2			// Xbox Elite paddle P3 (upper right, facing the back)
#define sc_gc_Btn_Paddle_3				SDL_CONTROLLER_BUTTON_PADDLE3			// Xbox Elite paddle P2 (lower left, facing the back)
#define sc_gc_Btn_Paddle_4				SDL_CONTROLLER_BUTTON_PADDLE4			// Xbox Elite paddle P4 (lower right, facing the back)
#define sc_gc_Touchpad					SDL_CONTROLLER_BUTTON_TOUCHPAD			// PS4/PS5 touchpad button
#define sc_gc_Axis_Left_Trigger			100										// SDL_CONTROLLER_AXIS_TRIGGERLEFT but value needs to be changed since it conflicts with buttons
#define sc_gc_Axis_Right_Trigger		101										// SDL_CONTROLLER_AXIS_TRIGGERRIGHT

typedef enum
{
	gc_nobutton = -1,
	gc_attack = 0,
	gc_use,
	gc_run,
	gc_strafe,
	gc_nextweapon,
	gc_prevweapon,
	gc_forward,
	gc_backward,
	gc_turnleft,
	gc_turnright,
	gc_straferight,
	gc_strafeleft,
	gc_weapon1,
	gc_weapon2,
	gc_weapon3,
	gc_weapon4,
#ifdef OVERHEAD_MAP
	gc_automap,
#endif
	gc_esc,
	gc_pause,
	gc_NUMBUTTONS
}GameControllerAction;

typedef enum ControllerType
{
	CT_XBOX,
	CT_PLAYSTATION,
	CT_NINTENDO,
	CT_GENERIC
} ControllerType;

typedef struct
{
	int a0X, a0Y;
	int a1X, a1Y;
} ControllerDelta;

typedef struct
{
	int dirX;
	int dirY;
	bool buttonA;
	bool buttonB;
	bool buttonX;
	bool buttonY;
} GcMenuState;

//===========================================================================

extern SDL_GameController* GameController;
extern SDL_JoystickID gcId;
extern float gcTurnSensitivity;
extern float gcMaxTurnSensitivity;
extern int gcBindings[gc_NUMBUTTONS];
extern bool gcHotplugDirty;

//===========================================================================

//
// Game controller default bindings
// 
// The game will set these binding if no config.* file exists.
// Once the config.* file has been created, it uses what is saved
// and this has no effect unless config.* is deleted and recreated.
//
static const int gcDefaultBindings[gc_NUMBUTTONS] = {
		sc_gc_Axis_Right_Trigger,					// gc_attack
		sc_gc_Btn_A,								// gc_use
		sc_gc_Btn_X,								// gc_run
		sc_gc_Btn_B,								// gc_strafe
		sc_gc_Left_Shoulder,						// gc_prevweapon
		sc_gc_Right_Shoulder,						// gc_nextweapon
		sc_gc_DPad_Up,								// gc_forward
		sc_gc_DPad_Down,							// gc_backward
		sc_gc_DPad_Left,							// gc_turnleft
		sc_gc_DPad_Right,							// gc_turnright
		sc_gc_NoButton,								// gc_straferight
		sc_gc_NoButton,								// gc_strafeleft
		sc_gc_NoButton,								// gc_weapon1
		sc_gc_NoButton,								// gc_weapon2
		sc_gc_NoButton,								// gc_weapon3
		sc_gc_NoButton,								// gc_weapon4
#ifdef OVERHEAD_MAP
		sc_gc_Axis_Left_Trigger,					// gc_automap
#endif	
		sc_gc_Btn_Back,								// gc_esc
		sc_gc_Btn_Start								// gc_pause
#define sc_gc_NoButton								-1
};

//===========================================================================

//
// Forbidden action buttons
// Any buttons in this list will be prevented to be mapped to an action
//
static const int gcRemapForbiddenButtons[] = {
	SDL_CONTROLLER_BUTTON_GUIDE,
	SDL_CONTROLLER_BUTTON_START,
	SDL_CONTROLLER_BUTTON_BACK,
};
static const int g_numBlockedGcButtons = sizeof(gcRemapForbiddenButtons) / sizeof(gcRemapForbiddenButtons[0]);

//===========================================================================

void GC_InitGameController(void);
boolean GC_IsPresent();
GcMenuState GC_GetMenuState(void);
ControllerType GC_GetControllerType(void);
ControllerDelta GC_GetDelta(void);
void GC_InitDefaultBindings(void);
const char* GC_GetScanName(int);
int GC_GetButtons(void);
int GC_EnterCtrlData(void);
void GC_PollMove(void);
void GC_PollActions(void);
bool GC_CheckEdge(bool, bool*);
bool GC_IsActionPressed(GameControllerAction, bool);
bool GC_GetButton(SDL_GameControllerButton);
void GC_ForceReleaseAllButtons(void);
bool GC_IsButtonForbidden(int);
void GC_ProcessEvents(const SDL_Event*);
void GC_IntroHotplugEvents(void);
void GC_CleanHotplugEvents(boolean[]);
void GC_PollMenuInputs(int*, bool*, bool*, bool*, bool*);
int ClampInt(int, int, int);
#endif
#endif