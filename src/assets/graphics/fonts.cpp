#include "assets/graphics/fonts.h"


/* Menu text: grey, selection in yellow. */
static const e64::Font::Style menu_styles[] = {
	{ MENU_STYLE_NORMAL, { .color = RGBA32(200, 200, 200, 255) } },
	{ MENU_STYLE_SELECTED, { .color = RGBA32(255, 220, 30, 255) } },
};

/* Titles: translucent white, warnings in red. */
static const e64::Font::Style title_styles[] = {
	{ MENU_STYLE_NORMAL, { .color = RGBA32(255, 255, 255, 200) } },
	{ TEXT_STYLE_RED, { .color = RGBA32(230, 30, 30, 255) } },
};

#define STYLES(s) s, sizeof(s)/sizeof(*s)

/* Indexed by font id, in the order fonts.h numbers them: slot 0 is the one
   rdpq reserves and stays empty. The %s is the display: the engine loads
   the copy rasterized for the video mode in force. */
const e64::Font::Def fonts[FONT_COUNT] = {

	/* 0 */ { NULL, NULL, 0 },
	/* DROID_SANS */ { "rom:/fonts/%s/DroidSans" ".font64", NULL, 0 },
	/* XOLONIUM_10 */ { "rom:/fonts/%s/Xolonium10" ".font64", NULL, 0 },
	/* XOLONIUM_14 */ { "rom:/fonts/%s/Xolonium14" ".font64", STYLES(menu_styles) },
	/* XOLONIUM_20 */ { "rom:/fonts/%s/Xolonium20" ".font64", STYLES(menu_styles) },
	/* XOLONIUM_40 */ { "rom:/fonts/%s/Xolonium40" ".font64", STYLES(title_styles) },
	/* XOLONIUM_60 */ { "rom:/fonts/%s/Xolonium60" ".font64", STYLES(title_styles) },

};
