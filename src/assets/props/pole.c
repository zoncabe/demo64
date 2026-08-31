#include "assets/props/pole.h"

static const PhysicsShapeDef pole_shapes[] = {
	{ .type = SHAPE_BOX, .box = {
		.tx = { .position = { 0.0f, 0.0f, 2.75f } },
		.e  = { 0.0422f, 0.0422f, 2.75f },
	}},
};

const EntityColliderDef pole_collider = { pole_shapes, 1 };

const Prefab pole = {
	.type     = PREFAB_PROP,
	.model    = pole_model,
	.collider = &pole_collider,
};
