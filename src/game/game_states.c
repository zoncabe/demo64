/*
	The demo's state table: what each state runs, what it draws and what it
	does with the controller. The engine brings the machinery; everything named here
	is content.

	A state leaves by naming where it goes and playing its way out. Every
	state with a screen holds the switch back until that animation ends.
*/
#include "resources/e64_resources.h"
#include "assets/graphics/sprites.h"
#include "assets/graphics/fonts.h"
#include "time/e64_time.h"
#include "scene3d/e64_scene3d.h"
#include "scene/scene.h"
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
#include "control/e64_player_control.h"
#include "control/e64_controller.h"
#include "control/e64_camera_control.h"
#include "control/controller.h"
#include "camera/camera.h"
#include "sound/e64_sound.h"
#include "sound/e64_prefab_sound.h"
#include "game/e64_game.h"
#include "viewport/e64_viewport.h"
#include "game/game_states.h"


static bool gameState_canLeave(void) { return !ui_isTransitioning(); }


/* --- intro -------------------------------------------------------------- */

static void gameState_enterIntro(void)
{
	ui_play(&intro_animation, false);
}

static void gameState_updateIntro(GameContext *ctx)
{
	/* The intro is its own way out: when the animation ends, so does it. */
	game_setState(ctx->game, DEMO_STATE_MAIN_MENU);
	ui_update(NULL);
}


/* --- main menu ---------------------------------------------------------- */

static void gameState_enterMainMenu(void)
{
	ui_play(&main_menu_enter, false);
}

static void gameState_updateMainMenu(GameContext *ctx)
{
	(void)ctx;
	ui_update(&main_menu_idle);
}

static void gameState_controlMainMenu(Game *game)
{
	if (ui_isTransitioning()) return;

	MenuControls menu;
	menuControls_map(&menu, &controller_get()[menu_binding.player], &menu_binding);

	if (menu.confirm) {
		int8_t idx = menuStack_getIndex();
		if (idx == 0) game_setState(game, DEMO_STATE_GAMEPLAY);
		if (idx == 1) game_setState(game, DEMO_STATE_CREDITS);
		if (idx == 0 || idx == 1) ui_play(&main_menu_enter, true);
	}
	if (menu.up)   menuStack_moveIndex(-1, 1);
	if (menu.down) menuStack_moveIndex(1,  1);
}


/* --- credits ------------------------------------------------------------ */

static void gameState_enterCredits(void)
{
	credits_ui_resetScroll();
	ui_play(&credits_enter, false);
}


static void gameState_updateCredits(GameContext *ctx)
{
	(void)ctx;
	if (ui_isTransitioning()) credits_ui_setScrollVelocity(0.0f);
	credits_ui_updateScroll(time_get()->delta);
	ui_update(NULL);
}

static void gameState_controlCredits(Game *game)
{
	if (ui_isTransitioning()) return;

	const Controller *controller = &controller_get()[menu_binding.player];

	MenuControls menu;
	menuControls_map(&menu, controller, &menu_binding);

	if (menu.cancel) {
		game_setState(game, DEMO_STATE_MAIN_MENU);
		ui_play(&credits_enter, true);
		return;
	}

	/* Held, not tapped: the roll reads at the speed the button is pushed,
	   and opposite directions cancel. */
	credits_ui_setScrollVelocity(
		(menu.down_held - menu.up_held) * CREDITS_SCROLL_SPEED);
}



/* --- gameplay ----------------------------------------------------------- */

static void gameState_bindGameplayCharacter(void)
{
	player_setCharacter(scene3d_getCharacter(0), &character_binding);
}

static void gameState_enterGameplay(void) { ui_play(&gameplay_enter, false); }
static void gameState_exitGameplay(void)  { stamina_wheel_reset(); }

static void gameState_updateGameplay(GameContext *ctx)
{
	float delta = time_get()->delta;
	uint8_t fb_index = ctx->viewport->fb_index;

	for (int i = 0; i < PLAYER_COUNT; i++)
		player_setCharacterControl(i, ctx->viewport);
	player_update();

	physics_update(scene3d_getPhysics(), delta);
	prefabSound_update(scene3d_getPhysics());
	water_update(delta);

	scene3d_updateCharacters(fb_index);

	/* Simulated props are placed by the solver, so their matrix comes from the
	   body. Static ones keep the one the load wrote. */
	Scene3D *scene = scene3d_get();
	for (int i = 0; i < scene->entity_count; i++)
		entity_setMatrixFromBody(scene->entity[i], fb_index);

	particles_update(ctx, fb_index);

	cameraControl_update(&ctx->viewport->camera, &camera_binding, delta);
	viewport_setPerspectiveCamera();

	ui_update(NULL);
}

static void gameState_controlGameplay(Game *game)
{
	MenuControls menu;
	menuControls_map(&menu, &controller_get()[menu_binding.player], &menu_binding);

	/* The pause opens with its own transition, so this one has none. */
	if (menu.pause) game_setState(game, DEMO_STATE_PAUSE);
}


/* --- pause -------------------------------------------------------------- */

static void gameState_enterPause(void)
{
	ui_play(&pause_enter, false);
}

static void gameState_updatePause(GameContext *ctx)
{
	(void)ctx;
	Scene3D *scene = scene3d_get();

	/* Matrices are per framebuffer and gameplay only writes the current one, so
	   the three hold three different instants. Frozen, that reads as a shake:
	   everything that moves has to fill all three. */
	for (int fb = 0; fb < FB_COUNT; fb++) {
		for (int i = 0; i < scene->character_count; i++)
			entity_setMatrix(scene->character[i]->entity, fb);

		for (int i = 0; i < scene->entity_count; i++)
			entity_setMatrixFromBody(scene->entity[i], fb);
	}

	ui_update(&pause_idle);
}

