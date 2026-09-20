#include "assets/graphics/fonts.h"


/* Menu text: grey, selection in yellow. */
static const FontStyle menu_styles[] = {
	{ MENU_STYLE_NORMAL,   { .color = RGBA32(200, 200, 200, 255) } },
	{ MENU_STYLE_SELECTED, { .color = RGBA32(255, 220, 30,  255) } },
};

/* Titles: translucent white, warnings in red. */
static const FontStyle title_styles[] = {
	{ MENU_STYLE_NORMAL, { .color = RGBA32(255, 255, 255, 200) } },
	{ TEXT_STYLE_RED,    { .color = RGBA32(230, 30, 30, 255) } },
};

#define STYLES(s) s, sizeof(s)/sizeof(*s)

/* Indexed by font id, in the order fonts.h numbers them: slot 0 is the one
   rdpq reserves and stays empty. */
const FontDef demo_fonts[FONT_COUNT] = {

	/* 0           */ { NULL, NULL, 0 },
	/* DROID_SANS  */ { "rom:/fonts/DroidSans"  ENGINE_FONT_EXT, NULL, 0 },
	/* XOLONIUM_10 */ { "rom:/fonts/Xolonium10" ENGINE_FONT_EXT, NULL, 0 },
	/* XOLONIUM_14 */ { "rom:/fonts/Xolonium14" ENGINE_FONT_EXT, STYLES(menu_styles) },
	/* XOLONIUM_20 */ { "rom:/fonts/Xolonium20" ENGINE_FONT_EXT, STYLES(menu_styles) },
	/* XOLONIUM_40 */ { "rom:/fonts/Xolonium40" ENGINE_FONT_EXT, STYLES(title_styles) },
	/* XOLONIUM_60 */ { "rom:/fonts/Xolonium60" ENGINE_FONT_EXT, STYLES(title_styles) },

};
