#include "assets/props/pole.h"

static const PhysicsShapeDef pole_shapes[] = {
	{ .type = SHAPE_BOX, .box = {
		.tx = { .position = { 0.0f, 0.0f, 2.75f } },
		.e  = { 0.0422f, 0.0422f, 2.75f },
	}},
};

const entity3d::ColliderDef pole_collider = { pole_shapes, 1 };

const Prefab3D pole = {
	.type     = PREFAB3D_PROP,
	.model    = pole_model,
	.collider = &pole_collider,
};
