/*
	The demo's state table: what each state runs, what it draws and what it
	does with the controller. The engine brings the machinery; everything named here
	is content.

	A state leaves by naming where it goes and playing its way out. Every
	state with a screen holds the switch back until that animation ends.
*/
#include ENGINE_HEADER(time, time)
#include ENGINE_HEADER(scene3d, scene3d)
#include "scene/scene.h"
#include "scene/demo_scene2d.h"
#include ENGINE_HEADER(scene2d, scene2d)
#include ENGINE_HEADER(render, render)
#include ENGINE_HEADER(ui, ui)
#include "cutscene/intro.h"
#include "ui/main_menu_ui.h"
#include "ui/pause_ui.h"
#include "ui/gameplay_ui.h"
#include "ui/credits_ui.h"
#include "ui/stamina_wheel.h"
#include ENGINE_HEADER(menu, menu)
#include ENGINE_HEADER(particles, particles)
#include ENGINE_HEADER(shaders, water)
#include ENGINE_HEADER(player, player)
#include ENGINE_HEADER(control, player_control)
#include ENGINE_HEADER(control, controller)
#include ENGINE_HEADER(control, camera_control)
#include ENGINE_HEADER(debug, debug)
#include "control/controller.h"
#include "camera/camera.h"
#include ENGINE_HEADER(sound, sound)
#include ENGINE_HEADER(sound, prop_sound)
#include ENGINE_HEADER(game, game)
#include ENGINE_HEADER(viewport, viewport)
#include "game/game_states.h"


static bool gameState_canLeave(void) { return !ui_isTransitioning(); }


/* --- intro -------------------------------------------------------------- */

static void gameState_enterIntro(void)
{
	ui_play(&intro_animation, false);
}

static void gameState_updateIntro(void)
{
	/* The intro is its own way out: when the animation ends, so does it. */
	game_setState(game_get(), MAIN_MENU);
	ui_update(NULL);
}


/* --- main menu ---------------------------------------------------------- */

static void gameState_enterMainMenu(void)
{
	ui_play(&main_menu_enter, false);
}

static void gameState_updateMainMenu(void)
{
	menu::control::update();
	ui_update(&main_menu_idle);
}

static void gameState_controlMainMenu(void)
{
	if (ui_isTransitioning()) return;

	menu::Controls menu;
	menu::control::read(&menu, &controller::get()[menu_binding.player], &menu_binding);

	if (menu.confirm) {
		int8_t idx = menuStack_getIndex();
		if (idx == 0) game_setState(game_get(), GAMEPLAY3D);
		if (idx == 1) game_setState(game_get(), GAMEPLAY2D);
		if (idx == 2) game_setState(game_get(), CREDITS);
		if (idx <= 2) ui_play(&main_menu_enter, true);
	}
	if (menu.up)   menuStack_moveIndex(-1, 2);
	if (menu.down) menuStack_moveIndex(1,  2);
}


/* --- credits ------------------------------------------------------------ */

static void gameState_enterCredits(void)
{
	credits_ui_resetScroll();
	ui_play(&credits_enter, false);
}


static void gameState_updateCredits(void)
{
	menu::control::update();
	if (ui_isTransitioning()) credits_ui_setScrollVelocity(0.0f);
	credits_ui_updateScroll(time_get()->delta);
	ui_update(NULL);

	/* Diagnostic: the frame cost while the credits lag on the controls. */
	debugUI_showFPS();
}

static void gameState_controlCredits(void)
{
	if (ui_isTransitioning()) return;

	const Controller *controller = &controller::get()[menu_binding.player];

	menu::Controls menu;
	menu::control::read(&menu, controller, &menu_binding);

	if (menu.cancel) {
		game_setState(game_get(), MAIN_MENU);
		ui_play(&credits_enter, true);
		return;
	}

	/* Held, not tapped: the roll reads at the speed the button is pushed,
	   and opposite directions cancel. */
	credits_ui_setScrollVelocity(
		(menu.down_held - menu.up_held) * CREDITS_SCROLL_SPEED);
}



/* --- gameplay ----------------------------------------------------------- */

static void gameState_enterGameplay(void)
{
	/* The ear on the camera: heard from where it is seen. */
	sound_setListenerMode(SOUND_LISTENER_CAMERA);

	stamina_wheel_load();
	ui_play(&gameplay_enter, false);
}
static void gameState_exitGameplay(void)  { stamina_wheel_unload(); }

