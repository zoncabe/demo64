/*
	The demo's screens. The engine opens none on its own: whoever wants one
	asks for it, and these are the ones this game asks for.
*/
#include "viewport/demo_viewport.h"


const ViewportModeDef demo_viewport_320x240 = {

	.width      = 320,
	.height     = 240,
	.interlaced = INTERLACE_OFF,
};

const ViewportModeDef demo_viewport_640x240 = {

	.width      = 640,
	.height     = 240,
	.interlaced = INTERLACE_OFF,

	/* Its pixels are half as wide as they are tall, so the art goes at two
	   across and one down and comes out the size it has at 320. */
	.scale_x    = 2.0f,
	.scale_y    = 1.0f,
};
