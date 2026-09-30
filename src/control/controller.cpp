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
#include "scene/scene2d.h"
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
   game share the buttons and each has a body of its own, so there is one
   binding per screen, naming that screen's body. */
#define CHARACTER2D_BUTTONS \
	.jump = e64::BTN_A, \
	.roll = e64::BTN_B, \
	.sprint = e64::BTN_L, \
	.left = e64::BTN_D_LEFT, \
	.right = e64::BTN_D_RIGHT

static const e64::character2d::ControlBinding pixel_a_binding = { .player = e64::PLAYER_1, .character = &pixel_a_placed[2], CHARACTER2D_BUTTONS };
static const e64::character2d::ControlBinding pixel_b_binding = { .player = e64::PLAYER_1, .character = &pixel_b_placed[2], CHARACTER2D_BUTTONS };
static const e64::character2d::ControlBinding industry_binding = { .player = e64::PLAYER_1, .character = &industry_placed[2], CHARACTER2D_BUTTONS };
static const e64::character2d::ControlBinding line_binding = { .player = e64::PLAYER_1, .character = &line_placed[2], CHARACTER2D_BUTTONS };

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

/* What the 3D gameplay drives with. The state names this and the engine
   wires it as it builds the scene: the camera and the body. The menus read
   theirs from the state's control callback. */
const e64::controls::Def controls = {

	.camera = &camera_binding,
	.character3d = &character3d_binding,
	.menu = &menu_binding,
};

/* What each 2D screen loads with: the state declares the first, and the
   gameplay hands the next screen its own as it opens it. */
const e64::controls::Def controls_pixel_a = { .character2d = &pixel_a_binding };
const e64::controls::Def controls_pixel_b = { .character2d = &pixel_b_binding };
const e64::controls::Def controls_industry = { .character2d = &industry_binding };
const e64::controls::Def controls_line = { .character2d = &line_binding };
