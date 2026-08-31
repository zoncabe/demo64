#include "assets/props/ball.h"
#include "assets/sound/sound.h"

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

const EntityColliderDef green_ball_collider  = { green_ball_shapes,  1 };
const EntityColliderDef yellow_ball_collider = { yellow_ball_shapes, 1 };
const EntityColliderDef red_ball_collider    = { red_ball_shapes,    1 };

const RigidBodyDef ball_body = {
	.body_type     = BODY_DYNAMIC,
	.gravity_scale = 1.0f,
	.allow_sleep   = 1,
	.awake         = 1,
	.active        = 1,
};

/* The splash is the pool's, not the ball's: the surface plays it for
   whatever falls in. What is left here is the bounce, on the crate thuds
   until the ball gets its own. */
static const SoundID ball_thud[] = { SOUND_THUD_1, SOUND_THUD_2,
                                     SOUND_THUD_3, SOUND_THUD_4 };

static const PrefabSound ball_sound[] = {
	{ PREFAB_SOUND_COLLISION, ball_thud, 4 },
};

const Prefab red_ball = {
	.type        = PREFAB_PROP,
	.model       = red_ball_model,
	.sound       = ball_sound,
	.sound_count = 1,
	.collider    = &red_ball_collider,
	.prop        = &ball_body,
};

const Prefab yellow_ball = {
	.type        = PREFAB_PROP,
	.model       = yellow_ball_model,
	.sound       = ball_sound,
	.sound_count = 1,
	.collider    = &yellow_ball_collider,
	.prop        = &ball_body,
};

const Prefab green_ball = {
	.type        = PREFAB_PROP,
	.model       = green_ball_model,
	.sound       = ball_sound,
	.sound_count = 1,
	.collider    = &green_ball_collider,
	.prop        = &ball_body,
};
