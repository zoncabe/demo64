#include "assets/levels/pool.h"
#include "assets/sound/sound.h"

/* The pool as a volume: footprint of the water plane, from the basin floor
   up to the resting surface. Sensor, so it reports who is inside without
   colliding; buoyancy reads its overlaps once the water binds to it. Local
   to the body, in metres: the plane is authored around its own origin, so
   the box hangs from z 0 and the placement is what sinks it in the basin. */
static const PhysicsShapeDef pool_shapes[] = {
	{ .type = SHAPE_BOX, .box = {
		.tx     = { .position = { 0.5357f, 0.2908f, -0.975f } },
		.e      = { 15.0f, 7.5f, 0.975f },
		.sensor = SENSOR_VOLUME,
	}},
};

const EntityColliderDef pool_collider = { pool_shapes, 1 };

/* The splash belongs to the surface, not to whatever falls in: the character's
   own swim splashes double for every body, until the props get their own. */
static const SoundID pool_splash[] = { SOUND_SWIM_SPLASH_1, SOUND_SWIM_SPLASH_2 };

const WaterDef pool_water = {
	.mesh_path = "rom:/collision/water.collision",

	.entry_sound       = pool_splash,
	.entry_sound_count = 2,

	.wave = {
		{ .direction_x =  1.0f, .direction_y =  0.3f, .amplitude = 0.05f, .frequency = 1.6f, .speed = 1.6f },
		{ .direction_x = -0.4f, .direction_y =  1.0f, .amplitude = 0.03f, .frequency = 2.9f, .speed = 2.3f },
		{ .direction_x =  0.6f, .direction_y = -1.0f, .amplitude = 0.02f, .frequency = 4.3f, .speed = 3.1f },
	},
	.wave_count = 3,
	.scroll_a = {  1.5f, 2.4f },
	.scroll_b = { -2.0f, 1.0f },
	.wrap_a = 64.0f,
	.wrap_b = 64.0f,
	.color  = { 110, 180, 215 },
};

const Prefab pool = {
	.type     = PREFAB_WATER,
	.model    = pool_model,
	.collider = &pool_collider,
	.water    = &pool_water,
};
