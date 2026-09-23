#include "assets/levels/room.h"

static const e64::physics::Shape::Def room_shapes[] = {
	{ .type = e64::physics::Shape::SHAPE_MESH, .mesh = {
		.path = "rom:/collision/room.collision",
		.friction = 0.9f,
		.restitution = 0.1f,
	}},
};

const e64::collider::Def room_collider = { room_shapes, 1 };

/* No body: a prop without one is static, which is what a room is. */
const e64::Prefab3D room = {
	.type = e64::prefab3d::PREFAB3D_PROP,
	.model = room_model,
	.collider = &room_collider,
};
