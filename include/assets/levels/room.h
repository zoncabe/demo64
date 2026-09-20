#ifndef ASSETS_ROOM_H
#define ASSETS_ROOM_H

#include ENGINE_HEADER(entity, entity3d)
#include ENGINE_HEADER(prefab, prefab3d)

#define room_model "rom:/models/room" ENGINE_MODEL_EXT

extern const entity3d::ColliderDef room_collider;

extern const Prefab3D room;

#endif
