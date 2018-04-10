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

	int n;

	friend bool operator==(const State& s1, const State& s2);
};

bool operator==(const State& s1, const State& s2) {	
	return (s1.pointsPosition == s2.pointsPosition && s1.n == s2.n);
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

		return seed + state.n * 523;

		//return seed;// +hash; // count hash here
	}
};

unordered_map<State, State, stateHash> dp;

vector<State> getStates(State state) {
	vector<State> newStates;
	
	// move up
	State stateUp = state;
	stateUp.prev = &dp[state];
	stateUp.move = 'U';
	stateUp.n = state.n + 1;

	State stateDown = state;
	stateDown.prev = &dp[state];
	stateDown.move = 'D';
	stateDown.n = state.n + 1;

	State stateLeft = state;
	stateLeft.prev = &dp[state];
	stateLeft.move = 'L';
	stateLeft.n = state.n + 1;

	State stateRight = state;
	stateRight.prev = &dp[state];
	stateRight.move = 'R';
	stateRight.n = state.n + 1;

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

	sort(stateUp.pointsPosition.begin(), stateUp.pointsPosition.end());
	stateUp.pointsPosition.erase(unique(stateUp.pointsPosition.begin(), stateUp.pointsPosition.end()), stateUp.pointsPosition.end());
	stateUp.goals = 0;
	for (Position& point : stateUp.pointsPosition)
		if (plansza[point.first][point.second] == Field::Goal)
			stateUp.goals++;

	sort(stateDown.pointsPosition.begin(), stateDown.pointsPosition.end());
	stateDown.pointsPosition.erase(unique(stateDown.pointsPosition.begin(), stateDown.pointsPosition.end()), stateDown.pointsPosition.end());
	stateDown.goals = 0;
	for (Position& point : stateDown.pointsPosition)
		if (plansza[point.first][point.second] == Field::Goal)
			stateDown.goals++;

	sort(stateLeft.pointsPosition.begin(), stateLeft.pointsPosition.end());
	stateLeft.pointsPosition.erase(unique(stateLeft.pointsPosition.begin(), stateLeft.pointsPosition.end()), stateLeft.pointsPosition.end());
	stateLeft.goals = 0;
	for (Position& point : stateLeft.pointsPosition)
		if (plansza[point.first][point.second] == Field::Goal)
			stateLeft.goals++;

	sort(stateRight.pointsPosition.begin(), stateRight.pointsPosition.end());
	stateRight.pointsPosition.erase(unique(stateRight.pointsPosition.begin(), stateRight.pointsPosition.end()), stateRight.pointsPosition.end());
	stateRight.goals = 0;
	for (Position& point : stateRight.pointsPosition)
		if (plansza[point.first][point.second] == Field::Goal)
			stateRight.goals++;

	newStates.push_back(stateUp);
	newStates.push_back(stateDown);
	newStates.push_back(stateLeft);
	newStates.push_back(stateRight);
	return newStates;
}

int dist[30][30][30][30] = {};
void initDist() {
	for (int i = 0; i < 30; i++) {
		for (int j = 0; j < 30; j++) {
			for (int k = 0; k < 30; k++) {
				for (int l = 0; l < 30; l++) {
					dist[i][j][k][l] = -1;
				}
			}
		}
	}
}

void countDistance(int i, int j) {
	if (plansza[i][j] == Field::Wall) return;

	queue<Position> q;
	q.push(Position(i, j));
	dist[i][j][i][j] = 0;

	while (!q.empty()) {
		Position pos = q.front();
		q.pop();

		if (plansza[pos.first][pos.second + 1] != Field::Wall) { // we can move right
			if (dist[i][j][pos.first][pos.second + 1] == -1) {
				dist[i][j][pos.first][pos.second + 1] = dist[i][j][pos.first][pos.second] + 1;
				q.push(Position(pos.first, pos.second + 1));
			}
		}

		if (plansza[pos.first][pos.second - 1] != Field::Wall) { // we can move left
			if (dist[i][j][pos.first][pos.second - 1] == -1) {
				dist[i][j][pos.first][pos.second - 1] = dist[i][j][pos.first][pos.second] + 1;
				q.push(Position(pos.first, pos.second - 1));
			}
		}

		if (plansza[pos.first + 1][pos.second] != Field::Wall) { // we can move down
			if (dist[i][j][pos.first + 1][pos.second] == -1) {
				dist[i][j][pos.first + 1][pos.second] = dist[i][j][pos.first][pos.second] + 1;
				q.push(Position(pos.first + 1, pos.second));
			}
		}

		if (plansza[pos.first - 1][pos.second] != Field::Wall) { // we can move up
			if (dist[i][j][pos.first - 1][pos.second] == -1) {
				dist[i][j][pos.first - 1][pos.second] = dist[i][j][pos.first][pos.second] + 1;
				q.push(Position(pos.first - 1, pos.second));
			}
		}
	}
}

vector<Position> Goals;

// admissible, therefore gives optimal solution
// Zad6: set k to at least 3 to pass all test cases.
// Zad7: Use sum to pass all tests or k = 10
int heuro(const State& state, int k = 15)
{
	// for each comando, find the closest goal point
	int max = 0;
	for (auto& point : state.pointsPosition) {
		int min = 1000000;
		for (auto& goal : Goals) {
			if (dist[point.first][point.second][goal.first][goal.second] < min)
				min = dist[point.first][point.second][goal.first][goal.second];
		}

		if (min > max)
			max = min;
	}

	return state.n + max + k * state.pointsPosition.size();
}

// not admissible, but very fast
int heuro_sum(const State& state) {
	// for each comando, find the closest goal point
	int sum = 0;
	for (auto& point : state.pointsPosition) {
		int min = 1000000;
		for (auto& goal : Goals) {
			if (dist[point.first][point.second][goal.first][goal.second] < min)
				min = dist[point.first][point.second][goal.first][goal.second];
		}

		sum += min;
	}

	return state.n + sum;
}

int heuro_points(const State& state) {
	return state.n + state.pointsPosition.size();
}

struct compare : public std::binary_function<State&, State&, bool>
{
	bool operator()(const State& st1, const State& st2) {
		return heuro(st1) > heuro(st2);
	}
};

State BFS(State startState) {
	priority_queue<State, vector<State>, compare> q;
	q.push(startState);
	dp[startState] = startState;

	while (!q.empty()) {
		State state = q.top();
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
	file.open("tests4.txt");

	bool bEnd = false;
BEGIN:
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
		startState.n = 0;

		Goals.clear();

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

				if (field == Field::Goal) {
					Goals.push_back(Position(i, j));
				}
			}
		}

		initDist();
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < m; j++) {
				countDistance(i, j);
			}
		}

		cout << "Test #" << testNumber++ << ": ";
		//State modifiedState = reduceStates(startState, 19);
		//printState(modifiedState);
		State lastState = BFS(startState);
		backTrackPos(lastState);
		cout << endl;
	}

	file.close();
	file.open("tests5.txt");

	if (bEnd) goto END;
	bEnd = true;
	goto BEGIN;
	END:

	system("PAUSE");

	return 0;
}