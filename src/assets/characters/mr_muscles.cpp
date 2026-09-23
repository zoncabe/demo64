#include "character3d/e64_character3d.h"
#include "assets/characters/mr_muscles.h"

/* Plant phases measured on the clips (walk 11/28 of 31 f, run 9/20 of 23 f,
   sprint 8/18 of 20 f): the proportion is near constant across the gaits, so
   one averaged pair covers the grid. Single source for the animation settings
   and the footstep sounds. */
#define MM_FOOTING_LEFT 0.38f
#define MM_FOOTING_RIGHT 0.89f

/* The swim clips pull an arm at the start and the middle of the cycle. */
#define MM_STROKE_A 0.0f
#define MM_STROKE_B 0.5f


static const e64::Animation::ClipDef mr_muscles_clips[] = {
	 
	[MM_ANIM_IDLE_L] = { "standing-idle-left", e64::Animation::SLOT_MAIN, true },
	[MM_ANIM_IDLE_R] = { "standing-idle-right", MM_SLOT_IDLE_R, true },

	[MM_ANIM_STAND_TO_WALK_L] = { "standing-to-walking-left", MM_SLOT_WALK, false },
	[MM_ANIM_STAND_TO_WALK_R] = { "standing-to-walking-right", MM_SLOT_WALK, false },

	[MM_ANIM_STAND_TO_RUN_L] = { "standing-to-running-left", MM_SLOT_RUN, false },
	[MM_ANIM_STAND_TO_RUN_R] = { "standing-to-running-right", MM_SLOT_RUN, false },

	[MM_ANIM_WALK] = { "walking", MM_SLOT_WALK, true },
	[MM_ANIM_TURN_WALK_L] = { "walking-turn-left", MM_SLOT_TURN_WALK, true },
	[MM_ANIM_TURN_WALK_R] = { "walking-turn-right", MM_SLOT_TURN_WALK, true },
	[MM_ANIM_WALK_CHANGE_DIR_L] = { "walking-to-change-direction-left", MM_SLOT_WALK, false },
	[MM_ANIM_WALK_CHANGE_DIR_R] = { "walking-to-change-direction-right", MM_SLOT_WALK, false },
	[MM_ANIM_WALK_TO_STAND_L] = { "walking-to-standing-left", MM_SLOT_WALK, false },
	[MM_ANIM_WALK_TO_STAND_R] = { "walking-to-standing-right", MM_SLOT_WALK, false },

	[MM_ANIM_WALK_BACK] = { "walking-backwards", MM_SLOT_STRAFE_WALK, true },
	[MM_ANIM_WALK_BACK_L] = { "walking-backwards-left", MM_SLOT_WALK, true },
	[MM_ANIM_WALK_BACK_R] = { "walking-backwards-right", MM_SLOT_WALK, true },
	[MM_ANIM_WALK_STRAFE_L] = { "walking-left", MM_SLOT_STRAFE_WALK, true },
	[MM_ANIM_WALK_STRAFE_R] = { "walking-right", MM_SLOT_STRAFE_WALK, true },

	[MM_ANIM_RUN] = { "running", MM_SLOT_RUN, true },
	[MM_ANIM_TURN_RUN_L] = { "running-turn-left", MM_SLOT_TURN_RUN, true },
	[MM_ANIM_TURN_RUN_R] = { "running-turn-right", MM_SLOT_TURN_RUN, true },
	[MM_ANIM_RUN_CHANGE_DIR_L] = { "running-to-change-direction-left", MM_SLOT_RUN, false },
	[MM_ANIM_RUN_CHANGE_DIR_R] = { "running-to-change-direction-right", MM_SLOT_RUN, false },
	[MM_ANIM_RUN_TO_STAND_L] = { "running-to-standing-left", MM_SLOT_RUN, false },
	[MM_ANIM_RUN_TO_STAND_R] = { "running-to-standing-right", MM_SLOT_RUN, false },

	[MM_ANIM_RUN_BACK] = { "running-backwards", MM_SLOT_STRAFE_RUN, true },
	[MM_ANIM_RUN_BACK_L] = { "running-backwards-left", MM_SLOT_RUN, true },
	[MM_ANIM_RUN_BACK_R] = { "running-backwards-right", MM_SLOT_RUN, true },
	[MM_ANIM_RUN_STRAFE_L] = { "running-left", MM_SLOT_STRAFE_RUN, true },
	[MM_ANIM_RUN_STRAFE_R] = { "running-right", MM_SLOT_STRAFE_RUN, true },

	[MM_ANIM_SPRINT] = { "sprinting", MM_SLOT_SPRINT, true },

	[MM_ANIM_STRAFE_LOCKED_WALK_FWD] = { "strafing-walk-forward-aiming", MM_SLOT_STRAFE_LOCKED_WALK, true },
	[MM_ANIM_STRAFE_LOCKED_WALK_BACK] = { "strafing-walk-backwards-aiming", MM_SLOT_STRAFE_LOCKED_WALK, true },
	[MM_ANIM_STRAFE_LOCKED_WALK_L] = { "strafing-walk-left-aiming", MM_SLOT_STRAFE_LOCKED_WALK_SIDE, true },
	[MM_ANIM_STRAFE_LOCKED_WALK_R] = { "strafing-walk-right-aiming", MM_SLOT_STRAFE_LOCKED_WALK_SIDE, true },
	[MM_ANIM_STRAFE_LOCKED_RUN_FWD] = { "strafing-run-forward-aiming", MM_SLOT_STRAFE_LOCKED_RUN, true },
	[MM_ANIM_STRAFE_LOCKED_RUN_BACK] = { "strafing-run-backwards-aiming", MM_SLOT_STRAFE_LOCKED_RUN, true },
	[MM_ANIM_STRAFE_LOCKED_RUN_L] = { "strafing-run-left-aiming", MM_SLOT_STRAFE_LOCKED_RUN_SIDE, true },
	[MM_ANIM_STRAFE_LOCKED_RUN_R] = { "strafing-run-right-aiming", MM_SLOT_STRAFE_LOCKED_RUN_SIDE, true },

	[MM_ANIM_JUMP_L] = { "jump-left", MM_SLOT_JUMP_L, false },
	[MM_ANIM_JUMP_R] = { "jump-right", MM_SLOT_JUMP_R, false },
	[MM_ANIM_FALL_L] = { "falling-idle-left", MM_SLOT_JUMP_L, true },
	[MM_ANIM_FALL_R] = { "falling-idle-right", MM_SLOT_JUMP_R, true },
	[MM_ANIM_LAND_L] = { "land-left", MM_SLOT_LAND_L, false },
	[MM_ANIM_LAND_R] = { "land-right", MM_SLOT_LAND_R, false },

	[MM_ANIM_ROLL_L] = { "running-to-roll-left", MM_SLOT_ROLL_RUN, false },
	[MM_ANIM_ROLL_R] = { "running-to-roll-right", MM_SLOT_ROLL_RUN, false },

	[MM_ANIM_SWIM_IDLE] = { "swimming-idle", MM_SLOT_SWIM_A, true },
	[MM_ANIM_SWIM_SLOW] = { "swimming-slow", MM_SLOT_SWIM_B, true },
	[MM_ANIM_SWIM_FAST] = { "swimming-fast", MM_SLOT_SWIM_A, true },

	/* Both directions on the same clip until the descent is exported: the
	   select swaps them, so the second name is the only thing to change. */
	[MM_ANIM_CLIMB_DOWN] = { "climb-ladder", MM_SLOT_CLIMB, true },
	[MM_ANIM_CLIMB_UP] = { "climb-ladder", MM_SLOT_CLIMB, true },

	[MM_ANIM_SLIDE_L] = { "slide-left", MM_SLOT_ROLL_RUN, false },
	[MM_ANIM_SLIDE_R] = { "slide-right", MM_SLOT_ROLL_RUN, false },

	[MM_ANIM_TRANSITION_L] = { "transition-left", MM_SLOT_TRANSITION, false },
	[MM_ANIM_TRANSITION_R] = { "transition-right", MM_SLOT_TRANSITION, false },

};

