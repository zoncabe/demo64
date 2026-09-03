#ifndef ASSETS_BALL_H
#define ASSETS_BALL_H

#include ENGINE_HEADER(entity, entity)
#include ENGINE_HEADER(prefab, prefab)

#define green_ball_model  "rom:/models/green_sphere"  ENGINE_MODEL_EXT
#define yellow_ball_model "rom:/models/yellow_sphere" ENGINE_MODEL_EXT
#define red_ball_model    "rom:/models/red_sphere"    ENGINE_MODEL_EXT

extern const EntityColliderDef green_ball_collider;
extern const EntityColliderDef yellow_ball_collider;
extern const EntityColliderDef red_ball_collider;

extern const RigidBodyDef ball_body;

extern const Prefab green_ball;
extern const Prefab yellow_ball;
extern const Prefab red_ball;

#endif
