#include <math.h>
#ifndef ENGINE_ULTRA64
#include <libdragon.h>
#endif

#include "assets/graphics/fonts.h"
#include ENGINE_HEADER(graphics, shapes)
#include "ui/credits_ui.h"


/* Scroll window: the roll lives in its own layer, clipped by the scissor.
   The control sets a velocity every frame and the scroll integrates it. */
#define CREDITS_CLIP_Y       50.0f
#define CREDITS_CLIP_H      162.0f
#define CREDITS_BASE_Y       80.0f
#define CREDITS_ROLL_X       36.0f
#define CREDITS_ROLL_WIDTH    248
#define CREDITS_SCROLL_END  190.0f


#ifdef ENGINE_ULTRA64
/* The engine's text spacing is whole pixels. */
static const TextParms h14_parms   = { .char_spacing = 1 };
static const TextParms h40_parms   = { .char_spacing = 0 };
static const TextParms roll_parms  = { .width = CREDITS_ROLL_WIDTH, .wrap = WRAP_WORD };
#else
static const rdpq_textparms_t h14_parms   = { .char_spacing = 0 };
static const rdpq_textparms_t h40_parms   = { .char_spacing = 0 };
static const rdpq_textparms_t roll_parms  = { .width = CREDITS_ROLL_WIDTH, .wrap = WRAP_WORD };
#endif


/* One enum per layer: the tracks and the scroll address an entity by the
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

} CreditsBackEntity;

typedef enum {

	CREDITS_ROLL,

	CREDITS_ROLL_COUNT,

} CreditsRollEntity;

typedef enum {

	CREDITS_FADE_TOP,
	CREDITS_FADE_BOTTOM,

	CREDITS_FADE_COUNT,

} CreditsFadeEntity;

typedef enum {

	CREDITS_HINT_SCROLL,
	CREDITS_HINT_BACK,
	CREDITS_D_UP,
	CREDITS_D_DOWN,
	CREDITS_BTN_B,

	CREDITS_HINT_COUNT,

} CreditsHintEntity;

typedef enum {

	CREDITS_COVER,

	CREDITS_COVER_COUNT,

} CreditsCoverEntity;


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


static const Prefab2D credits_back_prefab[CREDITS_BACK_COUNT] = {

	[CREDITS_BG]    = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_RECTANGLE, .rectangle = { .fill = SHAPE_FILL_GRADIENT, .gradient ={ RGBA32(25, 121, 201, 255), RGBA32(117, 175, 223, 255), RGBA32(117, 175, 223, 255), RGBA32(25, 121, 201, 255) } } } },
	[CREDITS_TITLE] = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_TEXT,      .text      ={ XOLONIUM_40, MENU_STYLE_NORMAL, "Credits", &h40_parms } } },
};

static const Scene2DPrefab credits_back_placed[CREDITS_BACK_COUNT] = {

	[CREDITS_BG]    = { &credits_back_prefab[CREDITS_BG],    {  0.0f,  0.0f }, { 320.0f, 240.0f } },
	[CREDITS_TITLE] = { &credits_back_prefab[CREDITS_TITLE], { 43.0f, 45.0f } },
};

static const Prefab2D credits_roll_prefab[CREDITS_ROLL_COUNT] = {

	[CREDITS_ROLL] = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_TEXT, .text ={ XOLONIUM_14, MENU_STYLE_NORMAL, credits_roll_text, &roll_parms } } },
};

static const Scene2DPrefab credits_roll_placed[CREDITS_ROLL_COUNT] = {

	[CREDITS_ROLL] = { &credits_roll_prefab[CREDITS_ROLL], { CREDITS_ROLL_X, 0.0f } },
};

/* Fade at the window edges: two gradients in the background colour, opaque
   toward the edge and transparent toward the centre, drawn over the text.
   The background runs horizontally, so each overlay copies the colours of
   its two ends. */
static const Prefab2D credits_fade_prefab[CREDITS_FADE_COUNT] = {

	[CREDITS_FADE_TOP]    = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_RECTANGLE, .rectangle = { .fill = SHAPE_FILL_GRADIENT, .gradient ={ RGBA32(25, 121, 201, 255), RGBA32(117, 175, 223, 255), RGBA32(117, 175, 223, 0), RGBA32(25, 121, 201, 0) } } } },
	[CREDITS_FADE_BOTTOM] = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_RECTANGLE, .rectangle = { .fill = SHAPE_FILL_GRADIENT, .gradient ={ RGBA32(25, 121, 201, 0), RGBA32(117, 175, 223, 0), RGBA32(117, 175, 223, 255), RGBA32(25, 121, 201, 255) } } } },
};

static const Scene2DPrefab credits_fade_placed[CREDITS_FADE_COUNT] = {

	[CREDITS_FADE_TOP]    = { &credits_fade_prefab[CREDITS_FADE_TOP],    { 0.0f,  50.0f }, { 320.0f, 14.0f } },
	[CREDITS_FADE_BOTTOM] = { &credits_fade_prefab[CREDITS_FADE_BOTTOM], { 0.0f, 198.0f }, { 320.0f, 14.0f } },
};

