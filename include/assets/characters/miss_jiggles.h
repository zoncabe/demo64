#ifndef MISS_JIGGLES_H
#define MISS_JIGGLES_H

#include ENGINE_HEADER(character, character)
#include ENGINE_HEADER(prefab, prefab)

#define miss_jiggles_model "rom:/models/miss_jiggles" ENGINE_MODEL_EXT

/* Soft-body driven bones present in the rig. */
#define MISS_JIGGLES_BONE_BREAST_L "Breast.L"
#define MISS_JIGGLES_BONE_BREAST_R "Breast.R"
#define MISS_JIGGLES_BONE_HAIR_1   "Hair1"
#define MISS_JIGGLES_BONE_HAIR_2   "Hair2"
#define MISS_JIGGLES_BONE_HAIR_3   "Hair3"
#define MISS_JIGGLES_BONE_HAIR_4   "Hair4"
#define MISS_JIGGLES_BONE_HEAD     "mixamorig:Head"
#define MISS_JIGGLES_BONE_NECK     "mixamorig:Neck"
#define MISS_JIGGLES_BONE_SPINE_2  "mixamorig:Spine2"
#define MISS_JIGGLES_BONE_ARM_R    "mixamorig:RightArm"


typedef enum {

	MJ_SLOT_IDLE_R,
	MJ_SLOT_WALK,
	MJ_SLOT_RUN,
	MJ_SLOT_SPRINT,
	MJ_SLOT_TURN_WALK,
	MJ_SLOT_TURN_RUN,
	MJ_SLOT_STRAFE_WALK,
	MJ_SLOT_STRAFE_RUN,
	MJ_SLOT_STRAFE_LOCKED_WALK,
	MJ_SLOT_STRAFE_LOCKED_WALK_SIDE,
	MJ_SLOT_STRAFE_LOCKED_RUN,
	MJ_SLOT_STRAFE_LOCKED_RUN_SIDE,
	MJ_SLOT_BOW_HOLD_IDLE,
	MJ_SLOT_BOW_HOLD_WALK,
	MJ_SLOT_BOW_HOLD_WALK_SIDE,
	MJ_SLOT_BOW_HOLD_RUN,
	MJ_SLOT_BOW_HOLD_RUN_SIDE,
	MJ_SLOT_BOW_AIM_IDLE,
	MJ_SLOT_BOW_AIM_WALK,
	MJ_SLOT_BOW_AIM_WALK_SIDE,
	MJ_SLOT_BOW_AIM_RUN,
	MJ_SLOT_BOW_AIM_RUN_SIDE,

	MJ_SLOT_JUMP_L,
	MJ_SLOT_JUMP_R,
	MJ_SLOT_LAND_L,
	MJ_SLOT_LAND_R,

	MJ_SLOT_ROLL_RUN,

	/* Two slots for the swim grid: adjacent columns must land in different
	   buffers or their poses overwrite each other instead of blending. */
	MJ_SLOT_SWIM_A,
	MJ_SLOT_SWIM_B,

	MJ_SLOT_CLIMB,

	MJ_SLOT_TRANSITION,

	MJ_SLOT_COUNT

} MissJigglesSlot;

typedef enum {

	MJ_NODE_IDLE,
	MJ_NODE_IDLE_R,
	MJ_NODE_LOCOMOTION,
	MJ_NODE_JUMP_L,
	MJ_NODE_JUMP_R,
	MJ_NODE_JUMP_L_LAYER,
	MJ_NODE_JUMP_R_LAYER,
	MJ_NODE_LAND_L,
	MJ_NODE_LAND_R,
	MJ_NODE_ROLL,
	MJ_NODE_ROLL_LAYER,
	MJ_NODE_STRAFE,
	MJ_NODE_STRAFE_LOCKED,
	MJ_NODE_BOW_HOLD_IDLE,
	MJ_NODE_BOW_HOLD,
	MJ_NODE_BOW_AIM_IDLE,
	MJ_NODE_BOW_AIM,
	MJ_NODE_SWIM,
	MJ_NODE_CLIMB,
	MJ_NODE_CLIMB_LAYER,

	MJ_NODE_COUNT

} MissJigglesNode;

