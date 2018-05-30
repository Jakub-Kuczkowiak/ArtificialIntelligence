#pragma once

#include "game.h"

class MiniMaxBot : public IPlayer
{
public:
	State bestMove(const State& state);

private:
	int heuro(const State& state);
};