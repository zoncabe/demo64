#ifndef GAME_STATES_H
#define GAME_STATES_H

#include "game/e64_game_states.h"


namespace gameState {

enum {

	INTRO,
	MAIN_MENU,
	CREDITS,
	GAMEPLAY3D,
	GAMEPLAY2D,
	PAUSE,
	GAME_OVER,
	COUNT,

};

extern const e64::Game::State::Def table[COUNT];

}


#endif
