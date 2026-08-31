#include "assets/props/ladder.h"

/* Two volumes over the same rungs.

   The solid one is what the body walks into: grown well past the mesh, 60
   wide against the rungs' 39 and 40 deep against a mesh that is 2. A slab
   that thin is nothing to push against, and the character's own capsule is
   64 across -- it would sit on the box's edge and slide off the side of it.

   The climbable one is the reach around the rungs, and its frame is what
   the climb is built on: local X runs along a rung, local Y is the face,
   and the top of the box is where the climb ends -- so it stops at the top
   rung, and stacking two ladders leaves no gap between one volume and the
   next.

   Its depth is dictated by the solid box in front of it: a body walking up
   is stopped 55 out, its own 35 of radius past that box's 20, and a volume
   that does not reach the body it stopped can never be grabbed. 65 leaves
   the grab a little room past where the walk ends.

   It also has to start below the ladder's foot. The probe tests the feet,
   and the feet of someone standing at the bottom are exactly level with
   it: flush, the test lands on the boundary and the ladder can only be
   caught by jumping at it. Dropped 40, standing on the ground is inside.
   The top is left where it was, since that is the height the climb hands
   the body over at. Both rooted at the model's origin, the ladder's foot. */
static const PhysicsShapeDef ladder_shapes[] = {
	{ .type = SHAPE_BOX, .box = {
		.tx = { .position = { 0.0f, 0.0f, 2.5f } },
		.e  = { 0.30f, 0.20f, 2.5f },
	}},
	{ .type = SHAPE_BOX, .box = {
		.tx     = { .position = { 0.0f, 0.0f, 2.3f } },
		.e      = { 0.30f, 0.65f, 2.7f },
		.sensor = SENSOR_CLIMBABLE,
	}},
};

const EntityColliderDef ladder_collider = { ladder_shapes, 2 };

const Prefab ladder = {
	.type     = PREFAB_PROP,
	.model    = ladder_model,
	.collider = &ladder_collider,
};
