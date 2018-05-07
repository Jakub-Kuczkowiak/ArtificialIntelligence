#include <iostream>
#include <vector>
#include <string>
#include "game.h"
#include "player.h"

using namespace std;

int main() {
	Player whitePlayer(COL_WHITE);
	Player blackPlayer(COL_BLACK);

	Game game;
	game.printBoard();
	game.setPlayers(whitePlayer, blackPlayer);

	game.play(true);

	system("PAUSE");

	return 0;
}