static void gameState_updateGameplay(void)
{
	Viewport *viewport = viewport_get();
	float delta = time_get()->delta;
	uint8_t fb_index = viewport->fb_index;

	menu::control::update();

	for (int i = 0; i < PLAYER_COUNT; i++)
		player::setCharacter3DControl((PlayerID)i, viewport);
	player::update();

	physics_update(scene3d_getPhysics(), delta);
	propSound_update(scene3d_getPhysics());
	water_update(delta);

	scene3d_updateCharacters(fb_index);

	/* Simulated props are placed by the solver, so their matrix comes from the
	   body. Static ones keep the one the load wrote. */
	Scene3D *scene = scene3d_get();
	for (int i = 0; i < scene->entity_count; i++)
		entity3d::setMatrixFromBody(scene->entity[i], fb_index);

	particles_update(fb_index);

	camera::control::update(&viewport->camera, viewport->camera.binding, scene, delta);
	viewport_setPerspectiveCamera();

	ui_update(NULL);

	debugUI_showFPS();
}

static void gameState_controlGameplay(void)
{
	menu::Controls menu;
	menu::control::read(&menu, &controller::get()[menu_binding.player], &menu_binding);

	/* The pause opens with its own transition, so this one has none. */
	if (menu.pause) game_setState(game_get(), GAMEPLAY3D_PAUSE);
}


/* --- gameplay 2D -------------------------------------------------------- */

/* Which stage is being played, and which edge the body left the last one
   through: walking off the right comes in on the next stage's left, walking
   off the left comes in on the previous stage's right. */
static uint8_t gameplay2d_stage;
static bool    gameplay2d_enter_from_right;

/* The one place a 2D stage is opened. The state declares no scene, so the
   engine loads none: a single stage is in memory at a time, and crossing an
   edge frees it before this opens the next.

   The body is seated here too: gameState_load only binds a character for a
   3D scene. Coming in from the right it is carried to that stage's return
   position, and the camera plants itself on it on its first update. */
static void gameState_loadGameplay2DStage(uint8_t stage)
{
	gameplay2d_stage = stage;

	const Scene2DDef *scene = demoStage_getScene(stage);
	scene2d_load(scene);
	controls::bind2D(&demo_controls, scene);

	Character2D *character = scene2d_getCharacter2D(0);

	if (gameplay2d_enter_from_right) {
		character->position = demoStage_getReturn(stage);
		gameplay2d_enter_from_right = false;
	}
}

static void gameState_enterGameplay2D(void)
{
	gameState_loadGameplay2DStage(gameplay2d_stage);
}

static void gameState_exitGameplay2D(void)
{
	player::init();
	scene2d_unload();
}

/* The stages run in table order, both ways, wrapping at the ends. */
static void gameState_changeStage2D(bool forward)
{
	uint8_t count = demoStage_getCount();
	uint8_t next  = forward
		? (gameplay2d_stage + 1) % count
		: (gameplay2d_stage + count - 1) % count;

	scene2d_unload();
	gameState_loadGameplay2DStage(next);
}

static void gameState_updateGameplay2D(void)
{
	float delta = time_get()->delta;

	menu::control::update();

	for (int i = 0; i < PLAYER_COUNT; i++)
		player::setCharacter2DControl((PlayerID)i);
	player::update();

	scene2d_updateCharacters(delta);

	Scene2D     *scene     = scene2d_get();
	Character2D *character = scene2d_getCharacter2D(0);

	scene2d_updateCamera(character, delta);

	/* Off an edge of the stage, which are the camera's limits, the
	   neighbouring one takes over. The state does not change: only the stage
	   under it does. */
	if (character->position.x > scene->camera.limit[camera2d::CAMERA2D_SIDE_RIGHT]) {
		gameplay2d_enter_from_right = false;
		gameState_changeStage2D(true);
	}
	else if (character->position.x < scene->camera.limit[camera2d::CAMERA2D_SIDE_LEFT]) {
		gameplay2d_enter_from_right = true;
		gameState_changeStage2D(false);
	}

	debugUI_showFPS();
}

