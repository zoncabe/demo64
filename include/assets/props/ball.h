#ifndef ASSETS_BALL_H
#define ASSETS_BALL_H

#include "entity/e64_entity.h"
#include "prefab/e64_prefab.h"

#define green_ball_model  "rom:/models/green_sphere.t3dm"
#define yellow_ball_model "rom:/models/yellow_sphere.t3dm"
#define red_ball_model    "rom:/models/red_sphere.t3dm"

extern const EntityColliderDef green_ball_collider;
extern const EntityColliderDef yellow_ball_collider;
extern const EntityColliderDef red_ball_collider;

extern const RigidBodyDef ball_body;

extern const Prefab green_ball;
extern const Prefab yellow_ball;
extern const Prefab red_ball;

#endif
