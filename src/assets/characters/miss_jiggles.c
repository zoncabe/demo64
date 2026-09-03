#include ENGINE_HEADER(character, character)
#include "assets/sound/sound.h"
#include "assets/characters/miss_jiggles.h"

/* Plant phases measured on the clips (walk 11/28 of 31 f, run 9/20 of 23 f,
   sprint 8/18 of 20 f): the proportion is near constant across the gaits, so
   one averaged pair covers the grid. Single source for the animation settings
   and the footstep sounds. */
#define MJ_FOOTING_LEFT  0.38f
#define MJ_FOOTING_RIGHT 0.89f

/* The swim clips pull an arm at the start and the middle of the cycle. */
#define MJ_STROKE_A 0.0f
#define MJ_STROKE_B 0.5f


static const CharacterAnimationClipDef miss_jiggles_clips[] = {

	[MJ_ANIM_IDLE_L]            = { "standing-idle-left",                 ANIMATION_SLOT_MAIN,   true  },
	[MJ_ANIM_IDLE_R]            = { "standing-idle-right",                MJ_SLOT_IDLE_R,        true  },

	[MJ_ANIM_STAND_TO_WALK_L]   = { "standing-to-walking-left",           MJ_SLOT_WALK,          false },
	[MJ_ANIM_STAND_TO_WALK_R]   = { "standing-to-walking-right",          MJ_SLOT_WALK,          false },

	[MJ_ANIM_STAND_TO_RUN_L]    = { "standing-to-running-left",           MJ_SLOT_RUN,           false },
	[MJ_ANIM_STAND_TO_RUN_R]    = { "standing-to-running-right",          MJ_SLOT_RUN,           false },

	[MJ_ANIM_WALK]              = { "walking",                            MJ_SLOT_WALK,          true  },
	[MJ_ANIM_TURN_WALK_L]       = { "walking-turn-left",                  MJ_SLOT_TURN_WALK,     true  },
	[MJ_ANIM_TURN_WALK_R]       = { "walking-turn-right",                 MJ_SLOT_TURN_WALK,     true  },
	[MJ_ANIM_WALK_CHANGE_DIR_L] = { "walking-to-change-direction-left",   MJ_SLOT_WALK,          false },
	[MJ_ANIM_WALK_CHANGE_DIR_R] = { "walking-to-change-direction-right",  MJ_SLOT_WALK,          false },
	[MJ_ANIM_WALK_TO_STAND_L]   = { "walking-to-standing-left",           MJ_SLOT_WALK,          false },
	[MJ_ANIM_WALK_TO_STAND_R]   = { "walking-to-standing-right",          MJ_SLOT_WALK,          false },

	[MJ_ANIM_WALK_BACK]         = { "walking-backwards",                  MJ_SLOT_STRAFE_WALK,   true  },
	[MJ_ANIM_WALK_BACK_L]       = { "walking-backwards-left",             MJ_SLOT_WALK,          true  },
	[MJ_ANIM_WALK_BACK_R]       = { "walking-backwards-right",            MJ_SLOT_WALK,          true  },
	[MJ_ANIM_WALK_STRAFE_L]     = { "walking-left",                       MJ_SLOT_STRAFE_WALK,   true  },
	[MJ_ANIM_WALK_STRAFE_R]     = { "walking-right",                      MJ_SLOT_STRAFE_WALK,   true  },

	[MJ_ANIM_RUN]               = { "running",                            MJ_SLOT_RUN,           true  },
	[MJ_ANIM_TURN_RUN_L]        = { "running-turn-left",                  MJ_SLOT_TURN_RUN,      true  },
	[MJ_ANIM_TURN_RUN_R]        = { "running-turn-right",                 MJ_SLOT_TURN_RUN,      true  },
	[MJ_ANIM_RUN_CHANGE_DIR_L]  = { "running-to-change-direction-left",   MJ_SLOT_RUN,           false },
	[MJ_ANIM_RUN_CHANGE_DIR_R]  = { "running-to-change-direction-right",  MJ_SLOT_RUN,           false },
	[MJ_ANIM_RUN_TO_STAND_L]    = { "running-to-standing-left",           MJ_SLOT_RUN,           false },
	[MJ_ANIM_RUN_TO_STAND_R]    = { "running-to-standing-right",          MJ_SLOT_RUN,           false },

	[MJ_ANIM_RUN_BACK]          = { "running-backwards",                  MJ_SLOT_STRAFE_RUN,    true  },
	[MJ_ANIM_RUN_BACK_L]        = { "running-backwards-left",             MJ_SLOT_RUN,           true  },
	[MJ_ANIM_RUN_BACK_R]        = { "running-backwards-right",            MJ_SLOT_RUN,           true  },
	[MJ_ANIM_RUN_STRAFE_L]      = { "running-left",                       MJ_SLOT_STRAFE_RUN,    true  },
	[MJ_ANIM_RUN_STRAFE_R]      = { "running-right",                      MJ_SLOT_STRAFE_RUN,    true  },

	[MJ_ANIM_SPRINT]            = { "sprinting",                          MJ_SLOT_SPRINT,        true  },

	[MJ_ANIM_STRAFE_LOCKED_WALK_FWD]  = { "strafing-walk-forward-aiming",   MJ_SLOT_STRAFE_LOCKED_WALK,      true },
	[MJ_ANIM_STRAFE_LOCKED_WALK_BACK] = { "strafing-walk-backwards-aiming", MJ_SLOT_STRAFE_LOCKED_WALK,      true },
	[MJ_ANIM_STRAFE_LOCKED_WALK_L]    = { "strafing-walk-left-aiming",      MJ_SLOT_STRAFE_LOCKED_WALK_SIDE, true },
	[MJ_ANIM_STRAFE_LOCKED_WALK_R]    = { "strafing-walk-right-aiming",     MJ_SLOT_STRAFE_LOCKED_WALK_SIDE, true },
	[MJ_ANIM_STRAFE_LOCKED_RUN_FWD]   = { "strafing-run-forward-aiming",    MJ_SLOT_STRAFE_LOCKED_RUN,       true },
	[MJ_ANIM_STRAFE_LOCKED_RUN_BACK]  = { "strafing-run-backwards-aiming",  MJ_SLOT_STRAFE_LOCKED_RUN,       true },
	[MJ_ANIM_STRAFE_LOCKED_RUN_L]     = { "strafing-run-left-aiming",       MJ_SLOT_STRAFE_LOCKED_RUN_SIDE,  true },
	[MJ_ANIM_STRAFE_LOCKED_RUN_R]     = { "strafing-run-right-aiming",      MJ_SLOT_STRAFE_LOCKED_RUN_SIDE,  true },

	[MJ_ANIM_BOW_HOLD_IDLE]      = { "bow-standing-idle",             MJ_SLOT_BOW_HOLD_IDLE,      true },
	[MJ_ANIM_BOW_HOLD_WALK_FWD]  = { "bow-holding-walking",           MJ_SLOT_BOW_HOLD_WALK,      true },
	[MJ_ANIM_BOW_HOLD_WALK_BACK] = { "bow-holding-walking-backwards", MJ_SLOT_BOW_HOLD_WALK,      true },
	[MJ_ANIM_BOW_HOLD_WALK_L]    = { "bow-holding-walking-left",      MJ_SLOT_BOW_HOLD_WALK_SIDE, true },
	[MJ_ANIM_BOW_HOLD_WALK_R]    = { "bow-holding-walking-right",     MJ_SLOT_BOW_HOLD_WALK_SIDE, true },
	[MJ_ANIM_BOW_HOLD_RUN_FWD]   = { "bow-holding-running",           MJ_SLOT_BOW_HOLD_RUN,       true },
	[MJ_ANIM_BOW_HOLD_RUN_BACK]  = { "bow-holding-running-backwards", MJ_SLOT_BOW_HOLD_RUN,       true },
	[MJ_ANIM_BOW_HOLD_RUN_L]     = { "bow-holding-running-left",      MJ_SLOT_BOW_HOLD_RUN_SIDE,  true },
	[MJ_ANIM_BOW_HOLD_RUN_R]     = { "bow-holding-running-right",     MJ_SLOT_BOW_HOLD_RUN_SIDE,  true },

	[MJ_ANIM_BOW_AIM_IDLE]       = { "bow-aiming-idle",                MJ_SLOT_BOW_AIM_IDLE,      true },
	[MJ_ANIM_BOW_AIM_WALK_FWD]   = { "bow-aiming-walking",             MJ_SLOT_BOW_AIM_WALK,      true },
	[MJ_ANIM_BOW_AIM_WALK_BACK]  = { "bow-aiming-walking-backwards",   MJ_SLOT_BOW_AIM_WALK,      true },
	[MJ_ANIM_BOW_AIM_WALK_L]     = { "bow-aiming-walking-left",        MJ_SLOT_BOW_AIM_WALK_SIDE, true },
	[MJ_ANIM_BOW_AIM_WALK_R]     = { "bow-aiming-walking-right",       MJ_SLOT_BOW_AIM_WALK_SIDE, true },
	[MJ_ANIM_BOW_AIM_RUN_FWD]    = { "bow-aiming-running",             MJ_SLOT_BOW_AIM_RUN,       true },
	[MJ_ANIM_BOW_AIM_RUN_BACK]   = { "bow-aiming-running-backwards",   MJ_SLOT_BOW_AIM_RUN,       true },
	[MJ_ANIM_BOW_AIM_RUN_L]      = { "bow-aiming-running-left",        MJ_SLOT_BOW_AIM_RUN_SIDE,  true },
	[MJ_ANIM_BOW_AIM_RUN_R]      = { "bow-aiming-running-right",       MJ_SLOT_BOW_AIM_RUN_SIDE,  true },

	[MJ_ANIM_JUMP_L]            = { "jump-left",                          MJ_SLOT_JUMP_L,        false },
	[MJ_ANIM_JUMP_R]            = { "jump-right",                         MJ_SLOT_JUMP_R,        false },
	[MJ_ANIM_FALL_L]            = { "falling-idle-left",                  MJ_SLOT_JUMP_L,        true  },
	[MJ_ANIM_FALL_R]            = { "falling-idle-right",                 MJ_SLOT_JUMP_R,        true  },
	[MJ_ANIM_LAND_L]            = { "land-left",                          MJ_SLOT_LAND_L,        false },
	[MJ_ANIM_LAND_R]            = { "land-right",                         MJ_SLOT_LAND_R,        false },

	[MJ_ANIM_ROLL_L]            = { "running-to-roll-left",               MJ_SLOT_ROLL_RUN,      false },
	[MJ_ANIM_ROLL_R]            = { "running-to-roll-right",              MJ_SLOT_ROLL_RUN,      false },

	[MJ_ANIM_SWIM_IDLE]         = { "swimming-idle",                      MJ_SLOT_SWIM_A,        true  },
	[MJ_ANIM_SWIM_SLOW]         = { "swimming-slow",                      MJ_SLOT_SWIM_B,        true  },
	[MJ_ANIM_SWIM_FAST]         = { "swimming-fast",                      MJ_SLOT_SWIM_A,        true  },

	[MJ_ANIM_CLIMB_DOWN]        = { "climb-ladder-down",                  MJ_SLOT_CLIMB,         true  },
	[MJ_ANIM_CLIMB_UP]          = { "climb-ladder",                       MJ_SLOT_CLIMB,         true  },

	[MJ_ANIM_SLIDE_L]           = { "slide-left",                         MJ_SLOT_ROLL_RUN,      false },
	[MJ_ANIM_SLIDE_R]           = { "slide-right",                        MJ_SLOT_ROLL_RUN,      false },

	[MJ_ANIM_TRANSITION_L]      = { "transition-left",                    MJ_SLOT_TRANSITION,    false },
	[MJ_ANIM_TRANSITION_R]      = { "transition-right",                   MJ_SLOT_TRANSITION,    false },

};