/* The clip lists each node reads, in the grid order the node declares. */
static const uint8_t mm_clips_idle[] = { MM_ANIM_IDLE_L };
static const uint8_t mm_clips_idle_r[] = { MM_ANIM_IDLE_R };

static const uint8_t mm_clips_locomotion[] = {
	MM_ANIM_TURN_WALK_L, MM_ANIM_WALK, MM_ANIM_TURN_WALK_R,
	MM_ANIM_TURN_RUN_L, MM_ANIM_RUN, MM_ANIM_TURN_RUN_R,
	MM_ANIM_TURN_RUN_L, MM_ANIM_SPRINT, MM_ANIM_TURN_RUN_R,
};

static const uint8_t mm_clips_strafe[] = {
	MM_ANIM_WALK_BACK, MM_ANIM_WALK_BACK_L, MM_ANIM_WALK_STRAFE_L, MM_ANIM_WALK, MM_ANIM_WALK_STRAFE_R, MM_ANIM_WALK_BACK_R, MM_ANIM_WALK_BACK,
	MM_ANIM_RUN_BACK, MM_ANIM_RUN_BACK_L, MM_ANIM_RUN_STRAFE_L, MM_ANIM_RUN, MM_ANIM_RUN_STRAFE_R, MM_ANIM_RUN_BACK_R, MM_ANIM_RUN_BACK,
};

