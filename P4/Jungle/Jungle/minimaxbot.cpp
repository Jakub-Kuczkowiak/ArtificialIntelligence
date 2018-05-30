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
				heuro += (distance * value);
			}
			else if (enemyFigures[i][j] != Figure::EMPTY) {
				int distance = abs(i - myDenI) + abs(j - myDenJ);
				int value = getFigureStrength(enemyFigures[i][j]);
				heuro -= (distance * value);
			}
		}
	}

	return heuro;
}

State MiniMaxBot::bestMove(const State& state) {
	vector<State> states = getMoves(state);

	// we always want to maximize as heuristic is player-side-oriented
	int maxHeuro = -1, maxStateIndex = -1;
	for (int i = 0; i < states.size(); i++) { // for each available state play some random games number
		int val = heuro(states[i]);
		if (val > maxHeuro) {
			maxHeuro = val;
			maxStateIndex = i;
		}
	}

	return states[maxStateIndex];
}