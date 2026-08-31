#include "assets/levels/room.h"

static const PhysicsShapeDef room_shapes[] = {
	{ .type = SHAPE_MESH, .mesh = {
		.path        = "rom:/collision/room.collision",
		.friction    = 0.9f,
		.restitution = 0.1f,
	}},
};

const EntityColliderDef room_collider = { room_shapes, 1 };

/* No body: a prop without one is static, which is what a room is. */
const Prefab room = {
	.type     = PREFAB_PROP,
	.model    = room_model,
	.collider = &room_collider,
};
