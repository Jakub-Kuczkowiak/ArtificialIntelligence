#pragma once

#include "game.h"

class Bot : public IPlayer
{
public:
	Bot(Color color);
	State bestMove(State& state);
	vector<pair<State, Move>> getAvailableStates(State& state);
	pair<int, State> minimax(State state, int depth, int maxDepth, int alpha, int beta);
	

private:
	Color color;
	int minHeuristic;
	int maxHeuristic;

	int heuro(const State& state);
	int cornersDifference(const State& state);
	int countWeights(const State& state);
	pair<bool, Color> isWinner(State& boardState, Color colorToMove);
};