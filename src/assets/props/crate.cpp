#include "assets/props/crate.h"
#include ENGINE_HEADER(sound, prop_sound)

/* Color says density, density says how deep it floats: the submerged
   fraction at rest is density over the water's 1000. Green rides at 30%,
   yellow at 80%, red outweighs the water and goes to the bottom. Same
   convention as the balls. */
#define CRATE_DENSITY_GREEN   300.0f
#define CRATE_DENSITY_YELLOW  800.0f
#define CRATE_DENSITY_RED    1200.0f

#define CRATE_SHAPE(d) { .type = SHAPE_BOX, .box = { \
	.e = { 0.5f, 0.5f, 0.5f }, \
	.friction = 0.4f, .restitution = 0.1f, .density = d }}

static const PhysicsShapeDef green_crate_shapes[]  = { CRATE_SHAPE(CRATE_DENSITY_GREEN) };
static const PhysicsShapeDef yellow_crate_shapes[] = { CRATE_SHAPE(CRATE_DENSITY_YELLOW) };
static const PhysicsShapeDef red_crate_shapes[]    = { CRATE_SHAPE(CRATE_DENSITY_RED) };

const entity3d::ColliderDef green_crate_collider  = { green_crate_shapes,  1 };
const entity3d::ColliderDef yellow_crate_collider = { yellow_crate_shapes, 1 };
const entity3d::ColliderDef red_crate_collider    = { red_crate_shapes,    1 };

const RigidBodyDef crate_body = {
	.gravity_scale = 1.0f,
	.body_type     = BODY_DYNAMIC,
	.allow_sleep   = 1,
	.awake         = 1,
	.active        = 1,
};

/* The splash is the pool's, not the crate's: the surface plays it for
   whatever falls in. */
#define CRATE_THUD(file) { \
	.path         = "rom:/audio/" file ENGINE_WAVE_EXT, \
	.volume       = 1.0f, \
	.min_distance = 0.8f, \
	.max_distance = 16.0f, \
	.loop         = false, \
	.trigger      = PROP_SOUND_COLLISION, \
	.priority     = SOUND_PRIORITY_ONESHOT, \
	.preload      = false, \
}

static const SoundDef crate_thud_1 = CRATE_THUD("thud_01");
static const SoundDef crate_thud_2 = CRATE_THUD("thud_02");
static const SoundDef crate_thud_3 = CRATE_THUD("thud_03");
static const SoundDef crate_thud_4 = CRATE_THUD("thud_04");

static const SoundDef *const crate_sound[] = {
	&crate_thud_1, &crate_thud_2, &crate_thud_3, &crate_thud_4,
};

const Prefab3D green_crate = {
	.type        = PREFAB3D_PROP,
	.model       = green_crate_model,
	.sound       = crate_sound,
	.sound_count = 4,
	.collider    = &green_crate_collider,
	.prop        = &crate_body,
};

const Prefab3D yellow_crate = {
	.type        = PREFAB3D_PROP,
	.model       = yellow_crate_model,
	.sound       = crate_sound,
	.sound_count = 4,
	.collider    = &yellow_crate_collider,
	.prop        = &crate_body,
};

const Prefab3D red_crate = {
	.type        = PREFAB3D_PROP,
	.model       = red_crate_model,
	.sound       = crate_sound,
	.sound_count = 4,
	.collider    = &red_crate_collider,
	.prop        = &crate_body,
};
