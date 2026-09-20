#ifndef ASSETS_LADDER_H
#define ASSETS_LADDER_H

#include ENGINE_HEADER(entity, entity3d)
#include ENGINE_HEADER(prefab, prefab3d)

#define ladder_model "rom:/models/ladder" ENGINE_MODEL_EXT

extern const entity3d::ColliderDef ladder_collider;

extern const Prefab3D ladder;

#endif
