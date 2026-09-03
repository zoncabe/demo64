#ifndef PAUSE_UI_H
#define PAUSE_UI_H

#include ENGINE_HEADER(scene2d, scene2d)
#include ENGINE_HEADER(ui, ui_animation)


extern const Scene2DDef pause_scene2d;

/* Applied every frame: follows the cursor. */
extern const UIAnimation pause_idle;

/* Played on the way in, and backwards on the way back to the game. */
extern const UIAnimation pause_enter;

/* Leaving for good: the panel slides out while the screen goes black. */
extern const UIAnimation pause_quit;

#endif
