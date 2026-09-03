#ifndef MAIN_MENU_UI_H
#define MAIN_MENU_UI_H

#include ENGINE_HEADER(scene2d, scene2d)
#include ENGINE_HEADER(ui, ui_animation)


extern const Scene2DDef main_menu_scene2d;

/* Applied every frame: follows the cursor. */
extern const UIAnimation main_menu_idle;

/* Played on the way in, and backwards on the way out. */
extern const UIAnimation main_menu_enter;

#endif
