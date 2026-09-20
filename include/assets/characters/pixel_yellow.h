#ifndef PIXEL_YELLOW_H
#define PIXEL_YELLOW_H

#include ENGINE_HEADER(prefab, prefab2d)


/* Kenney's pixel-platformer yellow blob, 24 px: a standing frame and a
   stepping one, so the walk is the pair and everything else holds one. */
typedef enum {

	PIXEL_YELLOW_ANIM_IDLE,
	PIXEL_YELLOW_ANIM_WALK,
	PIXEL_YELLOW_ANIM_JUMP,
	PIXEL_YELLOW_ANIM_FALL,
	PIXEL_YELLOW_ANIM_LAND,

	PIXEL_YELLOW_ANIM_COUNT,

} PixelYellowAnimation;


extern const Prefab2D pixel_yellow;

#endif
