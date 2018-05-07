#pragma once

#include <vector>

using namespace std;

enum Field
{
	EMPTY = '.',
	WHITE = 'W',
	BLACK = 'B'
};

enum Color {
	COL_WHITE,
	COL_BLACK
};

const int WIDTH = 8, HEIGHT = 8;
typedef vector< vector<Field>> Board;
typedef pair<Board, Color> State;
typedef string Move;
typedef pair<int, int> Position;

class Game
{
public:
	Game();
	~Game();

	void printBoard();
	void getBlackMove();
	void getWhiteMove();
	bool isWinner(Color col);
	string getMoveFromPlayer();

	vector<pair<State, Move>> getAvailableStates(State state);

private:
	State boardState;
};
