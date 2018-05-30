#pragma once

#include "game.h"
#include <chrono>
#include <random>

class MonteCarloBot : public IPlayer
{
public:
	MonteCarloBot(int nodeGames = 1) : nodeGames{ nodeGames } {}
	State bestMove(const State& state);

private:
	int nodeGames;
	int simulateGame(State state);
	std::mt19937 eng{ chrono::high_resolution_clock::now().time_since_epoch().count() };
};