static void gameState_controlPause(Game *game)
{
	if (ui_isTransitioning()) return;

	MenuControls menu;
	menuControls_map(&menu, &controller_get()[menu_binding.player], &menu_binding);

	if (menu.pause || menu.cancel || (menu.confirm && menuStack_getIndex() == 0)) {
		game_setState(game, DEMO_STATE_GAMEPLAY);
		ui_play(&pause_enter, true);
		menuStack_setIndex(0);
		return;
	}

	if (menu.confirm && menuStack_getIndex() == 1) {
		game_setState(game, DEMO_STATE_MAIN_MENU);
		ui_play(&pause_quit, false);
		menuStack_setIndex(0);
		return;
	}

	if (menu.up)   menuStack_moveIndex(-1, 1);
	if (menu.down) menuStack_moveIndex(1,  1);
}


/* --- game over ---------------------------------------------------------- */

static void gameState_updateGameOver(GameContext *ctx)
{
	(void)ctx;
}


/* --- resources ---------------------------------------------------------- */

#define RESOURCES(spr, fnt) { spr, sizeof(spr)/sizeof(*spr), fnt, sizeof(fnt)/sizeof(*fnt) }
#define NO_RESOURCES        { NULL, 0, NULL, 0 }

static const SpriteID intro_sprite[] = {
	SPRITE_LIBDRAGON, SPRITE_TINY3D,
};

static const uint8_t intro_font[] = { DROID_SANS, XOLONIUM_60 };

static const SpriteID mainmenu_sprite[] = {
	SPRITE_GORILLA, SPRITE_BTN_A, SPRITE_BTN_B, SPRITE_D_UP, SPRITE_D_DOWN, SPRITE_D_LEFT, SPRITE_D_RIGHT,
};

static const uint8_t mainmenu_font[] = {
	DROID_SANS, XOLONIUM_14, XOLONIUM_20, XOLONIUM_40,
};

static const SpriteID credits_sprite[] = {
	SPRITE_BTN_B, SPRITE_D_UP, SPRITE_D_DOWN,
};

static const uint8_t credits_font[] = {
	DROID_SANS, XOLONIUM_14, XOLONIUM_40,
};

/* The pause overlays gameplay and can open on any frame: its assets ride
   the gameplay set, loaded and freed with it, so no heap traffic happens
   mid-session. */
static const SpriteID gameplay_sprite[] = {
	SPRITE_CIRCLE_MASK, SPRITE_CIRCLE_PROGRESS,
	SPRITE_BTN_A, SPRITE_BTN_B, SPRITE_D_UP, SPRITE_D_DOWN, SPRITE_D_LEFT, SPRITE_D_RIGHT,
};

static const uint8_t gameplay_font[] = { DROID_SANS, XOLONIUM_14, XOLONIUM_20 };


/* --- the table ---------------------------------------------------------- */

const GameStateDef demo_states[DEMO_STATE_COUNT] = {

	[DEMO_STATE_INTRO] = {
		.update     = gameState_updateIntro,
		.onEnter    = gameState_enterIntro,
		.canLeave   = gameState_canLeave,
		.resources  = RESOURCES(intro_sprite, intro_font),
		.scene2d    = &intro_scene2d,
		.overlay_of = GAME_STATE_NONE,
	},

	[DEMO_STATE_MAIN_MENU] = {
		.update     = gameState_updateMainMenu,
		.onEnter    = gameState_enterMainMenu,
		.canLeave   = gameState_canLeave,
		.control    = gameState_controlMainMenu,
		.resources  = RESOURCES(mainmenu_sprite, mainmenu_font),
		.scene2d    = &main_menu_scene2d,
		.overlay_of = GAME_STATE_NONE,
	},

	[DEMO_STATE_CREDITS] = {
		.update     = gameState_updateCredits,
		.onEnter    = gameState_enterCredits,
		.canLeave   = gameState_canLeave,
		.control    = gameState_controlCredits,
		.resources  = RESOURCES(credits_sprite, credits_font),
		.scene2d    = &credits_scene2d,
		.overlay_of = GAME_STATE_NONE,
	},

	[DEMO_STATE_GAMEPLAY] = {
		.update        = gameState_updateGameplay,
		.bindCharacter = gameState_bindGameplayCharacter,
		.onEnter       = gameState_enterGameplay,
		.onExit        = gameState_exitGameplay,
		.canLeave      = gameState_canLeave,
		.control       = gameState_controlGameplay,
		.resources     = RESOURCES(gameplay_sprite, gameplay_font),
		.scene3d       = &demo_scene,
		.scene2d       = &gameplay_scene2d,
		.overlay_of    = GAME_STATE_NONE,
	},

	[DEMO_STATE_PAUSE] = {
		.update     = gameState_updatePause,
		.onEnter    = gameState_enterPause,
		.canLeave   = gameState_canLeave,
		.control    = gameState_controlPause,
		.resources  = NO_RESOURCES,
		.scene2d    = &pause_scene2d,
		.overlay_of = DEMO_STATE_GAMEPLAY,
	},

	[DEMO_STATE_GAME_OVER] = {
		.update     = gameState_updateGameOver,
		.resources  = NO_RESOURCES,
		.overlay_of = GAME_STATE_NONE,
	},

};
