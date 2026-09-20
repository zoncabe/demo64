#ifndef DEMO_SCENE2D_H
#define DEMO_SCENE2D_H

#include ENGINE_HEADER(scene2d, scene2d)


/* One scene per Kenney sample screen. */
extern const Scene2DDef demo_scene2d_pixel_a;
extern const Scene2DDef demo_scene2d_pixel_b;
extern const Scene2DDef demo_scene2d_industry;
extern const Scene2DDef demo_scene2d_line;


/* The stages of the 2D game, in the order they are walked. Read only and
   loaded by nobody here: whoever plays them opens one at a time, so a single
   stage is ever in memory. How many there are comes from the table itself,
   so adding one is a line in it.

   The two positions are where the feet land: coming in from the left, and
   coming back in from the right. Each is the centre of a column whose floor
   has the body's height of air above it, and the top edge of that floor, so
   the body stands instead of sinking into the cells. The maps differ, so
   every stage carries its own pair. */
uint8_t demoStage_getCount(void);

const Scene2DDef *demoStage_getScene(uint8_t index);
Vector2           demoStage_getStart(uint8_t index);
Vector2           demoStage_getReturn(uint8_t index);

#endif