static const CharacterAnimationNode miss_jiggles_nodes[] = {

	[MJ_NODE_IDLE]         = { ANIMATION_NODE_CLIP,     ANIMATION_CLIPS(MJ_ANIM_IDLE_L),                   1, 1, 0,               0                          },
	[MJ_NODE_IDLE_R]       = { ANIMATION_NODE_BLEND,    ANIMATION_CLIPS(MJ_ANIM_IDLE_R),                   1, 1, MJ_SLOT_IDLE_R,  ANIMATION_PARAM_IDLE_RIGHT },

	[MJ_NODE_LOCOMOTION]   = { ANIMATION_NODE_BLEND_2D,
	  ANIMATION_CLIPS(MJ_ANIM_TURN_WALK_L, MJ_ANIM_WALK,   MJ_ANIM_TURN_WALK_R,
	                  MJ_ANIM_TURN_RUN_L,  MJ_ANIM_RUN,    MJ_ANIM_TURN_RUN_R,
	                  MJ_ANIM_TURN_RUN_L,  MJ_ANIM_SPRINT, MJ_ANIM_TURN_RUN_R),
	  3, 3, 0, ANIMATION_PARAM_WALK_TURN, ANIMATION_PARAM_WALK_GAIT, ANIMATION_PARAM_WALK },

	[MJ_NODE_JUMP_L]       = { ANIMATION_NODE_SEQUENCE, ANIMATION_CLIPS(MJ_ANIM_JUMP_L, MJ_ANIM_FALL_L),   2, 1, MJ_SLOT_JUMP_L,  ANIMATION_PARAM_JUMP_L     },
	[MJ_NODE_JUMP_R]       = { ANIMATION_NODE_SEQUENCE, ANIMATION_CLIPS(MJ_ANIM_JUMP_R, MJ_ANIM_FALL_R),   2, 1, MJ_SLOT_JUMP_R,  ANIMATION_PARAM_JUMP_R     },
	[MJ_NODE_JUMP_L_LAYER] = { ANIMATION_NODE_LAYER,    NULL,                                              0, 0, MJ_SLOT_JUMP_L,  ANIMATION_PARAM_JUMP_L     },
	[MJ_NODE_JUMP_R_LAYER] = { ANIMATION_NODE_LAYER,    NULL,                                              0, 0, MJ_SLOT_JUMP_R,  ANIMATION_PARAM_JUMP_R     },

	[MJ_NODE_LAND_L]       = { ANIMATION_NODE_BLEND,    ANIMATION_CLIPS(MJ_ANIM_LAND_L),                   1, 1, MJ_SLOT_LAND_L,  ANIMATION_PARAM_LAND_L     },
	[MJ_NODE_LAND_R]       = { ANIMATION_NODE_BLEND,    ANIMATION_CLIPS(MJ_ANIM_LAND_R),                   1, 1, MJ_SLOT_LAND_R,  ANIMATION_PARAM_LAND_R     },

	[MJ_NODE_ROLL]         = { ANIMATION_NODE_SELECT,   ANIMATION_CLIPS(MJ_ANIM_ROLL_L, MJ_ANIM_ROLL_R),   2, 1, MJ_SLOT_ROLL_RUN, ANIMATION_PARAM_ROLL_DIR  },
	[MJ_NODE_ROLL_LAYER]   = { ANIMATION_NODE_LAYER,    NULL,                                              0, 0, MJ_SLOT_ROLL_RUN, ANIMATION_PARAM_ROLL_RUN  },

	[MJ_NODE_STRAFE]       = { ANIMATION_NODE_BLEND_2D,
	  ANIMATION_CLIPS(MJ_ANIM_WALK_BACK, MJ_ANIM_WALK_BACK_L, MJ_ANIM_WALK_STRAFE_L, MJ_ANIM_WALK, MJ_ANIM_WALK_STRAFE_R, MJ_ANIM_WALK_BACK_R, MJ_ANIM_WALK_BACK,
	                  MJ_ANIM_RUN_BACK,  MJ_ANIM_RUN_BACK_L,  MJ_ANIM_RUN_STRAFE_L,  MJ_ANIM_RUN,  MJ_ANIM_RUN_STRAFE_R,  MJ_ANIM_RUN_BACK_R,  MJ_ANIM_RUN_BACK),
	  7, 2, 0, ANIMATION_PARAM_STRAFE_DIR, ANIMATION_PARAM_STRAFE_GAIT, ANIMATION_PARAM_STRAFE },

	[MJ_NODE_STRAFE_LOCKED] = { ANIMATION_NODE_BLEND_2D,
	  ANIMATION_CLIPS(MJ_ANIM_STRAFE_LOCKED_WALK_BACK, MJ_ANIM_STRAFE_LOCKED_WALK_L, MJ_ANIM_STRAFE_LOCKED_WALK_FWD, MJ_ANIM_STRAFE_LOCKED_WALK_R, MJ_ANIM_STRAFE_LOCKED_WALK_BACK,
	                  MJ_ANIM_STRAFE_LOCKED_RUN_BACK,  MJ_ANIM_STRAFE_LOCKED_RUN_L,  MJ_ANIM_STRAFE_LOCKED_RUN_FWD,  MJ_ANIM_STRAFE_LOCKED_RUN_R,  MJ_ANIM_STRAFE_LOCKED_RUN_BACK),
	  5, 2, 0, ANIMATION_PARAM_STRAFE_LOCKED_DIR, ANIMATION_PARAM_STRAFE_LOCKED_GAIT, ANIMATION_PARAM_STRAFE_LOCKED },

	/* Bow modes: an idle of their own plus a grid shaped like the locked
	   strafe, direction wrapping the whole way round with no front/back
	   seam. Hold and aim cross-fade during the transition, so they keep
	   separate buffers. */
	[MJ_NODE_BOW_HOLD_IDLE] = { ANIMATION_NODE_BLEND, ANIMATION_CLIPS(MJ_ANIM_BOW_HOLD_IDLE), 1, 1, MJ_SLOT_BOW_HOLD_IDLE, ANIMATION_PARAM_AIMING_IDLE },

	[MJ_NODE_BOW_HOLD] = { ANIMATION_NODE_BLEND_2D,
	  ANIMATION_CLIPS(MJ_ANIM_BOW_HOLD_WALK_BACK, MJ_ANIM_BOW_HOLD_WALK_L, MJ_ANIM_BOW_HOLD_WALK_FWD, MJ_ANIM_BOW_HOLD_WALK_R, MJ_ANIM_BOW_HOLD_WALK_BACK,
	                  MJ_ANIM_BOW_HOLD_RUN_BACK,  MJ_ANIM_BOW_HOLD_RUN_L,  MJ_ANIM_BOW_HOLD_RUN_FWD,  MJ_ANIM_BOW_HOLD_RUN_R,  MJ_ANIM_BOW_HOLD_RUN_BACK),
	  5, 2, 0, ANIMATION_PARAM_AIMING_DIR, ANIMATION_PARAM_AIMING_GAIT, ANIMATION_PARAM_AIMING },

	[MJ_NODE_BOW_AIM_IDLE] = { ANIMATION_NODE_BLEND, ANIMATION_CLIPS(MJ_ANIM_BOW_AIM_IDLE), 1, 1, MJ_SLOT_BOW_AIM_IDLE, ANIMATION_PARAM_CHARGING_SHOOT_IDLE },

	[MJ_NODE_BOW_AIM] = { ANIMATION_NODE_BLEND_2D,
	  ANIMATION_CLIPS(MJ_ANIM_BOW_AIM_WALK_BACK, MJ_ANIM_BOW_AIM_WALK_L, MJ_ANIM_BOW_AIM_WALK_FWD, MJ_ANIM_BOW_AIM_WALK_R, MJ_ANIM_BOW_AIM_WALK_BACK,
	                  MJ_ANIM_BOW_AIM_RUN_BACK,  MJ_ANIM_BOW_AIM_RUN_L,  MJ_ANIM_BOW_AIM_RUN_FWD,  MJ_ANIM_BOW_AIM_RUN_R,  MJ_ANIM_BOW_AIM_RUN_BACK),
	  5, 2, 0, ANIMATION_PARAM_CHARGING_SHOOT_DIR, ANIMATION_PARAM_CHARGING_SHOOT_GAIT, ANIMATION_PARAM_CHARGING_SHOOT },

	[MJ_NODE_SWIM] = { ANIMATION_NODE_BLEND_2D,
	  ANIMATION_CLIPS(MJ_ANIM_SWIM_IDLE, MJ_ANIM_SWIM_SLOW, MJ_ANIM_SWIM_FAST),
	  3, 1, 0, ANIMATION_PARAM_SWIM_GAIT, ANIMATION_PARAM_SWIM_GAIT, ANIMATION_PARAM_SWIM },

	/* Same shape as the roll: a select picks the clip by direction and the
	   layer over it carries the weight, so the pose survives a standstill
	   with the clip parked wherever the last move left it. */
	[MJ_NODE_CLIMB]       = { ANIMATION_NODE_SELECT, ANIMATION_CLIPS(MJ_ANIM_CLIMB_DOWN, MJ_ANIM_CLIMB_UP), 2, 1, MJ_SLOT_CLIMB, ANIMATION_PARAM_CLIMB_DIR },
	[MJ_NODE_CLIMB_LAYER] = { ANIMATION_NODE_LAYER,  NULL,                                                  0, 0, MJ_SLOT_CLIMB, ANIMATION_PARAM_CLIMB     },
};

