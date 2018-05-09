#pragma once

#include <vector>
using namespace std;

enum Field
{
	LAND, // l¹d / ³¹ka
	TRAP, // pu³apka
	DEN, // jama
	RIVER // rzeka
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
	{ LAND, LAND, TRAP, DEN, TRAP, LAND, LAND},
	{ LAND, LAND, LAND, TRAP, LAND, LAND, LAND},
	{ LAND, LAND, LAND, LAND, LAND, LAND, LAND},
	{ LAND, RIVER, RIVER, LAND, RIVER, RIVER, LAND },
	{ LAND, RIVER, RIVER, LAND, RIVER, RIVER, LAND },
	{ LAND, RIVER, RIVER, LAND, RIVER, RIVER, LAND },
	{ LAND, LAND, LAND, LAND, LAND, LAND, LAND },
	{ LAND, LAND, LAND, TRAP, LAND, LAND, LAND },
	{ LAND, LAND, TRAP, DEN, TRAP, LAND, LAND },
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

	State(vector< vector<Figure> > whiteFigures, vector< vector<Figure> > blackFigures, Color color, int noCaptureMoves) {
		this->whiteFigures = whiteFigures;
		this->blackFigures = blackFigures;
		this->color = color;
		this->noCaptureMoves = noCaptureMoves;
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
	virtual State bestMove(const State& state) = 0;
};

class Game
{
public:
	Game();
	Color play(bool verifyMoves, bool printState);
	void printBoard();
	vector<State> getMoves(const State& state);
	bool isWinner(const State& state, Color& winner);

private:
	IPlayer* whitePlayer;
	IPlayer* blackPlayer;

	State boardState;
};