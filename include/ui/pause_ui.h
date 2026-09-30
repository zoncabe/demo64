#ifndef PAUSE_UI_H
#define PAUSE_UI_H

#include "ui/e64_ui.h"


extern const e64::ui::Def pause_ui;

/* Applied every frame: follows the cursor. */
extern const e64::UIAnimation pause_idle;

/* Played on the way in, and backwards on the way back to the game. */
extern const e64::UIAnimation pause_enter;

/* Leaving for good: the panel slides out while the screen goes black. */
extern const e64::UIAnimation pause_quit;

#endif
