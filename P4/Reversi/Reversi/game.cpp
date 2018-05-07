#include "game.h"
#include <vector>
#include <iostream>
#include <string>
#include <list>
#include <algorithm>

using namespace std;

Game::Game()
{
	boardState.first = vector< vector<Field> >(WIDTH);

	for (int i = 0; i < WIDTH; i++) {
		boardState.first[i] = vector<Field>(HEIGHT);
		for (int j = 0; j < HEIGHT; j++) {
			boardState.first[i][j] = EMPTY;
		}
	}

	boardState.first[3][3] = boardState.first[4][4] = Field::WHITE;
	boardState.first[3][4] = boardState.first[4][3] = Field::BLACK;

	boardState.second = COL_BLACK;
}

Game::~Game()
{
}

string Game::getMoveFromPlayer() {
	cout << "Move: ";
	string move;
	cin >> move;

	return move;
}

// (end, Color) = is it end and who won?
pair<bool, Color> Game::isWinner(Color colorToMove) {
	int countWhite = 0, countBlack = 0;
	bool isAnyEmpty = false;
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (boardState.first[i][j] == Field::WHITE)
				countWhite++;
			else if (boardState.first[i][j] == Field::BLACK)
				countBlack++;
		}
	}

	// check if any figures of each
	if (countWhite == 0) return pair<bool, Color>(true, COL_BLACK);
	if (countBlack == 0) return pair<bool, Color>(true, COL_WHITE);

	// check if full board
	if (countWhite + countBlack == WIDTH * HEIGHT) {
		if (countWhite >= countBlack) return pair<bool, Color>(true, COL_WHITE);
		else return pair<bool, Color>(true, COL_BLACK);
	}

	// can anyone move?
	if (getAvailableStates(boardState).size() == 0) {
		boardState.second = boardState.second == COL_WHITE ? COL_BLACK : COL_WHITE;
		if (getAvailableStates(boardState).size() == 0) {
			return pair<bool, Color>(true, (colorToMove == COL_WHITE) ? COL_BLACK : COL_WHITE);
		}
		boardState.second = boardState.second == COL_WHITE ? COL_BLACK : COL_WHITE;
	}

	return pair<bool, Color>(false, COL_WHITE);
}

