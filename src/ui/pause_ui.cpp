#ifndef ENGINE_ULTRA64
#include <libdragon.h>
#endif

#include "assets/graphics/fonts.h"
#include ENGINE_HEADER(graphics, shapes)
#include "ui/pause_ui.h"


/* The items align on their right edge: same X and same width for both,
   whatever the font. */
#define PAUSE_MENU_X     222.0f
#define PAUSE_MENU_WIDTH 80

#ifdef ENGINE_ULTRA64
/* The engine's text spacing is whole pixels. */
static const TextParms h14_parms      = { .char_spacing = 1 };
static const TextParms h20_menu_parms = { .width = PAUSE_MENU_WIDTH, .align = ALIGN_RIGHT };
#else
static const rdpq_textparms_t h14_parms      = { .char_spacing = 0 };
static const rdpq_textparms_t h20_menu_parms = { .width = PAUSE_MENU_WIDTH, .align = ALIGN_RIGHT };
#endif


typedef enum {

	PAUSE_BG,
	PAUSE_CONTINUE,
	PAUSE_QUIT,
	PAUSE_HINT_MOVE,
	PAUSE_HINT_SELECT,
	PAUSE_HINT_BACK,
	PAUSE_D_UP,
	PAUSE_D_DOWN,
	PAUSE_BTN_A,
	PAUSE_BTN_B,

	PAUSE_COUNT,

} PauseEntity;


static const Prefab2D pause_prefab[PAUSE_COUNT] = {

	[PAUSE_BG]          = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_RECTANGLE, .rectangle = { .fill = SHAPE_FILL_GRADIENT } } },
	[PAUSE_CONTINUE]    = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_TEXT,      .text      = { XOLONIUM_20, MENU_STYLE_NORMAL, "Continue", &h20_menu_parms } } },
	[PAUSE_QUIT]        = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_TEXT,      .text      = { XOLONIUM_20, MENU_STYLE_NORMAL, "Quit",     &h20_menu_parms } } },
	[PAUSE_HINT_MOVE]   = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_TEXT,      .text      = { XOLONIUM_14, 0,                 "Move",     &h14_parms } } },
	[PAUSE_HINT_SELECT] = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_TEXT,      .text      = { XOLONIUM_14, 0,                 "Select",   &h14_parms } } },
	[PAUSE_HINT_BACK]   = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_TEXT,      .text      = { XOLONIUM_14, 0,                 "Back",     &h14_parms } } },
	[PAUSE_D_UP]        = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_SPRITE,    .sprite    = { .path = "rom:/sprites/core/DUp.sprite"     } } },
	[PAUSE_D_DOWN]      = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_SPRITE,    .sprite    = { .path = "rom:/sprites/core/DDown.sprite"   } } },
	[PAUSE_BTN_A]       = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_SPRITE,    .sprite    = { .path = "rom:/sprites/core/AButton.sprite" } } },
	[PAUSE_BTN_B]       = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_SPRITE,    .sprite    = { .path = "rom:/sprites/core/BButton.sprite" } } },
};

static const Scene2DPrefab pause_placed[PAUSE_COUNT] = {

	[PAUSE_BG]          = { &pause_prefab[PAUSE_BG],          {   0.0f,   0.0f }, { 320.0f, 240.0f } },
	[PAUSE_CONTINUE]    = { &pause_prefab[PAUSE_CONTINUE],    { 320.0f,  50.0f } },
	[PAUSE_QUIT]        = { &pause_prefab[PAUSE_QUIT],        { 320.0f,  80.0f } },
	[PAUSE_HINT_MOVE]   = { &pause_prefab[PAUSE_HINT_MOVE],   { 347.0f, 196.0f } },
	[PAUSE_HINT_SELECT] = { &pause_prefab[PAUSE_HINT_SELECT], { 338.0f, 211.0f } },
	[PAUSE_HINT_BACK]   = { &pause_prefab[PAUSE_HINT_BACK],   { 338.0f, 226.0f } },
	[PAUSE_D_UP]        = { &pause_prefab[PAUSE_D_UP],        { 320.0f, 186.0f }, { 0.48f, 0.48f } },
	[PAUSE_D_DOWN]      = { &pause_prefab[PAUSE_D_DOWN],      { 330.0f, 187.0f }, { 0.48f, 0.48f } },
	[PAUSE_BTN_A]       = { &pause_prefab[PAUSE_BTN_A],       { 320.0f, 201.0f }, { 0.60f, 0.60f } },
	[PAUSE_BTN_B]       = { &pause_prefab[PAUSE_BTN_B],       { 320.0f, 216.0f }, { 0.60f, 0.60f } },
};

static const Scene2DLayer pause_layer[] = {
	{ pause_placed, PAUSE_COUNT },
};

const Scene2DDef pause_scene2d = { .layer = pause_layer, .layer_count = 1 };


static const float pause_style_continue[] = { MENU_STYLE_SELECTED, MENU_STYLE_NORMAL   };
static const float pause_style_quit[]     = { MENU_STYLE_NORMAL,   MENU_STYLE_SELECTED };

