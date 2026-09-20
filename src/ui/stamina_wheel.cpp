/*
	Radial stamina gauge, a textured particle anchored to the character hip
	bone so it follows the run cycle.

	One wheel per body, two slots: the stats live in the character, so when
	the player switches away the abandoned body keeps its wheel fading out
	in place — showing its real recovery — while the new body fades its
	own in. A slot follows its character until the fade completes.

	The radial cutout comes from the N64brew GameJam 2024 setup screen:
	TILE0 holds an angular sweep gradient and TILE1 the ring artwork; the
	combiner routes the sweep into alpha, and alpha compare discards every
	pixel whose angle lies past the current progress.
*/
#include <assert.h>
#ifndef ENGINE_ULTRA64
#include <libdragon.h>
#endif
#if defined(ENGINE_VOLCANO64)
#include ENGINE_HEADER(physics/math, matrix4)
#elif defined(ENGINE_ULTRA64)
#include ENGINE_HEADER(physics/math, fmath)
#include ENGINE_HEADER(physics/math, matrix4)
#include ENGINE_HEADER(resource, resource)
#else
#include <t3d/t3d.h>
#include <t3d/t3dskeleton.h>
#endif

#include ENGINE_HEADER(engine, common)
#include ENGINE_HEADER(physics/math, math_common)
#include ENGINE_HEADER(physics/math, math_functions)
#include ENGINE_HEADER(physics/math, vector3)
#include ENGINE_HEADER(time, time)
#include ENGINE_HEADER(viewport, viewport)
#include ENGINE_HEADER(entity, entity3d)
#include ENGINE_HEADER(character3d, character3d)
#include ENGINE_HEADER(player, player)
#include ENGINE_HEADER(game, game)
#include ENGINE_HEADER(particles, particles)
#include "ui/stamina_wheel.h"

#define STAMINA_WHEEL_BONE     "mixamorig:Hips"

/* Enough for the switch: the wheel leaving and the wheel arriving. */
#define STAMINA_WHEEL_COUNT    2

/* Offset from the hip, metres: X runs along the camera right axis so the
   wheel keeps its place on screen while the camera orbits, Z stays on the
   world vertical. */
#define STAMINA_WHEEL_OFFSET_X -0.18f
#define STAMINA_WHEEL_OFFSET_Z  1.15f

#define STAMINA_WHEEL_SIZE     6
#define STAMINA_WHEEL_SCALE    1.0f

/* Color bands over the normalized stamina, and the recovery pulse. Each band
   starts blending into the next one this span before its threshold. */
#define STAMINA_WHEEL_YELLOW_BAND  0.5f
#define STAMINA_WHEEL_RED_BAND     0.2f
#define STAMINA_WHEEL_BLEND_SPAN   0.05f
#define STAMINA_WHEEL_FADE_RATE    3.0f   /* alpha per second */
#define STAMINA_WHEEL_MAX_ALPHA    0.90f  /* never fully solid */
#define STAMINA_WHEEL_PULSE_SPEED  8.0f   /* radians per second */
#define STAMINA_WHEEL_GREEN_RATE   6.0f   /* recovery lerp response */

/* Seconds at full stamina before the wheel may leave: the burst plays out on
   screen instead of being cut off, and the wheel starts going the moment it
   ends. */
#define STAMINA_WHEEL_SETTLE_TIME  0.30f

/* Completion burst: the frame the wheel fills it pops in scale and the green
   flashes to white, then eases back over its own time. It is kept off the
   fade on purpose: tied to the alpha, the flash peaked on a wheel that was
   already invisible. */
#define STAMINA_WHEEL_BURST_TIME   0.3f
#define STAMINA_WHEEL_BURST_RISE   0.2f  /* the way into the flash, eased in;
                                            the rest of the burst is the way out */
#define STAMINA_WHEEL_BURST_SCALE  0.35f
#define STAMINA_WHEEL_FLASH_MIX    1.0f

static const float wheel_green[3]  = {  80.0f, 200.0f, 90.0f };
static const float wheel_yellow[3] = { 230.0f, 200.0f, 40.0f };
static const float wheel_red[3]    = { 220.0f,  50.0f, 40.0f };


