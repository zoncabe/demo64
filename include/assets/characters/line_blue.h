#ifndef LINE_BLUE_H
#define LINE_BLUE_H

#include ENGINE_HEADER(prefab, prefab2d)


/* Kenney's pixel-line-platformer blue runner, 16 px: two running frames,
   and three more with the gun for later. */
typedef enum {

	LINE_BLUE_ANIM_IDLE,
	LINE_BLUE_ANIM_WALK,
	LINE_BLUE_ANIM_JUMP,
	LINE_BLUE_ANIM_FALL,
	LINE_BLUE_ANIM_LAND,

	LINE_BLUE_ANIM_COUNT,

} LineBlueAnimation;


extern const Prefab2D line_blue;

#endif
