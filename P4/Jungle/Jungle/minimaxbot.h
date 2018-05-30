#pragma once

#include "game.h"

class MiniMaxBot : public IPlayer
{
public:
	State bestMove(const State& state);
	MiniMaxBot(int maxDepth = 3) : maxDepth{ maxDepth } {}

private:
	int maxDepth;
	int minHeuristic = -10000, maxHeuristic = 10000;
	int heuro(const State& state);
	pair<int, State> minimax(State state, int depth, int maxDepth, int alpha, int beta);
};