_Static_assert(MJ_ANIM_TURN_WALK_R == MJ_ANIM_TURN_WALK_L + 1, "turn_walk L/R must be contiguous (character_animation reads L+1)");
_Static_assert(MJ_ANIM_TURN_RUN_R  == MJ_ANIM_TURN_RUN_L  + 1, "turn_run L/R must be contiguous (character_animation reads L+1)");
_Static_assert(MJ_ANIM_JUMP_R      == MJ_ANIM_JUMP_L      + 1, "jump L/R must be contiguous (character_animation reads L+1)");
_Static_assert(MJ_ANIM_FALL_R      == MJ_ANIM_FALL_L      + 1, "fall L/R must be contiguous (character_animation reads L+1)");
_Static_assert(MJ_ANIM_LAND_R      == MJ_ANIM_LAND_L      + 1, "land L/R must be contiguous (character_animation reads L+1)");

const CharacterAnimationDef miss_jiggles_animation_def = {

	.clip            = miss_jiggles_clips,
	.node            = miss_jiggles_nodes,
	.settings        = &miss_jiggles_animation_settings,
	.clip_count      = MJ_ANIM_COUNT,
	.node_count      = sizeof(miss_jiggles_nodes) / sizeof(miss_jiggles_nodes[0]),
	.buffer_count    = MJ_SLOT_COUNT,
	.walk_animation       = MJ_ANIM_WALK,
	.run_animation        = MJ_ANIM_RUN,
	.sprint_animation     = MJ_ANIM_SPRINT,
	.turn_walk_animation  = MJ_ANIM_TURN_WALK_L,
	.turn_run_animation   = MJ_ANIM_TURN_RUN_L,
	.jump_animation       = MJ_ANIM_JUMP_L,
	.fall_animation       = MJ_ANIM_FALL_L,
	.land_animation       = MJ_ANIM_LAND_L,
	.roll_animation       = MJ_ANIM_ROLL_L,
	.locomotion_node      = MJ_NODE_LOCOMOTION,
	.strafe_node          = MJ_NODE_STRAFE,
	.strafe_locked_node   = MJ_NODE_STRAFE_LOCKED,
	.aiming_idle_node          = MJ_NODE_BOW_HOLD_IDLE,
	.aiming_node               = MJ_NODE_BOW_HOLD,
	.charging_shoot_idle_node  = MJ_NODE_BOW_AIM_IDLE,
	.charging_shoot_node       = MJ_NODE_BOW_AIM,
	.swim_node            = MJ_NODE_SWIM,
	.climb_node           = MJ_NODE_CLIMB,

};

