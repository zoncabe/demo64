#ifndef ASSETS_POOL_H
#define ASSETS_POOL_H

#include ENGINE_HEADER(entity, entity)
#include ENGINE_HEADER(prefab, prefab)

#define pool_model "rom:/models/water" ENGINE_MODEL_EXT

extern const EntityColliderDef pool_collider;
extern const WaterDef pool_water;

extern const Prefab pool;

#endif
