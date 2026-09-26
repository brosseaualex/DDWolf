/*
*	ID_GC.cpp - Game Controller Input Manager
*	By Alexandre Brosseau (DemolitionDerby)
*	v1.0 - September 2026
*	Developed for DDWolf
*/

#include "id_gc.h"

#if SDL_MAJOR_VERSION == 2 && defined(USE_MODERN_CONTROLS)

SDL_GameController* GameController = NULL;
SDL_JoystickID gcId = -1;

int gcBindings[gc_NUMBUTTONS];
static bool gcLastState[gc_NUMBUTTONS] = { false };
static bool gcConsumedState[gc_NUMBUTTONS] = { false };

// Sensitivity settings (10 = 100% normal speed, 5 = 50% speed, 20 = 200% speed)
// Capped at 10 in the menu
float gcTurnSensitivity = 5.0f;
float gcMaxTurnSensitivity = 10.0f;

bool gcHotplugDirty = false;

void GC_InitGameController(void)
{
	// FIXME - implement param_joystickindex
	//GameController = SDL_GameControllerOpen(param_joystickindex);

	SDL_GameControllerEventState(SDL_ENABLE);

	for (int i = 0; i < SDL_NumJoysticks(); i++)
	{
		if (SDL_IsGameController(i))
		{
			GameController = SDL_GameControllerOpen(i);
			if (GameController)
			{
				SDL_Joystick* joy = SDL_GameControllerGetJoystick(GameController);
				gcId = SDL_JoystickInstanceID(joy);
				GC_ForceReleaseAllButtons();
				printf("Successfully opened controller: %s\n", SDL_GameControllerName(GameController));
				break;
			}
			else
				printf("Could not open game controller %d! SDL_Error: %s\n", i, SDL_GetError());
		}
	}
}

void GC_ProcessEvents(const SDL_Event* event)
{
	if (!event)
		return;

	switch (event->type)
	{
	case SDL_CONTROLLERBUTTONDOWN:
	{
		if (event->cbutton.button == SDL_CONTROLLER_BUTTON_START)
		{
			Paused = !Paused;
			LastScan = Paused ? SDLK_PAUSE : 0;
			LastASCII = 0;

			GC_ForceReleaseAllButtons();
		}
		break;
	}
	case SDL_CONTROLLERDEVICEADDED:
	{
		if (!GameController)
		{
			GameController = SDL_GameControllerOpen(event->cdevice.which);
			if (GameController)
			{
				SDL_Joystick* joy = SDL_GameControllerGetJoystick(GameController);
				gcId = SDL_JoystickInstanceID(joy);

				GC_ForceReleaseAllButtons();
				controllerEnabled = true;
				gcHotplugDirty = true;
#if _DEBUG
				printf("Controller connected: %s (Instance ID: %d)\n",
					SDL_GameControllerName(GameController), gcId);
#endif
			}
		}
		break;
	}
	case SDL_CONTROLLERDEVICEREMOVED:
	{
		if (GameController && event->cdevice.which == gcId)
		{
			controllerEnabled = false;
			GC_ForceReleaseAllButtons();

			SDL_GameControllerClose(GameController);
			GameController = NULL;
			gcId = -1;

			gcHotplugDirty = true;
#if _DEBUG
			printf("Controller disconnected\n");
#endif
		}
		break;
	}
	default:
		break;
	}
}

