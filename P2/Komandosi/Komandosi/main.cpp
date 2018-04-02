#include <iostream>
#include <fstream>
#include <vector>
#include <queue>
#include <unordered_map>

using namespace std;

enum Field
{
	Wall = '#',
	Goal = 'G',
	Start = 'S',
	StartGoal = 'B',
	Empty = ' '
};

int n, m;
vector< vector<Field> > plansza;

typedef	pair<int, int> Position;

class State
{
public:
	vector<Position> pointsPosition;
	
	State* prev;
	char move;
	int goals;

	friend bool operator==(const State& s1, const State& s2);
};

bool operator==(const State& s1, const State& s2) {	
	return (s1.pointsPosition == s2.pointsPosition);
}

struct stateHash : public std::unary_function<State, std::size_t>
{
	std::size_t operator()(const State& state) const
	{
		//size_t hash = state.goals * 523 + state.move * 37;
		std::size_t seed = state.pointsPosition.size();
		for (auto& i : state.pointsPosition) {
			seed ^= i.first * i.second + 0x9e3779b9 + (seed << 6) + (seed >> 2);
		}

		return seed;// +hash; // count hash here
	}
};

unordered_map<State, State, stateHash> dp;

vector<State> getStates(State state) {
	vector<State> newStates;
	
	// move up
	State stateUp = state;
	stateUp.prev = &dp[state];
	stateUp.move = 'U';

	State stateDown = state;
	stateDown.prev = &dp[state];
	stateDown.move = 'D';

	State stateLeft = state;
	stateLeft.prev = &dp[state];
	stateLeft.move = 'L';

	State stateRight = state;
	stateRight.prev = &dp[state];
	stateRight.move = 'R';

	for (int i = 0; i < state.pointsPosition.size(); i++) {
		Position& point = state.pointsPosition[i];
		if (plansza[point.first - 1][point.second] != Field::Wall) {
			if (plansza[point.first][point.second] == Field::Goal) stateUp.goals--;

			stateUp.pointsPosition[i].first--;
			if (plansza[point.first - 1][point.second] == Field::Goal) {
				stateUp.goals++;
			}
		}

		if (plansza[point.first + 1][point.second] != Field::Wall) {
			if (plansza[point.first][point.second] == Field::Goal) stateDown.goals--;

			stateDown.pointsPosition[i].first++;
			if (plansza[point.first + 1][point.second] == Field::Goal) {
				stateDown.goals++;
			}
		}

		if (plansza[point.first][point.second - 1] != Field::Wall) {
			if (plansza[point.first][point.second] == Field::Goal) stateLeft.goals--;

			stateLeft.pointsPosition[i].second--;
			if (plansza[point.first][point.second - 1] == Field::Goal) {
				stateLeft.goals++;
			}
		}

		if (plansza[point.first][point.second + 1] != Field::Wall) {
			if (plansza[point.first][point.second] == Field::Goal) stateRight.goals--;

			stateRight.pointsPosition[i].second++;
			if (plansza[point.first][point.second + 1] == Field::Goal) {
				stateRight.goals++;
			}
		}
	}

	/*sort(stateUp.pointsPosition.begin(), stateUp.pointsPosition.end());
	stateUp.pointsPosition.erase(unique(stateUp.pointsPosition.begin(), stateUp.pointsPosition.end()), stateUp.pointsPosition.end());

	sort(stateDown.pointsPosition.begin(), stateDown.pointsPosition.end());
	stateDown.pointsPosition.erase(unique(stateDown.pointsPosition.begin(), stateDown.pointsPosition.end()), stateDown.pointsPosition.end());

	sort(stateLeft.pointsPosition.begin(), stateLeft.pointsPosition.end());
	stateLeft.pointsPosition.erase(unique(stateLeft.pointsPosition.begin(), stateLeft.pointsPosition.end()), stateLeft.pointsPosition.end());

	sort(stateRight.pointsPosition.begin(), stateRight.pointsPosition.end());
	stateRight.pointsPosition.erase(unique(stateRight.pointsPosition.begin(), stateRight.pointsPosition.end()), stateRight.pointsPosition.end());
*/
	newStates.push_back(stateUp);
	newStates.push_back(stateDown);
	newStates.push_back(stateLeft);
	newStates.push_back(stateRight);
	return newStates;
}

State BFS(State startState) {
	queue<State> q;
	q.push(startState);
	dp[startState] = startState;

	while (!q.empty()) {
		State state = q.front();
		q.pop();

		if (state.goals == state.pointsPosition.size()) {
			return state;
		}

		// get all states
		vector<State> newStates = getStates(state);
		for (int i = 0; i < newStates.size(); i++) {
			if (dp.find(newStates[i]) == dp.end()) {
				dp[newStates[i]] = newStates[i];
				q.push(newStates[i]);
			}
		}
	}

	State boom;
	return boom;
}

void backTrackPos(State lastState) {
	string sequence = "";
	while (lastState.prev != nullptr) {
		sequence += lastState.move;
		lastState = *(lastState.prev);
	}

	reverse(sequence.begin(), sequence.end());

	cout << sequence.c_str() << " " << sequence.size() << endl;
}

