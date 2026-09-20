#include "assets/props/ball.h"
#include ENGINE_HEADER(sound, prop_sound)

/* Color says density, density says how deep it floats: the submerged
   fraction at rest is density over the water's 1000. Green rides at 30%,
   yellow at 80%, red outweighs the water and goes to the bottom. Same
   convention as the crates. */
#define BALL_DENSITY_GREEN   300.0f
#define BALL_DENSITY_YELLOW  800.0f
#define BALL_DENSITY_RED    1200.0f

#define BALL_SHAPE(d) { .type = SHAPE_SPHERE, .sphere = { \
	.radius = 0.5f, \
	.friction = 0.4f, .restitution = 0.1f, .density = d }}

static const PhysicsShapeDef green_ball_shapes[]  = { BALL_SHAPE(BALL_DENSITY_GREEN) };
static const PhysicsShapeDef yellow_ball_shapes[] = { BALL_SHAPE(BALL_DENSITY_YELLOW) };
static const PhysicsShapeDef red_ball_shapes[]    = { BALL_SHAPE(BALL_DENSITY_RED) };

const entity3d::ColliderDef green_ball_collider  = { green_ball_shapes,  1 };
const entity3d::ColliderDef yellow_ball_collider = { yellow_ball_shapes, 1 };
const entity3d::ColliderDef red_ball_collider    = { red_ball_shapes,    1 };

const RigidBodyDef ball_body = {
	.gravity_scale = 1.0f,
	.body_type     = BODY_DYNAMIC,
	.allow_sleep   = 1,
	.awake         = 1,
	.active        = 1,
};

/* The splash is the pool's, not the ball's: the surface plays it for
   whatever falls in. What is left here is the bounce, on the crate thuds
   until the ball gets its own. */
#define BALL_THUD(file) { \
	.path         = "rom:/audio/" file ENGINE_WAVE_EXT, \
	.volume       = 1.0f, \
	.min_distance = 0.8f, \
	.max_distance = 16.0f, \
	.loop         = false, \
	.trigger      = PROP_SOUND_COLLISION, \
	.priority     = SOUND_PRIORITY_ONESHOT, \
	.preload      = false, \
}

static const SoundDef ball_thud_1 = BALL_THUD("thud_01");
static const SoundDef ball_thud_2 = BALL_THUD("thud_02");
static const SoundDef ball_thud_3 = BALL_THUD("thud_03");
static const SoundDef ball_thud_4 = BALL_THUD("thud_04");

static const SoundDef *const ball_sound[] = {
	&ball_thud_1, &ball_thud_2, &ball_thud_3, &ball_thud_4,
};

const Prefab3D red_ball = {
	.type        = PREFAB3D_PROP,
	.model       = red_ball_model,
	.sound       = ball_sound,
	.sound_count = 4,
	.collider    = &red_ball_collider,
	.prop        = &ball_body,
};

const Prefab3D yellow_ball = {
	.type        = PREFAB3D_PROP,
	.model       = yellow_ball_model,
	.sound       = ball_sound,
	.sound_count = 4,
	.collider    = &yellow_ball_collider,
	.prop        = &ball_body,
};

const Prefab3D green_ball = {
	.type        = PREFAB3D_PROP,
	.model       = green_ball_model,
	.sound       = ball_sound,
	.sound_count = 4,
	.collider    = &green_ball_collider,
	.prop        = &ball_body,
};