static void gameState_controlGameplay2D(void)
{
	const Controller *controller = &controller::get()[menu_binding.player];

	menu::Controls menu;
	menu::control::read(&menu, controller, &menu_binding);

	if (menu.pause) game_setState(game_get(), MAIN_MENU);
}


/* --- pause -------------------------------------------------------------- */

static void gameState_enterPause(void)
{
	ui_play(&pause_enter, false);
}

static void gameState_updatePause(void)
{
	Scene3D *scene = scene3d_get();

	menu::control::update();

	/* Matrices are per framebuffer and gameplay only writes the current one, so
	   the three hold three different instants. Frozen, that reads as a shake:
	   everything that moves has to fill all three. */
	for (int fb = 0; fb < FB_COUNT; fb++) {
		for (int i = 0; i < scene->character3d_count; i++)
			entity3d::setMatrix(scene->character[i]->entity, fb);

		for (int i = 0; i < scene->entity_count; i++)
			entity3d::setMatrixFromBody(scene->entity[i], fb);
	}

	ui_update(&pause_idle);
}

static void gameState_controlPause(void)
{
	if (ui_isTransitioning()) return;

	menu::Controls menu;
	menu::control::read(&menu, &controller::get()[menu_binding.player], &menu_binding);

	if (menu.pause || menu.cancel || (menu.confirm && menuStack_getIndex() == 0)) {
		game_setState(game_get(), GAMEPLAY3D);
		ui_play(&pause_enter, true);
		menuStack_setIndex(0);
		return;
	}

	if (menu.confirm && menuStack_getIndex() == 1) {
		game_setState(game_get(), MAIN_MENU);
		ui_play(&pause_quit, false);
		menuStack_setIndex(0);
		return;
	}

	if (menu.up)   menuStack_moveIndex(-1, 1);
	if (menu.down) menuStack_moveIndex(1,  1);
}


/* --- game over ---------------------------------------------------------- */

static void gameState_updateGameOver(void)
{
}


/* --- the table ---------------------------------------------------------- */

const GameStateDef game_states[STATE_COUNT] = {

	[INTRO] = {
		.update     = gameState_updateIntro,
		.onEnter    = gameState_enterIntro,
		.canLeave   = gameState_canLeave,
		.scene2d    = &intro_scene2d,
		.viewport   = SCREEN_320x240,
	},

	[MAIN_MENU] = {
		.update     = gameState_updateMainMenu,
		.onEnter    = gameState_enterMainMenu,
		.canLeave   = gameState_canLeave,
		.control    = gameState_controlMainMenu,
		.scene2d    = &main_menu_scene2d,
		.viewport   = SCREEN_320x240,
	},

	[CREDITS] = {
		.update     = gameState_updateCredits,
		.onEnter    = gameState_enterCredits,
		.canLeave   = gameState_canLeave,
		.control    = gameState_controlCredits,
		.scene2d    = &credits_scene2d,
		.viewport   = SCREEN_320x240,
	},

	[GAMEPLAY3D] = {
		.update     = gameState_updateGameplay,
		.onEnter    = gameState_enterGameplay,
		.onExit     = gameState_exitGameplay,
		.canLeave   = gameState_canLeave,
		.control    = gameState_controlGameplay,
		.scene3d    = &demo_scene,
		.scene2d    = &gameplay_scene2d,
		.controls   = &demo_controls,
		.viewport   = SCREEN_320x240,
	},

	/* No scene declared: the stage it plays is opened by the state itself, one
	   at a time, so the seat is bound there too. */
	[GAMEPLAY2D] = {
		.update     = gameState_updateGameplay2D,
		.onEnter    = gameState_enterGameplay2D,
		.onExit     = gameState_exitGameplay2D,
		.control    = gameState_controlGameplay2D,
		.viewport   = SCREEN_640x240,
	},

	[GAMEPLAY3D_PAUSE] = {
		.update     = gameState_updatePause,
		.onEnter    = gameState_enterPause,
		.canLeave   = gameState_canLeave,
		.control    = gameState_controlPause,
		.scene2d    = &pause_scene2d,
		.viewport   = SCREEN_320x240,
		.overlay_of = &game_states[GAMEPLAY3D],
	},

	[GAME_OVER] = {
		.update     = gameState_updateGameOver,
		.viewport   = SCREEN_320x240,
	},

};
