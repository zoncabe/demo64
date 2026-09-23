#ifndef ASSETS_BALL_H
#define ASSETS_BALL_H

#include "entity/e64_entity3d.h"
#include "prefab/e64_prefab3d.h"

#define green_ball_model "rom:/models/green_sphere" ".t3dm"
#define yellow_ball_model "rom:/models/yellow_sphere" ".t3dm"
#define red_ball_model "rom:/models/red_sphere" ".t3dm"

extern const e64::collider::Def green_ball_collider;
extern const e64::collider::Def yellow_ball_collider;
extern const e64::collider::Def red_ball_collider;

extern const e64::RigidBody::Def ball_body;

extern const e64::Prefab3D green_ball;
extern const e64::Prefab3D yellow_ball;
extern const e64::Prefab3D red_ball;

#endif
