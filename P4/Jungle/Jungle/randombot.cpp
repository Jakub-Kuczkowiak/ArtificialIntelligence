#include "randombot.h"

State RandomBot::bestMove(const State& state) {
	vector<State> states = getMoves(state);
	int randomNum = uniform_int_distribution<>(0, states.size() - 1)(eng);

	return states[randomNum];
}