#include "scene3d/e64_scene3d.h"
#include "scene/scene3d.h"
#include "camera/camera.h"
#include "assets/characters/miss_jiggles.h"
#include "assets/characters/mr_muscles.h"
#include "assets/levels/room.h"
#include "assets/props/lamp_post.h"
#include "assets/props/crate.h"
#include "assets/props/ball.h"
#include "assets/props/flagpole.h"
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
static const e64::light::Def light = {

	.ambient_color = {10, 10, 10, 0xFF},

	.source = {
		{ .type = e64::light::LIGHT_POINT,
		  .color = {255, 255, 255, 0xFF},
		  .point = { .position = { .x = 13.0f, .y = 0.0f, .z = LAMP_POST_HEIGHT - 0.42f }, .size = 25.0f } },

		{ .type = e64::light::LIGHT_POINT,
		  .color = {255, 255, 255, 0xFF},
		  .point = { .position = { .x = -13.0f, .y = 0.0f, .z = LAMP_POST_HEIGHT - 0.42f }, .size = 25.0f } },
	},
};

static const e64::fog::Def fog = {

	.color = {70, 80, 100, 0xFF},
	.near = 5.0f,
	.far = 55.0f,
	.enabled = true,
};


/* The scene's entities: prefab, position, rotation, scale. The load builds
   them in order. Not static: the character binding names its row. */
e64::scene3d::Entity scene_entities[] = {

	{ &room, {0.0f, 0.0f, -0.02f} },

	{ &red_crate, {12.0f, 10.0f, 0.50f} },
	{ &yellow_crate, {12.0f, 10.0f, 1.52f} },
	{ &green_crate, {12.0f, 10.0f, 2.54f} },

	{ &red_ball, {12.0f, 14.0f, 0.50f} },
	{ &yellow_ball, {12.0f, 14.0f, 1.40f}, {0}, {0.7f, 0.7f, 0.7f} },
	{ &green_ball, {12.0f, 14.0f, 2.00f}, {0}, {0.45f, 0.45f, 0.45f} },

	{ &ladder, {12.5f, 14.78f, -0.02f}, {0}, {1.0f, 1.0f, 1.03f} },
	{ &ladder, {12.5f, 14.78f, 5.13f}, {0}, {1.0f, 1.0f, 1.03f} },

	{ &lamp_post, { 13.0f, 0.0f, 0.0f} },
	{ &lamp_post, {-13.0f, 0.0f, 0.0f} },

	{ &mr_muscles, {16.97f, -18.03f, 2.0f}, {0.0f, 0.0f, 125.0f} },
	{ &miss_jiggles, {18.03f, -16.97f, 2.0f}, {0.0f, 0.0f, 125.0f} },

	{ &pool, {-5.5357f, 12.2092f, -0.5f} },
};


e64::scene3d::Def scene3d = {

	.light = &light,
	.fog = &fog,
	.camera = &camera_def,

	.wind = { 180.0f, 50.0f, 50.0f },

	.entity = scene_entities,
	.entity_count = sizeof(scene_entities) / sizeof(scene_entities[0]),
};
