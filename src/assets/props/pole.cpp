#include "assets/props/pole.h"

static const e64::physics::Shape::Def pole_shapes[] = {
	{ .type = e64::physics::Shape::SHAPE_BOX, .box = {
		.tx = { .position = { 0.0f, 0.0f, 2.75f } },
		.e = { 0.0422f, 0.0422f, 2.75f },
	}},
};

const e64::collider::Def pole_collider = { pole_shapes, 1 };

const e64::Prefab3D pole = {
	.type = e64::prefab3d::PREFAB3D_PROP,
	.model = pole_model,
	.collider = &pole_collider,
};
