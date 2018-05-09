#include "game.h"
#include <iostream>
#include <algorithm>

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

void Game::setPlayers(IPlayer* whitePlayer, IPlayer* blackPlayer) {
	this->whitePlayer = whitePlayer;
	this->blackPlayer = blackPlayer;
}

int getFigureStrength(Figure figure) {
	switch (figure)
	{
	case ELEPHANT:
		return 7;
	case LION:
		return 6;
	case TIGER:
		return 5;
	case JAGUAR:
		return 4;
	case WOLF:
		return 3;
	case DOG:
		return 2;
	case CAT:
		return 1;
	case RAT:
		return 0;
	}
}

// ONE SHOULD BE INCREASING ONE DECREASING!
struct figure_sort
{
	inline bool operator() (const pair<Figure, int>& figure1, const pair<Figure, int>& figure2)
	{
		return getFigureStrength(figure1.first) > getFigureStrength(figure2.first);
	}
};

struct distance_sort
{
	inline bool operator() (const pair<Figure, int>& figure1, const pair<Figure, int>& figure2)
	{
		return figure1.second < figure2.second;
	}
};

bool Game::isWinner(const State& state, Color& winner) {
	if (state.color == WHITE) {
		// then last move was done by black, so we check if black won.
		if (state.blackFigures[7][3] != Figure::EMPTY) {
			winner = BLACK;
			return true;
		}
	}
	else {
		// then last move was done by white, so we check if white won.
		if (state.whiteFigures[0][3] != Figure::EMPTY) {
			winner = WHITE;
			return true;
		}
	}

	if (state.noCaptureMoves == MAX_MOVES_WITHOUT_CAPTURE) {
		// check the strongest figures
		vector<pair<Figure, int>> whiteFigs;
		vector<pair<Figure, int>> blackFigs;
		for (int i = 0; i < 9; i++) {
			for (int j = 0; j < 7; j++) {
				if (state.whiteFigures[i][j] != Figure::EMPTY) {
					int distance = abs(i - 0) + abs(j - 3);
					whiteFigs.push_back(pair<Figure, int>(state.whiteFigures[i][j], distance));
				}
					
				else if (state.blackFigures[i][j] != Figure::EMPTY) {
					int distance = abs(i - 8) + abs(j - 3);
					blackFigs.push_back(pair<Figure, int>(state.blackFigures[i][j], distance));
				}
			}
		}

		// TODO: koniecznie sprawdzic czy malejaco sortuje!
		sort(whiteFigs.begin(), whiteFigs.end(), figure_sort());
		sort(blackFigs.begin(), blackFigs.end(), figure_sort());

		int minSize = min(whiteFigs.size(), blackFigs.size());
		for (int i = 0; i < minSize; i++) {
			if (getFigureStrength(whiteFigs[i].first) > getFigureStrength(blackFigs[i].first)) {
				winner = WHITE;
				return true;
			}
			else if (getFigureStrength(whiteFigs[i].first) < getFigureStrength(blackFigs[i].first)) {
				winner = BLACK;
				return true;
			}
		}

		if (whiteFigs.size() > blackFigs.size()) {
			winner = WHITE;
			return true;
		}
		else if (whiteFigs.size() < blackFigs.size()) {
			winner = BLACK;
			return true;
		}

		// here we are sure that whiteFigs.size() == blackFigs.size()

		// check taxi distance
		// TODO: check if sorting is proper
		sort(whiteFigs.begin(), whiteFigs.end(), distance_sort());
		sort(blackFigs.begin(), blackFigs.end(), distance_sort());

		for (int i = 0; i < whiteFigs.size(); i++) {
			if (whiteFigs[i].second < blackFigs[i].second) {
				winner = WHITE;
				return true;
			}
			else if (whiteFigs[i].second > blackFigs[i].second) {
				winner = BLACK;
				return true;
			}
		}

		// black wins because he is a player moving as second
		winner = BLACK;
		return true;
	}

	return false;
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

	// we should never get here!
	return WHITE;
}

vector<State> Game::getMoves(const State& state) {
	throw "not implemented";
}

void Game::printBoard() {
	for (int i = 0; i < 9; i++) {
		cout << i << " ";
		for (int j = 0; j < 7; j++) {
			if (boardState.whiteFigures[i][j] != Figure::EMPTY) {
				cout << (char)boardState.whiteFigures[i][j];
			}
			else if (boardState.blackFigures[i][j] != Figure::EMPTY) {
				cout << (char)boardState.blackFigures[i][j];
			}
			else {
				cout << (char)Board[i][j];
			}
		}

		cout << endl;
	}
}