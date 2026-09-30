#include "assets/props/crate.h"
#include "sound/e64_prop_sound.h"

/* Color says density, density says how deep it floats: the submerged
   fraction at rest is density over the water's 1000. Green rides at 30%,
   yellow at 80%, red outweighs the water and goes to the bottom. Same
   convention as the balls. */
#define CRATE_DENSITY_GREEN 300.0f
#define CRATE_DENSITY_YELLOW 800.0f
#define CRATE_DENSITY_RED 1200.0f

#define CRATE_SHAPE(d) { .type = e64::physics::Shape::SHAPE_BOX, .box = { \
	.e = { 0.5f, 0.5f, 0.5f }, \
	.friction = 0.4f, .restitution = 0.1f, .density = d }}

static const e64::physics::Shape::Def green_crate_shapes[] = { CRATE_SHAPE(CRATE_DENSITY_GREEN) };
static const e64::physics::Shape::Def yellow_crate_shapes[] = { CRATE_SHAPE(CRATE_DENSITY_YELLOW) };
static const e64::physics::Shape::Def red_crate_shapes[] = { CRATE_SHAPE(CRATE_DENSITY_RED) };

const e64::collider::Def green_crate_collider = { green_crate_shapes, 1 };
const e64::collider::Def yellow_crate_collider = { yellow_crate_shapes, 1 };
const e64::collider::Def red_crate_collider = { red_crate_shapes, 1 };

const e64::RigidBody::Def crate_body = {
	.gravity_scale = 1.0f,
	.body_type = e64::RigidBody::BODY_DYNAMIC,
	.allow_sleep = 1,
	.awake = 1,
	.active = 1,
};

/* The splash is the pool's, not the crate's: the surface plays it for
   whatever falls in. */
#define CRATE_THUD(file) { \
	.path = "rom:/audio/" file ".wav64", \
	.volume = 1.0f, \
	.min_distance = 0.8f, \
	.max_distance = 16.0f, \
	.loop = false, \
	.trigger = e64::propSound::TRIGGER_COLLISION, \
	.priority = e64::Sound::PRIORITY_ONESHOT, \
	.preload = false, \
}

static const e64::Sound::Def crate_thud_1 = CRATE_THUD("thud_01");
static const e64::Sound::Def crate_thud_2 = CRATE_THUD("thud_02");
static const e64::Sound::Def crate_thud_3 = CRATE_THUD("thud_03");
static const e64::Sound::Def crate_thud_4 = CRATE_THUD("thud_04");

static const e64::Sound::Def *const crate_sound[] = {
	&crate_thud_1, &crate_thud_2, &crate_thud_3, &crate_thud_4,
};

const e64::Prefab3D green_crate = {
	.type = e64::prefab3d::PREFAB3D_PROP,
	.mesh = { .model = green_crate_model },
	.collider = &green_crate_collider,
	.prop = &crate_body,
	.sound = crate_sound,
	.sound_count = 4,
};

const e64::Prefab3D yellow_crate = {
	.type = e64::prefab3d::PREFAB3D_PROP,
	.mesh = { .model = yellow_crate_model },
	.collider = &yellow_crate_collider,
	.prop = &crate_body,
	.sound = crate_sound,
	.sound_count = 4,
};

const e64::Prefab3D red_crate = {
	.type = e64::prefab3d::PREFAB3D_PROP,
	.mesh = { .model = red_crate_model },
	.collider = &red_crate_collider,
	.prop = &crate_body,
	.sound = crate_sound,
	.sound_count = 4,
};