bool GC_IsActionPressed(GameControllerAction action, bool allowHold = true)
{
	if (!GameController || !controllerEnabled || (unsigned)action >= gc_NUMBUTTONS)
		return false;

	bool currentState = false;
	int btn = gcBindings[action];

	if (btn >= 0 && btn < sc_gc_Axis_Left_Trigger && btn != sc_gc_NoButton)
	{
		if (SDL_GameControllerGetButton(GameController, (SDL_GameControllerButton)btn))
			currentState = true;
	}
	else if (btn == sc_gc_Axis_Left_Trigger)
	{
		if (SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > TRIGGER_THRESHOLD)
			currentState = true;
	}
	else if (btn == sc_gc_Axis_Right_Trigger)
	{
		if (SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > TRIGGER_THRESHOLD)
			currentState = true;
	}

	if (!currentState && (btn == sc_gc_NoButton || btn < 0))
	{
		if (action == gc_attack)
		{
			if (SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_TRIGGERRIGHT) > TRIGGER_THRESHOLD)
				currentState = true;
		}
		else if (action == gc_run || action == gc_strafe)
		{
			if (SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_TRIGGERLEFT) > TRIGGER_THRESHOLD)
				currentState = true;
		}
	}

	if (!currentState)
	{
		switch (action)
		{
		case gc_forward:
			currentState = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_LEFTY) < -STICK_THRESHOLD;
			break;
		case gc_backward:
			currentState = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_LEFTY) > STICK_THRESHOLD;
			break;
		case gc_strafeleft:
			currentState = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_LEFTX) < -STICK_THRESHOLD;
			break;
		case gc_straferight:
			currentState = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_LEFTX) > STICK_THRESHOLD;
			break;
		case gc_turnleft:
			currentState = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_RIGHTX) < -STICK_THRESHOLD;
			break;
		case gc_turnright:
			currentState = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_RIGHTX) > STICK_THRESHOLD;
			break;
		default:
			break;
		}
	}

	if (!currentState)
	{
		gcConsumedState[action] = false;
		gcLastState[action] = false;
		return false;
	}

	if (gcConsumedState[action])
	{
		gcLastState[action] = true;
		return false;
	}

	if (!allowHold)
		gcConsumedState[action] = true;

	gcLastState[action] = true;

	return true;
}

ControllerDelta GC_GetDelta(void)
{
	ControllerDelta delta = { 0, 0, 0, 0 };

	if (!GameController || !controllerEnabled)
		return delta;

	SDL_GameControllerUpdate();

	int a0X = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_LEFTX);
	int a0Y = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_LEFTY);
	int a1X = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_RIGHTX);
	int a1Y = SDL_GameControllerGetAxis(GameController, SDL_CONTROLLER_AXIS_RIGHTY);

	a0X = (abs(a0X) >= TRIGGER_THRESHOLD) ? (a0X >> 8) : 0;
	a0Y = (abs(a0Y) >= TRIGGER_THRESHOLD) ? (a0Y >> 8) : 0;
	a1X = (abs(a1X) >= TRIGGER_THRESHOLD) ? (a1X >> 8) : 0;
	a1Y = (abs(a1Y) >= TRIGGER_THRESHOLD) ? (a1Y >> 8) : 0;

	bool strafeHeld = GC_IsActionPressed(gc_strafe);

	if (strafeHeld && a1X != 0 && a0X == 0)
	{
		a0X = a1X;
		a1X = 0;
	}

	if (a0Y == 0)
	{
		if (GC_IsActionPressed(gc_backward))
			a0Y = 127;
		else if (GC_IsActionPressed(gc_forward))
			a0Y = -128;
	}

	if (a0X == 0)
	{
		if (strafeHeld)
		{
			if (GC_IsActionPressed(gc_turnright) || GC_IsActionPressed(gc_straferight))
				a0X = 127;
			else if (GC_IsActionPressed(gc_turnleft) || GC_IsActionPressed(gc_strafeleft))
				a0X = -128;
		}
		else
		{
			if (GC_IsActionPressed(gc_straferight))
				a0X = 127;
			else if (GC_IsActionPressed(gc_strafeleft))
				a0X = -128;
		}
	}

	if (a1X == 0 && !strafeHeld)
	{
		if (GC_IsActionPressed(gc_turnright))
			a1X = 127;
		else if (GC_IsActionPressed(gc_turnleft))
			a1X = -128;
	}

	if (gcTurnSensitivity > 0)
	{
		a1X = (a1X * (int)gcTurnSensitivity) / 10;
		a1Y = (a1Y * (int)gcTurnSensitivity) / 10;
	}

	delta.a0X = ClampInt(a0X, -128, 127);
	delta.a0Y = ClampInt(a0Y, -128, 127);
	delta.a1X = ClampInt(a1X, -128, 127);
	delta.a1Y = ClampInt(a1Y, -128, 127);

	return delta;
}

