#include "player.h"
#include <iostream>
#include <string>

Player::Player(Color color) {
	this->color = color;
}

State Player::bestMove(const State& state) {
	auto states = getMoves(state);
	cout << "Available moves: " << endl;
	for (auto& st : states) {
		cout << st.lastMove << endl;
	}

	State nextState;
	string move = "";
	do {
		cout << "Specify " << ((color == WHITE) ? "white" : "black") << "'s move: ";
		cin >> move;

		if (move.size() < 4) {
			cout << "Move consists of one letter and one number followed by one letter and one number." << endl;
			move = "";
		}

		bool bProperMove = false;
		for (auto& st : states) {
			if (st.lastMove == move) {
				nextState = st;
				bProperMove = true;
				break;
			}
		}

		if (!bProperMove) {
			move = "";
			cout << "specify move from the list.";
		}
	} while (move == "");

	return nextState;
}