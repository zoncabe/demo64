#ifndef ASSETS_POOL_H
#define ASSETS_POOL_H

#include "entity/e64_entity3d.h"
#include "prefab/e64_prefab3d.h"

#define pool_model "rom:/models/water" ".t3dm"

extern const e64::collider::Def pool_collider;
extern const e64::Water::Def pool_water;

extern const e64::Prefab3D pool;

#endif
