/*
	The 2D levels: one scene per Kenney sample screen, each with its stage at
	the origin and its body standing on a floor near the left edge. The
	camera's right limit is the screen's width, which is also where the
	gameplay state reads the edge to open the next one.

	Every scene is written out: a screen is where its enemies, its props and
	its pickups go, and each one gets its own set of them.
*/
#include "assets/characters/pixel_green.h"
#include "assets/characters/pixel_yellow.h"
#include "assets/characters/line_blue.h"
#include "scene/scene2d.h"


/* Pixel perfect: one world pixel is one screen pixel. */
#define CAMERA_ZOOM 1.0f

/* The backdrop is a map of its own, 24 px sky tiles on a slow parallax,
   larger than the screen and placed up and to the left so it still covers
   the view at the camera's limits.

   Where the corner goes comes out of the parallax: the camera of these
   screens runs x 160 to 308 and y 120 to 150, so at 0.15 the backdrop
   slides 24 to 46 px across and 18 to 22 down. For its left and top edges
   to stay off the screen at both ends of that run, the corner has to sit
   at x -136 or less and y -102 or less. These are the next multiples of
   the 24 px cell, so the grid lands on whole pixels. */
#define BACKDROP_X -144.0f
#define BACKDROP_Y -120.0f

/* Where the body stands when it walks into a screen from the left, and
   where it lands coming back in from the right. Each is the centre of a
   column whose floor has the body's height of air above it, and the top
   edge of that floor. */
#define PIXEL_A_START { 45.0f, 144.0f }
#define PIXEL_A_RETURN { 441.0f, 144.0f }

#define PIXEL_B_START { 45.0f, 162.0f }
#define PIXEL_B_RETURN { 441.0f, 144.0f }

#define INDUSTRY_START { 45.0f, 126.0f }
#define INDUSTRY_RETURN { 441.0f, 126.0f }

#define LINE_START { 40.0f, 144.0f }
#define LINE_RETURN { 408.0f, 128.0f }

/* The follow the four screens share: the body moves inside a fifth of the
   view before the camera answers, and the view eases after it. */
#define FOLLOW { \
	.drag_margin = { [e64::camera2d::CAMERA2D_SIDE_LEFT] = 0.2f, [e64::camera2d::CAMERA2D_SIDE_TOP] = 0.2f, [e64::camera2d::CAMERA2D_SIDE_RIGHT] = 0.2f, [e64::camera2d::CAMERA2D_SIDE_BOTTOM] = 0.2f }, \
	.drag_horizontal = true, \
	.drag_vertical = true, \
	.position_smoothing_speed = 6.0f, \
}


/* --- pixel a ------------------------------------------------------------ */

static const e64::stage2d::Def pixel_a_backdrop_file = { "rom:/stages/kenney_pixel-platformer/background.stage2d" };
static const e64::stage2d::Def pixel_a_level_file = { "rom:/stages/kenney_pixel-platformer/samplea.stage2d" };

static const e64::Prefab2D pixel_a_backdrop = { .type = e64::prefab2d::PREFAB2D_STAGE, .stage =&pixel_a_backdrop_file };
static const e64::Prefab2D pixel_a_level = { .type = e64::prefab2d::PREFAB2D_STAGE, .stage =&pixel_a_level_file };

/* The body collides with the last stage placed, so the level goes after the
   backdrop. The placements are read by the control bindings, which name the
   body's row. */
const e64::scene2d::Entity pixel_a_placed[] = {

	{ &pixel_a_backdrop, { BACKDROP_X, BACKDROP_Y } },
	{ &pixel_a_level, { 0.0f, 0.0f } },
	{ &pixel_green, PIXEL_A_START },
};

static const e64::scene2d::Layer pixel_a_layer[] = {
	{ pixel_a_placed, sizeof(pixel_a_placed) / sizeof(pixel_a_placed[0]) },
};

