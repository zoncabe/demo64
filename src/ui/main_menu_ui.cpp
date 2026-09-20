#ifndef ENGINE_ULTRA64
#include <libdragon.h>
#endif

#include "assets/graphics/fonts.h"
#include ENGINE_HEADER(graphics, shapes)
#include "ui/main_menu_ui.h"


#ifdef ENGINE_ULTRA64
/* The engine's text spacing is whole pixels. */
static const TextParms h14_parms = { .char_spacing = 1 };
static const TextParms h20_parms = { .char_spacing = 1 };
static const TextParms h40_parms = { .char_spacing = 0 };
#else
static const rdpq_textparms_t h14_parms = { .char_spacing = 0 };
static const rdpq_textparms_t h20_parms = { .char_spacing = 0 };
static const rdpq_textparms_t h40_parms = { .char_spacing = 0 };
#endif


typedef enum {

	MAIN_MENU_BG,
	MAIN_MENU_TITLE,
	MAIN_MENU_PLAY,
	MAIN_MENU_PLAY_2D,
	MAIN_MENU_CREDITS,
	MAIN_MENU_HINT_MOVE,
	MAIN_MENU_HINT_SELECT,
	MAIN_MENU_BTN_A,
	MAIN_MENU_D_UP,
	MAIN_MENU_D_DOWN,
	MAIN_MENU_GORILLA,

	MAIN_MENU_COUNT,

} MainMenuEntity;


static const Prefab2D main_menu_prefab[MAIN_MENU_COUNT] = {

	[MAIN_MENU_BG]          = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_RECTANGLE, .rectangle = { .fill = SHAPE_FILL_GRADIENT, .gradient ={ RGBA32(201, 121, 25, 255), RGBA32(223, 175, 117, 255), RGBA32(223, 175, 117, 255), RGBA32(201, 121, 25, 255) } } } },
	[MAIN_MENU_TITLE]       = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_TEXT,      .text      = { XOLONIUM_40, MENU_STYLE_NORMAL, "Demo 64", &h40_parms } } },
	[MAIN_MENU_PLAY]        = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_TEXT,      .text      = { XOLONIUM_20, MENU_STYLE_NORMAL, "Play",    &h20_parms } } },
	[MAIN_MENU_PLAY_2D]     = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_TEXT,      .text      = { XOLONIUM_20, MENU_STYLE_NORMAL, "Play 2D", &h20_parms } } },
	[MAIN_MENU_CREDITS]     = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_TEXT,      .text      = { XOLONIUM_20, MENU_STYLE_NORMAL, "Credits", &h20_parms } } },
	[MAIN_MENU_HINT_MOVE]   = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_TEXT,      .text      = { XOLONIUM_14, 0,                 "Move",    &h14_parms } } },
	[MAIN_MENU_HINT_SELECT] = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_TEXT,      .text      = { XOLONIUM_14, 0,                 "Select",  &h14_parms } } },
	[MAIN_MENU_BTN_A]       = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_SPRITE,    .sprite    = { .path = "rom:/sprites/core/AButton.sprite"        } } },
	[MAIN_MENU_D_UP]        = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_SPRITE,    .sprite    = { .path = "rom:/sprites/core/DUp.sprite"            } } },
	[MAIN_MENU_D_DOWN]      = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_SPRITE,    .sprite    = { .path = "rom:/sprites/core/DDown.sprite"          } } },
	[MAIN_MENU_GORILLA]     = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_SPRITE,    .sprite    = { .path = "rom:/sprites/title/gorilla.rgba32.sprite" } } },
};