static const Prefab2D credits_hint_prefab[CREDITS_HINT_COUNT] = {

	[CREDITS_HINT_SCROLL] = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_TEXT,   .text   = { XOLONIUM_14, 0, "Scroll", &h14_parms } } },
	[CREDITS_HINT_BACK]   = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_TEXT,   .text   = { XOLONIUM_14, 0, "Back",   &h14_parms } } },
	[CREDITS_D_UP]        = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_SPRITE, .sprite = { .path = "rom:/sprites/core/DUp.sprite"     } } },
	[CREDITS_D_DOWN]      = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_SPRITE, .sprite = { .path = "rom:/sprites/core/DDown.sprite"   } } },
	[CREDITS_BTN_B]       = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_SPRITE, .sprite = { .path = "rom:/sprites/core/BButton.sprite" } } },
};

static const Scene2DPrefab credits_hint_placed[CREDITS_HINT_COUNT] = {

	[CREDITS_HINT_SCROLL] = { &credits_hint_prefab[CREDITS_HINT_SCROLL], { 273.0f, 201.0f } },
	[CREDITS_HINT_BACK]   = { &credits_hint_prefab[CREDITS_HINT_BACK],   { 281.0f, 216.0f } },
	[CREDITS_D_UP]        = { &credits_hint_prefab[CREDITS_D_UP],        { 248.0f, 193.0f }, { 0.48f, 0.48f } },
	[CREDITS_D_DOWN]      = { &credits_hint_prefab[CREDITS_D_DOWN],      { 258.0f, 194.0f }, { 0.48f, 0.48f } },
	[CREDITS_BTN_B]       = { &credits_hint_prefab[CREDITS_BTN_B],       { 266.0f, 207.0f }, { 0.60f, 0.60f } },
};

static const Prefab2D credits_cover_prefab[CREDITS_COVER_COUNT] = {

	[CREDITS_COVER] = { .type = PREFAB2D_WIDGET, .graphic = { .type = GRAPHIC_RECTANGLE, .rectangle = { .fill = SHAPE_FILL_GRADIENT, .gradient ={ RGBA32(0, 0, 0, 255), RGBA32(0, 0, 0, 255), RGBA32(0, 0, 0, 255), RGBA32(0, 0, 0, 255) } }, .is_hidden = true } },
};

static const Scene2DPrefab credits_cover_placed[CREDITS_COVER_COUNT] = {

	[CREDITS_COVER] = { &credits_cover_prefab[CREDITS_COVER], { 0.0f, 0.0f }, { 320.0f, 240.0f } },
};

static const Scene2DLayer credits_layer[CREDITS_LAYER_COUNT] = {

	[CREDITS_LAYER_BACK]  = { credits_back_placed, CREDITS_BACK_COUNT },

	[CREDITS_LAYER_ROLL]  = { .prefab = credits_roll_placed, .prefab_count = CREDITS_ROLL_COUNT,
	                          .has_scissor = true,
	                          .scissor_x = 0.0f,   .scissor_y = CREDITS_CLIP_Y,
	                          .scissor_w = 320.0f, .scissor_h = CREDITS_CLIP_H },

	[CREDITS_LAYER_FADE]  = { credits_fade_placed, CREDITS_FADE_COUNT },

	/* The hints draw last, over the bottom fade. */
	[CREDITS_LAYER_HINT]  = { credits_hint_placed, CREDITS_HINT_COUNT },

	[CREDITS_LAYER_COVER] = { credits_cover_placed, CREDITS_COVER_COUNT },
};

const Scene2DDef credits_scene2d = { .layer = credits_layer, .layer_count = CREDITS_LAYER_COUNT };


/* The whole entry is the black cover, dither noise included, dropping its
   alpha: background, text and sprites come up together, with no motion.
   Reversed, the same cover fades to black on the way out. Once the window is
   over the rect sends no command to the RDP. */
static const UIAnimationTrack credits_enter_track[] = {

	{ .layer = CREDITS_LAYER_COVER, .entity = CREDITS_COVER, .field = UI_FIELD_GRADIENT_A, .corner = 0, .from = 255.0f, .to = 0.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },
	{ .layer = CREDITS_LAYER_COVER, .entity = CREDITS_COVER, .field = UI_FIELD_GRADIENT_A, .corner = 1, .from = 255.0f, .to = 0.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },
	{ .layer = CREDITS_LAYER_COVER, .entity = CREDITS_COVER, .field = UI_FIELD_GRADIENT_A, .corner = 2, .from = 255.0f, .to = 0.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },
	{ .layer = CREDITS_LAYER_COVER, .entity = CREDITS_COVER, .field = UI_FIELD_GRADIENT_A, .corner = 3, .from = 255.0f, .to = 0.0f, .duration = 0.25f, .easing = UI_EASING_CUBIC_IN },

	{ .layer = CREDITS_LAYER_COVER, .entity = CREDITS_COVER, .field = UI_FIELD_HIDDEN, .from_bool = true, .to_bool = false, .duration = 0.25f },
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
#ifdef ENGINE_ULTRA64
	TextBox box;
	text_measure(&credits_roll_prefab[CREDITS_ROLL].graphic.text, &box);
	credits_offset_max = (int)(CREDITS_BASE_Y + box.y1 - CREDITS_SCROLL_END);
#else
	int nbytes = sizeof(credits_roll_text) - 1;
	rdpq_paragraph_t *layout = rdpq_paragraph_build(&roll_parms, XOLONIUM_14, credits_roll_text, &nbytes);
	credits_offset_max = (int)(CREDITS_BASE_Y + layout->bbox.y1 - CREDITS_SCROLL_END);
	rdpq_paragraph_free(layout);
#endif

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

	Entity2D *roll = scene2d_getEntity(scene2d_get(), CREDITS_LAYER_ROLL, CREDITS_ROLL);
	roll->position.y = CREDITS_BASE_Y - credits_offset;
}
