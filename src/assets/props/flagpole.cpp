/*
	The flagpole: the same post as the lamp post's, 5.5012 tall, without the
	lamp. The flag is its own prefab; the scene hangs it from the top.

	The collision is a box as thin as the pole itself.
*/
#include "assets/props/flagpole.h"


static const e64::physics::Shape::Def flagpole_shapes[] = {
	{ .type = e64::physics::Shape::SHAPE_BOX, .box = {
		.tx = { .position = { 0.0f, 0.0f, 2.75f } },
		.e = { 0.0422f, 0.0422f, 2.75f },
	}},
};

const e64::collider::Def flagpole_collider = { flagpole_shapes, 1 };

const e64::Prefab3D flagpole = {
	.type = e64::prefab3d::PREFAB3D_PROP,
	.mesh = { .model = flagpole_model },
	.collider = &flagpole_collider,
};
