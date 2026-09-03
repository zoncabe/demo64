#ifndef CREDITS_UI_H
#define CREDITS_UI_H

#include ENGINE_HEADER(scene2d, scene2d)
#include ENGINE_HEADER(ui, ui_animation)


extern const Scene2DDef credits_scene2d;

/* Played on the way in, and backwards on the way out. */
extern const UIAnimation credits_enter;

/* The roll: the control sets the speed, the update advances the text inside
   its window a whole pixel at a time and stops at both ends. */
#define CREDITS_SCROLL_SPEED 60.0f   /* pixels per second, at full input */

void credits_ui_resetScroll(void);
void credits_ui_setScrollVelocity(float velocity);
void credits_ui_updateScroll(float dt);

#endif
