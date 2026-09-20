#include "assets/props/brew64_flag.h"

/* The flag hangs from a pole (its own prefab); the scene places the pair. */
const ClothDef brew64_flag_cloth = {
	.mesh_path  = "rom:/collision/brew_flag.collision",
	/* Jakobsen's paper: damped Verlet x' = 1.99x - 0.99x* (damping 0.01) and
	   3-4 relaxation passes. */
	.damping    = 0.01f,
	.iterations = 4,
	.pin_max_x  = 0.01f,
};

/* A loop: it plays from the flag for as long as the flag exists. */
static const SoundDef brew64_flag_flapping = {
	.path         = "rom:/audio/flag_flapping" ENGINE_WAVE_EXT,
	.volume       = 0.9f,
	.min_distance = 6.0f,
	.max_distance = 30.0f,
	.loop         = true,
	.priority     = SOUND_PRIORITY_AMBIENCE,
	.preload      = false,
};

static const SoundDef *const brew64_flag_sound[] = { &brew64_flag_flapping };

const Prefab3D brew64_flag = {
	.type        = PREFAB3D_CLOTH,
	.model       = brew64_flag_model,
	.sound       = brew64_flag_sound,
	.sound_count = 1,
	.cloth       = &brew64_flag_cloth,
};
