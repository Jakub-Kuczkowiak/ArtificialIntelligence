#pragma once

#include "game.h"
#include <chrono>
#include <random>

class RandomBot : public IPlayer
{
public:
	State bestMove(State& state);
	vector<pair<State, Move>> getAvailableStates(State& state);

private:
	std::mt19937 eng{ chrono::high_resolution_clock::now().time_since_epoch().count() };
};