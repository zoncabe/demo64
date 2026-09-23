/*
	Kenney's pixel-platformer green blob: two drawings, standing and
	stepping, cut into the clips the animator asks for. Speeds in pixels per
	second for a body 24 px tall on 18 px tiles.
*/
#include "assets/characters/pixel_green.h"


#define PG_FRAME(clip) "rom:/sprites/characters/pixel_green/" clip "/pixel_green_" clip "_f00.sprite"

static const e64::character2d::AnimationClipDef pixel_green_clips[PIXEL_GREEN_ANIM_COUNT] = {

	[PIXEL_GREEN_ANIM_IDLE] = { PG_FRAME("idle"), 1, 1.0f, true },
	[PIXEL_GREEN_ANIM_WALK] = { PG_FRAME("walk"), 2, 6.0f, true },
	[PIXEL_GREEN_ANIM_JUMP] = { PG_FRAME("jump"), 1, 1.0f, false },
	[PIXEL_GREEN_ANIM_FALL] = { PG_FRAME("jump"), 1, 1.0f, true },
	[PIXEL_GREEN_ANIM_LAND] = { PG_FRAME("idle"), 1, 4.0f, false },
};

static const e64::character2d::AnimationSettings pixel_green_animation_settings = {

	/* One frame of landing: it starts a quarter second before the floor. */
	.land_anim_ground = 0.25f,
};

static const e64::character2d::AnimationDef pixel_green_animation_def = {

	.clip = pixel_green_clips,
	.settings = &pixel_green_animation_settings,
	.clip_count = PIXEL_GREEN_ANIM_COUNT,

	.idle_animation = PIXEL_GREEN_ANIM_IDLE,
	.walk_animation = PIXEL_GREEN_ANIM_WALK,
	.run_animation = PIXEL_GREEN_ANIM_WALK,
	.sprint_animation = PIXEL_GREEN_ANIM_WALK,
	.jump_animation = PIXEL_GREEN_ANIM_JUMP,
	.fall_animation = PIXEL_GREEN_ANIM_FALL,
	.land_animation = PIXEL_GREEN_ANIM_LAND,
	.roll_animation = PIXEL_GREEN_ANIM_JUMP,
};

static const e64::character2d::GaitSettings pixel_green_gaits[] = {

	{ .target_speed = 40.0f, .response_rate = 8.0f },
	{ .target_speed = 80.0f, .response_rate = 9.0f },
	{ .target_speed = 110.0f, .response_rate = 10.0f },
};

static const e64::character2d::MovementSettings pixel_green_movement_settings = {

	.idle_response_rate = 12.0f,

	.gait = pixel_green_gaits,
	.gait_count = sizeof(pixel_green_gaits) / sizeof(pixel_green_gaits[0]),

	.roll_target_speed = 120.0f,
	.roll_launch_response_rate = 15.0f,
	.roll_spin_response_rate = 5.0f,
	.roll_grip_response_rate = 2.0f,
	.roll_ground_time = 0.3f,
	.roll_grip_time = 0.6f,
	.roll_timer_max = 0.8f,

	/* Snap: the floor is left on the press itself, holding the button
	   halves gravity on the way up, and the ledge lasts a tenth of a second
	   past its edge. */
	.jump_mode = e64::character2d::JUMP2D_SNAP,
	.jump_response_rate = 8.0f,
	.jump_base_speed = 200.0f,
	.jump_hold_gravity_scale = 0.5f,
	.jump_coyote_time = 0.1f,

	/* Steering in the air answers at jump_response_rate times air_control
	   per second, a little under the walk's 8. */
	.air_control = 0.8f,
};

/* The capsule inside the 24 px frame: the outline has air around it. */
static const e64::character2d::ColliderSettings pixel_green_collider_settings = {

	.radius = 6.0f,
	.height = 20.0f,
};

static const e64::character2d::Def pixel_green_character_def = {

	.movement_settings = &pixel_green_movement_settings,
	.animation_def = &pixel_green_animation_def,
	.collider_settings = &pixel_green_collider_settings,
};

const e64::Prefab2D pixel_green = {

	.type = e64::prefab2d::PREFAB2D_CHARACTER,
	.graphic = { .type = e64::Graphic::SPRITE, .sprite = { .path = PG_FRAME("idle") } },
	.character = &pixel_green_character_def,
	.parallax = 1.0f,
};
