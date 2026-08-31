#include <math.h>
#include <libdragon.h>

#include "assets/graphics/fonts.h"
#include "assets/graphics/sprites.h"
#include "graphics/e64_shapes.h"
#include "render/e64_render.h"
#include "ui/credits_ui.h"


/* Scroll window: the roll lives in its own layer, clipped by the scissor.
   The control sets a velocity every frame and the scroll integrates it. */
#define CREDITS_CLIP_Y       50.0f
#define CREDITS_CLIP_H      162.0f
#define CREDITS_BASE_Y       80.0f
#define CREDITS_ROLL_X       36.0f
#define CREDITS_ROLL_WIDTH    248
#define CREDITS_SCROLL_END  190.0f


static const rdpq_textparms_t h14_parms   = { .char_spacing = 0.95f };
static const rdpq_textparms_t h40_parms   = { .char_spacing = 0.0f  };
static const rdpq_textparms_t roll_parms  = { .width = CREDITS_ROLL_WIDTH, .wrap = WRAP_WORD };


/* One enum per layer: the tracks and the scroll address an element by the
   layer it sits in and its place inside it. */
typedef enum {

	CREDITS_LAYER_BACK,
	CREDITS_LAYER_ROLL,
	CREDITS_LAYER_FADE,
	CREDITS_LAYER_HINT,
	CREDITS_LAYER_COVER,

	CREDITS_LAYER_COUNT,

} CreditsLayer;

typedef enum {

	CREDITS_BG,
	CREDITS_TITLE,

	CREDITS_BACK_COUNT,

} CreditsBackElement;

typedef enum {

	CREDITS_ROLL,

	CREDITS_ROLL_COUNT,

} CreditsRollElement;

typedef enum {

	CREDITS_FADE_TOP,
	CREDITS_FADE_BOTTOM,

	CREDITS_FADE_COUNT,

} CreditsFadeElement;

typedef enum {

	CREDITS_HINT_SCROLL,
	CREDITS_HINT_BACK,
	CREDITS_D_UP,
	CREDITS_D_DOWN,
	CREDITS_BTN_B,

	CREDITS_HINT_COUNT,

} CreditsHintElement;

typedef enum {

	CREDITS_COVER,

	CREDITS_COVER_COUNT,

} CreditsCoverElement;


/* ^01 grey, ^02 yellow: the styles the font table declares. */
static const char credits_roll_text[] =
	"\n"
	"^02zoncabe\n"
	"^01yours truly, creator of the project\n"
	"\n"
	"the project was born after the 2022 N64brew jam, continuing the base that team achieved together\n"
	"\n"
	"\n"
	"^02SPECIAL THANKS:\n"
	"\n"
	"^02Buu342\n"
	"^01who taught me how to model in blender, welcomed me into the N64brew discord and made me feel right at home\n"
	"\n"
	"^02Jaltekruse\n"
	"^01creator of the entity system that survives to this day, who taught me the basics of gamedev on the calls we had for that jam\n"
	"\n"
	"^02libdragon and tiny3d teams\n"
	"^01for creating and maintaining the most refined libraries in the whole retro scene\n"
	"\n"
	"^02rasky, HailToDodongo, snacchus, anacierdem, meeq, networkfusion, SpookyIluha, gamemasterplc, thekovic and many others...\n"
	"\n"
	"you are the best!!";


static const Element2D credits_back_element[CREDITS_BACK_COUNT] = {

	[CREDITS_BG]    = { .type = ELEMENT2D_RECTANGLE, .position = { 0.0f,  0.0f }, .scale = { 320.0f, 240.0f }, .rectangle = { SHAPE_FILL_GRADIENT, .gradient = { RGBA32(25, 121, 201, 255), RGBA32(117, 175, 223, 255), RGBA32(117, 175, 223, 255), RGBA32(25, 121, 201, 255) } } },
	[CREDITS_TITLE] = { .type = ELEMENT2D_TEXT,      .position = { 43.0f, 45.0f },                              .text      = { XOLONIUM_40, MENU_STYLE_NORMAL, "Credits", &h40_parms } },
};

