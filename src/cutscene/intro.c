#include <libdragon.h>

#include "assets/graphics/sprites.h"
#include "assets/graphics/fonts.h"
#include ENGINE_HEADER(graphics, shapes)
#include ENGINE_HEADER(render, render)
#include "cutscene/intro.h"


typedef enum {

	INTRO_LIBDRAGON,
	INTRO_TINY3D,
	INTRO_ENGINE64,
	INTRO_RECT,

	INTRO_COUNT,

} IntroElement;


static const rdpq_textparms_t intro_title_parms = {
	.width  = 320,
	.height = 240,
	.align  = ALIGN_CENTER,
	.valign = VALIGN_CENTER,
};

static const Element2D intro_element[INTRO_COUNT] = {

	[INTRO_LIBDRAGON] = { .type = ELEMENT2D_SPRITE,    .position = { 0.0f, 0.0f }, .scale = { 1.0f, 1.0f },     .sprite    = { SPRITE_LIBDRAGON }, .is_hidden = true },
	[INTRO_TINY3D]    = { .type = ELEMENT2D_SPRITE,    .position = { 0.0f, 0.0f }, .scale = { 1.0f, 1.0f },     .sprite    = { SPRITE_TINY3D },    .is_hidden = true },
	[INTRO_ENGINE64]  = { .type = ELEMENT2D_TEXT,      .position = { 0.0f, 0.0f },                              .text      = { XOLONIUM_60, TEXT_STYLE_RED, "engine 64", &intro_title_parms }, .is_hidden = true },
	[INTRO_RECT]      = { .type = ELEMENT2D_RECTANGLE, .position = { 0.0f, 0.0f }, .scale = { 320.0f, 240.0f }, .rectangle = { .fill = SHAPE_FILL_SOLID, .color = RGBA32(0, 0, 0, 255) } },
};

static const Scene2DLayer intro_layer[] = {
	{ intro_element, INTRO_COUNT },
};

const Scene2DDef intro_scene2d = { intro_layer, 1 };


/* Each logo shows for a while; the black rectangle fades in and out over it
   to cut between them. */
static const UIAnimationTrack intro_track[] = {

	{ .element = INTRO_LIBDRAGON, .field = UI_FIELD_HIDDEN, .from_bool = true, .to_bool = false, .delay = 0.75f, .duration = 2.00f },
	{ .element = INTRO_TINY3D,    .field = UI_FIELD_HIDDEN, .from_bool = true, .to_bool = false, .delay = 2.75f, .duration = 2.00f },
	{ .element = INTRO_ENGINE64,  .field = UI_FIELD_HIDDEN, .from_bool = true, .to_bool = false, .delay = 4.75f, .duration = 3.00f },

	{ .element = INTRO_RECT, .field = UI_FIELD_COLOR_A, .from = 255.0f, .to = 0.0f,   .delay = 0.75f, .duration = 0.25f, .easing = UI_EASING_LINEAR },
	{ .element = INTRO_RECT, .field = UI_FIELD_COLOR_A, .from = 0.0f,   .to = 255.0f, .delay = 2.50f, .duration = 0.25f, .easing = UI_EASING_LINEAR },

	{ .element = INTRO_RECT, .field = UI_FIELD_COLOR_A, .from = 255.0f, .to = 0.0f,   .delay = 2.75f, .duration = 0.25f, .easing = UI_EASING_LINEAR },
	{ .element = INTRO_RECT, .field = UI_FIELD_COLOR_A, .from = 0.0f,   .to = 255.0f, .delay = 4.50f, .duration = 0.25f, .easing = UI_EASING_LINEAR },

	{ .element = INTRO_RECT, .field = UI_FIELD_COLOR_A, .from = 255.0f, .to = 0.0f,   .delay = 4.75f, .duration = 0.25f, .easing = UI_EASING_LINEAR },
	{ .element = INTRO_RECT, .field = UI_FIELD_COLOR_A, .from = 0.0f,   .to = 255.0f, .delay = 7.25f, .duration = 0.50f, .easing = UI_EASING_LINEAR },
};

const UIAnimation intro_animation = { intro_track, sizeof(intro_track)/sizeof(*intro_track) };
