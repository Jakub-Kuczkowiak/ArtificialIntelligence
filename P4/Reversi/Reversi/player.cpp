#include "player.h"
#include <iostream>
#include <string>
#include <list>

Player::Player(Color color) {
	this->color = color;
}

State Player::getMoveState(State& state, int i, int j) {
	Board& b = state.first;
	Color myColor = state.second;
	Color enemyColor = (myColor == COL_WHITE) ? COL_BLACK : COL_WHITE;

	Field myField = (myColor == COL_WHITE) ? Field::WHITE : Field::BLACK;
	Field enemyField = (myColor == COL_WHITE) ? Field::BLACK : Field::WHITE;

	if (b[i][j] != Field::EMPTY) return state; // in case of wrong move, we will return the same one

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

	if (figuresToChange.empty()) return state; // in case of wrong move, we will return the same one;
	figuresToChange.push_back(Position(i, j));

	State newState = state;
	newState.second = enemyColor;

	for (auto it = figuresToChange.begin(); it != figuresToChange.end(); it++) {
		newState.first[it->first][it->second] = myField;
	}

	return newState;
}

State Player::bestMove(State& state)
{
	string move = "";
	do {
		cout << "Specify " << ((color == COL_WHITE) ? "white" : "black") << "'s move: ";
		cin >> move;

		if (move.size() < 2) {
			cout << "Move consists of one letter and one number." << endl;
			move = "";
		}
	} while (move == "");

	return getMoveState(state, state.first.size() - (move[1] - '0'), move[0] - 'A');
}