static const Element2D credits_roll_element[CREDITS_ROLL_COUNT] = {

	[CREDITS_ROLL] = { .type = ELEMENT2D_TEXT, .position = { CREDITS_ROLL_X, 0.0f }, .text = { XOLONIUM_14, MENU_STYLE_NORMAL, credits_roll_text, &roll_parms } },
};

/* Fade at the window edges: two gradients in the background colour, opaque
   toward the edge and transparent toward the centre, drawn over the text.
   The background runs horizontally, so each overlay copies the colours of
   its two ends. */
static const Element2D credits_fade_element[CREDITS_FADE_COUNT] = {

	[CREDITS_FADE_TOP]    = { .type = ELEMENT2D_RECTANGLE, .position = { 0.0f,  50.0f }, .scale = { 320.0f, 14.0f }, .rectangle = { SHAPE_FILL_GRADIENT, .gradient = { RGBA32(25, 121, 201, 255), RGBA32(117, 175, 223, 255), RGBA32(117, 175, 223, 0), RGBA32(25, 121, 201, 0) } } },
	[CREDITS_FADE_BOTTOM] = { .type = ELEMENT2D_RECTANGLE, .position = { 0.0f, 198.0f }, .scale = { 320.0f, 14.0f }, .rectangle = { SHAPE_FILL_GRADIENT, .gradient = { RGBA32(25, 121, 201, 0), RGBA32(117, 175, 223, 0), RGBA32(117, 175, 223, 255), RGBA32(25, 121, 201, 255) } } },
};

static const Element2D credits_hint_element[CREDITS_HINT_COUNT] = {

	[CREDITS_HINT_SCROLL] = { .type = ELEMENT2D_TEXT,   .position = { 273.0f, 201.0f },                            .text   = { XOLONIUM_14, 0, "Scroll", &h14_parms } },
	[CREDITS_HINT_BACK]   = { .type = ELEMENT2D_TEXT,   .position = { 281.0f, 216.0f },                            .text   = { XOLONIUM_14, 0, "Back",   &h14_parms } },
	[CREDITS_D_UP]        = { .type = ELEMENT2D_SPRITE, .position = { 248.0f, 193.0f }, .scale = { 0.48f, 0.48f }, .sprite = { SPRITE_D_UP   } },
	[CREDITS_D_DOWN]      = { .type = ELEMENT2D_SPRITE, .position = { 258.0f, 194.0f }, .scale = { 0.48f, 0.48f }, .sprite = { SPRITE_D_DOWN } },
	[CREDITS_BTN_B]       = { .type = ELEMENT2D_SPRITE, .position = { 266.0f, 207.0f }, .scale = { 0.60f, 0.60f }, .sprite = { SPRITE_BTN_B  } },
};

static const Element2D credits_cover_element[CREDITS_COVER_COUNT] = {

	[CREDITS_COVER] = { .type = ELEMENT2D_RECTANGLE, .position = { 0.0f, 0.0f }, .scale = { 320.0f, 240.0f }, .rectangle = { SHAPE_FILL_GRADIENT, .gradient = { RGBA32(0, 0, 0, 255), RGBA32(0, 0, 0, 255), RGBA32(0, 0, 0, 255), RGBA32(0, 0, 0, 255) } }, .is_hidden = true },
};

