#include <iostream>
#include <vector>
#include <string>
#include "game.h"
#include "player.h"
#include "randombot.h"
#include "bot.h"

using namespace std;

const int TESTS = 1000;

int main() {
	Player whitePlayer(COL_WHITE);
	Player blackPlayer(COL_BLACK);
	RandomBot whiteRandomBot;
	RandomBot blackRandomBot;
	Bot whiteBot(COL_WHITE);
	Bot blackBot(COL_BLACK);

	int whiteWins = 0, blackWins = 0;

	clock_t begin = clock();
	for (int i = 0; i < 1000; i++) {
		Game game;
		//game.printBoard();
		game.setPlayers(whiteRandomBot, blackBot);
		//game.boardState = st;

		if (game.play(true) == COL_WHITE)
			whiteWins++;
		else
			blackWins++;
	}

	clock_t end = clock();
	cout << "white: " << whiteWins << ", black: " << blackWins << "time elapsed: " << double(end - begin) << endl << endl;

	system("PAUSE");

	return 0;
}