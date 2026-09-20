#ifndef GAME_STATES_H
#define GAME_STATES_H

#include ENGINE_HEADER(game, game_states)


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

extern const GameStateDef game_states[STATE_COUNT];


#endif
