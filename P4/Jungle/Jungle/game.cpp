#include "game.h"

Game::Game() {
	whitePlayer = blackPlayer = nullptr;
}

Color Game::play()
{
	if (!whitePlayer || !blackPlayer) {
		throw "There are no players!";
	}

	throw "not implemented";
}