typedef struct {

	Particle        *particle;
	const Character3D *character;   /* NULL leaves the slot free */

	int16_t bone;
	float   progress;
	float   alpha;
	float   pulse;
	float   settle_timer;   /* runs while stamina is full; the leave waits on it */
	float   burst_timer;    /* time since the wheel filled; drives the flash */
	float   color[3];

} StaminaWheel;


static StaminaWheel wheel_slot[STAMINA_WHEEL_COUNT];

/* Sweep gradient (the cutout alpha) and ring artwork: the wheel's own,
   loaded with the gameplay state and freed with it. */
#ifdef ENGINE_ULTRA64
static Texture *wheel_mask;
static Texture *wheel_progress;
#else
static sprite_t *wheel_mask;
static sprite_t *wheel_progress;
#endif


/* Banded while draining, pulsing yellow-to-red while tired (less red as it
   refills), easing back to green once full. */
static void staminaWheel_setColor(StaminaWheel *wheel, const character3d::Stats *stats, float dt)
{
	if (stats->tired) {
		wheel->pulse += dt * STAMINA_WHEEL_PULSE_SPEED;
		float red_mix = (0.5f + 0.5f * fm_sinf(wheel->pulse)) * (1.0f - stats->stamina);
		for (int i = 0; i < 3; i++)
			wheel->color[i] = wheel_yellow[i] + (wheel_red[i] - wheel_yellow[i]) * red_mix;
		return;
	}

	if (stats->stamina >= 1.0f) {
		float t = clampf(STAMINA_WHEEL_GREEN_RATE * dt, 0.0f, 1.0f);
		for (int i = 0; i < 3; i++)
			wheel->color[i] += (wheel_green[i] - wheel->color[i]) * t;
		return;
	}

	const float *lower, *upper;
	float band;
	if (stats->stamina >= STAMINA_WHEEL_YELLOW_BAND) {
		lower = wheel_yellow; upper = wheel_green;  band = STAMINA_WHEEL_YELLOW_BAND;
	} else {
		lower = wheel_red;    upper = wheel_yellow; band = STAMINA_WHEEL_RED_BAND;
	}

	float t = clampf((stats->stamina - band) / STAMINA_WHEEL_BLEND_SPAN, 0.0f, 1.0f);
	for (int i = 0; i < 3; i++)
		wheel->color[i] = lower[i] + (upper[i] - lower[i]) * t;
}


/* The driven character always owns a slot: a free one, or failing that the
   faintest wheel, restarted on the new body. The color starts on green and
   the first setColor writes the band the real stamina asks for. */
static void staminaWheel_assign(const Character3D *driven)
{
	if (driven == NULL) return;

	StaminaWheel *claim = &wheel_slot[0];
	for (int i = 0; i < STAMINA_WHEEL_COUNT; i++) {
		StaminaWheel *slot = &wheel_slot[i];
		if (slot->character == driven) return;
		if (claim->character != NULL
		    && (slot->character == NULL || slot->alpha < claim->alpha))
			claim = slot;
	}

	claim->character    = driven;
	claim->bone         = -1;
	claim->progress     = driven->stats.stamina;
	claim->alpha        = 0.0f;
	claim->pulse        = 0.0f;

	/* A body taken over while already full has no recovery to show: its
	   timer starts spent, or the wheel would flash in on every switch. */
	claim->settle_timer = (driven->stats.stamina >= 1.0f) ? STAMINA_WHEEL_SETTLE_TIME : 0.0f;
	claim->burst_timer  = STAMINA_WHEEL_BURST_TIME;
	for (int i = 0; i < 3; i++) claim->color[i] = wheel_green[i];
}