vector<pair<State, Move>> Game::getAvailableStates(State& state) {
	vector<pair<State, Move>> result;
	Board& b = state.first;
	Color myColor = state.second;
	Color enemyColor = (myColor == COL_WHITE) ? COL_BLACK : COL_WHITE;

	Field myField = (myColor == COL_WHITE) ? Field::WHITE : Field::BLACK;
	Field enemyField = (myColor == COL_WHITE) ? Field::BLACK : Field::WHITE;

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (b[i][j] != Field::EMPTY) continue;

			// we are to check all 9 lines \ | / - - / | \/
			list<Position> figuresToChange;

			list<Position> temp = list<Position>();

			// TopDiagonalLeft
			int newI = i - 1, newJ = j - 1;
			while (newI >= 0 && newJ >= 0) {
				if (b[newI][newJ] == enemyField)
					temp.push_back(Position(newI, newJ));
				else if (b[newI][newJ] == myField) // to usuwamy wszystkie pionki
					figuresToChange.splice(figuresToChange.end(), temp);
				else // we break here since its empty field
					break;

				newI--; newJ--;
			}
			temp.clear();

			newI = i + 1; newJ = j + 1;
			while (newI < WIDTH && newJ < HEIGHT) {
				if (b[newI][newJ] == enemyField)
					temp.push_back(Position(newI, newJ));
				else if (b[newI][newJ] == myField) // to usuwamy wszystkie pionki
					figuresToChange.splice(figuresToChange.end(), temp);
				else // we break here since its empty field
					break;

				newI++; newJ++;
			}
			temp.clear();

			// TopDiagonalRight
			newI = i - 1; newJ = j + 1;
			while (newI >= 0 && newJ < HEIGHT) {
				if (b[newI][newJ] == enemyField)
					temp.push_back(Position(newI, newJ));
				else if (b[newI][newJ] == myField) // to usuwamy wszystkie pionki
					figuresToChange.splice(figuresToChange.end(), temp);
				else // we break here since its empty field
					break;

				newI--; newJ++;
			}
			temp.clear();

			newI = i + 1; newJ = j - 1;
			while (newI < WIDTH && newJ >= 0) {
				if (b[newI][newJ] == enemyField)
					temp.push_back(Position(newI, newJ));
				else if (b[newI][newJ] == myField) // to usuwamy wszystkie pionki
					figuresToChange.splice(figuresToChange.end(), temp);
				else // we break here since its empty field
					break;

				newI++; newJ--;
			}
			temp.clear();

			// Horizontal
			newI = i - 1; newJ = j;
			while (newI >= 0) {
				if (b[newI][newJ] == enemyField)
					temp.push_back(Position(newI, newJ));
				else if (b[newI][newJ] == myField) // to usuwamy wszystkie pionki
					figuresToChange.splice(figuresToChange.end(), temp);
				else // we break here since its empty field
					break;

				newI--;
			}
			temp.clear();

			newI = i + 1; newJ = j;
			while (newI < WIDTH) {
				if (b[newI][newJ] == enemyField)
					temp.push_back(Position(newI, newJ));
				else if (b[newI][newJ] == myField) // to usuwamy wszystkie pionki
					figuresToChange.splice(figuresToChange.end(), temp);
				else // we break here since its empty field
					break;

				newI++;
			}
			temp.clear();

			// Vertical
			newI = i; newJ = j - 1;
			while (newJ >= 0) {
				if (b[newI][newJ] == enemyField)
					temp.push_back(Position(newI, newJ));
				else if (b[newI][newJ] == myField) // to usuwamy wszystkie pionki
					figuresToChange.splice(figuresToChange.end(), temp);
				else // we break here since its empty field
					break;

				newJ--;
			}
			temp.clear();

			newI = i; newJ = j + 1;
			while (newJ < HEIGHT) {
				if (b[newI][newJ] == enemyField)
					temp.push_back(Position(newI, newJ));
				else if (b[newI][newJ] == myField) // to usuwamy wszystkie pionki
					figuresToChange.splice(figuresToChange.end(), temp);
				else // we break here since its empty field
					break;

				newJ++;
			}

			if (figuresToChange.empty()) continue;
			figuresToChange.push_back(Position(i, j));

			State newState = state;
			newState.second = enemyColor;

			for (auto it = figuresToChange.begin(); it != figuresToChange.end(); it++) {
				newState.first[it->first][it->second] = myField;
			}

			char move[4];
			sprintf_s(move, 4, "%c%d", j + 'A', WIDTH - i);
			string sMove = move;
			result.push_back(pair<State, Move>(newState, sMove));
		}
	}

	return result;
}

void Game::setPlayers(IPlayer& whitePlayer, IPlayer& blackPlayer) {
	this->whitePlayer = &whitePlayer;
	this->blackPlayer = &blackPlayer;
}

bool Game::isCorrectState(State& state) {
	auto states = getAvailableStates(boardState);
	for (auto& st : states)
		if (st.first == state)
			return true;

	return false;
}

void Game::play(bool verifyMoves) {
	while (true)
	{
		State state;
		if (getAvailableStates(boardState).size() > 0) {
			do
			{
				state = blackPlayer->bestMove(boardState);
				if (!verifyMoves) break;
				if (isCorrectState(state)) break;
				cout << "Black played incorrect move!" << endl;
			} while (true);

			boardState = state;
			printBoard();

			auto endState = isWinner(COL_WHITE);
			if (endState.first) {
				cout << "***" << ((endState.second == COL_WHITE) ? "WHITE" : "BLACK") << " WINNER***" << endl;
				break;
			}
		}
		else {
			boardState.second = COL_WHITE;
		}

		if (getAvailableStates(boardState).size() > 0) {
			do
			{
				state = whitePlayer->bestMove(boardState);
				if (!verifyMoves) break;
				if (isCorrectState(state)) break;
				cout << "White played incorrect move!" << endl;
			} while (true);

			boardState = state;
			printBoard();

			auto endState = isWinner(COL_BLACK);
			if (endState.first) {
				cout << "***" << ((endState.second == COL_WHITE) ? "WHITE" : "BLACK") << " WINNER***" << endl;
				break;
			}
		}
		else {
			boardState.second = COL_BLACK;
		}
	}
}

void Game::printBoard() {
	cout << endl;

	for (int i = 0; i < WIDTH; i++) {
		cout << WIDTH - i;
		for (int j = 0; j < HEIGHT; j++) {
			cout << (char)boardState.first[i][j];
		}

		cout << endl;
	}

	char begin = 'A';
	cout << " ";
	for (int i = 0; i < HEIGHT; i++) {
		cout << (char)(begin + i);
	}

	cout << endl;
}