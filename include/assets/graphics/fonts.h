#ifndef FONTS_H
#define FONTS_H

#include "graphics/e64_font.h"


/* Indices into demo_fonts, the table handed to font_init. Slot 0 is
   reserved (rdpq font ids start at 1); slot 1 doubles as FONT_DEBUG. */
#define DROID_SANS    1
#define XOLONIUM_10   2
#define XOLONIUM_14   3
#define XOLONIUM_20   4
#define XOLONIUM_40   5
#define XOLONIUM_60   6
#define FONT_COUNT    7

#define MENU_STYLE_NORMAL   1
#define MENU_STYLE_SELECTED 2
#define TEXT_STYLE_RED      3

extern const FontDef demo_fonts[FONT_COUNT];


#endif