/* 26 cells of 18 px. The sky is the flat colour behind the pack's sample. */
static const e64::camera2d::Def pixel_a_camera = {

	.type = e64::camera2d::CAMERA2D_TYPE_FOLLOW,
	.position = PIXEL_A_START,
	.anchor = e64::camera2d::CAMERA2D_ANCHOR_CENTER,
	.zoom = CAMERA_ZOOM,

	.limit_enabled = true,
	.limit = { [e64::camera2d::CAMERA2D_SIDE_LEFT] = 0.0f, [e64::camera2d::CAMERA2D_SIDE_TOP] = 0.0f, [e64::camera2d::CAMERA2D_SIDE_RIGHT] = 468.0f, [e64::camera2d::CAMERA2D_SIDE_BOTTOM] = 270.0f },

	.follow = FOLLOW,
};

const e64::scene2d::Def scene2d_pixel_a = {

	.camera = &pixel_a_camera,
	.layer = pixel_a_layer,
	.layer_count = 1,
	.background = RGBA32(223, 246, 245, 255),
};

const e64::Vector2 pixel_a_return = PIXEL_A_RETURN;


/* --- pixel b ------------------------------------------------------------ */

static const e64::stage2d::Def pixel_b_backdrop_file = { "rom:/stages/kenney_pixel-platformer/background.stage2d" };
static const e64::stage2d::Def pixel_b_level_file = { "rom:/stages/kenney_pixel-platformer/sampleb.stage2d" };

static const e64::Prefab2D pixel_b_backdrop = { .type = e64::prefab2d::PREFAB2D_STAGE, .stage =&pixel_b_backdrop_file };
static const e64::Prefab2D pixel_b_level = { .type = e64::prefab2d::PREFAB2D_STAGE, .stage =&pixel_b_level_file };

const e64::scene2d::Entity pixel_b_placed[] = {

	{ &pixel_b_backdrop, { BACKDROP_X, BACKDROP_Y } },
	{ &pixel_b_level, { 0.0f, 0.0f } },
	{ &pixel_green, PIXEL_B_START },
};

static const e64::scene2d::Layer pixel_b_layer[] = {
	{ pixel_b_placed, sizeof(pixel_b_placed) / sizeof(pixel_b_placed[0]) },
};

static const e64::camera2d::Def pixel_b_camera = {

	.type = e64::camera2d::CAMERA2D_TYPE_FOLLOW,
	.position = PIXEL_B_START,
	.anchor = e64::camera2d::CAMERA2D_ANCHOR_CENTER,
	.zoom = CAMERA_ZOOM,

	.limit_enabled = true,
	.limit = { [e64::camera2d::CAMERA2D_SIDE_LEFT] = 0.0f, [e64::camera2d::CAMERA2D_SIDE_TOP] = 0.0f, [e64::camera2d::CAMERA2D_SIDE_RIGHT] = 468.0f, [e64::camera2d::CAMERA2D_SIDE_BOTTOM] = 270.0f },

	.follow = FOLLOW,
};

const e64::scene2d::Def scene2d_pixel_b = {

	.camera = &pixel_b_camera,
	.layer = pixel_b_layer,
	.layer_count = 1,
	.background = RGBA32(223, 246, 245, 255),
};

const e64::Vector2 pixel_b_return = PIXEL_B_RETURN;


/* --- industry ----------------------------------------------------------- */

static const e64::stage2d::Def industry_backdrop_file = { "rom:/stages/kenney_pixel-platformer-industrial-expansion/background.stage2d" };
static const e64::stage2d::Def industry_level_file = { "rom:/stages/kenney_pixel-platformer-industrial-expansion/sample.stage2d" };

static const e64::Prefab2D industry_backdrop = { .type = e64::prefab2d::PREFAB2D_STAGE, .stage =&industry_backdrop_file };
static const e64::Prefab2D industry_level = { .type = e64::prefab2d::PREFAB2D_STAGE, .stage =&industry_level_file };

