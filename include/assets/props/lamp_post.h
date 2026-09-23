#ifndef ASSETS_LAMP_POST_H
#define ASSETS_LAMP_POST_H

#include "entity/e64_entity3d.h"
#include "prefab/e64_prefab3d.h"

#define lamp_post_model "rom:/models/lamp_post" ".t3dm"

/* Where the lamp part sits over the post, metres: the scene puts each
   placement's point light at this height so it lands inside the glass. */
#define LAMP_POST_HEIGHT 6.252f

extern const e64::collider::Def lamp_post_collider;

extern const e64::Prefab3D lamp_post;

#endif
