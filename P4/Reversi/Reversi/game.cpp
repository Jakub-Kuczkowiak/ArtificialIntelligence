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

void Game::playWhiteMove() {
	vector<pair<State, Move>> states = getAvailableStates(boardState);
	cout << "Moves: ";
	for (auto& s : states) {
		cout << s.second << " ";
	}
	cout << endl;

	string move = "";
	bool bFound = false;
	while (move == "" && !bFound) {
		move = getMoveFromPlayer();
		for (auto& s : states) {
			if (s.second == move) {
				boardState = s.first;
				bFound = true;
				break;
			}
		}
		move = "";

		if (bFound) break;
		cout << "Invalid move for white." << endl;
	}
}

void Game::playBlackMove() {
	vector<pair<State, Move>> states = getAvailableStates(boardState);
	cout << "Moves: ";
	for (auto& s : states) {
		cout << s.second << " ";
	}
	cout << endl;

	string move = "";
	bool bFound = false;
	while (move == "") {
		move = getMoveFromPlayer();
		for (auto& s : states) {
			if (s.second == move) {
				boardState = s.first;
				bFound = true;
				break;
			}
		}
		move = "";

		if (bFound) break;
		cout << "Invalid move for black." << endl;
	}	
}

bool Game::isWinner(Color col) {
	return false;
	//throw "not implemented";
}

vector<pair<State, Move>> Game::getAvailableStates(State state) {
	vector<pair<State, Move>> result;
	Board& b = state.first;
	Color myColor = state.second;
	Color enemyColor = (myColor == Field::WHITE) ? COL_BLACK : COL_WHITE;

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
			temp.clear();

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

void Game::printBoard() {
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