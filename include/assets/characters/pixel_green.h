#ifndef PIXEL_GREEN_H
#define PIXEL_GREEN_H

#include "prefab/e64_prefab2d.h"


/* Kenney's pixel-platformer green blob, 24 px: a standing frame and a
   stepping one, so the walk is the pair and everything else holds one. */
typedef enum {

	PIXEL_GREEN_ANIM_IDLE,
	PIXEL_GREEN_ANIM_WALK,
	PIXEL_GREEN_ANIM_JUMP,
	PIXEL_GREEN_ANIM_FALL,
	PIXEL_GREEN_ANIM_LAND,

	PIXEL_GREEN_ANIM_COUNT,

} PixelGreenAnimation;


extern const e64::Prefab2D pixel_green;

#endif