static const uint8_t mm_clips_strafe_locked[] = {
	MM_ANIM_STRAFE_LOCKED_WALK_BACK, MM_ANIM_STRAFE_LOCKED_WALK_L, MM_ANIM_STRAFE_LOCKED_WALK_FWD, MM_ANIM_STRAFE_LOCKED_WALK_R, MM_ANIM_STRAFE_LOCKED_WALK_BACK,
	MM_ANIM_STRAFE_LOCKED_RUN_BACK, MM_ANIM_STRAFE_LOCKED_RUN_L, MM_ANIM_STRAFE_LOCKED_RUN_FWD, MM_ANIM_STRAFE_LOCKED_RUN_R, MM_ANIM_STRAFE_LOCKED_RUN_BACK,
};

static const uint8_t mm_clips_swim[] = { MM_ANIM_SWIM_IDLE, MM_ANIM_SWIM_SLOW, MM_ANIM_SWIM_FAST };
static const uint8_t mm_clips_jump_l[] = { MM_ANIM_JUMP_L, MM_ANIM_FALL_L };
static const uint8_t mm_clips_jump_r[] = { MM_ANIM_JUMP_R, MM_ANIM_FALL_R };
static const uint8_t mm_clips_land_l[] = { MM_ANIM_LAND_L };
static const uint8_t mm_clips_land_r[] = { MM_ANIM_LAND_R };
static const uint8_t mm_clips_roll[] = { MM_ANIM_ROLL_L, MM_ANIM_ROLL_R };
static const uint8_t mm_clips_climb[] = { MM_ANIM_CLIMB_DOWN, MM_ANIM_CLIMB_UP };

