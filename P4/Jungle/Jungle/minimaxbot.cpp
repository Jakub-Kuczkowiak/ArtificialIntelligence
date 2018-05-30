#include "minimaxbot.h"

int MiniMaxBot::heuro(const State& state) { // heuristic counted from perspective of player doing move
	Color myColor = (state.color == Color::BLACK ? WHITE : BLACK);
	Color enemyColor = state.color;
	int myDenI = (myColor == WHITE ? 8 : 0), myDenJ = 3;
	int enemyDenI = (enemyColor == WHITE ? 8 : 0), enemyDenJ = 3;
	const vector< vector<Figure> >& myFigures = (myColor == WHITE ? state.whiteFigures : state.blackFigures);
	const vector< vector<Figure> >& enemyFigures = (enemyColor == WHITE ? state.whiteFigures : state.blackFigures);

	int heuro = 0;
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 7; j++) {
			// what is the colour of this figure?
			if (myFigures[i][j] != Figure::EMPTY) {
				int distance = abs(i - enemyDenI) + abs(j - enemyDenJ);
				int value = getFigureStrength(myFigures[i][j]);
				heuro -= (distance * value);
			}
			else if (enemyFigures[i][j] != Figure::EMPTY) {
				int distance = abs(i - myDenI) + abs(j - myDenJ);
				int value = getFigureStrength(enemyFigures[i][j]);
				heuro += (distance * value);
			}
		}
	}

	return heuro;
}

pair<int, State> MiniMaxBot::minimax(State state, int depth, int maxDepth, int alpha, int beta) {
	// base case: leaf, when game is finished
	Color col;
	auto isWin = isWinner(state, col);
	if (isWin) {
		return pair<int, State>(maxHeuristic, state);
	}

	if (depth == maxDepth) { // base case of depth
		return pair<int, State>(heuro(state), state);
	}

	vector<State > states = getMoves(state);
	/*if (states.size() == 0) {
		state.second = (state.second == COL_WHITE ? COL_BLACK : COL_WHITE);
		return minimax(state, depth + 1, maxDepth, alpha, beta);
	}*/

	// we always maximize now
	int max = alpha; // instead of minHeuristic
	State& maxState = states[0];

	for (auto st : states) {
		pair<int, State> result = minimax(st, depth + 1, maxDepth, max, beta);
		if (result.first > max) {
			max = result.first;
			maxState = st;
		}

		if (max >= beta)	// beta cut off
			return pair<int, State>(max, maxState);
	}

	return pair<int, State>(max, maxState);
}

State MiniMaxBot::bestMove(const State& state) {
	vector<State> states = getMoves(state);

	return minimax(state, 0, maxDepth, minHeuristic, maxHeuristic).second;
}