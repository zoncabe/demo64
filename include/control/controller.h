#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "controller/e64_controls.h"

extern const e64::menu::ControlBinding menu_binding;
extern const e64::character3d::ControlBinding character3d_binding;
extern const e64::character2d::ControlBinding character2d_binding;
extern const e64::camera3d::ControlBinding camera_binding;

/* The four together, which is what a state declares. */
extern const e64::controls::Def controls;

#endif
