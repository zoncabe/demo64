#ifndef ASSETS_LAMP_POST_H
#define ASSETS_LAMP_POST_H

#include ENGINE_HEADER(entity, entity3d)
#include ENGINE_HEADER(prefab, prefab3d)

#define lamp_post_model "rom:/models/lamp_post" ENGINE_MODEL_EXT

/* Where the lamp part sits over the post, metres: the scene puts each
   placement's point light at this height so it lands inside the glass. */
#define LAMP_POST_HEIGHT 6.252f

extern const entity3d::ColliderDef lamp_post_collider;

extern const Prefab3D lamp_post;

#endif