/* Slightly slower than mr_muscles across the board. */
static const CharacterGaitSettings miss_jiggles_gaits[] = {

	{ .target_speed = 1.4f, .response_rate = 7.5f, .rotation_response_rate = 13.0f },
	{ .target_speed = 2.9f, .response_rate = 8.5f, .rotation_response_rate = 14.0f },
	{ .target_speed = 4.0f, .response_rate = 9.5f, .rotation_response_rate = 15.0f },

};

const CharacterMovementSettings miss_jiggles_movement_settings = {

	.idle_target_speed            = 0,
	.idle_response_rate           = 12.0f,
	.idle_rotation_response_rate  = 7.5f,

	.gait       = miss_jiggles_gaits,
	.gait_count = sizeof(miss_jiggles_gaits) / sizeof(miss_jiggles_gaits[0]),

	/* Timers stay equal to mr_muscles: they track the clip lengths, which
	   are the same animations. */
	.roll_target_speed         = 4.2f,
	.roll_launch_response_rate = 14.0f,
	.roll_spin_response_rate   = 4.5f,
	.roll_grip_response_rate   = 2.0f,
	.roll_ground_time          = 0.3f,
	.roll_grip_time            = 0.9f,
	.roll_timer_max            = 1.166666f,

	.jump_response_rate    = 0.5f,
	.jump_force_multiplier = 28.0f,
	.jump_base_speed       = 4.0f,
	.jump_timer_max        = 0.233333f,

	.swim_slow_speed    = 1.1f,
	.swim_fast_speed    = 2.4f,
	.swim_response_rate = 4.0f,

	/* climb-ladder holds a grip still at 0.64 m/s, which plays like wading
	   uphill. Above that the hand creeps down the rung it holds; this is
	   the trade, and it is the only knob — the clip follows the speed on
	   its own. */
	.climb_speed          = 1.1f,
	.climb_response_rate  = 8.0f,

	.water_equilibrium_idle = 0.70f,
	.water_equilibrium_swim = 0.45f,

};

