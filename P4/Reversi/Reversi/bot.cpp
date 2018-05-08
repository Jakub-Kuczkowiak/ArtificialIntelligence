#include "bot.h"
#include <queue>
#include <list>

int w[8][8] = {
	{200, -100, 100,  50,  50, 100, -100,  200},
	{-100, -200, -50, -50, -50, -50, -200, -100 },
	{100,  -50, 100,   0,   0, 100,  -50,  100},
	{50,  -50,   0,   0,   0,   0,  -50,   50 },
	{50,  -50,   0,   0,   0,   0,  -50,   50},
	{100,  -50, 100,   0,   0, 100,  -50,  100},
	{-100, -200, -50, -50, -50, -50, -200, -100},
	{200, -100, 100,  50,  50, 100, -100,  200} 
};

Bot::Bot(Color color) {
	this->color = color;

	int heuristic = 0;
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			heuristic += max(0, (w[i][j]));
		}
	}

	maxHeuristic = heuristic;
	minHeuristic = -maxHeuristic;
}

State Bot::bestMove(State& state)
{
	return minimax(state, 0, 2, minHeuristic, maxHeuristic).second;
}

// We want it to be always positive for white and negative for black
int Bot::heuro(const State& state) {
	const Board& board = state.first;
	// TODO: check if it's a winning or losing state.

	// whites
	int heuristic = 0;
	int countWhite = 0, countBlack = 0;
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (board[i][j] == Field::WHITE) {
				heuristic += w[i][j];
				countWhite++;
			}
			else if (board[i][j] == Field::BLACK) {
				heuristic -= w[i][j];
				countBlack++;
			}
		}
	}

	return heuristic;
}

pair<bool, Color> Bot::isWinner(State& boardState, Color colorToMove) {
	int countWhite = 0, countBlack = 0;
	bool isAnyEmpty = false;
	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (boardState.first[i][j] == Field::WHITE)
				countWhite++;
			else if (boardState.first[i][j] == Field::BLACK)
				countBlack++;
		}
	}

	// check if any figures of each
	if (countWhite == 0) return pair<bool, Color>(true, COL_BLACK);
	if (countBlack == 0) return pair<bool, Color>(true, COL_WHITE);

	// check if full board
	if (countWhite + countBlack == WIDTH * HEIGHT) {
		if (countWhite >= countBlack) return pair<bool, Color>(true, COL_WHITE);
		else return pair<bool, Color>(true, COL_BLACK);
	}

	// can anyone move?
	if (getAvailableStates(boardState).size() == 0) {
		boardState.second = boardState.second == COL_WHITE ? COL_BLACK : COL_WHITE;
		if (getAvailableStates(boardState).size() == 0) {
			return pair<bool, Color>(true, (colorToMove == COL_WHITE) ? COL_BLACK : COL_WHITE);
		}
		boardState.second = boardState.second == COL_WHITE ? COL_BLACK : COL_WHITE;
	}

	return pair<bool, Color>(false, COL_WHITE);
}

struct minimizing
{
	inline bool operator() (const pair<State, int>& state1, const pair<State, int>& state2)
	{
		return state1.second < state2.second;
	}
};

struct maximizing
{
	inline bool operator() (const pair<State, int>& state1, const pair<State, int>& state2)
	{
		return state1.second > state2.second;
	}
};

// alpha is the current lower bound of the possible returned value;
// beta is the current upper bound of the possible returned value.
pair<int, State> Bot::minimax(State state, int depth, int maxDepth, int alpha, int beta) {
	// base case: leaf, when game is finished
	auto isWin = isWinner(state, state.second);
	if (isWin.first) {
		if (isWin.second == COL_WHITE) {
			state.second = COL_WHITE;
			return pair<int, State>(maxHeuristic, state);
		}
		else {
			state.second = COL_BLACK;
			return pair<int, State>(minHeuristic, state);
		}
	}

	if (depth == maxDepth) { // base case of depth
		return pair<int, State>(heuro(state), state);
	}

	vector< pair<State, Move> > states = getAvailableStates(state);
	vector< pair<State, int> > statesProritySorted = vector< pair<State, int> >(states.size());

	for (int i = 0; i < states.size(); i++) {
		int heuroValue = heuro(states[i].first);
		statesProritySorted[i] = pair<State, int>(states[i].first, heuroValue);
	}

	if (state.second == COL_WHITE)
		sort(statesProritySorted.begin(), statesProritySorted.end(), maximizing());
	else
		sort(statesProritySorted.begin(), statesProritySorted.end(), minimizing());

	if (statesProritySorted.size() == 0) {
		state.second = (state.second == COL_WHITE ? COL_BLACK : COL_WHITE);
		return minimax(state, depth + 1, maxDepth, alpha, beta);
	}
	else {
		if (state.second == COL_WHITE) { // maximizing node
			int max = alpha; // instead of minHeuristic
			State& maxState = states[0].first;

			for (auto st : statesProritySorted) {
				pair<int, State> result = minimax(st.first, depth + 1, maxDepth, max, beta);
				if (result.first > max) {
					max = result.first;
					maxState = st.first;
				}

				if (max >= beta)	// beta cut off
					return pair<int, State>(max, maxState);
			}

			return pair<int, State>(max, maxState);
		}
		else { // minimizing node
			int min = beta;
			State& minState = states[0].first;

			for (auto st : states) {
				pair<int, State> result = minimax(st.first, depth + 1, maxDepth, alpha, min);
				if (result.first < min) {
					min = result.first;
					minState = st.first;
				}

				if (min <= alpha) // alpha cut off
					return pair<int, State>(min, minState);
			}

			return pair<int, State>(min, minState);
		}
	}
}

