#ifndef MAIN_MENU_UI_H
#define MAIN_MENU_UI_H

#include "scene2d/e64_scene2d.h"
#include "ui/e64_ui_animation.h"


extern const Scene2DDef main_menu_scene2d;

/* Applied every frame: follows the cursor. */
extern const UIAnimation main_menu_idle;

/* Played on the way in, and backwards on the way out. */
extern const UIAnimation main_menu_enter;

#endif
