#ifndef MAIN_MENU_UI_H
#define MAIN_MENU_UI_H

#include "ui/e64_ui.h"


extern const e64::ui::Def main_menu_ui;

/* Applied every frame: follows the cursor. */
extern const e64::UIAnimation main_menu_idle;

/* Played on the way in, and backwards on the way out. */
extern const e64::UIAnimation main_menu_enter;

#endif