void GC_PollMove(void)
{
	if (!GameController || !controllerEnabled)
		return;

	ControllerDelta gc = GC_GetDelta();

	int baseSpeed = (GC_IsActionPressed(gc_run) || buttonstate[bt_run]) ? (RUNMOVE * tics) : (BASEMOVE * tics);

	int turnSpeed = TURNMOVE * tics;

	if (gc.a0X != 0)
		controlx += (gc.a0X * baseSpeed) / 128;

	if (gc.a0Y != 0)
		controly += (gc.a0Y * baseSpeed) / 128;

	if (gc.a1X != 0)
		anglefrac += (gc.a1X * turnSpeed) / 128;
}

void GC_PollActions(void)
{
	if (!GameController || !controllerEnabled)
		return;

	if (GC_IsActionPressed(gc_attack))
		buttonstate[bt_attack] = true;
	if (GC_IsActionPressed(gc_use))
		buttonstate[bt_use] = true;
	if (GC_IsActionPressed(gc_run))
		buttonstate[bt_run] = true;
	if (GC_IsActionPressed(gc_strafe))
		buttonstate[bt_strafe] = true;
	if (GC_IsActionPressed(gc_prevweapon))
		buttonstate[bt_prevweapon] = true;
	if (GC_IsActionPressed(gc_nextweapon))
		buttonstate[bt_nextweapon] = true;

	if (GC_IsActionPressed(gc_weapon1))
		gamestate.weapon = wp_knife;
	else if (GC_IsActionPressed(gc_weapon2))
		gamestate.weapon = wp_pistol;
	else if (GC_IsActionPressed(gc_weapon3))
		gamestate.weapon = wp_machinegun;
	else if (GC_IsActionPressed(gc_weapon4))
		gamestate.weapon = wp_chaingun;

	if (GC_IsActionPressed(gc_pause, false))
		buttonstate[bt_pause] = true;
	if (GC_IsActionPressed(gc_esc, false))
		buttonstate[bt_esc] = true;

#ifdef OVERHEAD_MAP
	if (IN_GcIsActionPressed(gc_automap))
		buttonstate[bt_automap] = true;
#endif
}

GcMenuState GC_GetMenuState(void)
{
	GcMenuState menuState = { 0, 0, false, false, false, false };

	if (!GameController || !controllerEnabled)
		return menuState;

	ControllerDelta gc = GC_GetDelta();

	const int MENU_THRESHOLD = 30;

	bool dpadUp = GC_GetButton(SDL_CONTROLLER_BUTTON_DPAD_UP);
	bool dpadDown = GC_GetButton(SDL_CONTROLLER_BUTTON_DPAD_DOWN);
	bool dpadLeft = GC_GetButton(SDL_CONTROLLER_BUTTON_DPAD_LEFT);
	bool dpadRight = GC_GetButton(SDL_CONTROLLER_BUTTON_DPAD_RIGHT);

	if (gc.a1X < -MENU_THRESHOLD || gc.a0X < -MENU_THRESHOLD || dpadLeft)
		menuState.dirX = -1;
	else if (gc.a1X > MENU_THRESHOLD || gc.a0X > MENU_THRESHOLD || dpadRight)
		menuState.dirX = 1;

	if (gc.a0Y < -MENU_THRESHOLD || dpadUp)
		menuState.dirY = -1;
	else if (gc.a0Y > MENU_THRESHOLD || dpadDown)
		menuState.dirY = 1;

	static bool lastA = false, lastB = false, lastX = false, lastY = false;

	menuState.buttonA = GC_CheckEdge(GC_GetButton(SDL_CONTROLLER_BUTTON_A) || GC_IsActionPressed(gc_use, false), &lastA);
	menuState.buttonB = GC_CheckEdge(GC_GetButton(SDL_CONTROLLER_BUTTON_B) || GC_IsActionPressed(gc_esc, false), &lastB);
	menuState.buttonX = GC_CheckEdge(GC_GetButton(SDL_CONTROLLER_BUTTON_X) || GC_IsActionPressed(gc_attack, false), &lastX);
	menuState.buttonY = GC_CheckEdge(GC_GetButton(SDL_CONTROLLER_BUTTON_Y) || GC_IsActionPressed(gc_strafe, false), &lastY);

	return menuState;
}

