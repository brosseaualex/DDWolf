//
//	ID Engine
//	ID_IN.h - Header file for Input Manager
//	v1.0d1
//	By Jason Blochowiak
//

#ifndef __ID_IN_H_
#define __ID_IN_H_

#define KEYCOUNT 129

typedef int ScanCode;
#define sc_None 0
#define sc_Bad 0xff
#define sc_Return SDLK_RETURN
#define sc_Enter sc_Return
#define sc_Escape SDLK_ESCAPE
#define sc_Space SDLK_SPACE
#define sc_BackSpace SDLK_BACKSPACE
#define sc_Tab SDLK_TAB
#define sc_Alt SDLK_LALT
#define sc_Control SDLK_LCTRL
#define sc_CapsLock SDLK_CAPSLOCK
#define sc_LShift SDLK_LSHIFT
#define sc_RShift SDLK_RSHIFT
#define sc_UpArrow SDLK_UP
#define sc_DownArrow SDLK_DOWN
#define sc_LeftArrow SDLK_LEFT
#define sc_RightArrow SDLK_RIGHT
#ifdef USE_MODERN_CONTROLS
#define sc_StrafeLeft SDLK_a
#define sc_StrafeRight SDLK_d
#define sc_LeftBracket SDLK_LEFTBRACKET
#define sc_RightBracket SDLK_RIGHTBRACKET
#if defined(USE_MODERN_CONTROLS) && defined(SHOW_CUSTOM_CONTROLS)
#define sc_CusCtl_1 SDLK_KP_0
#define sc_CusCtl_2 SDLK_KP_1
#define sc_CusCtl_3 SDLK_KP_2
#define sc_CusCtl_4 SDLK_KP_3
#define sc_CusCtl_5 SDLK_KP_4
#define sc_CusCtl_6 SDLK_KP_5
#define sc_CusCtl_7 SDLK_KP_6
#define sc_CusCtl_8 SDLK_KP_7
#define sc_CusCtl_9 SDLK_KP_8
#define sc_CusCtl_10 SDLK_KP_9
#endif
#endif
#define sc_Insert SDLK_INSERT
#define sc_Delete SDLK_DELETE
#define sc_Home SDLK_HOME
#define sc_End SDLK_END
#define sc_PgUp SDLK_PAGEUP
#define sc_PgDn SDLK_PAGEDOWN
#define sc_F1 SDLK_F1
#define sc_F2 SDLK_F2
#define sc_F3 SDLK_F3
#define sc_F4 SDLK_F4
#define sc_F5 SDLK_F5
#define sc_F6 SDLK_F6
#define sc_F7 SDLK_F7
#define sc_F8 SDLK_F8
#define sc_F9 SDLK_F9
#define sc_F10 SDLK_F10
#define sc_F11 SDLK_F11
#define sc_F12 SDLK_F12

#define sc_ScrollLock SDLK_SCROLLOCK
#define sc_PrintScreen SDLK_PRINT

#define sc_1 SDLK_1
#define sc_2 SDLK_2
#define sc_3 SDLK_3
#define sc_4 SDLK_4
#define sc_5 SDLK_5
#define sc_6 SDLK_6
#define sc_7 SDLK_7
#define sc_8 SDLK_8
#define sc_9 SDLK_9
#define sc_0 SDLK_0

#define sc_A SDLK_a
#define sc_B SDLK_b
#define sc_C SDLK_c
#define sc_D SDLK_d
#define sc_E SDLK_e
#define sc_F SDLK_f
#define sc_G SDLK_g
#define sc_H SDLK_h
#define sc_I SDLK_i
#define sc_J SDLK_j
#define sc_K SDLK_k
#define sc_L SDLK_l
#define sc_M SDLK_m
#define sc_N SDLK_n
#define sc_O SDLK_o
#define sc_P SDLK_p
#define sc_Q SDLK_q
#define sc_R SDLK_r
#define sc_S SDLK_s
#define sc_T SDLK_t
#define sc_U SDLK_u
#define sc_V SDLK_v
#define sc_W SDLK_w
#define sc_X SDLK_x
#define sc_Y SDLK_y
#define sc_Z SDLK_z

#define key_None 0

