#include <libdragon.h>

#include "game/e64_game.h"
#include "debug/e64_debug.h"
#include "assets/graphics/fonts.h"
#include "sound/e64_sound.h"
#include "ui/stamina_wheel.h"
#include "game/game_states.h"


int main()
{
    debug_init_isviewer();
    debug_init_usblog();

    e64::game::init();
    e64::debug::ui::init();

    e64::font::init(fonts, FONT_COUNT);
    e64::sound::init();

    stamina_wheel_init();

    e64::game::state::start(gameState::table, gameState::COUNT, gameState::GAMEPLAY3D);

    for (;;) e64::game::runStep();

    e64::game::close();

    return 0;
}