void GC_PollMenuInputs(int* dir, bool* b0, bool* b1, bool* b2, bool* b3)
{
	if (!GameController || !controllerEnabled)
		return;

	GcMenuState gc = GC_GetMenuState();

	if (dir)
	{
		if (gc.dirX < 0)
			*dir = dir_West;
		else if (gc.dirX > 0)
			*dir = dir_East;
		if (gc.dirY < 0)
			*dir = dir_North;
		else if (gc.dirY > 0)
			*dir = dir_South;
	}

	if (b0 && gc.buttonA) *b0 = true;
	if (b1 && gc.buttonB) *b1 = true;
	if (b2 && gc.buttonX) *b2 = true;
	if (b3 && gc.buttonY) *b3 = true;
}

int GC_EnterCtrlData(void)
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
		case SDL_CONTROLLERBUTTONDOWN:
			return (int)event.cbutton.button;
		case SDL_CONTROLLERAXISMOTION:
			if (event.caxis.value > TRIGGER_THRESHOLD)
			{
				if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT)
					return sc_gc_Axis_Left_Trigger;
				if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERRIGHT)
					return sc_gc_Axis_Right_Trigger;
			}
			break;
		case SDL_KEYDOWN:
			if (event.key.keysym.sym == SDLK_BACKSPACE || event.key.keysym.sym == SDLK_DELETE)
				return -3; // Clear
			else if (event.key.keysym.sym == SDLK_ESCAPE)
				return -2; // Cancel
			break;
		}
	}

	return -1;
}

/*
==========================
=
= GC_GetControllerType
=
==========================
*/
ControllerType GC_GetControllerType(void)
{
	if (!GameController)
		return CT_GENERIC;

	SDL_GameControllerType type = SDL_GameControllerGetType(GameController);

	switch (type)
	{
	case SDL_CONTROLLER_TYPE_PS3:
	case SDL_CONTROLLER_TYPE_PS4:
	case SDL_CONTROLLER_TYPE_PS5:
		return CT_PLAYSTATION;

	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_PRO:
	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_LEFT:
	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_RIGHT:
	case SDL_CONTROLLER_TYPE_NINTENDO_SWITCH_JOYCON_PAIR:
		return CT_NINTENDO;

	case SDL_CONTROLLER_TYPE_XBOX360:
	case SDL_CONTROLLER_TYPE_XBOXONE:
		return CT_XBOX;

	default:
		return CT_GENERIC;
	}
}

/*
==========================
=
= GC_GetScanName
=
==========================
*/
const char* GC_GetScanName(int sc)
{
	if (sc == sc_gc_NoButton || sc < 0)
		return "?";

	ControllerType type = GC_GetControllerType();

	switch (sc)
	{
	case SDL_CONTROLLER_BUTTON_A:
		if (type == CT_PLAYSTATION)
			return "Cross";
		if (type == CT_NINTENDO)
			return "B";
		return "A";

	case SDL_CONTROLLER_BUTTON_B:
		if (type == CT_PLAYSTATION)
			return "Circle";
		if (type == CT_NINTENDO)
			return "A";
		return "B";

	case SDL_CONTROLLER_BUTTON_X:
		if (type == CT_PLAYSTATION)
			return "Square";
		if (type == CT_NINTENDO)
			return "Y";
		return "X";

	case SDL_CONTROLLER_BUTTON_Y:
		if (type == CT_PLAYSTATION)
			return "Triangle";
		if (type == CT_NINTENDO)
			return "X";
		return "Y";

	case SDL_CONTROLLER_BUTTON_BACK:
		if (type == CT_PLAYSTATION)
			return "Share/Select";
		if (type == CT_NINTENDO)
			return "-";
		return "Back";

	case SDL_CONTROLLER_BUTTON_START:
		if (type == CT_PLAYSTATION)
			return "Options";
		if (type == CT_NINTENDO)
			return "+";
		return "Start";

	case SDL_CONTROLLER_BUTTON_GUIDE:
		if (type == CT_PLAYSTATION)
			return "PS";
		if (type == CT_NINTENDO)
			return "Home";
		return "Guide";

	case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
		if (type == CT_PLAYSTATION)
			return "L1";
		if (type == CT_NINTENDO)
			return "L";
		return "LB";

	case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
		if (type == CT_PLAYSTATION)
			return "R1";
		if (type == CT_NINTENDO)
			return "R";
		return "RB";

	case SDL_CONTROLLER_BUTTON_LEFTSTICK:
		if (type == CT_PLAYSTATION)
			return "L3";
		return "L Stick Click";

	case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
		if (type == CT_PLAYSTATION)
			return "R3";
		return "R Stick Click";

	case SDL_CONTROLLER_BUTTON_DPAD_UP:
		return "D-Pad Up";
	case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
		return "D-Pad Down";
	case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
		return "D-Pad L";
	case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
		return "D-Pad R";

		// Elite Controllers
	case SDL_CONTROLLER_BUTTON_PADDLE1:
		return "Paddle 1 (P1)";
	case SDL_CONTROLLER_BUTTON_PADDLE2:
		return "Paddle 2 (P2)";
	case SDL_CONTROLLER_BUTTON_PADDLE3:
		return "Paddle 3 (P3)";
	case SDL_CONTROLLER_BUTTON_PADDLE4:
		return "Paddle 4 (P4)";

	case SDL_CONTROLLER_BUTTON_TOUCHPAD:
		return "Touchpad Click";

		// Other
	case SDL_CONTROLLER_BUTTON_MISC1:
		if (type == CT_PLAYSTATION)
			return "Mute";
		if (type == CT_NINTENDO)
			return "Capture";
		return "Share";

		// Custom Trigger Bindings (IDs >= 100)
	case sc_gc_Axis_Left_Trigger:
		if (type == CT_PLAYSTATION)
			return "L2";
		if (type == CT_NINTENDO)
			return "ZL";
		return "LT";

	case sc_gc_Axis_Right_Trigger:
		if (type == CT_PLAYSTATION)
			return "R2";
		if (type == CT_NINTENDO)
			return "ZR";
		return "RT";

	default:
		return "?";
	}
}

