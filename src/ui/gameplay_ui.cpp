#include <libdragon.h>

#include "graphics/e64_shapes.h"
#include "ui/gameplay_ui.h"


typedef enum {

	GAMEPLAY_FADE,

	GAMEPLAY_COUNT,

} GameplayWidget;


static const e64::Graphic gameplay_graphic[GAMEPLAY_COUNT] = {

	/* Hidden at rest: only the fade animation brings it up, so coming back
	   from the pause redraws nothing. */
	[GAMEPLAY_FADE] = { .type = e64::Graphic::RECTANGLE, .rectangle = { .fill = e64::Rectangle::GRADIENT, .gradient ={ RGBA32(0, 0, 0, 255), RGBA32(0, 0, 0, 255), RGBA32(0, 0, 0, 255), RGBA32(0, 0, 0, 255) } }, .is_hidden = true },
};

static const e64::ui::WidgetDef gameplay_placed[GAMEPLAY_COUNT] = {

	[GAMEPLAY_FADE] = { .position = { 0.0f, 0.0f }, .scale = { 320.0f, 240.0f }, .graphic = &gameplay_graphic[GAMEPLAY_FADE] },
};

static const e64::ui::Layer gameplay_layer[] = {
	{ gameplay_placed, GAMEPLAY_COUNT },
};

const e64::ui::Def gameplay_ui = { gameplay_layer, 1 };


static const e64::UIAnimation::Track gameplay_enter_track[] = {

	{ .entity = GAMEPLAY_FADE, .field = e64::UIAnimation::FIELD_GRADIENT_A, .corner = 0, .from = 255.0f, .to = 0.0f, .duration = 0.35f, .easing = e64::UIAnimation::EASING_CUBIC_OUT },
	{ .entity = GAMEPLAY_FADE, .field = e64::UIAnimation::FIELD_GRADIENT_A, .corner = 1, .from = 255.0f, .to = 0.0f, .duration = 0.35f, .easing = e64::UIAnimation::EASING_CUBIC_OUT },
	{ .entity = GAMEPLAY_FADE, .field = e64::UIAnimation::FIELD_GRADIENT_A, .corner = 2, .from = 255.0f, .to = 0.0f, .duration = 0.35f, .easing = e64::UIAnimation::EASING_CUBIC_OUT },
	{ .entity = GAMEPLAY_FADE, .field = e64::UIAnimation::FIELD_GRADIENT_A, .corner = 3, .from = 255.0f, .to = 0.0f, .duration = 0.35f, .easing = e64::UIAnimation::EASING_CUBIC_OUT },

	/* Visible only while the window lasts: once it ends the rect sends no
	   command to the RDP for the rest of the session. */
	{ .entity = GAMEPLAY_FADE, .field = e64::UIAnimation::FIELD_HIDDEN, .from_bool = true, .to_bool = false, .duration = 0.55f },
};

const e64::UIAnimation gameplay_enter = { gameplay_enter_track, sizeof(gameplay_enter_track)/sizeof(*gameplay_enter_track) };
