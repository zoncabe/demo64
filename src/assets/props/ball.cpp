#include "assets/props/ball.h"
#include "sound/e64_prop_sound.h"

/* Color says density, density says how deep it floats: the submerged
   fraction at rest is density over the water's 1000. Green rides at 30%,
   yellow at 80%, red outweighs the water and goes to the bottom. Same
   convention as the crates. */
#define BALL_DENSITY_GREEN 300.0f
#define BALL_DENSITY_YELLOW 800.0f
#define BALL_DENSITY_RED 1200.0f

#define BALL_SHAPE(d) { .type = e64::physics::Shape::SHAPE_SPHERE, .sphere = { \
	.radius = 0.5f, \
	.friction = 0.4f, .restitution = 0.1f, .density = d }}

static const e64::physics::Shape::Def green_ball_shapes[] = { BALL_SHAPE(BALL_DENSITY_GREEN) };
static const e64::physics::Shape::Def yellow_ball_shapes[] = { BALL_SHAPE(BALL_DENSITY_YELLOW) };
static const e64::physics::Shape::Def red_ball_shapes[] = { BALL_SHAPE(BALL_DENSITY_RED) };

const e64::collider::Def green_ball_collider = { green_ball_shapes, 1 };
const e64::collider::Def yellow_ball_collider = { yellow_ball_shapes, 1 };
const e64::collider::Def red_ball_collider = { red_ball_shapes, 1 };

const e64::RigidBody::Def ball_body = {
	.gravity_scale = 1.0f,
	.body_type = e64::RigidBody::BODY_DYNAMIC,
	.allow_sleep = 1,
	.awake = 1,
	.active = 1,
};

/* The splash is the pool's, not the ball's: the surface plays it for
   whatever falls in. What is left here is the bounce, on the crate thuds
   until the ball gets its own. */
#define BALL_THUD(file) { \
	.path = "rom:/audio/" file ".wav64", \
	.volume = 1.0f, \
	.min_distance = 0.8f, \
	.max_distance = 16.0f, \
	.loop = false, \
	.trigger = e64::propSound::TRIGGER_COLLISION, \
	.priority = e64::Sound::PRIORITY_ONESHOT, \
	.preload = false, \
}

static const e64::Sound::Def ball_thud_1 = BALL_THUD("thud_01");
static const e64::Sound::Def ball_thud_2 = BALL_THUD("thud_02");
static const e64::Sound::Def ball_thud_3 = BALL_THUD("thud_03");
static const e64::Sound::Def ball_thud_4 = BALL_THUD("thud_04");

static const e64::Sound::Def *const ball_sound[] = {
	&ball_thud_1, &ball_thud_2, &ball_thud_3, &ball_thud_4,
};

const e64::Prefab3D red_ball = {
	.type = e64::prefab3d::PREFAB3D_PROP,
	.mesh = { .model = red_ball_model },
	.collider = &red_ball_collider,
	.prop = &ball_body,
	.sound = ball_sound,
	.sound_count = 4,
};

const e64::Prefab3D yellow_ball = {
	.type = e64::prefab3d::PREFAB3D_PROP,
	.mesh = { .model = yellow_ball_model },
	.collider = &yellow_ball_collider,
	.prop = &ball_body,
	.sound = ball_sound,
	.sound_count = 4,
};

const e64::Prefab3D green_ball = {
	.type = e64::prefab3d::PREFAB3D_PROP,
	.mesh = { .model = green_ball_model },
	.collider = &green_ball_collider,
	.prop = &ball_body,
	.sound = ball_sound,
	.sound_count = 4,
};
