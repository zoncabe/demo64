/*
	The lamp post: one model holding two objects, "post" and "lamp", modelled
	one inside the other at the origin. Naming them as parts is what lets the
	lamp be drawn somewhere other than where it was modelled, and that is what
	puts it on the tip: the post reaches 5.5012 and the sphere has 0.75 of
	radius, so the two together leave it resting there.

	The console has no emissive materials. The lamp is an icosphere with its
	normals flipped inward, so the side facing the camera has its normal aimed
	at the point light the scene puts inside it, and comes out at full
	brightness from any angle: it reads as glass lit from within.

	The collision is the post alone, a box as thin as the pole itself.
*/
#include "assets/props/lamp_post.h"


static const PhysicsShapeDef lamp_post_shapes[] = {
	{ .type = SHAPE_BOX, .box = {
		.tx = { .position = { 0.0f, 0.0f, 2.75f } },
		.e  = { 0.0422f, 0.0422f, 2.75f },
	}},
};

const entity3d::ColliderDef lamp_post_collider = { lamp_post_shapes, 1 };

const Prefab3D lamp_post = {

	.type     = PREFAB3D_PROP,
	.model    = lamp_post_model,

	.part = MESH_PARTS(
		"post",
		"lamp"
	),

	/* A part left at zero is drawn where it was modelled, on the model's own
	   matrix, and costs nothing extra. */
	.part_position = MESH_PART_POSITIONS(
		{ 0.0f, 0.0f, 0.0f             },
		{ 0.0f, 0.0f, LAMP_POST_HEIGHT }
	),

	.part_count = 2,

	.collider = &lamp_post_collider,
};
