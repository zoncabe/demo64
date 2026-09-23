#ifndef PAUSE_UI_H
#define PAUSE_UI_H

#include "scene2d/e64_scene2d.h"
#include "ui/e64_ui_animation.h"


extern const e64::scene2d::Def pause_scene2d;

/* Applied every frame: follows the cursor. */
extern const e64::UIAnimation pause_idle;

/* Played on the way in, and backwards on the way back to the game. */
extern const e64::UIAnimation pause_enter;

/* Leaving for good: the panel slides out while the screen goes black. */
extern const e64::UIAnimation pause_quit;

#endif
