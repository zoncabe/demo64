/*
	The demo's state table: what each state runs, what it draws and what it
	does with the controller. The engine brings the machinery; everything named here
	is content.

	A state leaves by naming where it goes and playing its way out. Every
	state with a screen holds the switch back until that animation ends.
*/
#include "time/e64_time.h"
#include "scene3d/e64_scene3d.h"
#include "physics/e64_physics.h"
#include "scene/scene.h"
#include "scene/demo_scene2d.h"
#include "scene2d/e64_scene2d.h"
#include "render/e64_render.h"
#include "ui/e64_ui.h"
#include "cutscene/intro.h"
#include "ui/main_menu_ui.h"
#include "ui/pause_ui.h"
#include "ui/gameplay_ui.h"
#include "ui/credits_ui.h"
#include "ui/stamina_wheel.h"
#include "menu/e64_menu.h"
#include "particles/e64_particles.h"
#include "shaders/e64_water.h"
#include "player/e64_player.h"
#include "player/e64_player_control.h"
#include "controller/e64_controller.h"
#include "camera/e64_camera3d_control.h"
#include "debug/e64_debug.h"
#include "control/controller.h"
#include "camera/camera.h"
#include "sound/e64_sound.h"
#include "sound/e64_prop_sound.h"
#include "game/e64_game.h"
#include "viewport/e64_viewport.h"
#include "game/game_states.h"


static bool gameState_canLeave(void) { return !e64::ui::isTransitioning(); }


/* --- intro -------------------------------------------------------------- */

static void gameState_enterIntro(void)
{
	e64::ui::play(&intro_animation, false);
}

static void gameState_updateIntro(void)
{
	/* The intro is its own way out: when the animation ends, so does it. */
	e64::game::state::set(MAIN_MENU);
	e64::ui::update(NULL);
}


/* --- main menu ---------------------------------------------------------- */

static void gameState_enterMainMenu(void)
{
	e64::ui::play(&main_menu_enter, false);
}

static void gameState_updateMainMenu(void)
{
	e64::menu::control::update();
	e64::ui::update(&main_menu_idle);
}

static void gameState_controlMainMenu(void)
{
	if (e64::ui::isTransitioning()) return;

	e64::menu::Controls menu;
	e64::menu::control::read(&menu, &e64::controller::get()[menu_binding.player], &menu_binding);

	if (menu.confirm) {
		int8_t idx = e64::menu::getIndex();
		if (idx == 0) e64::game::state::set(GAMEPLAY3D);
		if (idx == 1) e64::game::state::set(GAMEPLAY2D);
		if (idx == 2) e64::game::state::set(CREDITS);
		if (idx <= 2) e64::ui::play(&main_menu_enter, true);
	}
	if (menu.up) e64::menu::moveIndex(-1, 2);
	if (menu.down) e64::menu::moveIndex(1, 2);
}


/* --- credits ------------------------------------------------------------ */

static void gameState_enterCredits(void)
{
	credits_ui_resetScroll();
	e64::ui::play(&credits_enter, false);
}


static void gameState_updateCredits(void)
{
	e64::menu::control::update();
	if (e64::ui::isTransitioning()) credits_ui_setScrollVelocity(0.0f);
	credits_ui_updateScroll(e64::time::get()->delta);
	e64::ui::update(NULL);

	/* Diagnostic: the frame cost while the credits lag on the controls. */
	e64::debug::ui::showFPS();
}

