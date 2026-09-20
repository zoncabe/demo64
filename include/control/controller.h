#ifndef CONTROLLER_H
#define CONTROLLER_H

#include ENGINE_HEADER(control, menu_control)
#include ENGINE_HEADER(control, character3d_control)
#include ENGINE_HEADER(control, character2d_control)
#include ENGINE_HEADER(control, camera_control)
#include ENGINE_HEADER(control, player_control)

extern const menu::ControlBinding         menu_binding;
extern const character3d::ControlBinding  character3d_binding;
extern const character2d::ControlBinding  character2d_binding;
extern const camera::ControlBinding       camera_binding;

/* The four together, which is what a state declares. */
extern const controls::Def demo_controls;

#endif
