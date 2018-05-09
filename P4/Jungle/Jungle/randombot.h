#pragma once

#include "game.h"

class RandomBot : public IPlayer
{
public:
	State bestMove(const State& state);
};