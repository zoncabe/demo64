#ifndef ASSETS_CRATE_H
#define ASSETS_CRATE_H

#include ENGINE_HEADER(entity, entity)
#include ENGINE_HEADER(prefab, prefab)

#define green_crate_model  "rom:/models/green_box"  ENGINE_MODEL_EXT
#define yellow_crate_model "rom:/models/yellow_box" ENGINE_MODEL_EXT
#define red_crate_model    "rom:/models/red_box"    ENGINE_MODEL_EXT

extern const EntityColliderDef green_crate_collider;
extern const EntityColliderDef yellow_crate_collider;
extern const EntityColliderDef red_crate_collider;

extern const RigidBodyDef crate_body;

extern const Prefab green_crate;
extern const Prefab yellow_crate;
extern const Prefab red_crate;

#endif