static void staminaWheel_setInput(Particle *particle, uint8_t fb_index)
{
	const Player *seat = &player::get()[0];
	const Character3D *driven = (seat->type == player::CHARACTER_3D) ? seat->character3d.character : NULL;

	/* The first wheel of the frame settles who owns which slot. */
	if (particle == wheel_slot[0].particle)
		staminaWheel_assign(driven);

	StaminaWheel *wheel = &wheel_slot[(particle == wheel_slot[0].particle) ? 0 : 1];
	const Character3D *character = wheel->character;

	if (!character) { particle->visible = false; return; }

	float dt = time_get()->delta;
	staminaWheel_setColor(wheel, &character->stats, dt);

	if (character->stats.stamina >= 1.0f) {
		/* The frame the bar fills opens the burst, a settle before the wheel
		   starts leaving: hung off the settle instead, the flash came out on
		   top of the fade and both were gone in the same instant. */
		if (wheel->settle_timer == 0.0f) wheel->burst_timer = 0.0f;
		wheel->settle_timer += dt;
	}
	else wheel->settle_timer = 0.0f;

	wheel->burst_timer += dt;

	/* Fade in whenever stamina is short; fade out while aiming, or once the
	   color has settled back on green so the lerp is seen before the wheel
	   leaves. An abandoned body's wheel just fades out where it stands. */
	bool is_driven = character == driven;
	bool aiming    = is_driven && character->movement.data.strafe;
	bool settled   = wheel->settle_timer >= STAMINA_WHEEL_SETTLE_TIME;

	float target = (is_driven && !aiming && !settled) ? STAMINA_WHEEL_MAX_ALPHA : 0.0f;
	float fade_step = STAMINA_WHEEL_FADE_RATE * dt;
	wheel->alpha += clampf(target - wheel->alpha, -fade_step, fade_step);

	if (!is_driven && wheel->alpha <= 0.0f) wheel->character = NULL;

	particle->visible = wheel->alpha > 0.0f;
	if (!particle->visible) return;

	wheel->progress = character->stats.stamina;

	/* Into the white and back out of it. The way in is eased, so the green
	   accelerates into the flash: jumping straight to white came out as one
	   frame of a different colour instead of as a change. */
	float burst;
	if (wheel->burst_timer < STAMINA_WHEEL_BURST_RISE) {
		float rise = wheel->burst_timer / STAMINA_WHEEL_BURST_RISE;
		burst = rise * rise;
	}
	else {
		float fall = (wheel->burst_timer - STAMINA_WHEEL_BURST_RISE)
		           / (STAMINA_WHEEL_BURST_TIME - STAMINA_WHEEL_BURST_RISE);
		burst = clampf(1.0f - fall, 0.0f, 1.0f);
	}

	/* The particle backend is tiny3d's tpx: on volcano64 and ultra64 the
	   module keeps its API but holds no particle to colour and draws
	   nothing. */
#if !defined(ENGINE_VOLCANO64) && !defined(ENGINE_ULTRA64)
	for (int i = 0; i < 3; i++) {
		float flash = wheel->color[i] + (255.0f - wheel->color[i]) * STAMINA_WHEEL_FLASH_MIX * burst;
		particle->buffer.s8[0].colorA[i] = (uint8_t)flash;
	}
#endif

#if defined(ENGINE_VOLCANO64) || defined(ENGINE_ULTRA64)
	const Armature *skeleton = &character->animation.main;
	if (wheel->bone < 0) {
		wheel->bone = (int16_t)armature_findBone((Armature *)skeleton, STAMINA_WHEEL_BONE);
		if (wheel->bone < 0) { particle->visible = false; return; }
	}

	Vector3 bone_position;
	Quaternion bone_rotation;
	character3d::getBonePose(skeleton, wheel->bone, &bone_position, &bone_rotation);

	/* Model space to world with the same matrix the renderer builds for the
	   mesh, then the offset keeps the wheel clear of the body. Matrix4 is
	   libdragon's fm_mat4_t, so fgeom does the multiply. */
	const RenderTransform *transform = &character->entity->transform;
	Matrix4 world;
	Vector3 rotation = {deg_to_rad(transform->rotation.x), deg_to_rad(transform->rotation.y), deg_to_rad(transform->rotation.z)};
	matrix4_fromSrtEuler(&world, &transform->scale, &rotation, &transform->position);

#ifdef ENGINE_ULTRA64
	/* ultra64's Matrix4 is row-major with the translation in row 3, the
	   layout the gu functions take: the point goes in as a row vector. */
	struct { float v[3]; } position;
	for (int i = 0; i < 3; i++)
		position.v[i] = bone_position.x * world.m[0][i]
		              + bone_position.y * world.m[1][i]
		              + bone_position.z * world.m[2][i]
		              + world.m[3][i];
#else
	fm_vec4_t position;
	fm_vec3_t bone = {{bone_position.x, bone_position.y, bone_position.z}};
	fm_mat4_mul_vec3(&position, &world, &bone);
#endif
#else
	const T3DSkeleton *skeleton = &character->animation.main;
	if (wheel->bone < 0) {
		wheel->bone = (int16_t)t3d_skeleton_find_bone((T3DSkeleton *)skeleton, STAMINA_WHEEL_BONE);
		if (wheel->bone < 0) { particle->visible = false; return; }
	}

	T3DVec3 bone_position;
	T3DQuat bone_rotation;
	character3d::getBonePose(skeleton, wheel->bone, &bone_position, &bone_rotation);

	/* Model space to world with the same matrix the renderer builds for the
	   mesh, then the offset keeps the wheel clear of the body.

	   The bone comes out of the model in render units, so the body's position,
	   which is in metres like everything the game declares, enters scaled the
	   same way mesh_setMatrix scales it. What comes out is render units, which
	   is what the particle matrix takes. */
	const RenderTransform *transform = &character->entity->transform;
	T3DMat4 world;
	t3d_mat4_from_srt_euler(&world,
		(float[3]){transform->scale.x,                 transform->scale.y,                 transform->scale.z},
		(float[3]){deg_to_rad(transform->rotation.x),  deg_to_rad(transform->rotation.y),  deg_to_rad(transform->rotation.z)},
		(float[3]){transform->position.x * RENDER_SCALE, transform->position.y * RENDER_SCALE, transform->position.z * RENDER_SCALE}
	);

	T3DVec4 position;
	t3d_mat4_mul_vec3(&position, &world, &bone_position);
#endif

	const Camera *camera = &viewport_get()->camera;
	Vector3 forward = vector3_difference(&camera->target, &camera->position);
	Vector3 up = {0.0f, 0.0f, 1.0f};
	Vector3 right = vector3_cross(&forward, &up);
	right = vector3_normalized(&right);

	float scale = STAMINA_WHEEL_SCALE * (1.0f + STAMINA_WHEEL_BURST_SCALE * burst);

	particleBuffer_setMatrix(&particle->buffer,
		(float[3]){scale, scale, scale},
		(float[3]){0.0f, 0.0f, 0.0f},
		(float[3]){
			position.v[0] + right.x * STAMINA_WHEEL_OFFSET_X * RENDER_SCALE,
			position.v[1] + right.y * STAMINA_WHEEL_OFFSET_X * RENDER_SCALE,
			position.v[2] + STAMINA_WHEEL_OFFSET_Z * RENDER_SCALE,
		},
		fb_index);

	particle->matrix = &particle->buffer.matrix[fb_index];
}

