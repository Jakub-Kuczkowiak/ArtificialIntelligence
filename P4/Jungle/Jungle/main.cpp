#include <iostream>
#include "game.h"

const int TESTS = 1000;

int main() {
	int whiteWins = 0, blackWins = 0;
	for (int i = 0; i < TESTS; i++) {
		Game game;

		if (game.play() == WHITE) {
			whiteWins++;
		}
		else {
			blackWins++;
		}
	}

	system("PAUSE");
	return 0;
}