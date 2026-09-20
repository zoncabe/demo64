#ifndef ASSETS_CRATE_H
#define ASSETS_CRATE_H

#include ENGINE_HEADER(entity, entity3d)
#include ENGINE_HEADER(prefab, prefab3d)

#define green_crate_model  "rom:/models/green_box"  ENGINE_MODEL_EXT
#define yellow_crate_model "rom:/models/yellow_box" ENGINE_MODEL_EXT
#define red_crate_model    "rom:/models/red_box"    ENGINE_MODEL_EXT

extern const entity3d::ColliderDef green_crate_collider;
extern const entity3d::ColliderDef yellow_crate_collider;
extern const entity3d::ColliderDef red_crate_collider;

extern const RigidBodyDef crate_body;

extern const Prefab3D green_crate;
extern const Prefab3D yellow_crate;
extern const Prefab3D red_crate;

#endif
