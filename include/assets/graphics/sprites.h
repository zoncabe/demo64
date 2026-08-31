#ifndef SPRITES_H
#define SPRITES_H

#include "graphics/e64_sprites.h"


/* Indices into demo_sprite_paths, the table handed to sprite_init. */
enum {

	SPRITE_GORILLA,
	SPRITE_BTN_A,
	SPRITE_BTN_B,
	SPRITE_D_UP,
	SPRITE_D_DOWN,
	SPRITE_D_LEFT,
	SPRITE_D_RIGHT,
	SPRITE_LIBDRAGON,
	SPRITE_TINY3D,
	SPRITE_ZONCABE,
	SPRITE_CIRCLE_MASK,
	SPRITE_CIRCLE_PROGRESS,
	SPRITE_COUNT,

};

extern const char *const demo_sprite_paths[SPRITE_COUNT];


#endif
