#ifndef ASSETS_ROOM_H
#define ASSETS_ROOM_H

#include ENGINE_HEADER(entity, entity)
#include ENGINE_HEADER(prefab, prefab)

#define room_model "rom:/models/room" ENGINE_MODEL_EXT

extern const EntityColliderDef room_collider;

extern const Prefab room;

#endif