#if SDL_MAJOR_VERSION == 1
#define SDLK_KP_0        SDLK_KP0
#define SDLK_KP_1        SDLK_KP1
#define SDLK_KP_2        SDLK_KP2
#define SDLK_KP_3        SDLK_KP3
#define SDLK_KP_4        SDLK_KP4
#define SDLK_KP_5        SDLK_KP5
#define SDLK_KP_6        SDLK_KP6
#define SDLK_KP_7        SDLK_KP7
#define SDLK_KP_8        SDLK_KP8
#define SDLK_KP_9        SDLK_KP9
#define SDLK_SCROLLLOCK  SDLK_SCROLLOCK
#define SDLK_PRINTSCREEN SDLK_PRINT
#define SDLK_NUMLOCKCLEAR SDLK_NUMLOCK
#endif

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
#ifdef OVERHEAD_MAP
	CTL_KB_MORE_ACTION_AUTOMAP,
#endif
	CTL_SPACE_KB_MORE_ACTION,
	CTL_BACK_ACTIONKEYS
};

#if SDL_MAJOR_VERSION == 2
//
// GAME CONTROLLER
//
enum
{
	CTL_GC_MOVE_FWRD,
	CTL_GC_MOVE_BWRD,
	CTL_GC_MOVE_LEFT,
	CTL_GC_MOVE_RIGHT,
	CTL_GC_STRAFE_LEFT,
	CTL_GC_STRAFE_RIGHT,
	CTL_GC_MOVE_SPACE,
	CTL_GC_ACTIONKEYS
};
enum
{
	CTL_GC_ACTION_RUN,
	CTL_GC_ACTION_OPEN,
	CTL_GC_ACTION_FIRE,
	CTL_GC_ACTION_STRAFE,
	CTL_GC_ACTION_PREV_WEP,
	CTL_GC_ACTION_NEXT_WEP,
	CTL_GC_ACTION_SPACE,
	CTL_GC_MOVEMENTKEYS
};
enum
{
	CTL_GC_MORE_ACTION_WEP1,
	CTL_GC_MORE_ACTION_WEP2,
	CTL_GC_MORE_ACTION_WEP3,
	CTL_GC_MORE_ACTION_WEP4,
#ifdef OVERHEAD_MAP
	CTL_GC_MORE_ACTION_AUTOMAP,
#endif
	CTL_GC_MORE_ACTION_SPACE,
	CTL_GC_BACK_ACTIONKEYS
};
#endif
enum
{
	CTL_JOYSTICK_RUN,
	CTL_JOYSTICK_OPEN,
	CTL_JOYSTICK_FIRE,
	CTL_JOYSTICK_STRAFE
};
#if defined(SHOW_CUSTOM_CONTROLS)
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

#ifdef SHOW_ATMOS_OPTIONS
enum
{
	ATMOS_USE_TEXTURED,
	ATMOS_USE_SHADING,
	ATMOS_USE_SKYBOX,
	ATMOS_USE_PRECIPITATION
};
#endif

//
// GAME CONTROLLER
//

#if SDL_MAJOR_VERSION == 2 && defined(USE_MODERN_CONTROLS)
#define TRIGGER_THRESHOLD	16000		// Analog trigger press depth (~50% on a XBox One Controller)
#define STICK_THRESHOLD		16000		// Stick deflection threshold for discrete action checks
#define DPAD_MAX_DELTA		32767

typedef enum ControllerType
{
	CT_XBOX,
	CT_PLAYSTATION,
	CT_NINTENDO,
	CT_GENERIC
} ControllerType;

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
	gc_automap,
	gc_esc,
	gc_pause,
	gc_NUMBUTTONS
}GameControllerAction;

extern int gcBindings[gc_NUMBUTTONS];

extern float gcTurnSensitivity;
extern float gcMaxTurnSensitivity;

extern float gcMoveSensitivity;
extern float gcDpadTurnMultiplier;

