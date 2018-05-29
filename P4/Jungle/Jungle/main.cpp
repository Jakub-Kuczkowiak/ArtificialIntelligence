#include <iostream>
#include "game.h"
#include "player.h"
#include "randombot.h"
#include "bot.h"

const int TESTS = 1;

int main() {
	Player whitePlayer(WHITE);
	Player blackPlayer(BLACK);
	RandomBot whiteRandomBot;
	RandomBot blackRandomBot;
	Bot whiteBot;
	Bot blackBot;

	int whiteWins = 0, blackWins = 0;
	for (int i = 0; i < TESTS; i++) {
		Game game;

		game.setPlayers(whitePlayer, blackRandomBot);
		game.printBoard();
		if (game.play(true, true) == WHITE) {
			whiteWins++;
		}
		else {
			blackWins++;
		}
		game.printBoard();
	}

	system("PAUSE");
	return 0;
}