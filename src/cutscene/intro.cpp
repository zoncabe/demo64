#include <libdragon.h>

#include "graphics/e64_shapes.h"
#include "cutscene/intro.h"


typedef enum {

	INTRO_LIBDRAGON,
	INTRO_TINY3D,
	INTRO_ZONCABE,
	INTRO_RECT,

	INTRO_COUNT,

} IntroWidget;


static const e64::Graphic intro_graphic[INTRO_COUNT] = {

	[INTRO_LIBDRAGON] = { .type = e64::Graphic::SPRITE, .sprite = { .path = "rom:/sprites/intro/libdragon.sprite" }, .is_hidden = true },
	[INTRO_TINY3D] = { .type = e64::Graphic::SPRITE, .sprite = { .path = "rom:/sprites/intro/tiny3d.sprite" }, .is_hidden = true },
	[INTRO_ZONCABE] = { .type = e64::Graphic::SPRITE, .sprite = { .path = "rom:/sprites/intro/zoncabe.sprite" }, .is_hidden = true },
	[INTRO_RECT] = { .type = e64::Graphic::RECTANGLE, .rectangle = { .fill = e64::Rectangle::SOLID, .color = RGBA32(0, 0, 0, 255) } },
};

static const e64::ui::WidgetDef intro_placed[INTRO_COUNT] = {

	[INTRO_LIBDRAGON] = { .position = { 0.0f, 0.0f }, .scale = { 1.0f, 1.0f }, .graphic = &intro_graphic[INTRO_LIBDRAGON] },
	[INTRO_TINY3D] = { .position = { 0.0f, 0.0f }, .scale = { 1.0f, 1.0f }, .graphic = &intro_graphic[INTRO_TINY3D] },
	[INTRO_ZONCABE] = { .position = { 0.0f, 0.0f }, .scale = { 1.0f, 1.0f }, .graphic = &intro_graphic[INTRO_ZONCABE] },
	[INTRO_RECT] = { .position = { 0.0f, 0.0f }, .scale = { 320.0f, 240.0f }, .graphic = &intro_graphic[INTRO_RECT] },
};

static const e64::ui::Layer intro_layer[] = {
	{ intro_placed, INTRO_COUNT },
};

const e64::ui::Def intro_ui = { intro_layer, 1 };


/* Each logo shows for a while; the black rectangle fades in and out over it
   to cut between them. */
static const e64::UIAnimation::Track intro_track[] = {

	{ .entity = INTRO_LIBDRAGON, .field = e64::UIAnimation::FIELD_HIDDEN, .from_bool = true, .to_bool = false, .delay = 0.75f, .duration = 2.00f },
	{ .entity = INTRO_TINY3D, .field = e64::UIAnimation::FIELD_HIDDEN, .from_bool = true, .to_bool = false, .delay = 2.75f, .duration = 2.00f },
	{ .entity = INTRO_ZONCABE, .field = e64::UIAnimation::FIELD_HIDDEN, .from_bool = true, .to_bool = false, .delay = 4.75f, .duration = 3.00f },

	{ .entity = INTRO_RECT, .field = e64::UIAnimation::FIELD_COLOR_A, .from = 255.0f, .to = 0.0f, .delay = 0.75f, .duration = 0.25f, .easing = e64::UIAnimation::EASING_LINEAR },
	{ .entity = INTRO_RECT, .field = e64::UIAnimation::FIELD_COLOR_A, .from = 0.0f, .to = 255.0f, .delay = 2.50f, .duration = 0.25f, .easing = e64::UIAnimation::EASING_LINEAR },

	{ .entity = INTRO_RECT, .field = e64::UIAnimation::FIELD_COLOR_A, .from = 255.0f, .to = 0.0f, .delay = 2.75f, .duration = 0.25f, .easing = e64::UIAnimation::EASING_LINEAR },
	{ .entity = INTRO_RECT, .field = e64::UIAnimation::FIELD_COLOR_A, .from = 0.0f, .to = 255.0f, .delay = 4.50f, .duration = 0.25f, .easing = e64::UIAnimation::EASING_LINEAR },

	{ .entity = INTRO_RECT, .field = e64::UIAnimation::FIELD_COLOR_A, .from = 255.0f, .to = 0.0f, .delay = 4.75f, .duration = 0.25f, .easing = e64::UIAnimation::EASING_LINEAR },
	{ .entity = INTRO_RECT, .field = e64::UIAnimation::FIELD_COLOR_A, .from = 0.0f, .to = 255.0f, .delay = 7.25f, .duration = 0.50f, .easing = e64::UIAnimation::EASING_LINEAR },
};

const e64::UIAnimation intro_animation = { intro_track, sizeof(intro_track)/sizeof(*intro_track) };
