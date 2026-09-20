#ifdef ENGINE_ULTRA64
/* The ROM's asset table, written by the build from the Makefile's list. */
#include ENGINE_HEADER(resource, resource)
#include "assets.h"
#else
#include <libdragon.h>
#endif

#include ENGINE_HEADER(game, game)
#include ENGINE_HEADER(debug, debug)
#include "assets/graphics/fonts.h"
#include ENGINE_HEADER(sound, sound)
#include "ui/stamina_wheel.h"
#include "game/game_states.h"


int main()
{
#ifndef ENGINE_ULTRA64
    debug_init_isviewer();
    debug_init_usblog();
#endif

    game_init();
#ifdef ENGINE_ULTRA64
    resource_init(asset_table, asset_count);
#endif
    debugUI_init();

    font_init(demo_fonts, FONT_COUNT);
    sound_init();

    stamina_wheel_init();

    game_start(game_states, STATE_COUNT, GAMEPLAY3D);

    for (;;) game_runStep();

    game_close();

    return 0;
}