static void staminaWheel_setRenderState(const StaminaWheel *wheel)
{
#ifdef ENGINE_ULTRA64
	/* Never reached until particles are ported: the engine's particle draw
	   is a stub. The state below is what the port has to reproduce. */
	(void)wheel;
#else
	rdpq_set_mode_standard();
	rdpq_mode_blender(RDPQ_BLENDER_MULTIPLY);

	/* The technique from the N64brew Discord (HailToDodongo/Tharo): in
	   2-cycle mode the alpha compare reads the FIRST cycle's alpha (the
	   sweep, TEX0) while the blender reads the second cycle's — so clip and
	   opacity are decoupled by hardware. The env alpha multiplies only the
	   blender side (ring coverage × fade): the wheel fades in and out
	   without moving the cutoff, and the threshold stays unscaled. */
	rdpq_mode_combiner(RDPQ_COMBINER2(
		(TEX1,0,PRIM,0),  (0,0,0,TEX0),
		(0,0,0,COMBINED), (TEX1,0,ENV,0)
	));
	rdpq_set_env_color(RGBA32(0, 0, 0, (uint8_t)(wheel->alpha * 255.0f)));
	rdpq_mode_alphacompare((1.0f - wheel->progress) * 255.0f);

	/* tpx maps UVs to an 8x8px base no matter the real size: scale_log
	   stretches the 32px textures over the whole particle. */
	rdpq_texparms_t parms = { .s = { .scale_log = -2 }, .t = { .scale_log = -2 } };
	rdpq_tex_multi_begin();
	rdpq_sprite_upload(TILE0, wheel_mask, &parms);
	rdpq_sprite_upload(TILE1, wheel_progress, &parms);
	rdpq_tex_multi_end();
#endif
}

