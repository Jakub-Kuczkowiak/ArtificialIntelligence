#include <iostream>
#include <vector>
#include <string>
#include "game.h"
#include "player.h"
#include "randombot.h"

using namespace std;

int main() {
	Player whitePlayer(COL_WHITE);
	Player blackPlayer(COL_BLACK);
	RandomBot whiteBot;
	RandomBot blackBot;

	Game game;
	game.printBoard();
	game.setPlayers(whiteBot, blackBot);

	game.play(true);

	system("PAUSE");

	return 0;
}