const e64::scene2d::Entity industry_placed[] = {

	{ &industry_backdrop, { BACKDROP_X, BACKDROP_Y } },
	{ &industry_level, { 0.0f, 0.0f } },
	{ &pixel_yellow, INDUSTRY_START },
};

static const e64::scene2d::Layer industry_layer[] = {
	{ industry_placed, sizeof(industry_placed) / sizeof(industry_placed[0]) },
};

static const e64::camera2d::Def industry_camera = {

	.type = e64::camera2d::CAMERA2D_TYPE_FOLLOW,
	.position = INDUSTRY_START,
	.anchor = e64::camera2d::CAMERA2D_ANCHOR_CENTER,
	.zoom = CAMERA_ZOOM,

	.limit_enabled = true,
	.limit = { [e64::camera2d::CAMERA2D_SIDE_LEFT] = 0.0f, [e64::camera2d::CAMERA2D_SIDE_TOP] = 0.0f, [e64::camera2d::CAMERA2D_SIDE_RIGHT] = 468.0f, [e64::camera2d::CAMERA2D_SIDE_BOTTOM] = 270.0f },

	.follow = FOLLOW,
};

const e64::scene2d::Def scene2d_industry = {

	.camera = &industry_camera,
	.layer = industry_layer,
	.layer_count = 1,
	.background = RGBA32(217, 163, 151, 255),
};

const e64::Vector2 industry_return = INDUSTRY_RETURN;


/* --- line --------------------------------------------------------------- */

/* This pack's backdrop is drawn out of the level tiles themselves, 16 px
   instead of the 24 px sky of the other packs, so it is placed on its own
   corner: a 480x320 map that still covers the view at both camera limits
   once the 0.15 parallax has moved it. */
#define LINE_BACKDROP_X -144.0f
#define LINE_BACKDROP_Y -112.0f

static const e64::stage2d::Def line_backdrop_file = { "rom:/stages/kenney_pixel-line-platformer/background.stage2d" };
static const e64::stage2d::Def line_level_file = { "rom:/stages/kenney_pixel-line-platformer/sample.stage2d" };

static const e64::Prefab2D line_backdrop = { .type = e64::prefab2d::PREFAB2D_STAGE, .stage =&line_backdrop_file };
static const e64::Prefab2D line_level = { .type = e64::prefab2d::PREFAB2D_STAGE, .stage =&line_level_file };

const e64::scene2d::Entity line_placed[] = {

	{ &line_backdrop, { LINE_BACKDROP_X, LINE_BACKDROP_Y } },
	{ &line_level, { 0.0f, 0.0f } },
	{ &line_blue, LINE_START },
};

static const e64::scene2d::Layer line_layer[] = {
	{ line_placed, sizeof(line_placed) / sizeof(line_placed[0]) },
};

/* 28 cells of 16 px. */
static const e64::camera2d::Def line_camera = {

	.type = e64::camera2d::CAMERA2D_TYPE_FOLLOW,
	.position = LINE_START,
	.anchor = e64::camera2d::CAMERA2D_ANCHOR_CENTER,
	.zoom = CAMERA_ZOOM,

	.limit_enabled = true,
	.limit = { [e64::camera2d::CAMERA2D_SIDE_LEFT] = 0.0f, [e64::camera2d::CAMERA2D_SIDE_TOP] = 0.0f, [e64::camera2d::CAMERA2D_SIDE_RIGHT] = 448.0f, [e64::camera2d::CAMERA2D_SIDE_BOTTOM] = 240.0f },

	.follow = FOLLOW,
};

const e64::scene2d::Def scene2d_line = {

	.camera = &line_camera,
	.layer = line_layer,
	.layer_count = 1,
	.background = RGBA32(252, 223, 205, 255),
};

const e64::Vector2 line_return = LINE_RETURN;
