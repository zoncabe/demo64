#ifndef ASSETS_POLE_H
#define ASSETS_POLE_H

#include ENGINE_HEADER(entity, entity3d)
#include ENGINE_HEADER(prefab, prefab3d)

#define pole_model "rom:/models/pole" ENGINE_MODEL_EXT

extern const entity3d::ColliderDef pole_collider;

extern const Prefab3D pole;

#endif
