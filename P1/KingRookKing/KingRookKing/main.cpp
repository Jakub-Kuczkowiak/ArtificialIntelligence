#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <queue>
#include <tuple>

using namespace std;

typedef vector<string> Position;

class State {
public:
	int n;
	Position prev;

	State(int n = -1) {
		this->n = n;
		this->prev = vector<string>({ "empty" });
	}

	State(int n, Position prev) {
		this->n = n;
		this->prev = prev;
	}
};

State dp[2][64][64][64];

void init_dp() {
	for (int i = 0; i < 2; i++) {
		for (int j = 0; j < 64; j++) {
			for (int k = 0; k < 64; k++) {
				for (int l = 0; l < 64; l++) {
					dp[i][j][k][l] = State();
				}
			}
		}
	}
}

__inline bool isWhiteKingCrossingLine(char wk, char wr, char bk) {
	return (wr < wk && wk < bk) || (wr > wk && wk > bk);
}

enum Color {
	WHITE = 0,
	BLACK = 1
};

bool is_mate(string whiteKingPosition, string whiteRookPosition, string blackKingPosition) {
	// black king is in 4 corners case
	if (blackKingPosition[0] == 'a' && blackKingPosition[1] == '1') {
		if (whiteKingPosition == "a3" || whiteKingPosition == "b3") {
			return (whiteRookPosition[0] > 'b' && whiteRookPosition[1] == '1');
		}
		else if (whiteKingPosition =="c1" || whiteKingPosition =="c2") {
			return (whiteRookPosition[0] == 'a' && whiteRookPosition[1] > '2');
		}
	}
	else if (blackKingPosition[0] == 'a' && blackKingPosition[1] == '8') {
		if (whiteKingPosition =="a6" || whiteKingPosition =="b6") {
			return (whiteRookPosition[0] > 'b' && whiteRookPosition[1] == '8');
		}
		else if (whiteKingPosition =="c8" || whiteKingPosition =="c7") {
			return (whiteRookPosition[0] == 'a' && whiteRookPosition[1] < '7');
		}
	}
	else if (blackKingPosition[0] == 'h' && blackKingPosition[1] == '1') {
		if (whiteKingPosition =="h3" || whiteKingPosition =="g3") {
			return (whiteRookPosition[0] < 'g' && whiteRookPosition[1] == '1');
		}
		else if (whiteKingPosition =="f1" || whiteKingPosition =="f2") {
			return (whiteRookPosition[0] == 'h' && whiteRookPosition[1] > '2');
		}
	}
	else if (blackKingPosition[0] == 'h' && blackKingPosition[1] == '8') {
		if (whiteKingPosition =="h6" || whiteKingPosition =="g6") {
			return (whiteRookPosition[0] < 'g' && whiteRookPosition[1] == '8');
		}
		else if (whiteKingPosition =="f8" || whiteKingPosition =="f7") {
			return (whiteRookPosition[0] == 'h' && whiteRookPosition[1] < '7');
		}
	}

	// the other cases are when white king is in oposition
	if (blackKingPosition[0] == 'a') {
		if (whiteKingPosition[0] == 'c' && whiteKingPosition[1] == blackKingPosition[1]) { // king's oposition
			return (whiteRookPosition[0] == 'a' && abs(whiteRookPosition[1] - blackKingPosition[1]) > 1); // rook in line and farer than 1
		}
	}
	else if (blackKingPosition[0] == 'h') {
		if (whiteKingPosition[0] == 'f' && whiteKingPosition[1] == blackKingPosition[1]) { // king's oposition
			return (whiteRookPosition[0] == 'h' && abs(whiteRookPosition[1] - blackKingPosition[1]) > 1);
		}
	}
	else if (blackKingPosition[1] == '1') {
		if (whiteKingPosition[1] == '3' && whiteKingPosition[0] == blackKingPosition[0]) { // king's oposition
			return (whiteRookPosition[1] == '1' && abs(whiteRookPosition[0] - blackKingPosition[0]) > 1);
		}
	}
	else if (blackKingPosition[1] == '8') {
		if (whiteKingPosition[1] == '6' && whiteKingPosition[0] == blackKingPosition[0]) { // king's oposition
			return (whiteRookPosition[1] == '8' && abs(whiteRookPosition[0] - blackKingPosition[0]) > 1);
		}
	}

	return false;
}