const CharacterStatsSettings miss_jiggles_stats_settings = {

	.stamina_drain_rate = 0.15f,
	.stamina_regen_rate = 0.25f,
	.tired_speed_scale  = 0.8f,

};

const CharacterColliderSettings miss_jiggles_collider_settings = {

	.radius = 0.32f,
	.height = 1.7f,

};


const char *const miss_jiggles_weapon_meshes[] = { "bow" };

const CharacterWeaponsDef miss_jiggles_weapons_def = {
	.mesh       = miss_jiggles_weapon_meshes,
	.mesh_count = 1,
	.weapon = {
		[WEAPON_SLOT_BACK] = &weapon_bow,
	},
};

/* The bow moved over from mr_muscles, which no longer carries one. Two
   things had to change with it: the mesh hangs off a bone of its own called
   "bow" here, where his was on the shared "rifle" bone, and the spot on the
   back is scaled to this torso -- her Spine2 is 15.1 against his 18.59, so
   every distance is 0.81 of what it was on him. */
const WeaponDef weapon_bow = {
	.mesh          = "bow",
	.bone          = "bow",
	.holster_bone  = "mixamorig:Spine2",
	.hand_bone     = "mixamorig:LeftHand",
	.type          = WEAPON_TYPE_BALLISTIC,
	.magazine_size = 1,
	.max_integrity = 100,
	.holster_position = { .x = 0.28f, .y = -10.78f, .z = -12.39f },
	.holster_rotation = { .x = -0.5719f, .y = 0.3900f, .z = -0.6504f, .w = -0.3127f },
	/* Grip past the hand bone's tail, spun 55 degrees about the bow's own
	   long axis. Both numbers measured on console. */
	.holding_position = { .x = -5.0f, .y = 7.0f, .z = 0.0f },
	.holding_rotation = { .x = 0.0f, .y = 0.4617f, .z = 0.0f, .w = 0.8870f },
};


