#include "montecarlobot.h"

State MonteCarloBot::bestMove(const State& state) {
	vector<State> states = getMoves(state);

	int maxGamesWon = -1, maxStateIndex = -1;
	for (int i = 0; i < states.size(); i++) { // for each available state play some random games number
		int gamesWon = simulateGame(states[i]);
		if (gamesWon > maxGamesWon) {
			maxGamesWon = gamesWon;
			maxStateIndex = i;
		}
	}

	return states[maxStateIndex];
}

int MonteCarloBot::simulateGame(State state) {
	Color myColor = (state.color == Color::BLACK ? WHITE : BLACK);
	Color enemyColor = state.color;
	State startState = state;
	int gamesWon = 0;

	for (int i = 0; i < nodeGames; i++) {
		state = startState;

		while (true) {
			// my move
			vector<State> states = getMoves(state);
			int randomNum = uniform_int_distribution<>(0, states.size() - 1)(eng);
			state = states[randomNum];
			Color col;
			if (isWinner(state, col)) {
				break;
			}

			states = getMoves(state);
			randomNum = uniform_int_distribution<>(0, states.size() - 1)(eng);
			state = states[randomNum];
			if (isWinner(state, col)) {
				gamesWon++;
				break;
			}
		}
	}

	return gamesWon;
}