/* One trampoline per slot: the render pass carries no particle context. */
static void staminaWheel_setRenderStateA(void) { staminaWheel_setRenderState(&wheel_slot[0]); }
static void staminaWheel_setRenderStateB(void) { staminaWheel_setRenderState(&wheel_slot[1]); }

void stamina_wheel_load(void)
{
#ifdef ENGINE_ULTRA64
	wheel_mask     = resource_load("rom:/sprites/core/CircleMask.i8.sprite",     RESOURCE_TEXTURE, NULL);
	wheel_progress = resource_load("rom:/sprites/core/CircleProgress.i8.sprite", RESOURCE_TEXTURE, NULL);
#else
	wheel_mask     = sprite_load("rom:/sprites/core/CircleMask.i8.sprite");
	wheel_progress = sprite_load("rom:/sprites/core/CircleProgress.i8.sprite");
#endif
	assert(wheel_mask && wheel_progress);
}

/* The scene dies with the gameplay state: a slot still holding a character
   would read freed memory, and a particle left visible would draw with its
   sprites already freed. */
void stamina_wheel_unload(void)
{
	for (int i = 0; i < STAMINA_WHEEL_COUNT; i++) {
		wheel_slot[i].character         = NULL;
		wheel_slot[i].alpha             = 0.0f;
		wheel_slot[i].particle->visible = false;
	}

#ifdef ENGINE_ULTRA64
	resource_unload(wheel_mask);
	resource_unload(wheel_progress);
#else
	sprite_free(wheel_mask);
	sprite_free(wheel_progress);
#endif
	wheel_mask     = NULL;
	wheel_progress = NULL;
}

float stamina_wheel_getProgress(void)
{
	const StaminaWheel *strongest = &wheel_slot[0];
	for (int i = 1; i < STAMINA_WHEEL_COUNT; i++)
		if (wheel_slot[i].alpha > strongest->alpha) strongest = &wheel_slot[i];
	return strongest->character ? strongest->progress : 1.0f;
}

void stamina_wheel_init(void)
{
	static void (*const render_state[STAMINA_WHEEL_COUNT])(void) = {
		staminaWheel_setRenderStateA,
		staminaWheel_setRenderStateB,
	};

	for (int i = 0; i < STAMINA_WHEEL_COUNT; i++) {
		Particle wheel = {
			.buffer           = particleBuffer_create(PARTICLE_S8, 2),
			.update           = staminaWheel_setInput,
			.set_render_state = render_state[i],
			.textured         = true,
		};

		/* One particle at the buffer origin; the pair rule leaves B at size 0.
		   Color goes out as prim color, its alpha is a texture offset. tpx
		   only: on volcano64 and ultra64 the buffer is untyped and never
		   drawn. */
#if !defined(ENGINE_VOLCANO64) && !defined(ENGINE_ULTRA64)
		wheel.buffer.s8[0] = (TPXParticleS8){
			.posA   = { 0, 0, 0 },
			.sizeA  = STAMINA_WHEEL_SIZE,
			.colorA = { 80, 200, 90, 0 },
		};
#endif

		wheel_slot[i] = (StaminaWheel){
			.particle     = particles_add(&wheel),
			.bone         = -1,
			.progress     = 1.0f,
			.burst_timer  = STAMINA_WHEEL_BURST_TIME,
			.color        = { wheel_green[0], wheel_green[1], wheel_green[2] },
		};
	}
}
