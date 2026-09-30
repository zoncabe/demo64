#include "assets/props/lamp.h"

/* Pure dressing: no collider. */
const e64::Prefab3D lamp = {
	.type = e64::prefab3d::PREFAB3D_PROP,
	.mesh = { .model = lamp_model },
};