/* Measured from the skinned vertices. The skull: centre at the Head verts'
   centroid, radius short of the resting ponytail so rest pose starts free.
   The torso: two horizontal capsules (local Y rotated onto X), one per
   torso band, stacked below the shoulders. */
static const SpringBoneColliderDef miss_jiggles_hair_colliders[] = {
	{
		/* Head and neck: grown down and forward; the back surface stays at
		   z -7.9 so the resting ponytail keeps its clearance. */
		.shape    = SPRING_BONE_COLLISION_SPHERE,
		.bone     = MISS_JIGGLES_BONE_HEAD,
		.position = { 0.0f, 0.055f, 0.041f },
		.radius   = 0.12f,
	},
	{
		/* Neck, bridging head and shoulders: upright from inside the chest
		   capsule up into the head sphere, so running backwards cannot push
		   the ponytail through the nape. */
		.shape    = SPRING_BONE_COLLISION_CAPSULE,
		.bone     = MISS_JIGGLES_BONE_NECK,
		.position = { 0.0f, 0.005f, 0.0f },
		.radius   = 0.06f,
		.height   = 0.15f,
	},
	{
		/* Chest, the torso's widest band (y 6.9 in Spine2 space): section
		   radius 7.4, caps flush with the 35-wide shoulder line. */
		.shape    = SPRING_BONE_COLLISION_CAPSULE,
		.bone     = MISS_JIGGLES_BONE_SPINE_2,
		.position = { 0.0f, 0.069f, 0.002f },
		.rotation = { 0.0f, 0.0f, -90.0f },
		.radius   = 0.075f,
		.height   = 0.50f,
	},
	{
		/* Below it, the waist band (y -1.1): section radius 4.8, same reach. */
		.shape    = SPRING_BONE_COLLISION_CAPSULE,
		.bone     = MISS_JIGGLES_BONE_SPINE_2,
		.position = { 0.0f, -0.011f, 0.002f },
		.rotation = { 0.0f, 0.0f, -90.0f },
		.radius   = 0.052f,
		.height   = 0.22f,
	},
	{
		/* Right upper arm, shoulder to elbow (24.2 on the rig): the string
		   arm, the one the ponytail slaps while aiming. */
		.shape    = SPRING_BONE_COLLISION_CAPSULE,
		.bone     = MISS_JIGGLES_BONE_ARM_R,
		.position = { 0.0f, 0.121f, 0.0f },
		.radius   = 0.050f,
		.height   = 0.26f,
	},
};