static const Scene2DPrefab main_menu_placed[MAIN_MENU_COUNT] = {

	[MAIN_MENU_BG]          = { &main_menu_prefab[MAIN_MENU_BG],          {   0.0f,   0.0f }, { 320.0f, 240.0f } },
	[MAIN_MENU_TITLE]       = { &main_menu_prefab[MAIN_MENU_TITLE],       {  43.0f,  65.0f } },
	[MAIN_MENU_PLAY]        = { &main_menu_prefab[MAIN_MENU_PLAY],        {  45.0f, 137.0f } },
	[MAIN_MENU_PLAY_2D]     = { &main_menu_prefab[MAIN_MENU_PLAY_2D],     {  45.0f, 162.0f } },
	[MAIN_MENU_CREDITS]     = { &main_menu_prefab[MAIN_MENU_CREDITS],     {  45.0f, 187.0f } },
	[MAIN_MENU_HINT_MOVE]   = { &main_menu_prefab[MAIN_MENU_HINT_MOVE],   {  65.0f, 225.0f } },
	[MAIN_MENU_HINT_SELECT] = { &main_menu_prefab[MAIN_MENU_HINT_SELECT], { 115.0f, 225.0f } },
	[MAIN_MENU_BTN_A]       = { &main_menu_prefab[MAIN_MENU_BTN_A],       { 102.0f, 216.0f }, { 0.60f, 0.60f } },
	[MAIN_MENU_D_UP]        = { &main_menu_prefab[MAIN_MENU_D_UP],        {  43.0f, 217.0f }, { 0.48f, 0.48f } },
	[MAIN_MENU_D_DOWN]      = { &main_menu_prefab[MAIN_MENU_D_DOWN],      {  53.0f, 217.0f }, { 0.48f, 0.48f } },
	[MAIN_MENU_GORILLA]     = { &main_menu_prefab[MAIN_MENU_GORILLA],     { 170.0f,   0.0f }, { 1.00f, 1.00f } },
};

static const Scene2DLayer main_menu_layer[] = {
	{ main_menu_placed, MAIN_MENU_COUNT },
};

const Scene2DDef main_menu_scene2d = { .layer = main_menu_layer, .layer_count = 1 };


/* The entry the cursor stands on is the one drawn in yellow. */
static const float main_menu_style_play[]    = { MENU_STYLE_SELECTED, MENU_STYLE_NORMAL,   MENU_STYLE_NORMAL   };
static const float main_menu_style_play_2d[] = { MENU_STYLE_NORMAL,   MENU_STYLE_SELECTED, MENU_STYLE_NORMAL   };
static const float main_menu_style_credits[] = { MENU_STYLE_NORMAL,   MENU_STYLE_NORMAL,   MENU_STYLE_SELECTED };

static const UIAnimationTrack main_menu_idle_track[] = {

	{ .entity = MAIN_MENU_PLAY,    .field = UI_FIELD_TEXT_STYLE, .source = UI_SOURCE_MENU_INDEX, .values_by_index = main_menu_style_play    },
	{ .entity = MAIN_MENU_PLAY_2D, .field = UI_FIELD_TEXT_STYLE, .source = UI_SOURCE_MENU_INDEX, .values_by_index = main_menu_style_play_2d },
	{ .entity = MAIN_MENU_CREDITS, .field = UI_FIELD_TEXT_STYLE, .source = UI_SOURCE_MENU_INDEX, .values_by_index = main_menu_style_credits },
};

const UIAnimation main_menu_idle = { main_menu_idle_track, sizeof(main_menu_idle_track)/sizeof(*main_menu_idle_track) };


/* Everything slides in from the side it will leave through, over a
   background that lights up from black. */
