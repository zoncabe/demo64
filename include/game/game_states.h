#ifndef GAME_STATES_H
#define GAME_STATES_H

#include "game/e64_game_states.h"


enum {

	DEMO_STATE_INTRO,
	DEMO_STATE_MAIN_MENU,
	DEMO_STATE_CREDITS,
	DEMO_STATE_GAMEPLAY,
	DEMO_STATE_PAUSE,
	DEMO_STATE_GAME_OVER,
	DEMO_STATE_COUNT,

};

extern const GameStateDef demo_states[DEMO_STATE_COUNT];


#endif
