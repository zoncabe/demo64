/*
	What every button means in this demo. The engine holds no binding of its
	own: it maps the controller through whatever the game hands it, so this is the
	one place a button is named.

	An action left out is BTN_NONE, which reads as never pressed.
*/
#include "control/controller.h"


const MenuControlBinding menu_binding = {

	.player    = PLAYER_1,

	.confirm   = BTN_A,
	.cancel    = BTN_B,
	.pause     = BTN_START,

	.up        = BTN_D_UP,
	.down      = BTN_D_DOWN,
	.left      = BTN_D_LEFT,
	.right     = BTN_D_RIGHT,

	.tab_left  = BTN_L,
	.tab_right = BTN_R,
};

const CharacterControlBinding character_binding = {

	.player = PLAYER_1,

	.jump   = BTN_A,
	.roll   = BTN_B,
	.sprint = BTN_L,
	.aim    = BTN_Z,

	/* On top of the aim: Z carries the bow, R draws the string. */
	.shoot  = BTN_R,

	/* The d-pad steps through what the body carries, unarmed included. */
	.weapon_next = BTN_D_RIGHT,
	.weapon_prev = BTN_D_LEFT,
};

/* The C stick swings the arm. Distance and field of view are left unbound:
   nothing in this demo pushes them by hand. */
const CameraControlBinding camera_binding = {

	.player = PLAYER_1,

	.pan_left  = BTN_C_LEFT,
	.pan_right = BTN_C_RIGHT,
	.tilt_up   = BTN_C_UP,
	.tilt_down = BTN_C_DOWN,
};