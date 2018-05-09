#pragma once

#include "game.h"

class Player : public IPlayer
{
public:
	State bestMove(const State& state);
};