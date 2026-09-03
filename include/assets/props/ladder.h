#ifndef ASSETS_LADDER_H
#define ASSETS_LADDER_H

#include ENGINE_HEADER(entity, entity)
#include ENGINE_HEADER(prefab, prefab)

#define ladder_model "rom:/models/ladder" ENGINE_MODEL_EXT

extern const EntityColliderDef ladder_collider;

extern const Prefab ladder;

#endif