/* Strand thickness at each hair joint, measured from its vertices; the last
   entry is the tip so its very end collides too. */
static const float miss_jiggles_hair_radius[] = { 0.027f, 0.035f, 0.025f, 0.02f };

/* One chain per set, root to end, as a Godot setting; a zeroed set ends the
   array. Metres: stiffness pushes toward the animated pose (m/s), drag
   bleeds velocity (0..1), gravity accumulates along its direction. Joint
   lengths are measured from the rig; end_bone_length covers the last joint,
   set to where its skinned vertices sit. */
const SpringBonesDef miss_jiggles_spring_bones[] = {
	{
		.root_bone         = MISS_JIGGLES_BONE_BREAST_L,
		.end_bone          = MISS_JIGGLES_BONE_BREAST_L,
		.count             = 1,
		.end_bone_length   = 0.06f,
		.stiffness         = 1.5f,
		.drag              = 0.55f,
		.gravity           = 0.3f,
		.gravity_direction = { 0.0f, 0.0f, -1.0f },
		.world_damping_location      = 0.85f,
		.world_damping_rotation      = 0.6f,
		.teleport_distance_threshold = 3.0f,
		.teleport_rotation_threshold = 0.175f,
	},
	{
		.root_bone         = MISS_JIGGLES_BONE_BREAST_R,
		.end_bone          = MISS_JIGGLES_BONE_BREAST_R,
		.count             = 1,
		.end_bone_length   = 0.06f,
		.stiffness         = 1.5f,
		.drag              = 0.55f,
		.gravity           = 0.3f,
		.gravity_direction = { 0.0f, 0.0f, -1.0f },
		.world_damping_location      = 0.85f,
		.world_damping_rotation      = 0.6f,
		.teleport_distance_threshold = 3.0f,
		.teleport_rotation_threshold = 0.175f,
	},
	{
		.root_bone         = MISS_JIGGLES_BONE_HAIR_1,
		.end_bone          = MISS_JIGGLES_BONE_HAIR_4,
		.count             = 4,
		.end_bone_length   = 0.13f,
		.stiffness         = 0.3f,
		.drag              = 0.2f,
		.gravity           = 0.4f,
		.gravity_direction = { 0.0f, 0.0f, -1.0f },
		.world_damping_location      = 0.85f,
		.world_damping_rotation      = 0.85f,
		.teleport_distance_threshold = 3.0f,
		.teleport_rotation_threshold = 0.175f,
		.radius         = 0.03f,
		.joint_radius   = miss_jiggles_hair_radius,
		.collider       = miss_jiggles_hair_colliders,
		.collider_count = 5,
	},
	{0},
};

/* Same clips as mr_muscles, so every duration-tied setting keeps its value. */
const CharacterAnimationSettings miss_jiggles_animation_settings = {

		.action_idle_max_blending_ratio = 0.85f,

		.footing_left  = MJ_FOOTING_LEFT,
		.footing_right = MJ_FOOTING_RIGHT,

		.turn_max_angle  = 5.0f,
		.turn_max_weight = 0.4f,

		.jump_max_blending_ratio = 0.55f,

		.jump_anim_length        = 0.633333f,
		.jump_anim_crouch        = 0.1f,
		.jump_anim_air           = 0.233333f,
		.jump_footing_speed      = 0.4f,

		.land_anim_length        = 0.9f,
		.land_anim_crouch        = 0.5f,
		.land_anim_ground        = 0.266666f,
		.land_anim_stand         = 0.833333f,

		.run_to_rolling_anim_lead     = 0.17f,
		.run_to_rolling_anim_ground   = 0.3f,
		.run_to_rolling_anim_grip     = 0.9f,
		.run_to_rolling_anim_stand    = 0.9f,
		.run_to_rolling_anim_length   = 1.166666f,

		.strafe_turn_rate        = 8.0f,
		.strafe_blend_rate       = 2.0f,

		.strafe_locked_blend_rate   = 2.0f,
		.aiming_blend_rate          = 4.0f,
		.charging_shoot_blend_rate  = 7.0f,
		.swim_blend_rate            = 7.0f,
		.climb_blend_rate           = 7.0f,

};