typedef enum {

	MJ_ANIM_IDLE_L,
	MJ_ANIM_IDLE_R,

	MJ_ANIM_STAND_TO_WALK_L,
	MJ_ANIM_STAND_TO_WALK_R,

	MJ_ANIM_STAND_TO_RUN_L,
	MJ_ANIM_STAND_TO_RUN_R,

	MJ_ANIM_WALK,
	MJ_ANIM_TURN_WALK_L,
	MJ_ANIM_TURN_WALK_R,
	MJ_ANIM_WALK_CHANGE_DIR_L,
	MJ_ANIM_WALK_CHANGE_DIR_R,
	MJ_ANIM_WALK_TO_STAND_L,
	MJ_ANIM_WALK_TO_STAND_R,

	MJ_ANIM_WALK_BACK,
	MJ_ANIM_WALK_BACK_L,
	MJ_ANIM_WALK_BACK_R,
	MJ_ANIM_WALK_STRAFE_L,
	MJ_ANIM_WALK_STRAFE_R,

	MJ_ANIM_RUN,
	MJ_ANIM_TURN_RUN_L,
	MJ_ANIM_TURN_RUN_R,
	MJ_ANIM_RUN_CHANGE_DIR_L,
	MJ_ANIM_RUN_CHANGE_DIR_R,
	MJ_ANIM_RUN_TO_STAND_L,
	MJ_ANIM_RUN_TO_STAND_R,

	MJ_ANIM_RUN_BACK,
	MJ_ANIM_RUN_BACK_L,
	MJ_ANIM_RUN_BACK_R,
	MJ_ANIM_RUN_STRAFE_L,
	MJ_ANIM_RUN_STRAFE_R,

	MJ_ANIM_SPRINT,

	MJ_ANIM_STRAFE_LOCKED_WALK_FWD,
	MJ_ANIM_STRAFE_LOCKED_WALK_BACK,
	MJ_ANIM_STRAFE_LOCKED_WALK_L,
	MJ_ANIM_STRAFE_LOCKED_WALK_R,
	MJ_ANIM_STRAFE_LOCKED_RUN_FWD,
	MJ_ANIM_STRAFE_LOCKED_RUN_BACK,
	MJ_ANIM_STRAFE_LOCKED_RUN_L,
	MJ_ANIM_STRAFE_LOCKED_RUN_R,

	MJ_ANIM_BOW_HOLD_IDLE,
	MJ_ANIM_BOW_HOLD_WALK_FWD,
	MJ_ANIM_BOW_HOLD_WALK_BACK,
	MJ_ANIM_BOW_HOLD_WALK_L,
	MJ_ANIM_BOW_HOLD_WALK_R,
	MJ_ANIM_BOW_HOLD_RUN_FWD,
	MJ_ANIM_BOW_HOLD_RUN_BACK,
	MJ_ANIM_BOW_HOLD_RUN_L,
	MJ_ANIM_BOW_HOLD_RUN_R,

	MJ_ANIM_BOW_AIM_IDLE,
	MJ_ANIM_BOW_AIM_WALK_FWD,
	MJ_ANIM_BOW_AIM_WALK_BACK,
	MJ_ANIM_BOW_AIM_WALK_L,
	MJ_ANIM_BOW_AIM_WALK_R,
	MJ_ANIM_BOW_AIM_RUN_FWD,
	MJ_ANIM_BOW_AIM_RUN_BACK,
	MJ_ANIM_BOW_AIM_RUN_L,
	MJ_ANIM_BOW_AIM_RUN_R,

	MJ_ANIM_JUMP_L,
	MJ_ANIM_JUMP_R,
	MJ_ANIM_FALL_L,
	MJ_ANIM_FALL_R,
	MJ_ANIM_LAND_L,
	MJ_ANIM_LAND_R,

	MJ_ANIM_ROLL_L,
	MJ_ANIM_ROLL_R,

	MJ_ANIM_SWIM_IDLE,
	MJ_ANIM_SWIM_SLOW,
	MJ_ANIM_SWIM_FAST,

	/* Down is its own clip so the cycle reads right in both directions:
	   tiny3d clamps a negative speed to zero, so a ladder cannot be
	   descended by running the climb backwards. */
	MJ_ANIM_CLIMB_DOWN,
	MJ_ANIM_CLIMB_UP,

	MJ_ANIM_SLIDE_L,
	MJ_ANIM_SLIDE_R,

	MJ_ANIM_TRANSITION_L,
	MJ_ANIM_TRANSITION_R,

	MJ_ANIM_COUNT

} MissJigglesAnim;


extern const CharacterMovementSettings  miss_jiggles_movement_settings;
extern const CharacterStatsSettings     miss_jiggles_stats_settings;
extern const CharacterColliderSettings  miss_jiggles_collider_settings;
extern const char *const               miss_jiggles_weapon_meshes[];
extern const CharacterWeaponsDef        miss_jiggles_weapons_def;
extern const WeaponDef                  weapon_bow;
extern const SpringBonesDef             miss_jiggles_spring_bones[];
extern const CharacterAnimationSettings miss_jiggles_animation_settings;
extern const CharacterAnimationDef      miss_jiggles_animation_def;
extern const CharacterDef               miss_jiggles_character_def;
extern const Prefab                     miss_jiggles;

#endif