static const e64::Animation::Node mr_muscles_nodes[] = {

	[MM_NODE_IDLE] = { e64::Animation::NODE_CLIP, mm_clips_idle, 1, 1, 0, 0 },
	[MM_NODE_IDLE_R] = { e64::Animation::NODE_BLEND, mm_clips_idle_r, 1, 1, MM_SLOT_IDLE_R, e64::character3d::ANIMATION_PARAM_IDLE_RIGHT },

	[MM_NODE_LOCOMOTION] = { e64::Animation::NODE_BLEND_2D, mm_clips_locomotion,
	  3, 3, 0, e64::character3d::ANIMATION_PARAM_WALK_TURN, e64::character3d::ANIMATION_PARAM_WALK_GAIT, e64::character3d::ANIMATION_PARAM_WALK },

	[MM_NODE_STRAFE] = { e64::Animation::NODE_BLEND_2D, mm_clips_strafe,
	  7, 2, 0, e64::character3d::ANIMATION_PARAM_STRAFE_DIR, e64::character3d::ANIMATION_PARAM_STRAFE_GAIT, e64::character3d::ANIMATION_PARAM_STRAFE },

	[MM_NODE_STRAFE_LOCKED] = { e64::Animation::NODE_BLEND_2D, mm_clips_strafe_locked,
	  5, 2, 0, e64::character3d::ANIMATION_PARAM_STRAFE_LOCKED_DIR, e64::character3d::ANIMATION_PARAM_STRAFE_LOCKED_GAIT, e64::character3d::ANIMATION_PARAM_STRAFE_LOCKED },

	[MM_NODE_SWIM] = { e64::Animation::NODE_BLEND_2D, mm_clips_swim,
	  3, 1, 0, e64::character3d::ANIMATION_PARAM_SWIM_GAIT, e64::character3d::ANIMATION_PARAM_SWIM_GAIT, e64::character3d::ANIMATION_PARAM_SWIM },

	/* Action layers, after every grid: they land on top of whichever grid is
	   driving the body, strafe included. */
	[MM_NODE_JUMP_L] = { e64::Animation::NODE_SEQUENCE, mm_clips_jump_l, 2, 1, MM_SLOT_JUMP_L, e64::character3d::ANIMATION_PARAM_JUMP_L },
	[MM_NODE_JUMP_R] = { e64::Animation::NODE_SEQUENCE, mm_clips_jump_r, 2, 1, MM_SLOT_JUMP_R, e64::character3d::ANIMATION_PARAM_JUMP_R },
	[MM_NODE_JUMP_L_LAYER] = { e64::Animation::NODE_LAYER, NULL, 0, 0, MM_SLOT_JUMP_L, e64::character3d::ANIMATION_PARAM_JUMP_L },
	[MM_NODE_JUMP_R_LAYER] = { e64::Animation::NODE_LAYER, NULL, 0, 0, MM_SLOT_JUMP_R, e64::character3d::ANIMATION_PARAM_JUMP_R },

	[MM_NODE_LAND_L] = { e64::Animation::NODE_BLEND, mm_clips_land_l, 1, 1, MM_SLOT_LAND_L, e64::character3d::ANIMATION_PARAM_LAND_L },
	[MM_NODE_LAND_R] = { e64::Animation::NODE_BLEND, mm_clips_land_r, 1, 1, MM_SLOT_LAND_R, e64::character3d::ANIMATION_PARAM_LAND_R },

	[MM_NODE_ROLL] = { e64::Animation::NODE_SELECT, mm_clips_roll, 2, 1, MM_SLOT_ROLL_RUN, e64::character3d::ANIMATION_PARAM_ROLL_DIR },
	[MM_NODE_ROLL_LAYER] = { e64::Animation::NODE_LAYER, NULL, 0, 0, MM_SLOT_ROLL_RUN, e64::character3d::ANIMATION_PARAM_ROLL_RUN },

	/* Same shape as the roll: a select picks the clip by direction and the
	   layer over it carries the weight, so the pose survives a standstill
	   with the clip parked wherever the last move left it. */
	[MM_NODE_CLIMB] = { e64::Animation::NODE_SELECT, mm_clips_climb, 2, 1, MM_SLOT_CLIMB, e64::character3d::ANIMATION_PARAM_CLIMB_DIR },
	[MM_NODE_CLIMB_LAYER] = { e64::Animation::NODE_LAYER, NULL, 0, 0, MM_SLOT_CLIMB, e64::character3d::ANIMATION_PARAM_CLIMB },
};

