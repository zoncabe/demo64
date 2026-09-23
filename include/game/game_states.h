#ifndef GAME_STATES_H
#define GAME_STATES_H

#include "game/e64_game_states.h"


enum {

	INTRO,
	MAIN_MENU,
	CREDITS,
	GAMEPLAY3D,
	GAMEPLAY2D,
	GAMEPLAY3D_PAUSE,
	GAME_OVER,
	STATE_COUNT,

};

extern const e64::Game::State::Def game_states[STATE_COUNT];


#endif