#if SDL_MAJOR_VERSION == 2 & defined(USE_MODERN_CONTROLS)
#define sc_gc_NoButton				-1
#define sc_gc_Btn_A					SDL_CONTROLLER_BUTTON_A					// PS = Cross
#define sc_gc_Btn_B					SDL_CONTROLLER_BUTTON_B					// PS = Circle
#define sc_gc_Btn_X					SDL_CONTROLLER_BUTTON_X					// PS = Square
#define sc_gc_Btn_Y					SDL_CONTROLLER_BUTTON_Y					// PS = Triangle
#define sc_gc_Btn_Back				SDL_CONTROLLER_BUTTON_BACK
#define sc_gc_Btn_Guide				SDL_CONTROLLER_BUTTON_GUIDE
#define sc_gc_Btn_Start				SDL_CONTROLLER_BUTTON_START
#define sc_gc_Left_Stick			SDL_CONTROLLER_BUTTON_LEFTSTICK			// Stick Click
#define sc_gc_Right_Stick			SDL_CONTROLLER_BUTTON_RIGHTSTICK		// Stick Click
#define sc_gc_Left_Shoulder			SDL_CONTROLLER_BUTTON_LEFTSHOULDER
#define sc_gc_Right_Shoulder		SDL_CONTROLLER_BUTTON_RIGHTSHOULDER
#define sc_gc_DPad_Up				SDL_CONTROLLER_BUTTON_DPAD_UP
#define sc_gc_DPad_Down				SDL_CONTROLLER_BUTTON_DPAD_DOWN
#define sc_gc_DPad_Left				SDL_CONTROLLER_BUTTON_DPAD_LEFT
#define sc_gc_DPad_Right			SDL_CONTROLLER_BUTTON_DPAD_RIGHT
#define sc_gc_Share_Micro			SDL_CONTROLLER_BUTTON_MISC1				// Xbox Series X share button, PS5 microphone button, Nintendo Switch Pro capture button, Amazon Luna microphone button
#define sc_gc_Btn_Paddle_1			SDL_CONTROLLER_BUTTON_PADDLE1			// Xbox Elite paddle P1 (upper left, facing the back)
#define sc_gc_Btn_Paddle_2			SDL_CONTROLLER_BUTTON_PADDLE2			// Xbox Elite paddle P3 (upper right, facing the back)
#define sc_gc_Btn_Paddle_3			SDL_CONTROLLER_BUTTON_PADDLE3			// Xbox Elite paddle P2 (lower left, facing the back)
#define sc_gc_Btn_Paddle_4			SDL_CONTROLLER_BUTTON_PADDLE4			// Xbox Elite paddle P4 (lower right, facing the back)
#define sc_gc_Touchpad				SDL_CONTROLLER_BUTTON_TOUCHPAD			// PS4/PS5 touchpad button
#define sc_gc_Axis_Left_Trigger		100										// SDL_CONTROLLER_AXIS_TRIGGERLEFT but value needs to be changed since it conflicts with buttons
#define sc_gc_Axis_Right_Trigger	101										// SDL_CONTROLLER_AXIS_TRIGGERRIGHT
#endif

#define sc_gc_NoButton		-1
static const int gcDefaults[gc_NUMBUTTONS] = {
	sc_gc_Axis_Right_Trigger,	// gc_attack
	sc_gc_Btn_A,				// gc_use
	sc_gc_Btn_X,				// gc_run
	sc_gc_Btn_B,				// gc_strafe
	sc_gc_Left_Shoulder,		// gc_prevweapon
	sc_gc_Right_Shoulder,		// gc_nextweapon
	sc_gc_DPad_Up,				// gc_forward
	sc_gc_DPad_Down,			// gc_backward
	sc_gc_DPad_Left,			// gc_turnleft
	sc_gc_DPad_Right,			// gc_turnright
	sc_gc_NoButton,				// gc_straferight
	sc_gc_NoButton,				// gc_strafeleft
	sc_gc_NoButton,				// gc_weapon1
	sc_gc_NoButton,				// gc_weapon2
	sc_gc_NoButton,				// gc_weapon3
	sc_gc_NoButton,				// gc_weapon4
	sc_gc_NoButton,				// gc_automap
	sc_gc_Btn_Back,				// gc_esc
	sc_gc_Btn_Start				// gc_pause
};

#endif

#if SDL_MAJOR_VERSION == 2
typedef SDL_Keymod ModState;
#define KEY_SCROLLLOCK SDLK_SCROLLLOCK
#define SET_GRAB_INPUT(grab) SDL_SetRelativeMouseMode((grab) ? SDL_TRUE : SDL_FALSE)
#elif SDL_MAJOR_VERSION == 1
typedef SDLMod ModState;
#define KEY_SCROLLLOCK SDLK_SCROLLOCK
#define SET_GRAB_INPUT(grab) SDL_WM_GrabInput((grab) ? SDL_GRAB_ON : SDL_GRAB_OFF)
#endif

