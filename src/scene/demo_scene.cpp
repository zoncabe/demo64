#include ENGINE_HEADER(scene3d, scene3d)
#include "scene/scene.h"
#include "camera/camera.h"
#include "assets/characters/miss_jiggles.h"
#include "assets/levels/room.h"
#include "assets/props/lamp_post.h"
#include "assets/props/crate.h"
#include "assets/props/ball.h"
#include "assets/props/brew64_flag.h"
#include "assets/props/ladder.h"
#include "assets/levels/pool.h"


/* Everything here is in metres, which is what the engine works in.

   The two lamps hanging over the room, plus barely enough ambient to keep
   the corners readable. Seven slots, shared between kinds, walked in order
   and cut at the first empty one: the five left over cost nothing.

   Each light sits inside the glass of one lamp post: the placements below are
   at x 13 and -13, and the height is the post's lamp part less the radius that
   puts it in the middle of the sphere — move one and the other has to
   follow. */
static const LightDef demo_light = {

	.ambient_color = {10, 10, 10, 0xFF},

	.source = {
		{ .type  = LIGHT_POINT,
		  .color = {255, 255, 255, 0xFF},
		  .point = { .position = { .x =  13.0f, .y = 0.0f, .z = LAMP_POST_HEIGHT - 0.42f }, .size = 25.0f } },

		{ .type  = LIGHT_POINT,
		  .color = {255, 255, 255, 0xFF},
		  .point = { .position = { .x = -13.0f, .y = 0.0f, .z = LAMP_POST_HEIGHT - 0.42f }, .size = 25.0f } },
	},
};

static const FogDef demo_fog = {

	.color   = {70, 80, 100, 0xFF},
	.near    = 10.0f,
	.far     = 45.0f,
	.enabled = true,
};


/* What the scene contains and where: prefab, position, rotation, scale.
   The load instances these in order; not const because each placement
   receives the reference to the entity it produced. */
static Scene3DPrefab demo_prefabs[] = {

	{ &miss_jiggles, {16.0f, -16.0f, 2.0f}, {0.0f, 0.0f, 125.0f} },

	{ &room, {0.0f, 0.0f, -0.02f} },

	/* Flag parked with its pole: the cloth's cost was eating the framerate.
	{ &lamp_post,   {0.0f, 0.0f, -0.02f} },
	{ &brew64_flag, {0.05f, 0.0f, 6.88f}, {0}, {2.5f, 1.0f, 2.5f} },
	*/

	{ &lamp_post, { 13.0f, 0.0f, 0.0f} },
	{ &lamp_post, {-13.0f, 0.0f, 0.0f} },


	{ &ladder, {12.5f, 14.78f, -0.02f}, {0}, {1.0f, 1.0f, 1.03f} },
	{ &ladder, {12.5f, 14.78f,  5.13f}, {0}, {1.0f, 1.0f, 1.03f} },

	/* Deck props parked for the pool stress test below.
	*/
	{ &red_crate,    {12.0f, 10.0f, 0.50f} },
	{ &yellow_crate, {12.0f, 10.0f, 1.52f} },
	{ &green_crate,  {12.0f, 10.0f, 2.54f} },

	{ &red_ball,    {12.0f, 14.0f, 0.50f} },
	{ &yellow_ball, {12.0f, 14.0f, 1.40f}, {0}, {0.7f, 0.7f, 0.7f} },
	{ &green_ball,  {12.0f, 14.0f, 2.00f}, {0}, {0.45f, 0.45f, 0.45f} },

	/* Last on purpose: the water is transparent and z-writes, so it has to
	   blend over everything already drawn. The model is authored centred on
	   its origin, so the placement is what puts the plane over the basin. */
	{ &pool, {-5.5357f, 12.2092f, -0.5f} },
};


Scene3DDef demo_scene = {

	.light  = &demo_light,
	.fog    = &demo_fog,
	.camera = &camera_def,

	.wind = { 2.2f, 0.5f, 0.5f },

	.prefab       = demo_prefabs,
	.prefab_count = sizeof(demo_prefabs) / sizeof(demo_prefabs[0]),
};
