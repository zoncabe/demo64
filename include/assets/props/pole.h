#ifndef ASSETS_POLE_H
#define ASSETS_POLE_H

#include ENGINE_HEADER(entity, entity)
#include ENGINE_HEADER(prefab, prefab)

#define pole_model "rom:/models/pole" ENGINE_MODEL_EXT

extern const EntityColliderDef pole_collider;

extern const Prefab pole;

#endif
