#include <iostream>
#include "game.h"
#include "player.h"
#include "randombot.h"
#include "montecarlobot.h"
#include "minimaxbot.h"

const int TESTS = 10;

int main() {
	Player whitePlayer(WHITE);
	Player blackPlayer(BLACK);
	RandomBot whiteRandomBot;
	RandomBot blackRandomBot;
	MonteCarloBot whiteMonteCarloBot(1);
	MonteCarloBot blackMonteCarloBot(5);
	MiniMaxBot whiteMiniMaxBot(3);
	MiniMaxBot blackMiniMaxBot(5);

	int whiteWins = 0, blackWins = 0;
	for (int i = 0; i < TESTS; i++) {
		Game game;

		game.setPlayers(whiteMiniMaxBot, blackMiniMaxBot);
		//game.printBoard();
		if (game.play(true, true) == WHITE) {
			whiteWins++;
		}
		else {
			blackWins++;
		}

		cout << "Result is: WHITE " << whiteWins << ", BLACK " << blackWins << endl;

		//game.printBoard();
	}

	system("PAUSE");
	return 0;
}