static void gameState_controlCredits(void)
{
	if (e64::ui::isTransitioning()) return;

	const e64::Controller *controller = &e64::controller::get()[menu_binding.player];

	e64::menu::Controls menu;
	e64::menu::control::read(&menu, controller, &menu_binding);

	if (menu.cancel) {
		e64::game::state::set(MAIN_MENU);
		e64::ui::play(&credits_enter, true);
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
	e64::sound::setListenerMode(e64::Sound::LISTENER_CAMERA);

	stamina_wheel_load();
	e64::ui::play(&gameplay_enter, false);
}
static void gameState_exitGameplay(void) { stamina_wheel_unload(); }

static void gameState_updateGameplay(void)
{
	e64::Viewport *viewport = e64::viewport::get();
	float delta = e64::time::get()->delta;
	uint8_t fb_index = viewport->fb_index;

	e64::menu::control::update();

	/* Demo only: the d-pad cycles player 1 through the scene's characters.
	   Read here and not through a binding, since it is not a control the
	   engine offers. */
	{
		const e64::Controller *pad = &e64::controller::get()[e64::PLAYER_1];
		int8_t direction = e64::controller::isPressed(pad, e64::BTN_D_UP) ? +1
		                 : e64::controller::isPressed(pad, e64::BTN_D_DOWN) ? -1 : 0;

		/* The camera glides from the body being left instead of cutting:
		   the blend starts at its position and the camera's own update
		   carries it to the new centre over half a second. */
		e64::Player *seat = &e64::player::get()[e64::PLAYER_1];
		if (direction && seat->entity) {
			e64::camera3d::setViewTarget(&viewport->camera, &seat->entity->transform.position, 0.5f);
			e64::player::switchCharacter3D(e64::PLAYER_1, direction);
		}
	}

	for (int i = 0; i < e64::PLAYER_COUNT; i++)
		e64::player::setCharacter3DControl((e64::PlayerID)i, viewport);
	e64::player::update();

	e64::physics::update(e64::scene3d::getPhysics(), delta);
	e64::propSound::update(e64::scene3d::getPhysics());
	e64::water::update(delta);

	e64::scene3d::updateCharacters(fb_index);

	/* Simulated props are placed by the solver, so their matrix comes from the
	   body. Static ones keep the one the load wrote. */
	e64::Scene3D *scene = e64::scene3d::get();
	for (int i = 0; i < scene->entity_count; i++)
		e64::entity3d::setMatrixFromBody(scene->entity[i], fb_index);

	e64::particles::update(fb_index);

	e64::camera3d::control::update(&viewport->camera, viewport->camera.binding, scene, delta);
	e64::viewport::setPerspectiveCamera();

	e64::ui::update(NULL);

	e64::debug::ui::showFPS();
}

static void gameState_controlGameplay(void)
{
	e64::menu::Controls menu;
	e64::menu::control::read(&menu, &e64::controller::get()[menu_binding.player], &menu_binding);

	/* The pause opens with its own transition, so this one has none. */
	if (menu.pause) e64::game::state::set(GAMEPLAY3D_PAUSE);
}


/* --- gameplay 2D -------------------------------------------------------- */

/* Which stage is being played, and which edge the body left the last one
   through: walking off the right comes in on the next stage's left, walking
   off the left comes in on the previous stage's right. */
static uint8_t gameplay2d_stage;
static bool gameplay2d_enter_from_right;

/* The one place a 2D stage is opened. The state declares no scene, so the
   engine loads none: a single stage is in memory at a time, and crossing an
   edge frees it before this opens the next.

   The body is seated here too: the 2D binding names no scene entity, since
   one layout serves the four stages. Coming in from the right it is carried
   to that stage's return position, and the camera plants itself on it on
   its first update. */
static void gameState_loadGameplay2DStage(uint8_t stage)
{
	gameplay2d_stage = stage;

	const e64::scene2d::Def *scene = stage_getScene(stage);
	e64::scene2d::load(scene, &controls);

	e64::Character2D *character = e64::scene2d::getCharacter2D(0);
	e64::player::setCharacter2D(character, &character2d_binding);

	if (gameplay2d_enter_from_right) {
		character->position = stage_getReturn(stage);
		gameplay2d_enter_from_right = false;
	}
}

static void gameState_enterGameplay2D(void)
{
	gameState_loadGameplay2DStage(gameplay2d_stage);
}

static void gameState_exitGameplay2D(void)
{
	e64::player::init();
	e64::scene2d::unload();
}

/* The stages run in table order, both ways, wrapping at the ends. */
static void gameState_changeStage2D(bool forward)
{
	uint8_t count = stage_getCount();
	uint8_t next = forward
		? (gameplay2d_stage + 1) % count
		: (gameplay2d_stage + count - 1) % count;

	e64::scene2d::unload();
	gameState_loadGameplay2DStage(next);
}

static void gameState_updateGameplay2D(void)
{
	float delta = e64::time::get()->delta;

	e64::menu::control::update();

	for (int i = 0; i < e64::PLAYER_COUNT; i++)
		e64::player::setCharacter2DControl((e64::PlayerID)i);
	e64::player::update();

	e64::scene2d::updateCharacters(delta);

	e64::Scene2D *scene = e64::scene2d::get();
	e64::Character2D *character = e64::scene2d::getCharacter2D(0);

	e64::scene2d::updateCamera(character, delta);

	/* Off an edge of the stage, which are the camera's limits, the
	   neighbouring one takes over. The state does not change: only the stage
	   under it does. */
	if (character->position.x > scene->camera.limit[e64::camera2d::CAMERA2D_SIDE_RIGHT]) {
		gameplay2d_enter_from_right = false;
		gameState_changeStage2D(true);
	}
	else if (character->position.x < scene->camera.limit[e64::camera2d::CAMERA2D_SIDE_LEFT]) {
		gameplay2d_enter_from_right = true;
		gameState_changeStage2D(false);
	}

	e64::debug::ui::showFPS();
}

static void gameState_controlGameplay2D(void)
{
	const e64::Controller *controller = &e64::controller::get()[menu_binding.player];

	e64::menu::Controls menu;
	e64::menu::control::read(&menu, controller, &menu_binding);

	if (menu.pause) e64::game::state::set(MAIN_MENU);
}


/* --- pause -------------------------------------------------------------- */

static void gameState_enterPause(void)
{
	e64::ui::play(&pause_enter, false);
}

static void gameState_updatePause(void)
{
	e64::Scene3D *scene = e64::scene3d::get();

	e64::menu::control::update();

	/* Matrices are per framebuffer and gameplay only writes the current one, so
	   the three hold three different instants. Frozen, that reads as a shake:
	   everything that moves has to fill all three. */
	for (int fb = 0; fb < e64::Viewport::FB_COUNT; fb++) {
		for (int i = 0; i < scene->character3d_count; i++)
			e64::entity3d::setMatrix(scene->character[i]->entity, fb);

		for (int i = 0; i < scene->entity_count; i++)
			e64::entity3d::setMatrixFromBody(scene->entity[i], fb);
	}

	e64::ui::update(&pause_idle);
}

static void gameState_controlPause(void)
{
	if (e64::ui::isTransitioning()) return;

	e64::menu::Controls menu;
	e64::menu::control::read(&menu, &e64::controller::get()[menu_binding.player], &menu_binding);

	if (menu.pause || menu.cancel || (menu.confirm && e64::menu::getIndex() == 0)) {
		e64::game::state::set(GAMEPLAY3D);
		e64::ui::play(&pause_enter, true);
		e64::menu::setIndex(0);
		return;
	}

	if (menu.confirm && e64::menu::getIndex() == 1) {
		e64::game::state::set(MAIN_MENU);
		e64::ui::play(&pause_quit, false);
		e64::menu::setIndex(0);
		return;
	}

	if (menu.up) e64::menu::moveIndex(-1, 1);
	if (menu.down) e64::menu::moveIndex(1, 1);
}


/* --- game over ---------------------------------------------------------- */

static void gameState_updateGameOver(void)
{
}


/* --- the table ---------------------------------------------------------- */

const e64::Game::State::Def game_states[STATE_COUNT] = {

	[INTRO] = {
		.update = gameState_updateIntro,
		.onEnter = gameState_enterIntro,
		.canLeave = gameState_canLeave,
		.scene2d = &intro_scene2d,
		.viewport = SCREEN_320x240,
	},

	[MAIN_MENU] = {
		.update = gameState_updateMainMenu,
		.onEnter = gameState_enterMainMenu,
		.canLeave = gameState_canLeave,
		.control = gameState_controlMainMenu,
		.scene2d = &main_menu_scene2d,
		.viewport = SCREEN_320x240,
	},

	[CREDITS] = {
		.update = gameState_updateCredits,
		.onEnter = gameState_enterCredits,
		.canLeave = gameState_canLeave,
		.control = gameState_controlCredits,
		.scene2d = &credits_scene2d,
		.viewport = SCREEN_320x240,
	},

	[GAMEPLAY3D] = {
		.update = gameState_updateGameplay,
		.onEnter = gameState_enterGameplay,
		.onExit = gameState_exitGameplay,
		.canLeave = gameState_canLeave,
		.control = gameState_controlGameplay,
		.scene3d = &scene3d,
		.scene2d = &gameplay_scene2d,
		.controls = &controls,
		.viewport = SCREEN_320x240,
	},

	/* No scene declared: the stage it plays is opened by the state itself, one
	   at a time, so the seat is bound there too. */
	[GAMEPLAY2D] = {
		.update = gameState_updateGameplay2D,
		.onEnter = gameState_enterGameplay2D,
		.onExit = gameState_exitGameplay2D,
		.control = gameState_controlGameplay2D,
		.viewport = SCREEN_640x240,
	},

	[GAMEPLAY3D_PAUSE] = {
		.update = gameState_updatePause,
		.onEnter = gameState_enterPause,
		.canLeave = gameState_canLeave,
		.control = gameState_controlPause,
		.scene2d = &pause_scene2d,
		.viewport = SCREEN_320x240,
		.overlay_of = &game_states[GAMEPLAY3D],
	},

	[GAME_OVER] = {
		.update = gameState_updateGameOver,
		.viewport = SCREEN_320x240,
	},

};
