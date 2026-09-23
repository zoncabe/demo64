#ifndef DEMO_VIEWPORT_H
#define DEMO_VIEWPORT_H

#include "viewport/e64_viewport.h"


/* The screens the demo runs on.

   The menus and the 3D game use the console's usual one. The 2D game
   doubles the columns: the picture stays 4:3 and nothing interlaces, every
   column is simply split in two, so a body drawn at twice the width moves
   in half steps of its own art instead of whole ones. Its pixels are half
   as wide as they are tall, so whatever is drawn under it goes at twice the
   width to keep its shape. */
extern const e64::Viewport::ModeDef viewport_320x240;
extern const e64::Viewport::ModeDef viewport_640x240;

#endif