typedef enum
{
	demo_Off,
	demo_Record,
	demo_Playback,
	demo_PlayDone
} Demo;
typedef enum
{
	ctrl_Keyboard,
	ctrl_Keyboard1 = ctrl_Keyboard,
	ctrl_Keyboard2,
	ctrl_Joystick,
	ctrl_Joystick1 = ctrl_Joystick,
	ctrl_Joystick2,
	ctrl_Mouse
} ControlType;
typedef enum
{
	motion_Left = -1,
	motion_Up = -1,
	motion_None = 0,
	motion_Right = 1,
	motion_Down = 1
} Motion;
typedef enum
{
	dir_North,
	dir_NorthEast,
	dir_East,
	dir_SouthEast,
	dir_South,
	dir_SouthWest,
	dir_West,
	dir_NorthWest,
	dir_None
} Direction;
typedef struct
{
	boolean button0, button1, button2, button3;
	short x, y;
	Motion xaxis, yaxis;
	Direction dir;
} CursorInfo;
typedef CursorInfo ControlInfo;
typedef struct
{
	ScanCode button0, button1,
		upleft, up, upright,
		left, right,
		downleft, down, downright;
} KeyboardDef;
typedef struct
{
	word joyMinX, joyMinY,
		threshMinX, threshMinY,
		threshMaxX, threshMaxY,
		joyMaxX, joyMaxY,
		joyMultXL, joyMultYL,
		joyMultXH, joyMultYH;
} JoystickDef;

// Global variables
extern volatile boolean KeyboardState[129];
extern boolean MousePresent;
extern volatile boolean Paused;
extern volatile char LastASCII;
extern volatile ScanCode LastScan;
extern int JoyNumButtons;

// Function prototypes
#define IN_KeyDown(code) (Keyboard((code)))
#define IN_ClearKey(code)         \
	{                             \
		KeyboardSet(code, false); \
		if (code == LastScan)     \
			LastScan = sc_None;   \
	}

// DEBUG - put names in prototypes
extern void IN_Startup(void), IN_Shutdown(void);
extern void IN_ClearKeysDown(void);
extern void IN_ReadControl(int, ControlInfo*);
extern void IN_Ack(void);
extern boolean IN_UserInput(longword delay);
extern char IN_WaitForASCII(void);
extern ScanCode IN_WaitForKey(void);
extern const char* IN_GetScanName(ScanCode);

boolean Keyboard(int key);
void KeyboardSet(int key, boolean state);
int KeyboardLookup(int key);

void IN_WaitAndProcessEvents();
void IN_ProcessEvents();

int IN_MouseButtons(void);

#if SDL_MAJOR_VERSION == 1 || !defined(USE_MODERN_CONTROLS)
void IN_GetJoyDelta(int* dx, int* dy);
void IN_GetJoyDelta(int* dx, int* dy, int* strafe);
int IN_JoyButtons(void);
boolean IN_JoyPresent();
void PollJoystickMove(void);
void IN_GetJoyFineDelta(int* dx, int* dy);
#endif

void IN_StartAck(void);
boolean IN_CheckAck(void);
bool IN_IsInputGrabbed();
void IN_CenterMouse();
void IN_MouseGrab(void);

// Game controller
#if (SDL_MAJOR_VERSION == 2) && defined(USE_MODERN_CONTROLS)
boolean IN_GcPresent();
ControllerType IN_GcGetControllerType(void);
void IN_InitGcBindings(void);
int IN_GcButtons(void);
const char* IN_GcGetScanName(ScanCode);
int IN_GcRemapReadButton(void);
void IN_GcCheckMenuInputs(void);
void IN_GcPollActions(void);
void IN_GcGetDelta(int* analog0X, int* analog0Y, int* analog1X, int* analog1Y);
bool IN_GcIsActionPressed(GameControllerAction, bool allowHold = true);
bool IN_GcGetButton(SDL_GameControllerButton);
void IN_GcForceReleaseAll(void);
bool IN_GcIsButtonBlocked(int);
#endif
#endif
