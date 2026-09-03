/*
	A spring arm over the character's shoulder: the arm trails the body, the
	C stick swings it around. What it follows is not written here — the
	control binding names the player, and the camera takes that player's
	body.

	The C stick is read normalized, 0 to 1 at full travel, so the max
	velocity is the turn rate in degrees per second the end of its throw
	asks for.
*/
#include <fmath.h>

#include ENGINE_HEADER(physics/math, math_common)
#include ENGINE_HEADER(control, camera_control)
#include "control/controller.h"
#include "camera/camera.h"


const CameraDef camera_def = {

	.type = CAMERA_TYPE_SPRING_ARM,

	.field_of_view = 60.0f,
	.near_clipping = 100.0f,
	.far_clipping  = 8000.0f,

	.spring_arm = {
		.arm_length   = 200.0f,
		.side_offset  = 50.0f,
		.yaw          = -48.0f,
		.pitch        = 8.0f,
		.pivot_height = 130.0f,

		.settings = {
			.response_rate = {  10.0f,  10.0f },
			.max_velocity  = { 136.8f, 121.6f },
			.direction     = {   1.0f,  -1.0f },

			.zoom_response_rate = 6.0f,

			.max_pitch =  80.0f,
			.min_pitch = -50.0f,

			/* What the aim adds while it is held: the arm out a little, the
			   view narrower, the shoulder further off centre, and the swing
			   at half speed so the shot can be lined up. */
			.aim_arm_length     =  60.0f,
			.aim_field_of_view  = -22.0f,
			.aim_side_offset    =  10.0f,
			.aim_velocity_scale =   0.5f,
		},
	},
};


