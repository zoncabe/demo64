#ifndef ASSETS_POOL_H
#define ASSETS_POOL_H

#include ENGINE_HEADER(entity, entity3d)
#include ENGINE_HEADER(prefab, prefab3d)

#define pool_model "rom:/models/water" ENGINE_MODEL_EXT

extern const entity3d::ColliderDef pool_collider;
extern const WaterDef pool_water;

extern const Prefab3D pool;

#endif
