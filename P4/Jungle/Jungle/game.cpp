#include "game.h"
#include <iostream>
#include <algorithm>

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
	case EMPTY: // used just to know if we can step into this field.
		return -1;
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

void Game::setPlayers(IPlayer& whitePlayer, IPlayer& blackPlayer) {
	this->whitePlayer = &whitePlayer;
	this->blackPlayer = &blackPlayer;
}

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
	if (whitePlayer == nullptr || blackPlayer == nullptr) {
		throw "There are no players!";
	}

	while (true)
	{
		vector<State> states = getMoves(boardState);
		State newState;

		if (boardState.color == WHITE) {
			newState = whitePlayer->bestMove(boardState);
		}
		else {
			newState = blackPlayer->bestMove(boardState);
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


vector<State> getMoves(const State& state) {
	vector<State> result;

	Field myDen = (state.color == WHITE) ? WHITEDEN : BLACKDEN;
	auto& myFigures = (state.color == WHITE) ? state.whiteFigures : state.blackFigures;
	auto& enemyFigures = (state.color == WHITE) ? state.blackFigures : state.whiteFigures;

	for (int i = 0; i < 9; i++) {
		for (int j = 0; j < 7; j++) {
			vector< pair<int, int> > indices = { { i - 1, j },{ i + 1, j },{ i, j - 1 },{ i, j + 1 } };

			switch (myFigures[i][j])
			{
			case RAT:
				for (auto index : indices) {
					int newI = index.first, newJ = index.second;
					if (newI < 0 || newI > 8 || newJ < 0 || newJ > 6) continue;

					// we cannot capture our own figure
					if (myFigures[newI][newJ] != Figure::EMPTY) continue;

					if (Board[newI][newJ] != myDen) { // we check if it is our DEN
						if (Board[newI][newJ] == Field::TRAP ||
							getFigureStrength(myFigures[i][j]) >= getFigureStrength(enemyFigures[newI][newJ]) ||
							enemyFigures[newI][newJ] == Figure::ELEPHANT) { // here we know that we can capture this field

							// rat cannot capture from water to land
							if (Board[i][j] == Field::RIVER && Board[newI][newJ] == Field::LAND) {
								if (enemyFigures[newI][newJ] != Figure::EMPTY) continue;
							}

							State newState = state;
							newState.color = (state.color == WHITE ? BLACK : WHITE);
							if (enemyFigures[newI][newJ] != Figure::EMPTY) newState.noCaptureMoves = 0;
							else newState.noCaptureMoves++;

							if (state.color == WHITE) {
								newState.whiteFigures[i][j] = Figure::EMPTY;
								newState.whiteFigures[newI][newJ] = Figure::RAT;
								newState.blackFigures[newI][newJ] = Figure::EMPTY;
							}
							else {
								newState.blackFigures[i][j] = Figure::EMPTY;
								newState.blackFigures[newI][newJ] = Figure::RAT;
								newState.whiteFigures[newI][newJ] = Figure::EMPTY;
							}

							char move[6];
							sprintf_s(move, 6, "%c%d%c%d", j + 'A', 9 - i, newJ + 'A', 9 - newI);
							string sMove = move;
							newState.lastMove = move;

							result.push_back(newState);
						}
					}
				}
				break;
			case CAT:
				for (auto index : indices) {
					int newI = index.first, newJ = index.second;
					if (newI < 0 || newI > 8 || newJ < 0 || newJ > 6) continue;

					// we cannot capture our own figure
					if (myFigures[newI][newJ] != Figure::EMPTY) continue;

					if (Board[newI][newJ] != myDen && Board[newI][newJ] != Field::RIVER) { // we check if it is our DEN or WATER
						if (Board[newI][newJ] == Field::TRAP ||
							getFigureStrength(myFigures[i][j]) >= getFigureStrength(enemyFigures[newI][newJ])) { // here we know that we can capture this field

							State newState = state;
							newState.color = (state.color == WHITE ? BLACK : WHITE);
							if (enemyFigures[newI][newJ] != Figure::EMPTY) newState.noCaptureMoves = 0;
							else newState.noCaptureMoves++;

							if (state.color == WHITE) {
								newState.whiteFigures[i][j] = Figure::EMPTY;
								newState.whiteFigures[newI][newJ] = Figure::CAT;
								newState.blackFigures[newI][newJ] = Figure::EMPTY;
							}
							else {
								newState.blackFigures[i][j] = Figure::EMPTY;
								newState.blackFigures[newI][newJ] = Figure::CAT;
								newState.whiteFigures[newI][newJ] = Figure::EMPTY;
							}

							char move[6];
							sprintf_s(move, 6, "%c%d%c%d", j + 'A', 9 - i, newJ + 'A', 9 - newI);
							string sMove = move;
							newState.lastMove = move;

							result.push_back(newState);
						}
					}
				}
				break;
			case DOG:
				for (auto index : indices) {
					int newI = index.first, newJ = index.second;
					if (newI < 0 || newI > 8 || newJ < 0 || newJ > 6) continue;

					// we cannot capture our own figure
					if (myFigures[newI][newJ] != Figure::EMPTY) continue;

					if (Board[newI][newJ] != myDen && Board[newI][newJ] != Field::RIVER) { // we check if it is our DEN or WATER
						if (Board[newI][newJ] == Field::TRAP ||
							getFigureStrength(myFigures[i][j]) >= getFigureStrength(enemyFigures[newI][newJ])) { // here we know that we can capture this field

							State newState = state;
							newState.color = (state.color == WHITE ? BLACK : WHITE);
							if (enemyFigures[newI][newJ] != Figure::EMPTY) newState.noCaptureMoves = 0;
							else newState.noCaptureMoves++;

							if (state.color == WHITE) {
								newState.whiteFigures[i][j] = Figure::EMPTY;
								newState.whiteFigures[newI][newJ] = Figure::DOG;
								newState.blackFigures[newI][newJ] = Figure::EMPTY;
							}
							else {
								newState.blackFigures[i][j] = Figure::EMPTY;
								newState.blackFigures[newI][newJ] = Figure::DOG;
								newState.whiteFigures[newI][newJ] = Figure::EMPTY;
							}

							char move[6];
							sprintf_s(move, 6, "%c%d%c%d", j + 'A', 9 - i, newJ + 'A', 9 - newI);
							string sMove = move;
							newState.lastMove = move;

							result.push_back(newState);
						}
					}
				}
				break;
			case WOLF:
				for (auto index : indices) {
					int newI = index.first, newJ = index.second;
					if (newI < 0 || newI > 8 || newJ < 0 || newJ > 6) continue;

					// we cannot capture our own figure
					if (myFigures[newI][newJ] != Figure::EMPTY) continue;

					if (Board[newI][newJ] != myDen && Board[newI][newJ] != Field::RIVER) { // we check if it is our DEN or WATER
						if (Board[newI][newJ] == Field::TRAP ||
							getFigureStrength(myFigures[i][j]) >= getFigureStrength(enemyFigures[newI][newJ])) { // here we know that we can capture this field

							State newState = state;
							newState.color = (state.color == WHITE ? BLACK : WHITE);
							if (enemyFigures[newI][newJ] != Figure::EMPTY) newState.noCaptureMoves = 0;
							else newState.noCaptureMoves++;

							if (state.color == WHITE) {
								newState.whiteFigures[i][j] = Figure::EMPTY;
								newState.whiteFigures[newI][newJ] = Figure::WOLF;
								newState.blackFigures[newI][newJ] = Figure::EMPTY;
							}
							else {
								newState.blackFigures[i][j] = Figure::EMPTY;
								newState.blackFigures[newI][newJ] = Figure::WOLF;
								newState.whiteFigures[newI][newJ] = Figure::EMPTY;
							}

							char move[6];
							sprintf_s(move, 6, "%c%d%c%d", j + 'A', 9 - i, newJ + 'A', 9 - newI);
							string sMove = move;
							newState.lastMove = move;

							result.push_back(newState);
						}
					}
				}
				break;
			case JAGUAR:
				for (auto index : indices) {
					int newI = index.first, newJ = index.second;
					if (newI < 0 || newI > 8 || newJ < 0 || newJ > 6) continue;

					// we cannot capture our own figure
					if (myFigures[newI][newJ] != Figure::EMPTY) continue;

					if (Board[newI][newJ] != myDen && Board[newI][newJ] != Field::RIVER) { // we check if it is our DEN or WATER
						if (Board[newI][newJ] == Field::TRAP ||
							getFigureStrength(myFigures[i][j]) >= getFigureStrength(enemyFigures[newI][newJ])) { // here we know that we can capture this field

							State newState = state;
							newState.color = (state.color == WHITE ? BLACK : WHITE);
							if (enemyFigures[newI][newJ] != Figure::EMPTY) newState.noCaptureMoves = 0;
							else newState.noCaptureMoves++;

							if (state.color == WHITE) {
								newState.whiteFigures[i][j] = Figure::EMPTY;
								newState.whiteFigures[newI][newJ] = Figure::JAGUAR;
								newState.blackFigures[newI][newJ] = Figure::EMPTY;
							}
							else {
								newState.blackFigures[i][j] = Figure::EMPTY;
								newState.blackFigures[newI][newJ] = Figure::JAGUAR;
								newState.whiteFigures[newI][newJ] = Figure::EMPTY;
							}

							char move[6];
							sprintf_s(move, 6, "%c%d%c%d", j + 'A', 9 - i, newJ + 'A', 9 - newI);
							string sMove = move;
							newState.lastMove = move;

							result.push_back(newState);
						}
					}
				}
				break;
			case TIGER:
				for (auto index : indices) {
					int newI = index.first, newJ = index.second;
					if (newI < 0 || newI > 8 || newJ < 0 || newJ > 6) continue;

					if (Board[newI][newJ] != myDen) { // we check if it is our DEN
						if (Board[newI][newJ] == Field::RIVER) { // we handle jumping over river if there was no rat met
							bool bWasRat = false;
							int iDiff = newI - i, jDiff = newJ - j;
							do
							{
								if (enemyFigures[newI][newJ] == Figure::RAT) {
									bWasRat = true;
									break;
								}

								newI += iDiff;
								newJ += jDiff;
							} while (Board[newI][newJ] != Field::RIVER);
						}

						// we cannot capture our own figure
						if (myFigures[newI][newJ] != Figure::EMPTY) continue;

						if (Board[newI][newJ] == Field::TRAP ||
							getFigureStrength(myFigures[i][j]) >= getFigureStrength(enemyFigures[newI][newJ])) { // here we know that we can capture this field

							State newState = state;
							newState.color = (state.color == WHITE ? BLACK : WHITE);
							if (enemyFigures[newI][newJ] != Figure::EMPTY) newState.noCaptureMoves = 0;
							else newState.noCaptureMoves++;

							if (state.color == WHITE) {
								newState.whiteFigures[i][j] = Figure::EMPTY;
								newState.whiteFigures[newI][newJ] = Figure::TIGER;
								newState.blackFigures[newI][newJ] = Figure::EMPTY;
							}
							else {
								newState.blackFigures[i][j] = Figure::EMPTY;
								newState.blackFigures[newI][newJ] = Figure::TIGER;
								newState.whiteFigures[newI][newJ] = Figure::EMPTY;
							}

							char move[6];
							sprintf_s(move, 6, "%c%d%c%d", j + 'A', 9 - i, newJ + 'A', 9 - newI);
							string sMove = move;
							newState.lastMove = move;

							result.push_back(newState);
						}
					}
				}
				break;
			case LION:
				for (auto index : indices) {
					int newI = index.first, newJ = index.second;
					if (newI < 0 || newI > 8 || newJ < 0 || newJ > 6) continue;

					if (Board[newI][newJ] != myDen) { // we check if it is our DEN
						if (Board[newI][newJ] == Field::RIVER) { // we handle jumping over river if there was no rat met
							bool bWasRat = false;
							int iDiff = newI - i, jDiff = newJ - j;
							do
							{
								if (enemyFigures[newI][newJ] == Figure::RAT) {
									bWasRat = true;
									break;
								}

								newI += iDiff;
								newJ += jDiff;
							} while (Board[newI][newJ] != Field::RIVER);
						}

						// we cannot capture our own figure
						if (myFigures[newI][newJ] != Figure::EMPTY) continue;

						if (Board[newI][newJ] == Field::TRAP ||
							getFigureStrength(myFigures[i][j]) >= getFigureStrength(enemyFigures[newI][newJ])) { // here we know that we can capture this field

							State newState = state;
							newState.color = (state.color == WHITE ? BLACK : WHITE);
							if (enemyFigures[newI][newJ] != Figure::EMPTY) newState.noCaptureMoves = 0;
							else newState.noCaptureMoves++;

							if (state.color == WHITE) {
								newState.whiteFigures[i][j] = Figure::EMPTY;
								newState.whiteFigures[newI][newJ] = Figure::LION;
								newState.blackFigures[newI][newJ] = Figure::EMPTY;
							}
							else {
								newState.blackFigures[i][j] = Figure::EMPTY;
								newState.blackFigures[newI][newJ] = Figure::LION;
								newState.whiteFigures[newI][newJ] = Figure::EMPTY;
							}

							char move[6];
							sprintf_s(move, 6, "%c%d%c%d", j + 'A', 9 - i, newJ + 'A', 9 - newI);
							string sMove = move;
							newState.lastMove = move;

							result.push_back(newState);
						}
					}
				}
				break;
			case ELEPHANT:
				for (auto index : indices) {
					int newI = index.first, newJ = index.second;
					if (newI < 0 || newI > 8 || newJ < 0 || newJ > 6) continue;

					// we cannot capture our own figure
					if (myFigures[newI][newJ] != Figure::EMPTY) continue;

					if (Board[newI][newJ] != myDen && Board[newI][newJ] != Field::RIVER) { // we check if it is our DEN or WATER
						if ((Board[newI][newJ] == Field::TRAP ||
							getFigureStrength(myFigures[i][j]) >= getFigureStrength(enemyFigures[newI][newJ])) &&
							enemyFigures[i][j] != Figure::RAT) { // here we know that we can capture this field; elephant is weaker than RAT

							State newState = state;
							newState.color = (state.color == WHITE ? BLACK : WHITE);
							if (enemyFigures[newI][newJ] != Figure::EMPTY) newState.noCaptureMoves = 0;
							else newState.noCaptureMoves++;

							if (state.color == WHITE) {
								newState.whiteFigures[i][j] = Figure::EMPTY;
								newState.whiteFigures[newI][newJ] = Figure::ELEPHANT;
								newState.blackFigures[newI][newJ] = Figure::EMPTY;
							}
							else {
								newState.blackFigures[i][j] = Figure::EMPTY;
								newState.blackFigures[newI][newJ] = Figure::ELEPHANT;
								newState.whiteFigures[newI][newJ] = Figure::EMPTY;
							}

							char move[6];
							sprintf_s(move, 6, "%c%d%c%d", j + 'A', 9 - i, newJ + 'A', 9 - newI);
							string sMove = move;
							newState.lastMove = move;

							result.push_back(newState);
						}
					}
				}
				break;

			default:
				break;
			}
		}
	}

	return result;
}

void Game::printBoard() {
	for (int i = 0; i < 9; i++) {
		cout << 9 - i;
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

	cout << " ABCDEFG" << endl;
}