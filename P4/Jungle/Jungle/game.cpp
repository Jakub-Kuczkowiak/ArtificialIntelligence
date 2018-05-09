#include "game.h"
#include <iostream>

Game::Game() {
	whitePlayer = blackPlayer = nullptr;

	vector< vector<Figure> > whiteFigures = {
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ ELEPHANT, EMPTY, WOLF, EMPTY, JAGUAR, EMPTY, RAT },
		{ EMPTY, CAT, EMPTY, EMPTY, EMPTY, DOG, EMPTY },
		{ TIGER, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, LION }
	};

	vector< vector<Figure> > blackFigures = {
		{ LION, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TIGER },
		{ EMPTY, DOG, EMPTY, EMPTY, EMPTY, CAT, EMPTY },
		{ RAT, EMPTY, JAGUAR, EMPTY, WOLF, EMPTY, ELEPHANT },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY }
	};

	boardState = State(whiteFigures, blackFigures, WHITE, 0);
}

bool Game::isWinner(const State& state, Color& winner) {
	return true;
}

Color Game::play(bool verifyMoves, bool printState)
{
	if (!whitePlayer || !blackPlayer) {
		throw "There are no players!";
	}

	while (true)
	{
		vector<State> states = getMoves(boardState);
		State newState;

		if (boardState.color == WHITE) {
			State newState = whitePlayer->bestMove(boardState);
		}
		else {
			State newState = blackPlayer->bestMove(boardState);
		}

		if (verifyMoves) {
			bool bFound = false;
			for (auto& st : states) {
				if (st == newState) {
					bFound = true;
				}
			}

			if (!bFound) {
				cout << "INVALID MOVE OF " << (boardState.color == WHITE ? "WHITE" : "BLACK") << "!" << endl;
				continue;
			}
		}

		boardState = newState;
		
		if (printState) printBoard();

		Color colWinner;
		if (isWinner(boardState, colWinner)) {
			cout << "***" << (colWinner == WHITE ? "WHITE" : "BLACK") << "WINNER***" << endl;
			return colWinner;
		}
	}

	throw "not implemented";
	return WHITE;
}

vector<State> Game::getMoves(const State& state) {

}

void Game::printBoard() {
	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 7; j++) {

		}
	}
}