bool is_valid(int c, string whiteKingPosition, string whiteRookPosition, string blackKingPosition) {
	if (whiteKingPosition == whiteRookPosition || whiteKingPosition == blackKingPosition || whiteRookPosition == blackKingPosition) {
		return false;
	}

	if (abs(whiteKingPosition[0] - blackKingPosition[0]) <= 1 && abs(whiteKingPosition[1] - blackKingPosition[1]) <= 1) {
		return false;
	}

	if (c == WHITE) { // White's move and check
		// is it check for black?
		// check from white rook if and only if the rook is in the same row or column and there is no white king between
		if (whiteRookPosition[0] == blackKingPosition[0]) {
			if (whiteKingPosition[0] == whiteRookPosition[0]) {
				// the same letter, so king cannot be between
				if (!isWhiteKingCrossingLine(whiteKingPosition[1], whiteRookPosition[1], blackKingPosition[1])) {
					return false;
				}
			}
		}
		else if (whiteRookPosition[1] == blackKingPosition[1]) {
			if (whiteKingPosition[1] == whiteRookPosition[1]) {
				if (!isWhiteKingCrossingLine(whiteKingPosition[0], whiteRookPosition[0], blackKingPosition[0])) {
					return false;
				}
			}
		}
	}

	return true;
}

vector<Position> GetWhiteAvailableMoves(string whiteKingPosition, string whiteRookPosition, string blackKingPosition) {
	vector<Position> positions = vector<Position>();
	
	// white king moves
	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) continue;

			string newPosition = whiteKingPosition;

			newPosition[0] += i;
			newPosition[1] += j;

			if (newPosition[0] >= 'a' && newPosition[0] <= 'h' && newPosition[1] >= '1' && newPosition[1] <= '8') {
				// we must assert whiteKing is not checked by black and its not in the rook place
				if (abs(newPosition[0] - blackKingPosition[0]) > 1 || abs(newPosition[1] - blackKingPosition[1]) > 1) {
					if (newPosition != whiteRookPosition) {
						positions.push_back(vector<string>({ newPosition, whiteRookPosition, blackKingPosition }));
					}
				}
			}
		}
	}

	// white rook moves
	// first in row
	for (char i = '1'; i <= '8'; i++) {
		string newPosition = whiteRookPosition;
		if (whiteRookPosition[1] != i) { // we eliminate if it stays in the same place
			newPosition[1] = i;

			if (newPosition != whiteKingPosition) { // if it doesnt stack in white king, there is no possibility to destroy black king
				// so we dont need to check it.
				if (whiteKingPosition[0] == whiteRookPosition[0]) {// if it is the same letter
					if (!isWhiteKingCrossingLine(whiteKingPosition[1], whiteRookPosition[1], i)) {
						positions.push_back(vector<string>({ whiteKingPosition, newPosition, blackKingPosition }));
					}
				}
				else {
					// add
					positions.push_back(vector<string>({ whiteKingPosition, newPosition, blackKingPosition }));
				}
			}
		}
	}

	// in column
	for (char j = 'a'; j <= 'h'; j++) {
		string newPosition = whiteRookPosition;
		if (whiteRookPosition[0] != j) { // we eliminate if it stays in the same place
			newPosition[0] = j;

			if (newPosition != whiteKingPosition) { // if it doesnt stack in white king, there is no possibility to destroy black king
													// so we dont need to check it.
				if (whiteKingPosition[1] == whiteRookPosition[1]) {// if it is the same number
					if (!isWhiteKingCrossingLine(whiteKingPosition[0], whiteRookPosition[0], j)) {
						positions.push_back(vector<string>({ whiteKingPosition, newPosition, blackKingPosition }));
					}
				}
				else {
					// add
					positions.push_back(vector<string>({ whiteKingPosition, newPosition, blackKingPosition }));
				}
			}
		}
	}

	return positions;
}