_Static_assert(MM_ANIM_TURN_WALK_R == MM_ANIM_TURN_WALK_L + 1, "turn_walk L/R must be contiguous (character3d_animation reads L+1)");
_Static_assert(MM_ANIM_TURN_RUN_R == MM_ANIM_TURN_RUN_L + 1, "turn_run L/R must be contiguous (character3d_animation reads L+1)");
_Static_assert(MM_ANIM_JUMP_R == MM_ANIM_JUMP_L + 1, "jump L/R must be contiguous (character3d_animation reads L+1)");
_Static_assert(MM_ANIM_FALL_R == MM_ANIM_FALL_L + 1, "fall L/R must be contiguous (character3d_animation reads L+1)");
_Static_assert(MM_ANIM_LAND_R == MM_ANIM_LAND_L + 1, "land L/R must be contiguous (character3d_animation reads L+1)");

const e64::character3d::AnimationDef mr_muscles_animation_def = {

	.graph = {
		.clip = mr_muscles_clips,
		.node = mr_muscles_nodes,
		.clip_count = MM_ANIM_COUNT,
		.node_count = sizeof(mr_muscles_nodes) / sizeof(mr_muscles_nodes[0]),
		.buffer_count = MM_SLOT_COUNT,
		.param_count = e64::character3d::ANIMATION_PARAM_COUNT,
	},
	.settings = &mr_muscles_animation_settings,
	.walk_animation = MM_ANIM_WALK,
	.run_animation = MM_ANIM_RUN,
	.sprint_animation = MM_ANIM_SPRINT,
	.turn_walk_animation = MM_ANIM_TURN_WALK_L,
	.turn_run_animation = MM_ANIM_TURN_RUN_L,
	.jump_animation = MM_ANIM_JUMP_L,
	.fall_animation = MM_ANIM_FALL_L,
	.land_animation = MM_ANIM_LAND_L,
	.roll_animation = MM_ANIM_ROLL_L,
	.locomotion_node = MM_NODE_LOCOMOTION,
	.strafe_node = MM_NODE_STRAFE,
	.strafe_locked_node = MM_NODE_STRAFE_LOCKED,
	.swim_node = MM_NODE_SWIM,
	.climb_node = MM_NODE_CLIMB,

};

static const e64::character3d::GaitSettings mr_muscles_gaits[] = {

	{ .target_speed = 1.55f, .response_rate = 8.0f, .rotation_response_rate = 14.0f },

	/* Past this much of the stick's travel the body runs: the same line the
	   old raw threshold drew, 65 of the 85 the stick really reaches. The last
	   gait has no threshold, so it belongs to the sprint button. */
	{ .target_speed = 3.2f, .response_rate = 9.0f, .rotation_response_rate = 15.0f, .stick_threshold = 0.76f },

	{ .target_speed = 4.4f, .response_rate = 10.0f, .rotation_response_rate = 16.0f },

};

const e64::character3d::MovementSettings mr_muscles_movement_settings = {

	.idle_response_rate = 12.0f,
	.idle_rotation_response_rate = 8.0f,

	.gait = mr_muscles_gaits,
	.gait_count = sizeof(mr_muscles_gaits) / sizeof(mr_muscles_gaits[0]),

	.roll_target_speed = 4.6f,
	.roll_launch_response_rate = 15.0f,
	.roll_spin_response_rate = 5.0f,
	.roll_grip_response_rate = 2.0f,
	.roll_ground_time = 0.3f,
	.roll_grip_time = 0.9f,
	.roll_timer_max = 1.166666f,

	.jump_response_rate = 0.5f,
	.jump_base_speed = 4.4f,
	.jump_force_multiplier = 30.0f,
	.jump_timer_max = 0.233333f,

	.swim_slow_speed = 1.2f,
	.swim_fast_speed = 2.6f,
	.swim_response_rate = 4.0f,

	.climb_speed = 1.1f,
	.climb_response_rate = 8.0f,

	.water_equilibrium_idle = 0.75f,
	.water_equilibrium_swim = 0.55f,

};

