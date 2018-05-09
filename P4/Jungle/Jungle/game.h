#pragma once

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

class State
{
public:
	Figure whiteFigures[9][7];
	Figure blackFigures[9][7];
	Color color;
};

class IPlayer
{
public:
	virtual State bestMove(const State& state);
};

enum Color
{
	WHITE,
	BLACK
};

class Game
{
public:
	Game();
	Color play();

private:
	IPlayer* whitePlayer;
	IPlayer* blackPlayer;

	Figure whiteFigures[9][7] = {
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ ELEPHANT, EMPTY, WOLF, EMPTY, JAGUAR, EMPTY, RAT },
		{ EMPTY, CAT, EMPTY, EMPTY, EMPTY, DOG, EMPTY },
		{ TIGER, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, LION }
	};

	Figure blackFigures[9][7] = {
		{ LION, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, TIGER },
		{ EMPTY, DOG, EMPTY, EMPTY, EMPTY, CAT, EMPTY },
		{ RAT, EMPTY, JAGUAR, EMPTY, WOLF, EMPTY, ELEPHANT },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY },
		{ EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY, EMPTY }
	};
};