vector<Position> GetBlackAvailableMoves(string whiteKingPosition, string whiteRookPosition, string blackKingPosition) {
	vector<Position> positions = vector<Position>();

	for (int i = -1; i <= 1; i++) {
		for (int j = -1; j <= 1; j++) {
			if (i == 0 && j == 0) continue;

			string newPosition = blackKingPosition;

			newPosition[0] += i;
			newPosition[1] += j;

			if (newPosition[0] >= 'a' && newPosition[0] <= 'h' && newPosition[1] >= '1' && newPosition[1] <= '8') {
				// now we assert if there is a check

				// check from white king if and only if distance is less than 1
				if (abs(whiteKingPosition[0] - newPosition[0]) > 1 || abs(whiteKingPosition[1] - newPosition[1]) > 1) {
					// we exclude this case since there is no chance for checkmate and black king is 'to help' being checkmated.
					if (newPosition != whiteRookPosition) {
						// check from white rook if and only if the rook is in the same row or column and there is no white king between
						if (whiteRookPosition[0] == newPosition[0]) {
							if (whiteKingPosition[0] == whiteRookPosition[0]) {
								// the same letter, so king cannot be between
								if (isWhiteKingCrossingLine(whiteKingPosition[1], whiteRookPosition[1], newPosition[1])) {
									// we can add this position
									positions.push_back(vector<string>({ whiteKingPosition, whiteRookPosition, newPosition }));
								}
							}							
						}
						else if (whiteRookPosition[1] == newPosition[1]) {
							if (whiteKingPosition[1] == whiteRookPosition[1]) {
								if (isWhiteKingCrossingLine(whiteKingPosition[0], whiteRookPosition[0], newPosition[0])) {
									// we add
									positions.push_back(vector<string>({ whiteKingPosition, whiteRookPosition, newPosition }));
								}
							}
						}
						else {
							positions.push_back(vector<string>({ whiteKingPosition, whiteRookPosition, newPosition }));
						}
					}
				}
			}
		}
	}

	return positions;
}

int position(string coordinates) {
	return (coordinates[0] - 'a') * 8 + coordinates[1] - '1';
}

vector<int> position_dp(Position pos) {
	vector<int> res = vector<int>();
	res.push_back(position(pos[0]));
	res.push_back(position(pos[1]));
	res.push_back(position(pos[2]));

	return res;
}

typedef tuple<Color, State, Position> StateTuple;

tuple<Color, State, Position> solve(StateTuple startState) {
	queue<StateTuple> q = queue<StateTuple>();

	q.push(startState);

	while (!q.empty()) { // BFS search here.
		StateTuple el = q.front();
		Color color = get<0>(el);
		State state = get<1>(el);
		Position pos = get<2>(el);

		q.pop();

		vector<int> indexes = position_dp(pos);
		if (dp[color][indexes[0]][indexes[1]][indexes[2]].n != -1) continue;

		dp[color][indexes[0]][indexes[1]][indexes[2]] = state;

		if (is_mate(pos[0], pos[1], pos[2])) {
			return el;
		}

		vector<Position> positions;
		if (color == WHITE)
			positions = GetWhiteAvailableMoves(pos[0], pos[1], pos[2]);
		else
			positions = GetBlackAvailableMoves(pos[0], pos[1], pos[2]);

		for (auto position : positions) {
			vector<int> dp_indexes = position_dp(position);
			if (dp[color][dp_indexes[0]][dp_indexes[1]][dp_indexes[2]].n == -1) {
				// only if not checked before we add it
				State newState = State(state.n + 1, pos);
				Color newColor = (color == WHITE ? BLACK : WHITE);
				StateTuple newTuple = StateTuple(newColor, newState, position);
				q.push(newTuple);
			}
		}
	}

	return StateTuple(WHITE, State(-1), Position({ "invalid" })); // this should never occur, since we can always give mate.
}

void printState(Position pos) {
	for (int i = '8'; i >= '1'; i--) {
		cout << i - '0';

		for (int j = 'a'; j <= 'h'; j++) {
			string field = string(1, j);
			field += i;

			if (pos[0] == field) {
				cout << "K";
			}
			else if (pos[1] == field) {
				cout << "R";
			}
			else if (pos[2] == field) {
				cout << "k";
			}
			else {
				cout << ".";
			}
		}

		cout << endl;
	}

	cout << " ABCDEFGH";

	cout << endl;
}

vector<Position> backtrack(StateTuple endState) {
	vector<Position> result = vector<Position>();

	StateTuple stateTuple = endState;

	Color color = get<0>(stateTuple);
	State state = get<1>(stateTuple);
	Position pos = get<2>(stateTuple);

	result.push_back(pos);

	while (state.n != 0) {
		Position prev = state.prev;
		vector<int> dp_indexes = position_dp(prev);
		color = (color == WHITE ? BLACK : WHITE);
		state = dp[color][dp_indexes[0]][dp_indexes[1]][dp_indexes[2]];
	
		result.push_back(prev);
	}

	reverse(result.begin(), result.end());

	return result;
}

