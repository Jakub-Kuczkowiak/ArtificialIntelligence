#include <iostream>
#include "game.h"

const int TESTS = 1000;

int main() {
	IPlayer* whitePlayer = nullptr;
	IPlayer* blackPlayer = nullptr;

	int whiteWins = 0, blackWins = 0;
	for (int i = 0; i < TESTS; i++) {
		Game game;

		if (game.play(true, true) == WHITE) {
			whiteWins++;
		}
		else {
			blackWins++;
		}
	}

	// TODO: CHECK IF SORTING IS PROPER, THAT IS DECREASING!

	system("PAUSE");
	return 0;
}