static const Scene2DLayer credits_layer[CREDITS_LAYER_COUNT] = {

	[CREDITS_LAYER_BACK]  = { credits_back_element, CREDITS_BACK_COUNT },

	[CREDITS_LAYER_ROLL]  = { credits_roll_element, CREDITS_ROLL_COUNT,
	                          .has_scissor = true,
	                          .scissor_x = 0.0f,   .scissor_y = CREDITS_CLIP_Y,
	                          .scissor_w = 320.0f, .scissor_h = CREDITS_CLIP_H },

	[CREDITS_LAYER_FADE]  = { credits_fade_element, CREDITS_FADE_COUNT },

	/* The hints draw last, over the bottom fade. */
	[CREDITS_LAYER_HINT]  = { credits_hint_element, CREDITS_HINT_COUNT },

	[CREDITS_LAYER_COVER] = { credits_cover_element, CREDITS_COVER_COUNT },
};

const Scene2DDef credits_scene2d = { credits_layer, CREDITS_LAYER_COUNT };


/* The whole entry is the black cover, dither noise included, dropping its
   alpha: background, text and sprites come up together, with no motion.
   Reversed, the same cover fades to black on the way out. Once the window is
   over the rect sends no command to the RDP. */
static const UIAnimationTrack credits_enter_track[] = {

	{ .layer = CREDITS_LAYER_COVER, .element = CREDITS_COVER, .field = UI_FIELD_GRADIENT_A, .corner = 0, .from = 255.0f, .to = 0.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },
	{ .layer = CREDITS_LAYER_COVER, .element = CREDITS_COVER, .field = UI_FIELD_GRADIENT_A, .corner = 1, .from = 255.0f, .to = 0.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },
	{ .layer = CREDITS_LAYER_COVER, .element = CREDITS_COVER, .field = UI_FIELD_GRADIENT_A, .corner = 2, .from = 255.0f, .to = 0.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },
	{ .layer = CREDITS_LAYER_COVER, .element = CREDITS_COVER, .field = UI_FIELD_GRADIENT_A, .corner = 3, .from = 255.0f, .to = 0.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },

	{ .layer = CREDITS_LAYER_COVER, .element = CREDITS_COVER, .field = UI_FIELD_HIDDEN, .from_bool = true, .to_bool = false, .duration = 0.25f },
};

const UIAnimation credits_enter = { credits_enter_track, sizeof(credits_enter_track)/sizeof(*credits_enter_track) };


/* The roll travels in whole pixels: at 320x240 a fractional position lands the
   glyphs on half a texel and the text crawls blurry. The velocity stays a
   float, so what it asks for between pixels is carried here until it adds up
   to one. */
static int   credits_offset;
static int   credits_offset_max;
static float credits_carry;
static float credits_velocity;


void credits_ui_setScrollVelocity(float velocity)
{
	credits_velocity = velocity;
}

void credits_ui_resetScroll(void)
{
	credits_offset   = 0;
	credits_carry    = 0.0f;
	credits_velocity = 0.0f;

	/* How far the roll can travel: its own height, past the window. */
	int nbytes = sizeof(credits_roll_text) - 1;
	rdpq_paragraph_t *layout = rdpq_paragraph_build(&roll_parms, XOLONIUM_14, credits_roll_text, &nbytes);
	credits_offset_max = (int)(CREDITS_BASE_Y + layout->bbox.y1 - CREDITS_SCROLL_END);
	rdpq_paragraph_free(layout);

	if (credits_offset_max < 0) credits_offset_max = 0;
}

void credits_ui_updateScroll(float dt)
{
	credits_carry += credits_velocity * dt;

	/* One pixel per step, however fast the carry filled up. */
	while (credits_carry >= 1.0f) { credits_offset++; credits_carry -= 1.0f; }
	while (credits_carry <= -1.0f) { credits_offset--; credits_carry += 1.0f; }

	if (credits_offset < 0) {
		credits_offset = 0;
		credits_carry  = 0.0f;
	}
	if (credits_offset > credits_offset_max) {
		credits_offset = credits_offset_max;
		credits_carry  = 0.0f;
	}

	Element2D *roll = scene2d_getElement(scene2d_get(), CREDITS_LAYER_ROLL, CREDITS_ROLL);
	roll->position.y = CREDITS_BASE_Y - credits_offset;
}