vector<Position> frontTrace(StateTuple startState) {
	vector<Position> result = vector<Position>();

	StateTuple stateTuple = startState;

	Color color = get<0>(stateTuple);
	State state = get<1>(stateTuple);
	Position pos = get<2>(stateTuple);

	result.push_back(pos);

	while (state.n != 0) {
		Position next = state.prev;
		vector<int> dp_indexes = position_dp(next);
		color = (color == WHITE ? BLACK : WHITE);
		state = dp[color][dp_indexes[0]][dp_indexes[1]][dp_indexes[2]];

		result.push_back(next);
	}

	return result;
}

string posToString(int index) {
	string coordinates = "";
	char letter = 'a' + index / 8;
	char number = '1' + index % 8;
	coordinates += letter;
	coordinates += number;

	return coordinates;
}

void computeDP() {
	init_dp();
	
	// initialize
	for (int c = 0; c <= 1; c++) {
		for (int i = 0; i < 64; i++) {
			for (int j = 0; j < 64; j++) {
				for (int k = 0; k < 64; k++) {
					string whiteKingPosition = posToString(i);
					string whiteRookPosition = posToString(j);
					string blackKingPosition = posToString(k);
					if (!is_valid(c, whiteKingPosition, whiteRookPosition, blackKingPosition)) {
						dp[c][i][j][k] = -2; // invalid_position
						continue;
					}

					if (is_mate(whiteKingPosition, whiteRookPosition, blackKingPosition)) {
						dp[c][i][j][k] = 0;
					}
				}
			}
		}
	}

	for (int mate = 1; mate < 16; mate++) {
		for (int c = 0; c <= 1; c++) {
			for (int i = 0; i < 64; i++) {
				for (int j = 0; j < 64; j++) {
					for (int k = 0; k < 64; k++) {
						if (dp[c][i][j][k].n == -1) { // not counted yet
							string whiteKingPosition = posToString(i);
							string whiteRookPosition = posToString(j);
							string blackKingPosition = posToString(k);

							vector<Position> states;
							if (c == 0) {
								states = GetWhiteAvailableMoves(whiteKingPosition, whiteRookPosition, blackKingPosition);
							}
							else {
								states = GetBlackAvailableMoves(whiteKingPosition, whiteRookPosition, blackKingPosition);
							}

							int newColor = c == 0 ? 1 : 0;

							for (Position state : states) {
								vector<int> position = position_dp(state);
								if (dp[newColor][position[0]][position[1]][position[2]].n != -1) { // this state was counted
									dp[c][i][j][k].prev = state;
									dp[c][i][j][k].n = mate;
									break;
								}
							}
						}
					}
				}
			}
		}

		cout << "Counted for mate: " << mate << endl;
	}
}

int main()
{
	computeDP();

	ifstream file;
	file.open("querries.txt");

	while (!file.eof()) {
		string color, whiteKingPosition, whiteRookPosition, blackKingPosition;
		file >> color >> whiteKingPosition >> whiteRookPosition >> blackKingPosition;

		Color col = WHITE;
		if (color == "black") col = BLACK;

		Position startPos = Position({ whiteKingPosition, whiteRookPosition, blackKingPosition });
		vector<int> dpIndexes = position_dp(startPos);

		StateTuple startTuple = tuple<Color, State, Position>(col,
			dp[col][dpIndexes[0]][dpIndexes[1]][dpIndexes[2]],
			startPos
			);

		vector<Position> states = frontTrace(startTuple);

		cout << "Solution in " << states.size() - 1 << " moves: " << endl;
		for (int i = 0; i < states.size(); i++) {
			printState(states[i]);
			cout << endl;
		}
	}

	/*while (!file.eof()) {
		init_dp();

		string color, whiteKingPosition, whiteRookPosition, blackKingPosition;
		file >> color >> whiteKingPosition >> whiteRookPosition >> blackKingPosition;

		Color col = WHITE;
		if (color == "black") col = BLACK;

		tuple<Color, State, Position> startState = tuple<Color, State, Position>(col, 
			State(0, vector<string>({ "startpos" })), 
			vector<string>({ whiteKingPosition, whiteRookPosition, blackKingPosition }));

		cout << endl << "Analyzing position: " << endl;
		printState(get<2>(startState));
		cout << endl;

		StateTuple result = solve(startState);
		vector<Position> states = backtrack(result);

		cout << "Solution in " << states.size() << " moves: " << endl;
		for (int i = 0; i < states.size(); i++) {
			printState(states[i]);
			cout << endl;
		}
	}*/

	file.close();

	system("PAUSE");

	return 0;
}