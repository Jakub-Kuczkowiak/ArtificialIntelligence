#pragma once

#include "game.h"

class Bot : public IPlayer
{
public:
	Bot(Color color);
	State bestMove(State& state);
	vector<pair<State, Move>> getAvailableStates(State& state);
	pair<int, State> minimax(State state, int depth, int maxDepth, int alpha, int beta);
	int heuro(const State& state);

private:
	Color color;
	int minHeuristic;
	int maxHeuristic;

	pair<bool, Color> isWinner(State& boardState, Color colorToMove);
};