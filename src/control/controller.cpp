/*
	What every button means in this demo. The engine holds no binding of its
	own: it maps the controller through whatever the game hands it, so this is the
	one place a button is named.

	A binding is a mapping plus what it moves: besides the buttons it names the
	pieces it can drive, by the declarations the scene placed them from. The
	engine reads them when a state is entered, so nothing here is called from
	the game: it seats the player on whichever of those bodies the scene holds
	and hands the camera the buttons that name it.

	An action left out is BTN_NONE, which reads as never pressed.
*/
#include "assets/characters/miss_jiggles.h"
#include "assets/characters/mr_muscles.h"
#include "assets/characters/pixel_green.h"
#include "assets/characters/pixel_yellow.h"
#include "assets/characters/line_blue.h"
#include "camera/camera.h"
#include "control/controller.h"


const menu::ControlBinding menu_binding = {

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

/* The bodies this layout can drive. The scene decides which one is there:
   whichever of these it placed is the one the player is seated on. */
static const Prefab3D *const character3d_prefab[] = {
	&miss_jiggles,
	&mr_muscles,
};

const character3d::ControlBinding character3d_binding = {

	.player          = PLAYER_1,
	.character       = character3d_prefab,
	.character_count = E64_ARRAY_COUNT(character3d_prefab),

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

/* The side view: the d-pad walks, the stick too. The four screens of the 2D
   game place three different bodies, and the buttons are the same on all of
   them: one layout naming the three, and each screen seats the one it holds. */
static const Prefab2D *const character2d_prefab[] = {
	&pixel_green,
	&pixel_yellow,
	&line_blue,
};

const character2d::ControlBinding character2d_binding = {

	.player          = PLAYER_1,
	.character       = character2d_prefab,
	.character_count = E64_ARRAY_COUNT(character2d_prefab),

	.jump   = BTN_A,
	.roll   = BTN_B,
	.sprint = BTN_L,

	.left   = BTN_D_LEFT,
	.right  = BTN_D_RIGHT,
};

/* The C stick swings the arm. Distance and field of view are left unbound:
   nothing in this demo pushes them by hand.

   Naming the camera is what hands it these buttons: the scene's own answers
   to them if it is the one declared here. */
const camera::ControlBinding camera_binding = {

	.player = PLAYER_1,
	.camera = &camera_def,

	.pan_left  = BTN_C_LEFT,
	.pan_right = BTN_C_RIGHT,
	.tilt_up   = BTN_C_UP,
	.tilt_down = BTN_C_DOWN,
};

/* What the demo drives with, all of it. A state names this and the engine
   wires whatever of it belongs to the scenes that state carries: the 3D
   gameplay takes the camera and the body, the 2D one takes its own body,
   and the menus read theirs from the state's control callback. */
const controls::Def demo_controls = {

	.camera      = &camera_binding,
	.character3d = &character3d_binding,
	.character2d = &character2d_binding,
	.menu        = &menu_binding,
};