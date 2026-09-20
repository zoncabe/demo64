#ifndef ENGINE_ULTRA64
#include <libdragon.h>
#endif

#include ENGINE_HEADER(graphics, shapes)
#include "ui/gameplay_ui.h"


typedef enum {

	GAMEPLAY_FADE,

	GAMEPLAY_COUNT,

} GameplayEntity;


static const Prefab2D gameplay_prefab[GAMEPLAY_COUNT] = {

	/* Hidden at rest: only the fade animation brings it up, so coming back
	   from the pause redraws nothing. */
	[GAMEPLAY_FADE] = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_RECTANGLE, .rectangle = { .fill = SHAPE_FILL_GRADIENT, .gradient ={ RGBA32(0, 0, 0, 255), RGBA32(0, 0, 0, 255), RGBA32(0, 0, 0, 255), RGBA32(0, 0, 0, 255) } }, .is_hidden = true } },
};

static const Scene2DPrefab gameplay_placed[GAMEPLAY_COUNT] = {

	[GAMEPLAY_FADE] = { &gameplay_prefab[GAMEPLAY_FADE], { 0.0f, 0.0f }, { 320.0f, 240.0f } },
};

static const Scene2DLayer gameplay_layer[] = {
	{ gameplay_placed, GAMEPLAY_COUNT },
};

const Scene2DDef gameplay_scene2d = { .layer = gameplay_layer, .layer_count = 1 };


static const UIAnimationTrack gameplay_enter_track[] = {

	{ .entity = GAMEPLAY_FADE, .field = UI_FIELD_GRADIENT_A, .corner = 0, .from = 255.0f, .to = 0.0f, .duration = 0.35f, .easing = UI_EASING_CUBIC_OUT },
	{ .entity = GAMEPLAY_FADE, .field = UI_FIELD_GRADIENT_A, .corner = 1, .from = 255.0f, .to = 0.0f, .duration = 0.35f, .easing = UI_EASING_CUBIC_OUT },
	{ .entity = GAMEPLAY_FADE, .field = UI_FIELD_GRADIENT_A, .corner = 2, .from = 255.0f, .to = 0.0f, .duration = 0.35f, .easing = UI_EASING_CUBIC_OUT },
	{ .entity = GAMEPLAY_FADE, .field = UI_FIELD_GRADIENT_A, .corner = 3, .from = 255.0f, .to = 0.0f, .duration = 0.35f, .easing = UI_EASING_CUBIC_OUT },

	/* Visible only while the window lasts: once it ends the rect sends no
	   command to the RDP for the rest of the session. */
	{ .entity = GAMEPLAY_FADE, .field = UI_FIELD_HIDDEN, .from_bool = true, .to_bool = false, .duration = 0.55f },
};

const UIAnimation gameplay_enter = { gameplay_enter_track, sizeof(gameplay_enter_track)/sizeof(*gameplay_enter_track) };
