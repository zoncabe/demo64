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

const FontDef demo_fonts[FONT_COUNT] = {

	[DROID_SANS]  = { "rom:/fonts/DroidSans.font64",  NULL, 0 },
	[XOLONIUM_10] = { "rom:/fonts/Xolonium10.font64", NULL, 0 },
	[XOLONIUM_14] = { "rom:/fonts/Xolonium14.font64", STYLES(menu_styles) },
	[XOLONIUM_20] = { "rom:/fonts/Xolonium20.font64", STYLES(menu_styles) },
	[XOLONIUM_40] = { "rom:/fonts/Xolonium40.font64", STYLES(title_styles) },
	[XOLONIUM_60] = { "rom:/fonts/Xolonium60.font64", STYLES(title_styles) },

};
