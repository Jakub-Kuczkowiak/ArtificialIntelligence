#pragma once

#include <vector>
using namespace std;

enum Field
{
	LAND = '.', // l¹d / ³¹ka
	TRAP = '#', // pu³apka
	WHITEDEN = '*', // jama
	BLACKDEN = '@', // czarna jama
	RIVER = '~' // rzeka
};

enum Figure
{
	EMPTY = '.',
	ELEPHANT = 'E',
	LION = 'L',
	TIGER = 'T',
	JAGUAR = 'J',
	WOLF = 'W',
	DOG = 'D',
	CAT = 'C',
	RAT = 'R'
};

const Field Board[9][7] = {
	{ LAND, LAND, TRAP, BLACKDEN, TRAP, LAND, LAND},
	{ LAND, LAND, LAND, TRAP, LAND, LAND, LAND},
	{ LAND, LAND, LAND, LAND, LAND, LAND, LAND},
	{ LAND, RIVER, RIVER, LAND, RIVER, RIVER, LAND },
	{ LAND, RIVER, RIVER, LAND, RIVER, RIVER, LAND },
	{ LAND, RIVER, RIVER, LAND, RIVER, RIVER, LAND },
	{ LAND, LAND, LAND, LAND, LAND, LAND, LAND },
	{ LAND, LAND, LAND, TRAP, LAND, LAND, LAND },
	{ LAND, LAND, TRAP, WHITEDEN, TRAP, LAND, LAND },
};

enum Color
{
	WHITE,
	BLACK
};

class State
{
public:
	vector< vector<Figure> > whiteFigures;
	vector< vector<Figure> > blackFigures;
	Color color;
	int noCaptureMoves;
	string lastMove;

	State(vector< vector<Figure> > whiteFigures, vector< vector<Figure> > blackFigures, Color color, int noCaptureMoves) {
		this->whiteFigures = whiteFigures;
		this->blackFigures = blackFigures;
		this->color = color;
		this->noCaptureMoves = noCaptureMoves;
		this->lastMove = "";
	}

	State() {

	}

	bool operator==(const State& state) {
		return whiteFigures == state.whiteFigures && blackFigures == state.blackFigures
			&& color == state.color && noCaptureMoves == state.noCaptureMoves;
	}
};

class IPlayer
{
public:
	~IPlayer() { }
	virtual State bestMove(const State& state) = 0;
};

vector<State> getMoves(const State& state);

class Game
{
public:
	Game();
	void setPlayers(IPlayer& whitePlayer, IPlayer& blackPlayer);
	Color play(bool verifyMoves, bool printState);
	void printBoard();
	bool isWinner(const State& state, Color& winner);

private:
	IPlayer* whitePlayer;
	IPlayer* blackPlayer;

	State boardState;

	const int MAX_MOVES_WITHOUT_CAPTURE = 50;
};
