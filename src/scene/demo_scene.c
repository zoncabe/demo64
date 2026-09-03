#include ENGINE_HEADER(scene3d, scene3d)
#include "scene/scene.h"
#include "camera/camera.h"
#include "assets/characters/miss_jiggles.h"
#include "assets/levels/room.h"
#include "assets/props/pole.h"
#include "assets/props/lamp.h"
#include "assets/props/crate.h"
#include "assets/props/ball.h"
#include "assets/props/brew64_flag.h"
#include "assets/props/ladder.h"
#include "assets/levels/pool.h"


/* The two lamps hanging over the room, plus barely enough ambient to keep
   the corners readable. Seven slots, shared between kinds, walked in order
   and cut at the first empty one: the five left over cost nothing. The
   positions are the lamp placements below — move one and the other has to
   follow. */
static const LightDef demo_light = {

	.ambient_color = {10, 10, 10, 0xFF},

	.source = {
		{ .type  = LIGHT_POINT,
		  .color = {255, 255, 255, 0xFF},
		  .point = { .position = { .x =  1300.0f, .y = 0.0f, .z = 869.0f }, .size = 2500.0f } },

		{ .type  = LIGHT_POINT,
		  .color = {255, 255, 255, 0xFF},
		  .point = { .position = { .x = -1300.0f, .y = 0.0f, .z = 869.0f }, .size = 2500.0f } },
	},
};

static const FogDef demo_fog = {

	.color   = {70, 80, 100, 0xFF},
	.near    = 800.0f,
	.far     = 4500.0f,
	.enabled = true,
};


/* What the scene contains and where: prefab, position, rotation, scale.
   The load instances these in order; not const because each placement
   receives the reference to the entity it produced. */
static Scene3DPrefab demo_prefabs[] = {

	{ &miss_jiggles, {1600.0f, -1600.0f, 200.0f}, {0.0f, 0.0f, 125.0f} },

	{ &room, {0.0f, 0.0f, -2.0f} },

	/* Flag parked with its pole: the cloth's cost was eating the framerate.
	{ &pole,        {0.0f, 0.0f,  -2.0f}, {0}, {1.0f, 1.0f, 2.0f} },
	{ &brew64_flag, {5.0f, 0.0f, 688.0f}, {0}, {2.5f, 1.0f, 2.5f} },
	*/

	{ &pole, { 1300.0f, 0.0f,   0.0f}, {0}, {1.0f, 1.0f, 1.4f} },
	{ &lamp, { 1300.0f, 0.0f, 869.0f}, {0}, {2.0f, 2.0f, 2.0f} },
	{ &pole, {-1300.0f, 0.0f,   0.0f}, {0}, {1.0f, 1.0f, 1.4f} },
	{ &lamp, {-1300.0f, 0.0f, 869.0f}, {0}, {2.0f, 2.0f, 2.0f} },


	{ &ladder, {1250.0f, 1478.0f,  -2.0f}, {0}, {1.0f, 1.0f, 1.03f} },
	{ &ladder, {1250.0f, 1478.0f, 513.0f}, {0}, {1.0f, 1.0f, 1.03f} },

	/* Deck props parked for the pool stress test below.
	{ &red_crate,    {1200.0f, 1000.0f,  50.0f} },
	{ &yellow_crate, {1200.0f, 1000.0f, 152.0f} },
	{ &green_crate,  {1200.0f, 1000.0f, 254.0f} },

	{ &red_ball,    {1200.0f, 1500.0f,  50.0f} },
	{ &yellow_ball, {1200.0f, 1500.0f, 137.0f}, {0}, {0.7f, 0.7f, 0.7f} },
	{ &green_ball,  {1200.0f, 1500.0f, 196.0f}, {0}, {0.45f, 0.45f, 0.45f} },
	*/

	/* Last on purpose: the water is transparent and z-writes, so it has to
	   blend over everything already drawn. The model is authored centred on
	   its origin, so the placement is what puts the plane over the basin. */
	{ &pool, {-553.57f, 1220.92f, -50.0f} },
};


Scene3DDef demo_scene = {

	.light  = &demo_light,
	.fog    = &demo_fog,
	.camera = &camera_def,

	.wind = { 220.0f, 50.0f, 50.0f },

	.prefab       = demo_prefabs,
	.prefab_count = sizeof(demo_prefabs) / sizeof(demo_prefabs[0]),
};
