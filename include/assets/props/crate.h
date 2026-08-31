#ifndef ASSETS_CRATE_H
#define ASSETS_CRATE_H

#include "entity/e64_entity.h"
#include "prefab/e64_prefab.h"

#define green_crate_model  "rom:/models/green_box.t3dm"
#define yellow_crate_model "rom:/models/yellow_box.t3dm"
#define red_crate_model    "rom:/models/red_box.t3dm"

extern const EntityColliderDef green_crate_collider;
extern const EntityColliderDef yellow_crate_collider;
extern const EntityColliderDef red_crate_collider;

extern const RigidBodyDef crate_body;

extern const Prefab green_crate;
extern const Prefab yellow_crate;
extern const Prefab red_crate;

#endif
