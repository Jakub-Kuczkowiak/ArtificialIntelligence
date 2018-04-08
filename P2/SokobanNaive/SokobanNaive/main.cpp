#include <iostream>
#include <vector>
#include <fstream>
#include <queue>
#include <unordered_map>

using namespace std;

enum Field
{
	Empty = '.',
	Wall = 'W',
	Magazynier = 'K',
	Box = 'B',
	Goal = 'G',
	BoxGoal = '*',
	MagazynierGoal = '+'
};

typedef pair<int, int> Position;

class State {
public:
	Position magazinier;
	vector<Position> boxes;
	State* prevState;
	char lastDirection;

	friend bool operator==(const State& s1, const State& s2);
};

bool operator==(const State& s1, const State& s2) {
	return (s1.magazinier == s2.magazinier &&
		s1.boxes == s2.boxes);
}

struct stateHash : public std::unary_function<State, std::size_t>
{
	std::size_t operator()(const State& state) const
	{
		size_t hash = state.magazinier.first * 523 + state.magazinier.second * 37;
		std::size_t seed = state.boxes.size();
		for (auto& i : state.boxes) {
			seed ^= i.first * i.second + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}

		return seed + hash; // count hash here
	}
};

//class Result {
//public:
//	bool bVisited;
//	State prevState;
//	char direction;
//
//	Result(bool bVisited, State prevState, char direction) {
//		this->bVisited = bVisited;
//		this->prevState = prevState;
//		this->direction = direction;
//	}
//};

int n, m;
vector< vector<Field> > plansza;
unordered_map<State, State, stateHash> dp;

int isBox(const State& state, int i, int j) {
	for (int it = 0; it < state.boxes.size(); it++) {
		if (state.boxes[it].first == i && state.boxes[it].second == j) {
			return it;
		}
	}

	return -1;
}

// boxI and boxJ as parameters
bool isBoxSurroundedWithoutGoal(int i, int j) {
	if (plansza[i][j] == 'G') return false;

	// lewy gorny rog
	if (plansza[i - 1][j] == Field::Wall && plansza[i][j - 1] == Field::Wall) {
		return true;
	}

	// prawy gorny rog
	if (plansza[i - 1][j] == Field::Wall && plansza[i][j + 1] == Field::Wall) {
		return true;
	}

	if (plansza[i + 1][j] == Field::Wall && plansza[i][j - 1] == Field::Wall) {
		return true;
	}

	if (plansza[i + 1][j] == Field::Wall && plansza[i][j + 1] == Field::Wall) {
		return true;
	}

	return false;
}

