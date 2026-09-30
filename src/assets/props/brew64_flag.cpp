#include "assets/props/brew64_flag.h"

/* The flag hangs from the flagpole (its own prefab); the scene places the pair. */
const e64::Cloth::Def brew64_flag_cloth = {
	.mesh_path = "rom:/collision/brew_flag.collision",
	/* Jakobsen's paper: damped Verlet x' = 1.99x - 0.99x* (damping 0.01) and
	   3-4 relaxation passes. */
	.damping = 0.01f,
	.iterations = 4,
	.pin_max_x = 0.01f,
};

/* A loop: it plays from the flag for as long as the flag exists. */
static const e64::Sound::Def brew64_flag_flapping = {
	.path = "rom:/audio/flag_flapping" ".wav64",
	.volume = 0.9f,
	.min_distance = 6.0f,
	.max_distance = 30.0f,
	.loop = true,
	.priority = e64::Sound::PRIORITY_AMBIENCE,
	.preload = false,
};

static const e64::Sound::Def *const brew64_flag_sound[] = { &brew64_flag_flapping };

const e64::Prefab3D brew64_flag = {
	.type = e64::prefab3d::PREFAB3D_CLOTH,
	.mesh = { .model = brew64_flag_model },
	.cloth = &brew64_flag_cloth,
	.sound = brew64_flag_sound,
	.sound_count = 1,
};
