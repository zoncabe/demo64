/*
	The game's state table: what each state runs, what it draws and what it
	does with the controller. The engine brings the machinery and plays the
	frame of what a state declares; everything named here is content.

	A state leaves by naming where it goes and playing its way out. Every
	state with a screen holds the switch back until that animation ends.
*/
#include <libdragon.h>

#include "time/e64_time.h"
#include "scene3d/e64_scene3d.h"
#include "scene/scene3d.h"
#include "scene/scene2d.h"
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
#include "menu/e64_menu_control.h"
#include "player/e64_player.h"
#include "controller/e64_controller.h"
#include "camera/e64_camera3d.h"
#include "debug/e64_debug.h"
#include "control/controller.h"
#include "camera/camera.h"
#include "sound/e64_sound.h"
#include "game/e64_game.h"
#include "viewport/demo_viewport.h"
#include "game/game_states.h"


namespace gameState {

static bool canLeave(void) { return !e64::ui::isTransitioning(); }


/* --- intro -------------------------------------------------------------- */

static void enterIntro(void)
{
	e64::ui::play(&intro_animation, false);
}

static void updateIntro(void)
{
	/* The intro is its own way out: when the animation ends, so does it. */
	e64::game::state::set(MAIN_MENU);
	e64::ui::update(NULL);
}


/* --- main menu ---------------------------------------------------------- */

static void enterMainMenu(void)
{
	e64::ui::play(&main_menu_enter, false);
}

static void updateMainMenu(void)
{
	e64::ui::update(&main_menu_idle);
}

static void controlMainMenu(void)
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

static void enterCredits(void)
{
	credits_ui_resetScroll();
	e64::ui::play(&credits_enter, false);
}


static void updateCredits(void)
{
	if (e64::ui::isTransitioning()) credits_ui_setScrollVelocity(0.0f);
	credits_ui_updateScroll(e64::time::get()->delta);
	e64::ui::update(NULL);
}

static void controlCredits(void)
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



/* --- gameplay 3D -------------------------------------------------------- */

static void enterGameplay(void)
{
	/* The ear on the camera: heard from where it is seen. */
	e64::sound::setListenerMode(e64::Sound::LISTENER_CAMERA);

	stamina_wheel_load();
	e64::ui::play(&gameplay_enter, false);
}
static void exitGameplay(void) { stamina_wheel_unload(); }

/* Only what is this game's: the world itself the engine plays from what the
   state declares. */
static void updateGameplay(void)
{
	e64::Viewport *viewport = e64::viewport::get();

	/* The d-pad cycles player 1 through the scene's characters. Read here
	   and not through a binding, since it is not a control the engine
	   offers. */
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

	e64::ui::update(NULL);

	e64::debug::ui::showFPS();
}

static void controlGameplay(void)
{
	e64::menu::Controls menu;
	e64::menu::control::read(&menu, &e64::controller::get()[menu_binding.player], &menu_binding);

	/* The pause opens with its own transition, so this one has none. */
	if (menu.pause) e64::game::state::set(PAUSE);
}


/* --- gameplay 2D -------------------------------------------------------- */

/* The screens in the order they are walked, wrapping at the ends: off the
   right edge of one comes in on the left of the next. Each brings the
   controls that name its body, and where the feet land coming back in from
   the right. The state declares the first; the others are opened here. */
static const struct {

	const e64::scene2d::Def *scene;
	const e64::controls::Def *controls;
	const e64::Vector2 *back;

} stage[] = {

	{ &scene2d_pixel_a, &controls_pixel_a, &pixel_a_return },
	{ &scene2d_pixel_b, &controls_pixel_b, &pixel_b_return },
	{ &scene2d_industry, &controls_industry, &industry_return },
	{ &scene2d_line, &controls_line, &line_return },
};

#define STAGE_COUNT (sizeof(stage) / sizeof(stage[0]))

/* Which screen is up, which one the fade is heading to (-1 for none), and
   whether the body left through the left edge. */
static uint8_t gameplay2d_stage;
static int8_t gameplay2d_next = -1;
static bool gameplay2d_from_right;

static void enterGameplay2D(void)
{
	/* The engine opened the screen the state declares: the first. */
	gameplay2d_stage = 0;
	gameplay2d_next = -1;

	e64::ui::play(&gameplay_enter, false);
}

/* Once the screen is black. The load frees what the frame in flight may
   still be drawing, so the RDP finishes first; the load seats the player on
   the body the screen's binding names, and the time it took is no frame. */
static void changeStage2D(void)
{
	rspq_wait();
	e64::scene2d::load(stage[gameplay2d_next].scene, stage[gameplay2d_next].controls);

	if (gameplay2d_from_right)
		e64::scene2d::getCharacter2D(0)->position = *stage[gameplay2d_next].back;

	gameplay2d_stage = gameplay2d_next;
	gameplay2d_next = -1;

	e64::time::reset();
	e64::ui::play(&gameplay_enter, false);
}

static void updateGameplay2D(void)
{
	e64::ui::update(NULL);

	/* The fade runs to black before the switch. The check goes after the
	   animation's update, so the frame the fade ends on is drawn from the
	   new screen's fade, not from the old screen bare. */
	if (gameplay2d_next >= 0) {
		if (!e64::ui::isTransitioning()) changeStage2D();
	}
	else {
		const e64::Scene2D *scene = e64::scene2d::get();
		const e64::Character2D *character = e64::scene2d::getCharacter2D(0);

		/* Off an edge of the stage, which are the camera's limits, the
		   neighbouring one takes over: once the whole body is past it, since
		   the view stops at the limit and the body is held there for the
		   fade. The entity is the drawn frame, centred on the feet. */
		const e64::Entity2D *drawn = character->entity;
		float left = drawn->position.x;
		float right = left + drawn->graphic->sprite.asset->width * drawn->scale.x;

		if (left > scene->camera.limit[e64::camera2d::CAMERA2D_SIDE_RIGHT]) {
			gameplay2d_next = (gameplay2d_stage + 1) % STAGE_COUNT;
			gameplay2d_from_right = false;
		}
		else if (right < scene->camera.limit[e64::camera2d::CAMERA2D_SIDE_LEFT]) {
			gameplay2d_next = (gameplay2d_stage + STAGE_COUNT - 1) % STAGE_COUNT;
			gameplay2d_from_right = true;
		}

		/* Past the edge there is no floor: driven on, the body would fall
		   through the fade and drag the view down with it. Nobody drives it
		   until the next screen's load seats the player again. */
		if (gameplay2d_next >= 0) {
			e64::player::init();
			e64::ui::play(&gameplay_enter, true);
		}
	}
}

static void controlGameplay2D(void)
{
	e64::menu::Controls menu;
	e64::menu::control::read(&menu, &e64::controller::get()[menu_binding.player], &menu_binding);

	/* The pause opens with its own transition, so this one has none. */
	if (menu.pause) e64::game::state::set(PAUSE);
}


/* --- pause -------------------------------------------------------------- */

/* One pause over either game: continuing goes back to whichever it was
   opened from, which the engine remembers. */
static void enterPause(void)
{
	/* The index is the menus', shared: it still holds the main menu's pick. */
	e64::menu::setIndex(0);
	e64::ui::play(&pause_enter, false);
}

static void updatePause(void)
{
	e64::Scene3D *scene = e64::scene3d::get();

	/* Matrices are per framebuffer and gameplay only writes the current one, so
	   the three hold three different instants. Frozen, that reads as a shake:
	   everything that moves has to fill all three. Over the 2D game the lists
	   are empty and this does nothing. */
	for (int fb = 0; fb < e64::Viewport::FB_COUNT; fb++) {
		for (int i = 0; i < scene->character3d_count; i++)
			e64::entity3d::setMatrix(scene->character[i]->entity, fb);

		for (int i = 0; i < scene->entity_count; i++)
			e64::entity3d::setMatrixFromBody(scene->entity[i], fb);
	}

	e64::ui::update(&pause_idle);
}

static void controlPause(void)
{
	if (e64::ui::isTransitioning()) return;

	e64::menu::Controls menu;
	e64::menu::control::read(&menu, &e64::controller::get()[menu_binding.player], &menu_binding);

	if (menu.pause || menu.cancel || (menu.confirm && e64::menu::getIndex() == 0)) {
		e64::game::state::set(e64::game::get()->state.base);
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

static const e64::Game::State::Def *const pause_bases[] = {
	&table[GAMEPLAY3D],
	&table[GAMEPLAY2D],
};


/* --- game over ---------------------------------------------------------- */

static void updateGameOver(void)
{
}


/* --- the table ---------------------------------------------------------- */

const e64::Game::State::Def table[COUNT] = {

	[INTRO] = {
		.update = updateIntro,
		.onEnter = enterIntro,
		.canLeave = canLeave,
		.ui = &intro_ui,
		.viewport = &viewport_320x240,
	},

	[MAIN_MENU] = {
		.update = updateMainMenu,
		.onEnter = enterMainMenu,
		.canLeave = canLeave,
		.control = controlMainMenu,
		.ui = &main_menu_ui,
		.viewport = &viewport_320x240,
	},

	[CREDITS] = {
		.update = updateCredits,
		.onEnter = enterCredits,
		.canLeave = canLeave,
		.control = controlCredits,
		.ui = &credits_ui,
		.viewport = &viewport_320x240,
	},

	[GAMEPLAY3D] = {
		.update = updateGameplay,
		.onEnter = enterGameplay,
		.onExit = exitGameplay,
		.canLeave = canLeave,
		.control = controlGameplay,
		.scene3d = &scene3d,
		.ui = &gameplay_ui,
		.controls = &controls,
		.viewport = &viewport_320x240,
	},

	/* The first screen is the state's; the update opens the next ones in its
	   place, each with its own controls. */
	[GAMEPLAY2D] = {
		.update = updateGameplay2D,
		.onEnter = enterGameplay2D,
		.canLeave = canLeave,
		.control = controlGameplay2D,
		.scene2d = &scene2d_pixel_a,
		.ui = &gameplay_ui,
		.controls = &controls_pixel_a,
		.viewport = &viewport_640x240,
	},

	[PAUSE] = {
		.update = updatePause,
		.onEnter = enterPause,
		.canLeave = canLeave,
		.control = controlPause,
		.ui = &pause_ui,
		.viewport = &viewport_320x240,
		.overlay_of = pause_bases,
		.overlay_count = sizeof(pause_bases) / sizeof(pause_bases[0]),
	},

	[GAME_OVER] = {
		.update = updateGameOver,
		.viewport = &viewport_320x240,
	},

};

}