vector<State> getMoves(State state) {
	vector<State> availableStates = vector<State>();

	// we check if we can move in each direction or push box
	// move left
	if (plansza[state.magazinier.first][state.magazinier.second - 1] != Field::Wall) {
		int i = isBox(state, state.magazinier.first, state.magazinier.second - 1);
		if (i != -1) {
			// we check if there is box
			if (plansza[state.magazinier.first][state.magazinier.second - 2] != Field::Wall
				&& !isBoxSurroundedWithoutGoal(state.magazinier.first, state.magazinier.second - 2)) {
				// we must also check if there is not another box
				if (isBox(state, state.magazinier.first, state.magazinier.second - 2) == -1) {
					// we can push
					State newState;
					newState.prevState = &dp[state];
					newState.boxes = state.boxes;
					newState.boxes[i].second -= 1;
					newState.lastDirection = 'L';
					newState.magazinier = Position(state.magazinier.first, state.magazinier.second - 1);

					availableStates.push_back(newState);
				}
			}
		}
		else {
			// we just move
			State newState;
			newState.boxes = state.boxes;
			newState.prevState = &dp[state];
			newState.lastDirection = 'L';
			newState.magazinier = Position(state.magazinier.first, state.magazinier.second - 1);

			availableStates.push_back(newState);
		}
	}

	if (plansza[state.magazinier.first][state.magazinier.second + 1] != Field::Wall) {
		// we check if there is box
		int i = isBox(state, state.magazinier.first, state.magazinier.second + 1);
		if (i != -1) {
			if (plansza[state.magazinier.first][state.magazinier.second + 2] != Field::Wall
				&& !isBoxSurroundedWithoutGoal(state.magazinier.first, state.magazinier.second + 2)) {
				if (isBox(state, state.magazinier.first, state.magazinier.second + 2) == -1) {
					// we can push
					State newState;
					newState.prevState = &dp[state];
					newState.boxes = state.boxes;
					newState.boxes[i].second += 1;
					newState.lastDirection = 'R';
					newState.magazinier = Position(state.magazinier.first, state.magazinier.second + 1);

					availableStates.push_back(newState);
				}
			}
		}
		else {
			// we just move
			State newState;
			newState.boxes = state.boxes;
			newState.prevState = &dp[state];
			newState.lastDirection = 'R';
			newState.magazinier = Position(state.magazinier.first, state.magazinier.second + 1);

			availableStates.push_back(newState);
		}
	}

	if (plansza[state.magazinier.first - 1][state.magazinier.second] != Field::Wall) {
		// we check if there is box
		int i = isBox(state, state.magazinier.first - 1, state.magazinier.second);
		if (i != -1) {
			// we must see if we can push box at position state.magiziner.second - 2
			if (plansza[state.magazinier.first - 2][state.magazinier.second] != Field::Wall
				&& !isBoxSurroundedWithoutGoal(state.magazinier.first - 2, state.magazinier.second)) {
				if (isBox(state, state.magazinier.first - 2, state.magazinier.second) == -1) {
					// we can push
					State newState;
					newState.prevState = &dp[state];
					newState.boxes = state.boxes;
					newState.boxes[i].first -= 1;
					newState.lastDirection = 'U';
					newState.magazinier = Position(state.magazinier.first - 1, state.magazinier.second);

					availableStates.push_back(newState);
				}
			}
		}
		else {
			// we just move
			State newState;
			newState.boxes = state.boxes;
			newState.prevState = &dp[state];
			newState.lastDirection = 'U';
			newState.magazinier = Position(state.magazinier.first - 1, state.magazinier.second);

			availableStates.push_back(newState);
		}
	}

	if (plansza[state.magazinier.first + 1][state.magazinier.second] != Field::Wall) {
		// we check if there is box
		int i = isBox(state, state.magazinier.first + 1, state.magazinier.second);
		if (i != -1) {
			// we must see if we can push box at position state.magiziner.second - 2
			if (plansza[state.magazinier.first + 2][state.magazinier.second] != Field::Wall
				&& !isBoxSurroundedWithoutGoal(state.magazinier.first + 2, state.magazinier.second)) {
				if (isBox(state, state.magazinier.first + 2, state.magazinier.second) == -1) {
					// we can push
					State newState;
					newState.prevState = &dp[state];
					newState.boxes = state.boxes;
					newState.boxes[i].first += 1;
					newState.lastDirection = 'D';
					newState.magazinier = Position(state.magazinier.first + 1, state.magazinier.second);

					availableStates.push_back(newState);
				}
			}
		}
		else {
			// we just move
			State newState;
			newState.boxes = state.boxes;
			newState.prevState = &dp[state];
			newState.lastDirection = 'D';
			newState.magazinier = Position(state.magazinier.first + 1, state.magazinier.second);

			availableStates.push_back(newState);
		}
	}

	return availableStates;
}

