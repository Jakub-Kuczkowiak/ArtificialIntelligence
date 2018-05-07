#pragma once

#include "game.h"

class Player : public IPlayer
{
public:
	State bestMove(State& state);
	State getMoveState(State& state, int i, int j);
	Player(Color color);

private:
	Color color;
};