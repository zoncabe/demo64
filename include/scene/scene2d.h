#ifndef SCENE2D_H
#define SCENE2D_H

#include "scene2d/e64_scene2d.h"


/* One scene per Kenney sample screen. The 2D gameplay state declares the
   first and opens the others in its place as the body walks off an edge. */
extern const e64::scene2d::Def scene2d_pixel_a;
extern const e64::scene2d::Def scene2d_pixel_b;
extern const e64::scene2d::Def scene2d_industry;
extern const e64::scene2d::Def scene2d_line;

/* Each screen's placements: the control binding names the body's row, the
   third entry of every screen. */
extern const e64::scene2d::Entity pixel_a_placed[];
extern const e64::scene2d::Entity pixel_b_placed[];
extern const e64::scene2d::Entity industry_placed[];
extern const e64::scene2d::Entity line_placed[];

/* Where the feet land coming back in from the right: the centre of a column
   whose floor has the body's height of air above it, and the top edge of
   that floor. The maps differ, so every screen carries its own. Walking in
   from the left is the placement itself. */
extern const e64::Vector2 pixel_a_return;
extern const e64::Vector2 pixel_b_return;
extern const e64::Vector2 industry_return;
extern const e64::Vector2 line_return;

#endif