void printState(State state) {
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < m; j++)
		{
			if (state.magazinier.first == i && state.magazinier.second == j) {
				if (plansza[i][j] == Field::Goal)
					cout << (char)Field::MagazynierGoal;
				else
					printf("K");

				continue;
			}

			// we check if any boxes is contained in our position
			bool bPrinted = false;
			for (auto& box : state.boxes) {
				if (box.first == i && box.second == j) {
					if (plansza[i][j] == Field::Goal)
						cout << (char)Field::BoxGoal;
					else
						cout << "B";
					bPrinted = true;
					break;
				}
			}

			if (bPrinted) continue;

			if (plansza[i][j] == Field::Wall) {
				printf("W");
			}
			else if (plansza[i][j] == Field::Goal) {
				printf("G");
			}
			else {
				printf(".");
			}
		}

		cout << endl;
	}

	cout << endl;
}

void backTrack(State lastState) {
	while (lastState.prevState != nullptr) {
		printState(lastState);
		lastState = *(lastState.prevState);
	}
}

void backTrackPos(State lastState) {
	string moves = "";
	while (lastState.prevState != nullptr) {
		moves += lastState.lastDirection;
		lastState = *(lastState.prevState);
	}

	reverse(moves.begin(), moves.end());
	cout << moves.c_str() << " " << moves.length();
}

bool correctState(State state) {
	// we check for each box if it's in one of the goal places.
	for (auto& box : state.boxes) {
		if (plansza[box.first][box.second] != Field::Goal)
			return false;
	}

	return true;
}

int heuro(const State& state) {
	// lets count distance of boxes to goals
	int distance = 0;
	for (auto& box : state.boxes) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				if (plansza[i][j] == Field::Goal && !isBox(state, i, j)) {
					distance += abs(box.first - i);
					distance += abs(box.second - j);
				}
			}
		}
	}

	return distance;
}

struct compare : public std::binary_function<State&, State&, bool>
{
	bool operator()(const State& st1, const State& st2) {
		return heuro(st1) < heuro(st2);
	}
};

State BFS(State startState) {
	queue<State> q = queue<State>();
	q.push(startState);

	while (!q.empty()) {
		State state = q.front();
		q.pop();
		dp[state] = state;

		if (correctState(state)) {
			return state;
		}

		vector<State> states = getMoves(state);

		for (int i = 0; i < states.size(); i++) {
			if (dp.find(states[i]) == dp.end())
				q.push(states[i]);
		}
	}
	
	State stateRet;
	return stateRet;
}

State BFS_A(State startState) {
	priority_queue<State, vector<State>, compare> q;
	q.push(startState);

	while (!q.empty()) {
		State state = q.top();
		q.pop();
		dp[state] = state;

		if (correctState(state)) {
			return state;
		}

		vector<State> states = getMoves(state);

		for (int i = 0; i < states.size(); i++) {
			if (dp.find(states[i]) == dp.end())
				q.push(states[i]);
		}
	}

	State stateRet;
	return stateRet;
}

int main() {
	ifstream file;
	file.open("tests.txt");

	int testNumber = 1;
	while (!file.eof()) {
		dp.clear();
		file >> n >> m;
		char temp[32];
		file.getline(temp, 32);

		plansza = vector< vector<Field> >(20);

		State startState;
		startState.prevState = nullptr;

		for (int i = 0; i < n; i++) {
			char line[32];
			file.getline(line, 32);

			for (int j = 0; j < m; j++) {
				Field field = (Field)line[j];
				plansza[i].push_back(Field::Empty);

				if (field == Field::Magazynier) {
					startState.magazinier = Position(i, j);
				}
				else if (field == Field::Box) {
					startState.boxes.push_back(Position(i, j));
				}
				else if (field == Field::BoxGoal) {
					plansza[i][j] = Field::Goal;
					startState.boxes.push_back(Position(i, j));
				}
				else if (field == Field::MagazynierGoal) {
					plansza[i][j] = Field::Goal;
					startState.magazinier = Position(i, j);
				}
				else {
					plansza[i][j] = field;
				}
			}
		}

		State lastState = BFS(startState);
		backTrack(lastState);
		cout << "Test #" << testNumber++ << ": ";
		backTrackPos(lastState);

		cout << endl;
	}

	cout << endl;
	system("PAUSE");

	return 0;
}