vector<pair<State, Move>> Bot::getAvailableStates(State& state) {
	vector<pair<State, Move>> result;
	Board& b = state.first;
	Color myColor = state.second;
	Color enemyColor = (myColor == COL_WHITE) ? COL_BLACK : COL_WHITE;

	Field myField = (myColor == COL_WHITE) ? Field::WHITE : Field::BLACK;
	Field enemyField = (myColor == COL_WHITE) ? Field::BLACK : Field::WHITE;

	for (int i = 0; i < WIDTH; i++) {
		for (int j = 0; j < HEIGHT; j++) {
			if (b[i][j] != Field::EMPTY) continue;

			// we are to check all 9 lines \ | / - - / | \/
			list<Position> figuresToChange;

			list<Position> temp = list<Position>();

			// TopDiagonalLeft
			int newI = i - 1, newJ = j - 1;
			while (newI >= 0 && newJ >= 0) {
				if (b[newI][newJ] == enemyField)
					temp.push_back(Position(newI, newJ));
				else if (b[newI][newJ] == myField) { // to usuwamy wszystkie pionki
					figuresToChange.splice(figuresToChange.end(), temp);
					break;
				}
				else // we break here since its empty field
					break;

				newI--; newJ--;
			}
			temp.clear();

			newI = i + 1; newJ = j + 1;
			while (newI < WIDTH && newJ < HEIGHT) {
				if (b[newI][newJ] == enemyField)
					temp.push_back(Position(newI, newJ));
				else if (b[newI][newJ] == myField) { // to usuwamy wszystkie pionki
					figuresToChange.splice(figuresToChange.end(), temp);
					break;
				}
				else // we break here since its empty field
					break;

				newI++; newJ++;
			}
			temp.clear();

			// TopDiagonalRight
			newI = i - 1; newJ = j + 1;
			while (newI >= 0 && newJ < HEIGHT) {
				if (b[newI][newJ] == enemyField)
					temp.push_back(Position(newI, newJ));
				else if (b[newI][newJ] == myField) { // to usuwamy wszystkie pionki
					figuresToChange.splice(figuresToChange.end(), temp);
					break;
				}
				else // we break here since its empty field
					break;

				newI--; newJ++;
			}
			temp.clear();

			newI = i + 1; newJ = j - 1;
			while (newI < WIDTH && newJ >= 0) {
				if (b[newI][newJ] == enemyField)
					temp.push_back(Position(newI, newJ));
				else if (b[newI][newJ] == myField) { // to usuwamy wszystkie pionki
					figuresToChange.splice(figuresToChange.end(), temp);
					break;
				}
				else // we break here since its empty field
					break;

				newI++; newJ--;
			}
			temp.clear();

			// Horizontal
			newI = i - 1; newJ = j;
			while (newI >= 0) {
				if (b[newI][newJ] == enemyField)
					temp.push_back(Position(newI, newJ));
				else if (b[newI][newJ] == myField) { // to usuwamy wszystkie pionki
					figuresToChange.splice(figuresToChange.end(), temp);
					break;
				}
				else // we break here since its empty field
					break;

				newI--;
			}
			temp.clear();

			newI = i + 1; newJ = j;
			while (newI < WIDTH) {
				if (b[newI][newJ] == enemyField)
					temp.push_back(Position(newI, newJ));
				else if (b[newI][newJ] == myField) { // to usuwamy wszystkie pionki
					figuresToChange.splice(figuresToChange.end(), temp);
					break;
				}
				else // we break here since its empty field
					break;

				newI++;
			}
			temp.clear();

			// Vertical
			newI = i; newJ = j - 1;
			while (newJ >= 0) {
				if (b[newI][newJ] == enemyField)
					temp.push_back(Position(newI, newJ));
				else if (b[newI][newJ] == myField) { // to usuwamy wszystkie pionki
					figuresToChange.splice(figuresToChange.end(), temp);
					break;
				}
				else // we break here since its empty field
					break;

				newJ--;
			}
			temp.clear();

			newI = i; newJ = j + 1;
			while (newJ < HEIGHT) {
				if (b[newI][newJ] == enemyField)
					temp.push_back(Position(newI, newJ));
				else if (b[newI][newJ] == myField) { // to usuwamy wszystkie pionki
					figuresToChange.splice(figuresToChange.end(), temp);
					break;
				}
				else // we break here since its empty field
					break;

				newJ++;
			}

			if (figuresToChange.empty()) continue;
			figuresToChange.push_back(Position(i, j));

			State newState = state;
			newState.second = enemyColor;

			for (auto it = figuresToChange.begin(); it != figuresToChange.end(); it++) {
				newState.first[it->first][it->second] = myField;
			}

			char move[4];
			sprintf_s(move, 4, "%c%d", j + 'A', WIDTH - i);
			string sMove = move;
			result.push_back(pair<State, Move>(newState, sMove));
		}
	}

	return result;
}