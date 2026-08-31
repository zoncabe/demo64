#include <libdragon.h>

#include "game/e64_game.h"
#include "assets/graphics/sprites.h"
#include "assets/graphics/fonts.h"
#include "assets/sound/sound.h"
#include "ui/stamina_wheel.h"
#include "game/game_states.h"


int main()
{
    debug_init_isviewer();
    debug_init_usblog();

    game_init();

    sprite_init(demo_sprite_paths, SPRITE_COUNT);
    font_init(demo_fonts, FONT_COUNT);
    sound_init(demo_sound_bank, SOUND_COUNT);

    stamina_wheel_init();

    game_start(demo_states, DEMO_STATE_COUNT, DEMO_STATE_MAIN_MENU);

    for (;;) game_runStep();

    game_close();

    return 0;
}
