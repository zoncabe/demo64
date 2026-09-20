#ifndef ASSETS_BALL_H
#define ASSETS_BALL_H

#include ENGINE_HEADER(entity, entity3d)
#include ENGINE_HEADER(prefab, prefab3d)

#define green_ball_model  "rom:/models/green_sphere"  ENGINE_MODEL_EXT
#define yellow_ball_model "rom:/models/yellow_sphere" ENGINE_MODEL_EXT
#define red_ball_model    "rom:/models/red_sphere"    ENGINE_MODEL_EXT

extern const entity3d::ColliderDef green_ball_collider;
extern const entity3d::ColliderDef yellow_ball_collider;
extern const entity3d::ColliderDef red_ball_collider;

extern const RigidBodyDef ball_body;

extern const Prefab3D green_ball;
extern const Prefab3D yellow_ball;
extern const Prefab3D red_ball;

#endif