const e64::character3d::StatsSettings mr_muscles_stats_settings = {

	.stamina_drain_rate = 0.15f,
	.stamina_regen_rate = 0.25f,
	.tired_speed_scale = 0.8f,

};

const e64::character3d::ColliderSettings mr_muscles_collider_settings = {

	.radius = 0.35f,
	.height = 1.8f,

};


const char *const mr_muscles_weapon_meshes[] = { "ak47", "knife", "m1911" };

const e64::character3d::WeaponsDef mr_muscles_weapons_def = {
	.mesh = mr_muscles_weapon_meshes,
	.mesh_count = 3,
	/* Slots in declaration order: waist, back, melee. */
	/* Out for measuring: the meshes stay registered, so the three weapon
	   parts of the model start hidden and nothing equips them. */
	.weapon = { NULL, NULL, NULL },
};

const e64::character3d::WeaponDef weapon_ak47 = {
	.mesh = "ak47",
	.bone = "rifle",
	.holster_bone = "mixamorig:Spine2",
	.hand_bone = "mixamorig:RightHand",
	.type = e64::character3d::WEAPON_TYPE_HITSCAN,
	.magazine_size = 30,
	.max_integrity = 100,
	.holster_position = { .x = -11.5f, .y = 11.0f, .z = -15.5f },
	.holster_rotation = { .x = -0.6255f, .y = -0.1841f, .z = -0.7357f, .w = 0.1833f },
	.holding_rotation = { .x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f },
};

const e64::character3d::WeaponDef weapon_m1911 = {
	.mesh = "m1911",
	.bone = "handgun",
	.holster_bone = "mixamorig:Hips",
	.hand_bone = "mixamorig:RightHand",
	.type = e64::character3d::WEAPON_TYPE_HITSCAN,
	.magazine_size = 7,
	.max_integrity = 100,
	.holster_position = { .x = -22.0f, .y = -3.93f, .z = 0.2f },
	.holster_rotation = { .x = 0.9995f, .y = 0.0f, .z = 0.0f, .w = 0.0316f },
	.holding_rotation = { .x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f },
};

const e64::character3d::WeaponDef weapon_knife = {
	.mesh = "knife",
	.bone = "melee-weapon",
	.holster_bone = "mixamorig:RightUpLeg",
	.hand_bone = "mixamorig:RightHand",
	.type = e64::character3d::WEAPON_TYPE_MELEE,
	.magazine_size = 0,
	.max_integrity = 100,
	.holster_position = { .x = 8.74f, .y = -8.38f, .z = 0.59f },
	.holster_rotation = { .x = 0.0130f, .y = -0.9991f, .z = 0.0390f, .w = 0.0066f },
	.holding_rotation = { .x = 0.0f, .y = 0.0f, .z = 0.0f, .w = 1.0f },
};

const e64::character3d::AnimationSettings mr_muscles_animation_settings = {

		.action_idle_max_blending_ratio = 0.85f,

		.footing_left = MM_FOOTING_LEFT,
		.footing_right = MM_FOOTING_RIGHT,

		.turn_max_angle = 5.0f,
		.turn_max_weight = 0.4f,

		.jump_max_blending_ratio = 0.55f,

		.jump_anim_length = 0.633333f,
		.jump_anim_crouch = 0.1f,
		.jump_anim_air = 0.233333f,
		.jump_footing_speed = 0.4f,

		.land_anim_length = 0.9f,
		.land_anim_ground = 0.333333f,
		.land_anim_crouch = 0.5f,
		.land_anim_stand = 0.833333f,

		.run_to_rolling_anim_lead = 0.17f,
		.run_to_rolling_anim_ground = 0.3f,
		.run_to_rolling_anim_grip = 0.9f,
		.run_to_rolling_anim_stand = 0.9f,
		.run_to_rolling_anim_length = 1.166666f,

		.strafe_turn_rate = 8.0f,
		.strafe_blend_rate = 2.0f,

		.strafe_locked_blend_rate = 2.0f,
		.swim_blend_rate = 7.0f,
		.climb_blend_rate = 7.0f,

};



