#pragma once

#include "game.h"

class Bot : public IPlayer
{
public:
	State bestMove(const State& state);
};