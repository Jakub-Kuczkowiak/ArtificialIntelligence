#pragma once

#include "game.h"

class Player : public IPlayer
{
public:
	Player(Color color);
	State bestMove(const State& state);

private:
	Color color;
};