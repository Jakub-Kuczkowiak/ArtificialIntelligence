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

class IPlayer
{
public:
	virtual ~IPlayer() { }
	virtual State bestMove(State& state) = 0;
};

class Game
{
public:
	Game();
	~Game();

	void setPlayers(IPlayer& whitePlayer, IPlayer& blackPlayer);
	void printBoard();
	pair<bool, Color> isWinner(Color colorToMove);
	string getMoveFromPlayer();
	bool isCorrectState(State& state);

	vector<pair<State, Move>> getAvailableStates(State& state);

	Color play(bool verifyMoves);

private:
	State boardState;
	IPlayer* whitePlayer;
	IPlayer* blackPlayer;
};