static const UIAnimationTrack pause_idle_track[] = {

	{ .entity = PAUSE_CONTINUE, .field = UI_FIELD_TEXT_STYLE, .source = UI_SOURCE_MENU_INDEX, .values_by_index = pause_style_continue },
	{ .entity = PAUSE_QUIT,     .field = UI_FIELD_TEXT_STYLE, .source = UI_SOURCE_MENU_INDEX, .values_by_index = pause_style_quit     },
};

const UIAnimation pause_idle = { pause_idle_track, sizeof(pause_idle_track)/sizeof(*pause_idle_track) };


/* The panel slides in from the right while a gradient darkens that side of
   the frozen game. */
static const UIAnimationTrack pause_enter_track[] = {

	{ .entity = PAUSE_BG, .field = UI_FIELD_GRADIENT_A, .corner = 0, .from = 0.0f, .to =  76.5f, .duration = 0.16f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_BG, .field = UI_FIELD_GRADIENT_A, .corner = 1, .from = 0.0f, .to = 255.0f, .duration = 0.16f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_BG, .field = UI_FIELD_GRADIENT_A, .corner = 2, .from = 0.0f, .to = 255.0f, .duration = 0.16f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_BG, .field = UI_FIELD_GRADIENT_A, .corner = 3, .from = 0.0f, .to =  76.5f, .duration = 0.16f, .easing = UI_EASING_LINEAR },

	{ .entity = PAUSE_CONTINUE,    .field = UI_FIELD_POSITION_X, .from = 320.0f, .to = PAUSE_MENU_X, .duration = 0.12f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_QUIT,        .field = UI_FIELD_POSITION_X, .from = 320.0f, .to = PAUSE_MENU_X, .duration = 0.12f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_HINT_MOVE,   .field = UI_FIELD_POSITION_X, .from = 347.0f, .to = 262.0f, .duration = 0.12f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_HINT_SELECT, .field = UI_FIELD_POSITION_X, .from = 338.0f, .to = 262.0f, .duration = 0.12f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_HINT_BACK,   .field = UI_FIELD_POSITION_X, .from = 338.0f, .to = 262.0f, .duration = 0.12f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_D_UP,        .field = UI_FIELD_POSITION_X, .from = 320.0f, .to = 235.0f, .duration = 0.12f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_D_DOWN,      .field = UI_FIELD_POSITION_X, .from = 330.0f, .to = 245.0f, .duration = 0.12f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_BTN_A,       .field = UI_FIELD_POSITION_X, .from = 320.0f, .to = 244.0f, .duration = 0.12f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_BTN_B,       .field = UI_FIELD_POSITION_X, .from = 320.0f, .to = 244.0f, .duration = 0.12f, .easing = UI_EASING_LINEAR },
};

const UIAnimation pause_enter = { pause_enter_track, sizeof(pause_enter_track)/sizeof(*pause_enter_track) };


/* Quitting is not the entrance backwards: the panel leaves the same way,
   but the gradient closes over the whole frame instead of opening. */
static const UIAnimationTrack pause_quit_track[] = {

	{ .entity = PAUSE_BG, .field = UI_FIELD_GRADIENT_A, .corner = 0, .from = 76.5f, .to = 255.0f, .duration = 0.15f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_BG, .field = UI_FIELD_GRADIENT_A, .corner = 3, .from = 76.5f, .to = 255.0f, .duration = 0.15f, .easing = UI_EASING_LINEAR },

	{ .entity = PAUSE_CONTINUE,    .field = UI_FIELD_POSITION_X, .from = PAUSE_MENU_X, .to = 320.0f, .duration = 0.15f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_QUIT,        .field = UI_FIELD_POSITION_X, .from = PAUSE_MENU_X, .to = 320.0f, .duration = 0.15f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_HINT_MOVE,   .field = UI_FIELD_POSITION_X, .from = 262.0f, .to = 347.0f, .duration = 0.15f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_HINT_SELECT, .field = UI_FIELD_POSITION_X, .from = 262.0f, .to = 338.0f, .duration = 0.15f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_HINT_BACK,   .field = UI_FIELD_POSITION_X, .from = 262.0f, .to = 338.0f, .duration = 0.15f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_D_UP,        .field = UI_FIELD_POSITION_X, .from = 235.0f, .to = 320.0f, .duration = 0.15f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_D_DOWN,      .field = UI_FIELD_POSITION_X, .from = 245.0f, .to = 330.0f, .duration = 0.15f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_BTN_A,       .field = UI_FIELD_POSITION_X, .from = 244.0f, .to = 320.0f, .duration = 0.15f, .easing = UI_EASING_LINEAR },
	{ .entity = PAUSE_BTN_B,       .field = UI_FIELD_POSITION_X, .from = 244.0f, .to = 320.0f, .duration = 0.15f, .easing = UI_EASING_LINEAR },
};

const UIAnimation pause_quit = { pause_quit_track, sizeof(pause_quit_track)/sizeof(*pause_quit_track) };
