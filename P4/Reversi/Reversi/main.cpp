#include <iostream>
#include <vector>
#include <string>
#include "game.h"

using namespace std;



string player() {
	cout << "Move: ";
	string move;
	cin >> move;
	return move;
}

int main() {
	Game game;
	game.printBoard();

	while (true)
	{
		game.playBlackMove();
		game.printBoard();

		if (game.isWinner(COL_BLACK)) {
			cout << "BLACK WINNER";
			break;
		}

		game.playWhiteMove();
		game.printBoard();

		if (game.isWinner(COL_WHITE)) {
			cout << "WHITE WINNER";
			break;
		}
	}

	system("PAUSE");

	return 0;
}