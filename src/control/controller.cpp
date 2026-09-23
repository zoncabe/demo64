/*
	What every button means in this demo. The engine holds no binding of its
	own: it maps the controller through whatever the game hands it, so this is the
	one place a button is named.

	A binding is a mapping plus what it moves: besides the buttons it names the
	placement of the body it drives. The scene load reads it, so nothing here
	is called from the game: it seats the player on that body as it builds it
	and hands the camera the buttons that name it.

	An action left out is BTN_NONE, which reads as never pressed.
*/
#include "camera/camera.h"
#include "control/controller.h"

/* The 3D scene's entity table: the character binding names its row. */
extern e64::scene3d::Entity scene_entities[];


const e64::menu::ControlBinding menu_binding = {

	.player = e64::PLAYER_1,

	.confirm = e64::BTN_A,
	.cancel = e64::BTN_B,
	.pause = e64::BTN_START,

	.up = e64::BTN_D_UP,
	.down = e64::BTN_D_DOWN,
	.left = e64::BTN_D_LEFT,
	.right = e64::BTN_D_RIGHT,

	.tab_left = e64::BTN_L,
	.tab_right = e64::BTN_R,
};

/* The body the player starts on: the mr_muscles row of the scene. The d-pad
   switches between the bodies the scene holds, same buttons on each. */
const e64::character3d::ControlBinding character3d_binding = {

	.player = e64::PLAYER_1,
	.character = &scene_entities[11],

	.jump = e64::BTN_A,
	.roll = e64::BTN_B,
	.sprint = e64::BTN_L,
	.aim = e64::BTN_Z,

	/* On top of the aim: Z carries the bow, R draws the string. */
	.shoot = e64::BTN_R,

	/* The d-pad steps through what the body carries, unarmed included. */
	.weapon_next = e64::BTN_D_RIGHT,
	.weapon_prev = e64::BTN_D_LEFT,
};

/* The side view: the d-pad walks, the stick too. The four screens of the 2D
   game are four scenes with the same buttons, so this names no placement:
   the load seats nobody, and the stage opener seats the body it holds. */
const e64::character2d::ControlBinding character2d_binding = {

	.player = e64::PLAYER_1,
	.character = NULL,

	.jump = e64::BTN_A,
	.roll = e64::BTN_B,
	.sprint = e64::BTN_L,

	.left = e64::BTN_D_LEFT,
	.right = e64::BTN_D_RIGHT,
};

/* The C stick swings the arm. Distance and field of view are left unbound:
   nothing in this demo pushes them by hand.

   Naming the camera is what hands it these buttons: the scene's own answers
   to them if it is the one declared here. */
const e64::camera3d::ControlBinding camera_binding = {

	.player = e64::PLAYER_1,
	.camera = &camera_def,

	.pan_left = e64::BTN_C_LEFT,
	.pan_right = e64::BTN_C_RIGHT,
	.tilt_up = e64::BTN_C_UP,
	.tilt_down = e64::BTN_C_DOWN,
};

/* What the demo drives with, all of it. A state names this and the engine
   wires whatever of it belongs to the scenes that state carries: the 3D
   gameplay takes the camera and the body, the 2D one takes its own body,
   and the menus read theirs from the state's control callback. */
const e64::controls::Def controls = {

	.camera = &camera_binding,
	.character3d = &character3d_binding,
	.character2d = &character2d_binding,
	.menu = &menu_binding,
};