/* Every sample the character plays. The prefab lists them below, the
   entity opens them, and the events name them by index into that list. */
#define MM_ONESHOT(file, min, max) { \
	.path = "rom:/audio/" file ".wav64", \
	.volume = 1.0f, \
	.min_distance = min, \
	.max_distance = max, \
	.loop = false, \
	.priority = e64::Sound::PRIORITY_ONESHOT, \
	.preload = false, \
}

static const e64::Sound::Def mm_footstep_1 = MM_ONESHOT("footstep_01", 0.8f, 14.0f);
static const e64::Sound::Def mm_footstep_2 = MM_ONESHOT("footstep_02", 0.8f, 14.0f);
static const e64::Sound::Def mm_footstep_3 = MM_ONESHOT("footstep_03", 0.8f, 14.0f);
static const e64::Sound::Def mm_footstep_4 = MM_ONESHOT("footstep_04", 0.8f, 14.0f);
static const e64::Sound::Def mm_roll_1 = MM_ONESHOT("roll_01", 0.8f, 16.0f);
static const e64::Sound::Def mm_roll_2 = MM_ONESHOT("roll_02", 0.8f, 16.0f);
static const e64::Sound::Def mm_stroke_light_1 = MM_ONESHOT("swim_stroke_light", 1.5f, 14.0f);
static const e64::Sound::Def mm_stroke_light_2 = MM_ONESHOT("swim_stroke_light_2", 1.5f, 14.0f);
static const e64::Sound::Def mm_stroke_heavy_1 = MM_ONESHOT("swim_stroke_heavy", 1.5f, 14.0f);
static const e64::Sound::Def mm_stroke_heavy_2 = MM_ONESHOT("swim_stroke_heavy_2", 1.5f, 14.0f);
static const e64::Sound::Def mm_splash_1 = MM_ONESHOT("swim_splash", 1.5f, 14.0f);
static const e64::Sound::Def mm_splash_2 = MM_ONESHOT("swim_splash_2", 1.5f, 14.0f);

enum {

	MM_SOUND_FOOTSTEP_1,
	MM_SOUND_FOOTSTEP_2,
	MM_SOUND_FOOTSTEP_3,
	MM_SOUND_FOOTSTEP_4,
	MM_SOUND_ROLL_1,
	MM_SOUND_ROLL_2,
	MM_SOUND_STROKE_LIGHT_1,
	MM_SOUND_STROKE_LIGHT_2,
	MM_SOUND_STROKE_HEAVY_1,
	MM_SOUND_STROKE_HEAVY_2,
	MM_SOUND_SPLASH_1,
	MM_SOUND_SPLASH_2,

	MM_SOUND_COUNT,

};

static const e64::Sound::Def *const mr_muscles_sounds[MM_SOUND_COUNT] = {

	[MM_SOUND_FOOTSTEP_1] = &mm_footstep_1,
	[MM_SOUND_FOOTSTEP_2] = &mm_footstep_2,
	[MM_SOUND_FOOTSTEP_3] = &mm_footstep_3,
	[MM_SOUND_FOOTSTEP_4] = &mm_footstep_4,
	[MM_SOUND_ROLL_1] = &mm_roll_1,
	[MM_SOUND_ROLL_2] = &mm_roll_2,
	[MM_SOUND_STROKE_LIGHT_1] = &mm_stroke_light_1,
	[MM_SOUND_STROKE_LIGHT_2] = &mm_stroke_light_2,
	[MM_SOUND_STROKE_HEAVY_1] = &mm_stroke_heavy_1,
	[MM_SOUND_STROKE_HEAVY_2] = &mm_stroke_heavy_2,
	[MM_SOUND_SPLASH_1] = &mm_splash_1,
	[MM_SOUND_SPLASH_2] = &mm_splash_2,
};

