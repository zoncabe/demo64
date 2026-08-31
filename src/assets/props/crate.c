#include "assets/props/crate.h"
#include "assets/sound/sound.h"

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

const EntityColliderDef green_crate_collider  = { green_crate_shapes,  1 };
const EntityColliderDef yellow_crate_collider = { yellow_crate_shapes, 1 };
const EntityColliderDef red_crate_collider    = { red_crate_shapes,    1 };

const RigidBodyDef crate_body = {
	.body_type     = BODY_DYNAMIC,
	.gravity_scale = 1.0f,
	.allow_sleep   = 1,
	.awake         = 1,
	.active        = 1,
};

/* The splash is the pool's, not the crate's: the surface plays it for
   whatever falls in. */
static const SoundID crate_thud[] = { SOUND_THUD_1, SOUND_THUD_2,
                                      SOUND_THUD_3, SOUND_THUD_4 };

static const PrefabSound crate_sound[] = {
	{ PREFAB_SOUND_COLLISION, crate_thud, 4 },
};

const Prefab green_crate = {
	.type        = PREFAB_PROP,
	.model       = green_crate_model,
	.sound       = crate_sound,
	.sound_count = 1,
	.collider    = &green_crate_collider,
	.prop        = &crate_body,
};

const Prefab yellow_crate = {
	.type        = PREFAB_PROP,
	.model       = yellow_crate_model,
	.sound       = crate_sound,
	.sound_count = 1,
	.collider    = &yellow_crate_collider,
	.prop        = &crate_body,
};

const Prefab red_crate = {
	.type        = PREFAB_PROP,
	.model       = red_crate_model,
	.sound       = crate_sound,
	.sound_count = 1,
	.collider    = &red_crate_collider,
	.prop        = &crate_body,
};