static const SoundID miss_jiggles_footsteps[] = {
	SOUND_FOOTSTEP_1, SOUND_FOOTSTEP_2, SOUND_FOOTSTEP_3,
	SOUND_FOOTSTEP_4,
};

static const SoundID miss_jiggles_rolls[] = {
	SOUND_ROLL_1, SOUND_ROLL_2,
};

/* Both feet land at a quarter and three quarters of the locomotion clip. */
static const float miss_jiggles_footings[] = { MJ_FOOTING_LEFT, MJ_FOOTING_RIGHT };
static const float miss_jiggles_strokes[]  = { MJ_STROKE_A, MJ_STROKE_B };

static const SoundID miss_jiggles_strokes_light[] = { SOUND_SWIM_STROKE_LIGHT_1, SOUND_SWIM_STROKE_LIGHT_2 };
static const SoundID miss_jiggles_strokes_heavy[] = { SOUND_SWIM_STROKE_HEAVY_1, SOUND_SWIM_STROKE_HEAVY_2 };
static const SoundID miss_jiggles_splashes[]      = { SOUND_SWIM_SPLASH_1, SOUND_SWIM_SPLASH_2 };

static const CharacterSoundDef miss_jiggles_sound_def = {

	.footstep       = miss_jiggles_footsteps,
	.footstep_count = sizeof(miss_jiggles_footsteps)/sizeof(*miss_jiggles_footsteps),
	.footing        = miss_jiggles_footings,
	.footing_count  = sizeof(miss_jiggles_footings)/sizeof(*miss_jiggles_footings),

	.footstep_volume_min = 0.05f,
	.footstep_volume_max = 0.4f,
	.footstep_speed_max  = 4.0f,

	.roll        = miss_jiggles_rolls,
	.roll_count  = sizeof(miss_jiggles_rolls)/sizeof(*miss_jiggles_rolls),
	.roll_volume = 0.8f,
	.roll_delay  = 0.07f,
	.roll_launch_gap      = 0.1f,
	.roll_launch_volume   = 0.18f,
	.roll_stand_volume = 0.20f,

	/* No sample of its own yet: the step doubles as the push off the floor
	   and as the touchdown. */
	.jump        = miss_jiggles_footsteps,
	.jump_count  = sizeof(miss_jiggles_footsteps)/sizeof(*miss_jiggles_footsteps),
	.jump_volume = 0.15f,
	.jump_launch_gap = 0.1f,

	.land            = miss_jiggles_footsteps,
	.land_count      = sizeof(miss_jiggles_footsteps)/sizeof(*miss_jiggles_footsteps),
	.land_volume_min = 0.35f,
	.land_volume_max = 0.8f,
	.land_speed_max  = 15.0f,

	.swim_stroke_light       = miss_jiggles_strokes_light,
	.swim_stroke_light_count = sizeof(miss_jiggles_strokes_light)/sizeof(*miss_jiggles_strokes_light),
	.swim_stroke_heavy       = miss_jiggles_strokes_heavy,
	.swim_stroke_heavy_count = sizeof(miss_jiggles_strokes_heavy)/sizeof(*miss_jiggles_strokes_heavy),

	.stroke        = miss_jiggles_strokes,
	.stroke_count  = sizeof(miss_jiggles_strokes)/sizeof(*miss_jiggles_strokes),
	.stroke_volume = 0.4f,

	.splash            = miss_jiggles_splashes,
	.splash_count      = sizeof(miss_jiggles_splashes)/sizeof(*miss_jiggles_splashes),
	.splash_volume_min = 0.05f,
	.splash_volume_max = 0.85f,
	.splash_speed_max  = 8.0f,

};


/* The aim bend, one third of the camera pitch per vertebra. */
static const char *const miss_jiggles_spine[] = {
	"mixamorig:Spine", "mixamorig:Spine1", "mixamorig:Spine2",
};

const CharacterAimingSettings miss_jiggles_aiming_settings = {
	.bone        = miss_jiggles_spine,
	.count       = 3,
	.pitch_scale = 1.0f,
};

const CharacterDef miss_jiggles_character_def = {

	.movement_settings  = &miss_jiggles_movement_settings,
	.animation_def      = &miss_jiggles_animation_def,
	.collider_settings  = &miss_jiggles_collider_settings,
	.weapons_def        = &miss_jiggles_weapons_def,
	.spring_bones       = miss_jiggles_spring_bones,
	.aiming_settings    = &miss_jiggles_aiming_settings,
	.sound_def          = &miss_jiggles_sound_def,
	.stats_settings     = &miss_jiggles_stats_settings,

};

const Prefab miss_jiggles = {
	.type      = PREFAB_CHARACTER,
	.model     = miss_jiggles_model,
	.character = &miss_jiggles_character_def,
};
