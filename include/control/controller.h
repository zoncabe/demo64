#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "controller/e64_controls.h"

extern const e64::menu::ControlBinding menu_binding;
extern const e64::character3d::ControlBinding character3d_binding;
extern const e64::camera3d::ControlBinding camera_binding;

/* What the 3D gameplay declares: the camera and the body. */
extern const e64::controls::Def controls;

/* What each 2D screen loads with: the same buttons on that screen's body. */
extern const e64::controls::Def controls_pixel_a;
extern const e64::controls::Def controls_pixel_b;
extern const e64::controls::Def controls_industry;
extern const e64::controls::Def controls_line;

#endif
