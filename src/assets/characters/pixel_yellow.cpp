/*
	Kenney's pixel-platformer yellow blob: the same two drawings as the
	green one, standing and stepping, for the industrial screen.
*/
#include "assets/characters/pixel_yellow.h"


#define PY_FRAME(clip) "rom:/sprites/characters/pixel_yellow/" clip "/pixel_yellow_" clip "_f00.sprite"

static const character2d::AnimationClipDef pixel_yellow_clips[PIXEL_YELLOW_ANIM_COUNT] = {

	[PIXEL_YELLOW_ANIM_IDLE] = { PY_FRAME("idle"), 1, 1.0f, true  },
	[PIXEL_YELLOW_ANIM_WALK] = { PY_FRAME("walk"), 2, 6.0f, true  },
	[PIXEL_YELLOW_ANIM_JUMP] = { PY_FRAME("jump"), 1, 1.0f, false },
	[PIXEL_YELLOW_ANIM_FALL] = { PY_FRAME("jump"), 1, 1.0f, true  },
	[PIXEL_YELLOW_ANIM_LAND] = { PY_FRAME("idle"), 1, 4.0f, false },
};

static const character2d::AnimationSettings pixel_yellow_animation_settings = {

	.land_anim_ground = 0.25f,
};

static const character2d::AnimationDef pixel_yellow_animation_def = {

	.clip       = pixel_yellow_clips,
	.settings   = &pixel_yellow_animation_settings,
	.clip_count = PIXEL_YELLOW_ANIM_COUNT,

	.idle_animation   = PIXEL_YELLOW_ANIM_IDLE,
	.walk_animation   = PIXEL_YELLOW_ANIM_WALK,
	.run_animation    = PIXEL_YELLOW_ANIM_WALK,
	.sprint_animation = PIXEL_YELLOW_ANIM_WALK,
	.jump_animation   = PIXEL_YELLOW_ANIM_JUMP,
	.fall_animation   = PIXEL_YELLOW_ANIM_FALL,
	.land_animation   = PIXEL_YELLOW_ANIM_LAND,
	.roll_animation   = PIXEL_YELLOW_ANIM_JUMP,
};

static const character2d::GaitSettings pixel_yellow_gaits[] = {

	{ .target_speed =  40.0f, .response_rate =  8.0f },
	{ .target_speed =  80.0f, .response_rate =  9.0f },
	{ .target_speed = 110.0f, .response_rate = 10.0f },
};

static const character2d::MovementSettings pixel_yellow_movement_settings = {

	.idle_response_rate = 12.0f,

	.gait       = pixel_yellow_gaits,
	.gait_count = sizeof(pixel_yellow_gaits) / sizeof(pixel_yellow_gaits[0]),

	.roll_target_speed         = 120.0f,
	.roll_launch_response_rate = 15.0f,
	.roll_spin_response_rate   = 5.0f,
	.roll_grip_response_rate   = 2.0f,
	.roll_ground_time          = 0.3f,
	.roll_grip_time            = 0.6f,
	.roll_timer_max            = 0.8f,

	.jump_mode               = character2d::JUMP2D_SNAP,
	.jump_response_rate      = 8.0f,
	.jump_base_speed         = 200.0f,
	.jump_hold_gravity_scale = 0.5f,
	.jump_coyote_time        = 0.1f,

	.air_control        = 0.8f,
};

static const character2d::ColliderSettings pixel_yellow_collider_settings = {

	.radius = 6.0f,
	.height = 20.0f,
};

static const character2d::Def pixel_yellow_character_def = {

	.movement_settings = &pixel_yellow_movement_settings,
	.animation_def     = &pixel_yellow_animation_def,
	.collider_settings = &pixel_yellow_collider_settings,
};

const Prefab2D pixel_yellow = {

	.type      = PREFAB2D_CHARACTER,
	.graphic   = { .type = GRAPHIC_SPRITE, .sprite = { .path = PY_FRAME("idle") } },
	.character = &pixel_yellow_character_def,
	.parallax  = 1.0f,
};