static const uint8_t mr_muscles_footsteps[] = {
	MM_SOUND_FOOTSTEP_1, MM_SOUND_FOOTSTEP_2, MM_SOUND_FOOTSTEP_3,
	MM_SOUND_FOOTSTEP_4,
};

static const uint8_t mr_muscles_rolls[] = {
	MM_SOUND_ROLL_1, MM_SOUND_ROLL_2,
};

static const float mr_muscles_footings[] = { MM_FOOTING_LEFT, MM_FOOTING_RIGHT };
static const float mr_muscles_strokes[] = { MM_STROKE_A, MM_STROKE_B };

static const uint8_t mr_muscles_strokes_light[] = { MM_SOUND_STROKE_LIGHT_1, MM_SOUND_STROKE_LIGHT_2 };
static const uint8_t mr_muscles_strokes_heavy[] = { MM_SOUND_STROKE_HEAVY_1, MM_SOUND_STROKE_HEAVY_2 };
static const uint8_t mr_muscles_splashes[] = { MM_SOUND_SPLASH_1, MM_SOUND_SPLASH_2 };

static const e64::character3d::SoundDef mr_muscles_sound_def = {

	.footstep = mr_muscles_footsteps,
	.footstep_count = sizeof(mr_muscles_footsteps)/sizeof(*mr_muscles_footsteps),
	.footing = mr_muscles_footings,
	.footing_count = sizeof(mr_muscles_footings)/sizeof(*mr_muscles_footings),

	.footstep_volume_min = 0.3f,
	.footstep_volume_max = 0.65f,
	.footstep_speed_max = 4.4f,

	.roll = mr_muscles_rolls,
	.roll_count = sizeof(mr_muscles_rolls)/sizeof(*mr_muscles_rolls),
	.roll_volume = 0.8f,
	.roll_delay = 0.07f,
	.roll_launch_volume = 0.20f,
	.roll_stand_volume = 0.22f,
	.roll_launch_gap = 0.1f,

	.jump = mr_muscles_footsteps,
	.jump_count = sizeof(mr_muscles_footsteps)/sizeof(*mr_muscles_footsteps),
	.jump_volume = 0.2f,
	.jump_launch_gap = 0.1f,

	.land = mr_muscles_footsteps,
	.land_count = sizeof(mr_muscles_footsteps)/sizeof(*mr_muscles_footsteps),
	.land_volume_min = 0.4f,
	.land_volume_max = 0.85f,
	.land_speed_max = 15.0f,

	.swim_stroke_light = mr_muscles_strokes_light,
	.swim_stroke_light_count = sizeof(mr_muscles_strokes_light)/sizeof(*mr_muscles_strokes_light),
	.swim_stroke_heavy = mr_muscles_strokes_heavy,
	.swim_stroke_heavy_count = sizeof(mr_muscles_strokes_heavy)/sizeof(*mr_muscles_strokes_heavy),

	.stroke = mr_muscles_strokes,
	.stroke_count = sizeof(mr_muscles_strokes)/sizeof(*mr_muscles_strokes),
	.stroke_volume = 0.4f,

	.splash = mr_muscles_splashes,
	.splash_count = sizeof(mr_muscles_splashes)/sizeof(*mr_muscles_splashes),
	.splash_volume_min = 0.05f,
	.splash_volume_max = 0.85f,
	.splash_speed_max = 8.0f,

};


const e64::character3d::Def mr_muscles_character_def = {

	.movement_settings = &mr_muscles_movement_settings,
	.animation_def = &mr_muscles_animation_def,
	.collider_settings = &mr_muscles_collider_settings,
	.weapons_def = &mr_muscles_weapons_def,
	.sound_def = &mr_muscles_sound_def,
	.stats_settings = &mr_muscles_stats_settings,

};

const e64::Prefab3D mr_muscles = {
	.type = e64::prefab3d::PREFAB3D_CHARACTER,
	.model = mr_muscles_model,
	.sound = mr_muscles_sounds,
	.sound_count = MM_SOUND_COUNT,
	.character = &mr_muscles_character_def,
};
