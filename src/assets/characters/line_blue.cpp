/*
	Kenney's pixel-line-platformer blue runner: two running drawings cut
	into the clips the animator asks for. Speeds in pixels per second for a
	body 16 px tall on 16 px tiles.
*/
#include "assets/characters/line_blue.h"


#define LB_FRAME(clip) "rom:/sprites/characters/line_blue/" clip "/line_blue_" clip "_f00.sprite"

static const e64::character2d::AnimationClipDef line_blue_clips[LINE_BLUE_ANIM_COUNT] = {

	[LINE_BLUE_ANIM_IDLE] = { LB_FRAME("idle"), 1, 1.0f, true },
	[LINE_BLUE_ANIM_WALK] = { LB_FRAME("walk"), 2, 6.0f, true },
	[LINE_BLUE_ANIM_JUMP] = { LB_FRAME("jump"), 1, 1.0f, false },
	[LINE_BLUE_ANIM_FALL] = { LB_FRAME("jump"), 1, 1.0f, true },
	[LINE_BLUE_ANIM_LAND] = { LB_FRAME("idle"), 1, 4.0f, false },
};

static const e64::character2d::AnimationSettings line_blue_animation_settings = {

	.land_anim_ground = 0.25f,
};

static const e64::character2d::AnimationDef line_blue_animation_def = {

	.clip = line_blue_clips,
	.settings = &line_blue_animation_settings,
	.clip_count = LINE_BLUE_ANIM_COUNT,

	.idle_animation = LINE_BLUE_ANIM_IDLE,
	.walk_animation = LINE_BLUE_ANIM_WALK,
	.run_animation = LINE_BLUE_ANIM_WALK,
	.sprint_animation = LINE_BLUE_ANIM_WALK,
	.jump_animation = LINE_BLUE_ANIM_JUMP,
	.fall_animation = LINE_BLUE_ANIM_FALL,
	.land_animation = LINE_BLUE_ANIM_LAND,
	.roll_animation = LINE_BLUE_ANIM_JUMP,
};

static const e64::character2d::GaitSettings line_blue_gaits[] = {

	{ .target_speed = 35.0f, .response_rate = 8.0f },
	{ .target_speed = 70.0f, .response_rate = 9.0f },
	{ .target_speed = 95.0f, .response_rate = 10.0f },
};

static const e64::character2d::MovementSettings line_blue_movement_settings = {

	.idle_response_rate = 12.0f,

	.gait = line_blue_gaits,
	.gait_count = sizeof(line_blue_gaits) / sizeof(line_blue_gaits[0]),

	.roll_target_speed = 100.0f,
	.roll_launch_response_rate = 15.0f,
	.roll_spin_response_rate = 5.0f,
	.roll_grip_response_rate = 2.0f,
	.roll_ground_time = 0.3f,
	.roll_grip_time = 0.6f,
	.roll_timer_max = 0.8f,

	/* Two cells of the 16 px grid on the tap, four with the button held,
	   which is what the map's ledges ask for. */
	.jump_mode = e64::character2d::JUMP2D_SNAP,
	.jump_response_rate = 8.0f,
	.jump_base_speed = 240.0f,
	.jump_hold_gravity_scale = 0.5f,
	.jump_coyote_time = 0.1f,

	.air_control = 0.8f,
};

static const e64::character2d::ColliderSettings line_blue_collider_settings = {

	.radius = 4.0f,
	.height = 14.0f,
};

static const e64::character2d::Def line_blue_character_def = {

	.movement_settings = &line_blue_movement_settings,
	.animation_def = &line_blue_animation_def,
	.collider_settings = &line_blue_collider_settings,
};

const e64::Prefab2D line_blue = {

	.type = e64::prefab2d::PREFAB2D_CHARACTER,
	.graphic = { .type = e64::Graphic::SPRITE, .sprite = { .path = LB_FRAME("idle") } },
	.character = &line_blue_character_def,
	.parallax = 1.0f,
};