State reduceStates(State state, const int TRIES) {
	// go many times right, then mamy times down
	State stateRight = state;

	for (int j = 0; j < TRIES; j++) {
		bool bIsNew = false;
		for (int i = 0; i < stateRight.pointsPosition.size(); i++) {
			Position& point = stateRight.pointsPosition[i];

			if (plansza[point.first][point.second - 1] != Field::Wall) {
				stateRight.pointsPosition[i].second--;
				bIsNew = true;
			}
		}
		if (bIsNew) cout << "L";
	}

	for (int j = 0; j < TRIES; j++) {
		bool bIsNew = false;
		for (int i = 0; i < stateRight.pointsPosition.size(); i++) {
			Position& point = stateRight.pointsPosition[i];

			if (plansza[point.first - 1][point.second] != Field::Wall) {
				stateRight.pointsPosition[i].first--;
				bIsNew = true;
			}
		}
		if (bIsNew) cout << "U";
	}

	for (int j = 0; j < TRIES; j++) {
		bool bIsNew = false;
		for (int i = 0; i < stateRight.pointsPosition.size(); i++) {
			Position& point = stateRight.pointsPosition[i];

			if (plansza[point.first][point.second + 1] != Field::Wall) {
				stateRight.pointsPosition[i].second++;
				bIsNew = true;
			}
		}
		if (bIsNew) cout << "R";
	}

	for (int j = 0; j < TRIES; j++) {
		bool bIsNew = false;
		for (int i = 0; i < stateRight.pointsPosition.size(); i++) {
			Position& point = stateRight.pointsPosition[i];

			if (plansza[point.first + 1][point.second] != Field::Wall) {
				stateRight.pointsPosition[i].first++;
				bIsNew = true;
			}
		}
		if (bIsNew) cout << "D";
	}

	for (int j = 0; j < TRIES; j++) {
		bool bIsNew = false;
		for (int i = 0; i < stateRight.pointsPosition.size(); i++) {
			Position& point = stateRight.pointsPosition[i];

			if (plansza[point.first][point.second - 1] != Field::Wall) {
				stateRight.pointsPosition[i].second--;
				bIsNew = true;
			}
		}
		if (bIsNew) cout << "L";
	}

	for (int j = 0; j < TRIES; j++) {
		bool bIsNew = false;
		for (int i = 0; i < stateRight.pointsPosition.size(); i++) {
			Position& point = stateRight.pointsPosition[i];

			if (plansza[point.first][point.second + 1] != Field::Wall) {
				stateRight.pointsPosition[i].second++;
				bIsNew = true;
			}
		}
		if (bIsNew) cout << "R";

		for (int k = 0; k < TRIES; k++) {
			bool bIsNew = false;
			for (int i = 0; i < stateRight.pointsPosition.size(); i++) {
				Position& point = stateRight.pointsPosition[i];

				if (plansza[point.first + 1][point.second] != Field::Wall) {
					stateRight.pointsPosition[i].first++;
					bIsNew = true;
				}
			}
			if (bIsNew) cout << "D";
		}
	}

	// for all similar points, delete them
	sort(stateRight.pointsPosition.begin(), stateRight.pointsPosition.end());
	stateRight.pointsPosition.erase(unique(stateRight.pointsPosition.begin(), stateRight.pointsPosition.end()), stateRight.pointsPosition.end());

	// for each point now, check if it is on goal
	stateRight.goals = 0;
	for (Position point : stateRight.pointsPosition) {
		if (plansza[point.first][point.second] == Field::Goal)
			stateRight.goals++;
	}

	stateRight.prev = nullptr;
	dp[stateRight] = stateRight;

	return stateRight;
}

void printState(State state) {
	cout << endl;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			bool bIsPoint = false;
			for (auto& point : state.pointsPosition) {
				if (point.first == i && point.second == j) {
					if (plansza[i][j] == Field::Goal)
						cout << "B";
					else cout << "X";
					bIsPoint = true;
					break;
				}
			}

			if (!bIsPoint)
				cout << (char)plansza[i][j];
		}

		cout << endl;
	}

	cout << endl;
}

int main() {
	ifstream file;
	file.open("zad_input.txt");

	int testNumber = 1;
	while (!file.eof()) {
		dp.clear();
		file >> n >> m;
		char temp[32];
		file.getline(temp, 32);

		plansza = vector< vector<Field> >(n);
		State startState;
		startState.goals = 0;
		startState.prev = nullptr;

		for (int i = 0; i < n; i++) {
			char line[32];
			file.getline(line, 32);

			for (int j = 0; j < m; j++) {
				Field field = (Field)line[j];
				plansza[i].push_back(field);

				if (field == Field::Start) {
					startState.pointsPosition.push_back(Position(i, j));
				}
				else if (field == Field::StartGoal) {
					startState.goals++;
					plansza[i][j] = Field::Goal;
					startState.pointsPosition.push_back(Position(i, j));
				}
			}
		}

		cout << "Test #" << testNumber++ << ": ";
		State modifiedState = reduceStates(startState, 19);
		//printState(modifiedState);
		State lastState = BFS(modifiedState);
		backTrackPos(lastState);
		cout << endl;
	}

	system("PAUSE");

	return 0;
}