boolean GC_IsPresent()
{
	return GameController != NULL;
}

void GC_InitDefaultBindings(void)
{
	for (int i = 0; i < gc_NUMBUTTONS; i++)
		gcBindings[i] = gcDefaultBindings[i];
}

int GC_GetButtons()
{
	if (!GameController || !controllerEnabled)
		return 0;

	int i;
	int res = 0;

	SDL_GameControllerUpdate();

	for (i = 0; i < SDL_CONTROLLER_BUTTON_MAX; i++)
		res |= SDL_GameControllerGetButton(GameController, (SDL_GameControllerButton)i) << i;

	return res;
}

bool GC_GetButton(SDL_GameControllerButton button)
{
	return SDL_GameControllerGetButton(GameController, button) != 0;
}

void GC_ForceReleaseButton(GameControllerAction action)
{
	if ((unsigned)action < gc_NUMBUTTONS)
		gcConsumedState[action] = true;
}

void GC_ForceReleaseAllButtons(void)
{
	for (int i = 0; i < gc_NUMBUTTONS; i++)
		gcConsumedState[i] = true;
}

bool GC_IsButtonBound(int targetButton)
{
	if (targetButton == sc_gc_NoButton || targetButton < 0)
		return false;

	for (int i = 0; i < gc_NUMBUTTONS; i++)
		if (gcBindings[i] == targetButton)
			return true;

	return false;
}

bool GC_IsButtonForbidden(int button)
{
	if (button < 0)
		return false;

	for (int i = 0; i < g_numBlockedGcButtons; i++)
	{
		if (gcRemapForbiddenButtons[i] == button)
			return true;
	}
	return false;
}

bool GC_CheckEdge(bool isDown, bool* lastState)
{
	bool edge = isDown && !*lastState;
	*lastState = isDown;
	return edge;
}

void GC_ProcessHotplugEvents(void)
{
	if (gcHotplugDirty)
	{
		gcHotplugDirty = false;
		LastScan = 0;
		LastASCII = 0;

		IntroScreen();
		VW_UpdateScreen();
	}
}

void GC_CleanHotplugEvents(boolean state[NUMBUTTONS])
{
	memset(state, 0, sizeof(state));
	LastScan = 0;
	LastASCII = 0;
}

int ClampInt(int val, int minVal, int maxVal)
{
	if (val < minVal) return minVal;
	if (val > maxVal) return maxVal;
	return val;
}
#endif