static const UIAnimationTrack main_menu_enter_track[] = {

	{ .entity = MAIN_MENU_TITLE,   .field = UI_FIELD_POSITION_X, .from = -260.0f, .to =  43.0f, .delay = 0.10f, .duration = 0.25f, .easing = UI_EASING_CUBIC_OUT },

	{ .entity = MAIN_MENU_PLAY,    .field = UI_FIELD_POSITION_X, .from = 400.0f, .to = 45.0f, .delay = 0.20f, .duration = 0.25f, .easing = UI_EASING_CUBIC_OUT },
	{ .entity = MAIN_MENU_PLAY_2D, .field = UI_FIELD_POSITION_X, .from = 400.0f, .to = 45.0f, .delay = 0.25f, .duration = 0.25f, .easing = UI_EASING_CUBIC_OUT },
	{ .entity = MAIN_MENU_CREDITS, .field = UI_FIELD_POSITION_X, .from = 400.0f, .to = 45.0f, .delay = 0.30f, .duration = 0.25f, .easing = UI_EASING_CUBIC_OUT },

	{ .entity = MAIN_MENU_GORILLA, .field = UI_FIELD_POSITION_X, .from = 400.0f, .to = 170.0f, .delay = 0.10f, .duration = 0.25f, .easing = UI_EASING_CUBIC_OUT },

	{ .entity = MAIN_MENU_HINT_MOVE,   .field = UI_FIELD_POSITION_X, .from = -238.0f, .to =  65.0f, .delay = 0.10f, .duration = 0.25f, .easing = UI_EASING_CUBIC_OUT },
	{ .entity = MAIN_MENU_HINT_SELECT, .field = UI_FIELD_POSITION_X, .from = -188.0f, .to = 115.0f, .delay = 0.10f, .duration = 0.25f, .easing = UI_EASING_CUBIC_OUT },
	{ .entity = MAIN_MENU_BTN_A,       .field = UI_FIELD_POSITION_X, .from = -201.0f, .to = 102.0f, .delay = 0.10f, .duration = 0.25f, .easing = UI_EASING_CUBIC_OUT },
	{ .entity = MAIN_MENU_D_UP,        .field = UI_FIELD_POSITION_X, .from = -260.0f, .to =  43.0f, .delay = 0.10f, .duration = 0.25f, .easing = UI_EASING_CUBIC_OUT },
	{ .entity = MAIN_MENU_D_DOWN,      .field = UI_FIELD_POSITION_X, .from = -250.0f, .to =  53.0f, .delay = 0.10f, .duration = 0.25f, .easing = UI_EASING_CUBIC_OUT },

	{ .entity = MAIN_MENU_BG, .field = UI_FIELD_GRADIENT_R, .corner = 0, .from = 0.0f, .to = 201.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },
	{ .entity = MAIN_MENU_BG, .field = UI_FIELD_GRADIENT_G, .corner = 0, .from = 0.0f, .to = 121.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },
	{ .entity = MAIN_MENU_BG, .field = UI_FIELD_GRADIENT_B, .corner = 0, .from = 0.0f, .to =  25.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },

	{ .entity = MAIN_MENU_BG, .field = UI_FIELD_GRADIENT_R, .corner = 1, .from = 0.0f, .to = 223.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },
	{ .entity = MAIN_MENU_BG, .field = UI_FIELD_GRADIENT_G, .corner = 1, .from = 0.0f, .to = 175.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },
	{ .entity = MAIN_MENU_BG, .field = UI_FIELD_GRADIENT_B, .corner = 1, .from = 0.0f, .to = 117.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },

	{ .entity = MAIN_MENU_BG, .field = UI_FIELD_GRADIENT_R, .corner = 2, .from = 0.0f, .to = 223.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },
	{ .entity = MAIN_MENU_BG, .field = UI_FIELD_GRADIENT_G, .corner = 2, .from = 0.0f, .to = 175.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },
	{ .entity = MAIN_MENU_BG, .field = UI_FIELD_GRADIENT_B, .corner = 2, .from = 0.0f, .to = 117.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },

	{ .entity = MAIN_MENU_BG, .field = UI_FIELD_GRADIENT_R, .corner = 3, .from = 0.0f, .to = 201.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },
	{ .entity = MAIN_MENU_BG, .field = UI_FIELD_GRADIENT_G, .corner = 3, .from = 0.0f, .to = 121.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },
	{ .entity = MAIN_MENU_BG, .field = UI_FIELD_GRADIENT_B, .corner = 3, .from = 0.0f, .to =  25.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },
};

const UIAnimation main_menu_enter = { main_menu_enter_track, sizeof(main_menu_enter_track)/sizeof(*main_menu_enter_track) };
