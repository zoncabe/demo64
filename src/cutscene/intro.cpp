#ifndef ENGINE_ULTRA64
#include <libdragon.h>
#endif

#include "assets/graphics/fonts.h"
#include ENGINE_HEADER(graphics, shapes)
#include "cutscene/intro.h"


typedef enum {

	INTRO_LIBDRAGON,
	INTRO_TINY3D,
	INTRO_ENGINE64,
	INTRO_RECT,

	INTRO_COUNT,

} IntroEntity;


#ifdef ENGINE_ULTRA64
static const TextParms intro_title_parms = {
#else
static const rdpq_textparms_t intro_title_parms = {
#endif
	.width  = 320,
	.height = 240,
	.align  = ALIGN_CENTER,
	.valign = VALIGN_CENTER,
};

static const Prefab2D intro_prefab[INTRO_COUNT] = {

	[INTRO_LIBDRAGON] = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_SPRITE,    .sprite    = { .path = "rom:/sprites/intro/libdragon.sprite" }, .is_hidden = true } },
	[INTRO_TINY3D]    = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_SPRITE,    .sprite    = { .path = "rom:/sprites/intro/tiny3d.sprite" },    .is_hidden = true } },
	[INTRO_ENGINE64]  = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_TEXT,      .text      = { XOLONIUM_60, TEXT_STYLE_RED, "engine 64", &intro_title_parms }, .is_hidden = true } },
	[INTRO_RECT]      = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_RECTANGLE, .rectangle = { .fill = SHAPE_FILL_SOLID, .color = RGBA32(0, 0, 0, 255) } } },
};

static const Scene2DPrefab intro_placed[INTRO_COUNT] = {

	[INTRO_LIBDRAGON] = { &intro_prefab[INTRO_LIBDRAGON], { 0.0f, 0.0f }, {   1.0f,   1.0f } },
	[INTRO_TINY3D]    = { &intro_prefab[INTRO_TINY3D],    { 0.0f, 0.0f }, {   1.0f,   1.0f } },
	[INTRO_ENGINE64]  = { &intro_prefab[INTRO_ENGINE64],  { 0.0f, 0.0f } },
	[INTRO_RECT]      = { &intro_prefab[INTRO_RECT],      { 0.0f, 0.0f }, { 320.0f, 240.0f } },
};

static const Scene2DLayer intro_layer[] = {
	{ intro_placed, INTRO_COUNT },
};

const Scene2DDef intro_scene2d = { .layer = intro_layer, .layer_count = 1 };


/* Each logo shows for a while; the black rectangle fades in and out over it
   to cut between them. */
static const UIAnimationTrack intro_track[] = {

	{ .entity = INTRO_LIBDRAGON, .field = UI_FIELD_HIDDEN, .from_bool = true, .to_bool = false, .delay = 0.75f, .duration = 2.00f },
	{ .entity = INTRO_TINY3D,    .field = UI_FIELD_HIDDEN, .from_bool = true, .to_bool = false, .delay = 2.75f, .duration = 2.00f },
	{ .entity = INTRO_ENGINE64,  .field = UI_FIELD_HIDDEN, .from_bool = true, .to_bool = false, .delay = 4.75f, .duration = 3.00f },

	{ .entity = INTRO_RECT, .field = UI_FIELD_COLOR_A, .from = 255.0f, .to = 0.0f,   .delay = 0.75f, .duration = 0.25f, .easing = UI_EASING_LINEAR },
	{ .entity = INTRO_RECT, .field = UI_FIELD_COLOR_A, .from = 0.0f,   .to = 255.0f, .delay = 2.50f, .duration = 0.25f, .easing = UI_EASING_LINEAR },

	{ .entity = INTRO_RECT, .field = UI_FIELD_COLOR_A, .from = 255.0f, .to = 0.0f,   .delay = 2.75f, .duration = 0.25f, .easing = UI_EASING_LINEAR },
	{ .entity = INTRO_RECT, .field = UI_FIELD_COLOR_A, .from = 0.0f,   .to = 255.0f, .delay = 4.50f, .duration = 0.25f, .easing = UI_EASING_LINEAR },

	{ .entity = INTRO_RECT, .field = UI_FIELD_COLOR_A, .from = 255.0f, .to = 0.0f,   .delay = 4.75f, .duration = 0.25f, .easing = UI_EASING_LINEAR },
	{ .entity = INTRO_RECT, .field = UI_FIELD_COLOR_A, .from = 0.0f,   .to = 255.0f, .delay = 7.25f, .duration = 0.50f, .easing = UI_EASING_LINEAR },
};

const UIAnimation intro_animation = { intro_track, sizeof(intro_track)/sizeof(*intro_track) };
