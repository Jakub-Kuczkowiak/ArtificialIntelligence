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
	//char lastDirection;

	friend bool operator==(const State& s1, const State& s2);
};

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

struct positionHash : public std::unary_function<Position, std::size_t>
{
	std::size_t operator()(const Position& position) const
	{
		return position.first * 523 + position.second * 37;
	}
};

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

vector<Position> getPositions(State state, Position pos) {

	vector<Position> result;
	// get all possible moves here
	// can we move up, left, right down
	// left
	Position ptmp = Position(pos.first - 1, pos.second);
	if (plansza[ptmp.first][ptmp.second] != Field::Wall && isBox(state, ptmp.first, ptmp.second) == -1)
		result.push_back(ptmp);

	//right
	ptmp = Position(pos.first + 1, pos.second);
	if (plansza[ptmp.first][ptmp.second] != Field::Wall && isBox(state, ptmp.first, ptmp.second) == -1)
		result.push_back(ptmp);

	ptmp = Position(pos.first, pos.second + 1);
	if (plansza[ptmp.first][ptmp.second] != Field::Wall && isBox(state, ptmp.first, ptmp.second) == -1)
		result.push_back(ptmp);

	ptmp = Position(pos.first, pos.second - 1);
	if (plansza[ptmp.first][ptmp.second] != Field::Wall && isBox(state, ptmp.first, ptmp.second) == -1)
		result.push_back(ptmp);

	return result;
}

#include <unordered_set>
bool playerBFS(const State& startState, Position p1, Position p2) {
	queue<Position> q;
	q.push(p1);
	unordered_set<Position, positionHash> dp;

	while (!q.empty()) {
		Position pos = q.front();
		q.pop();
		dp.insert(pos);

		if (pos == p2) {
			return true;
		}

		vector<Position> positions = getPositions(startState, pos);
		for (auto& position : positions) {
			if (position == p2) return true;
			if (dp.find(position) == dp.end()) {
				dp.insert(position);
				q.push(position);
			}
		}
	}

	return false;
}

bool operator==(const State& s1, const State& s2) {
	// two states are equal if there exists BFS movement from one to the other such that:
	if (s1.boxes != s2.boxes) return false;

	// we try bfs now
	return playerBFS(s1, s1.magazinier, s2.magazinier);
}

unordered_set<Position, positionHash> BFSMovement(State& startState) {
	unordered_set<Position, positionHash> dpPos;
	queue<Position> q;
	q.push(startState.magazinier);
	dpPos.insert(startState.magazinier);

	while (!q.empty()) {
		Position pos = q.front();
		q.pop();

		vector<Position> res = getPositions(startState, pos);
		for (Position& position : res) {
			if (dpPos.find(position) == dpPos.end()) { // no such element 
				dpPos.insert(position);
				q.push(position);
			}
		}
	}

	return dpPos;
}

vector<State> getMoves(State& state) {
	vector<State> availableStates;

	unordered_set<Position, positionHash> availablePositions = BFSMovement(state);

	// for each box we must now find if we can move it!
	for (int i = 0; i < state.boxes.size(); i++) {
		// we must see if we can stand at each side of the box and push it
		// let's take a look at all fields around this field.
		Position& box = state.boxes[i];
		bool bLeftWall = plansza[box.first][box.second - 1] == Field::Wall;
		bool bRightWall = plansza[box.first][box.second + 1] == Field::Wall;
		bool bUpWall = plansza[box.first - 1][box.second] == Field::Wall;
		bool bDownWall = plansza[box.first + 1][box.second] == Field::Wall;

		bool bLeftBox = isBox(state, box.first, box.second - 1) != -1;
		bool bRightBox = isBox(state, box.first, box.second + 1) != -1;
		bool bUpBox = isBox(state, box.first - 1, box.second) != -1;
		bool bDownBox = isBox(state, box.first + 1, box.second) != -1;

		if (availablePositions.find(Position(box.first, box.second - 1)) != availablePositions.end()) {
			// we must see if we can move it right
			if (!bRightBox && !bRightWall && !isBoxSurroundedWithoutGoal(box.first, box.second + 1)) {
				// add this state
				State newState = state;
				newState.prevState = &dp[state];
				newState.magazinier = Position(box.first, box.second);
				newState.boxes[i].second += 1;
				availableStates.push_back(newState);
			}
		}

		if (availablePositions.find(Position(box.first, box.second + 1)) != availablePositions.end()) {
			// we must see if we can move it right
			if (!bLeftBox && !bLeftWall && !isBoxSurroundedWithoutGoal(box.first, box.second - 1)) {
				// add this state
				State newState = state;
				newState.prevState = &dp[state];
				newState.magazinier = Position(box.first, box.second);
				newState.boxes[i].second -= 1;
				availableStates.push_back(newState);
			}
		}

		if (availablePositions.find(Position(box.first - 1, box.second)) != availablePositions.end()) {
			// we must see if we can move it right
			if (!bDownBox && !bDownWall && !isBoxSurroundedWithoutGoal(box.first + 1, box.second)) {
				// add this state
				State newState = state;
				newState.prevState = &dp[state];
				newState.magazinier = Position(box.first, box.second);
				newState.boxes[i].first += 1;
				availableStates.push_back(newState);
			}
		}

		if (availablePositions.find(Position(box.first + 1, box.second)) != availablePositions.end()) {
			// we must see if we can move it right
			if (!bUpBox && !bUpWall && !isBoxSurroundedWithoutGoal(box.first - 1, box.second)) {
				// add this state
				State newState = state;
				newState.prevState = &dp[state];
				newState.magazinier = Position(box.first, box.second);
				newState.boxes[i].first -= 1;
				availableStates.push_back(newState);
			}
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
		//moves += lastState.lastDirection;
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
	dp[startState] = startState;

	while (!q.empty()) {
		State state = q.front();
		q.pop();

		if (correctState(state)) {
			return state;
		}

		vector<State> states = getMoves(state);

		for (int i = 0; i < states.size(); i++) {
			if (dp.find(states[i]) == dp.end()) {
				dp[states[i]] = states[i];
				q.push(states[i]);
			}
		}
	}
	
	State stateRet;
	return stateRet;
}

State BFS_A(State startState) {
	priority_queue<State, vector<State>, compare> q;
	q.push(startState);
	dp[startState] = startState;

	while (!q.empty()) {
		State state = q.top();
		q.pop();

		if (correctState(state)) {
			return state;
		}

		vector<State> states = getMoves(state);

		for (int i = 0; i < states.size(); i++) {
			if (dp.find(states[i]) == dp.end()) {
				dp[states[i]] = states[i];
				q.push(states[i]);
			}
		}
	}

	State stateRet;
	return stateRet;
}

int main() {
	ifstream file;
	file.open("tests_new.txt");

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
		//backTrackPos(lastState);

		cout << endl;
	}

	cout << endl;
	system("PAUSE");

	return 0;
}