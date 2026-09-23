#ifndef ASSETS_CRATE_H
#define ASSETS_CRATE_H

#include "entity/e64_entity3d.h"
#include "prefab/e64_prefab3d.h"

#define green_crate_model "rom:/models/green_box" ".t3dm"
#define yellow_crate_model "rom:/models/yellow_box" ".t3dm"
#define red_crate_model "rom:/models/red_box" ".t3dm"

extern const e64::collider::Def green_crate_collider;
extern const e64::collider::Def yellow_crate_collider;
extern const e64::collider::Def red_crate_collider;

extern const e64::RigidBody::Def crate_body;

extern const e64::Prefab3D green_crate;
extern const e64::Prefab3D yellow_crate;
extern const e64::Prefab3D red_crate;

#endif
