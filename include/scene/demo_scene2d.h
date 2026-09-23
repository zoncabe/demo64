#ifndef DEMO_SCENE2D_H
#define DEMO_SCENE2D_H

#include "scene2d/e64_scene2d.h"


/* One scene per Kenney sample screen. */
extern const e64::scene2d::Def scene2d_pixel_a;
extern const e64::scene2d::Def scene2d_pixel_b;
extern const e64::scene2d::Def scene2d_industry;
extern const e64::scene2d::Def scene2d_line;


/* The stages of the 2D game, in the order they are walked. Read only and
   loaded by nobody here: whoever plays them opens one at a time, so a single
   stage is ever in memory. How many there are comes from the table itself,
   so adding one is a line in it.

   The two positions are where the feet land: coming in from the left, and
   coming back in from the right. Each is the centre of a column whose floor
   has the body's height of air above it, and the top edge of that floor, so
   the body stands instead of sinking into the cells. The maps differ, so
   every stage carries its own pair. */
uint8_t stage_getCount(void);

const e64::scene2d::Def *stage_getScene(uint8_t index);
e64::Vector2 stage_getStart(uint8_t index);
e64::Vector2 stage_getReturn(uint8_t index);

#endif
