#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <tuple>

#pragma warning(disable: 4996) // to be able to use strtok

using namespace std;

int n, m;

typedef tuple<int, int, int> Change;

//enum Field {
//	UNKNOWN,
//	NOT_DETERMINED,
//	SET,
//	UNSET
//};

vector< vector<int> > generateRowPossibilities(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture, int rowId, int elem, int index);
vector< vector<int> > generateColumnPossibilities(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture, int columnId, int elem, int index);

bool deduceRow(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture, int rowId, vector<Change>& changes) {
	vector< vector<int> > possibilities = generateRowPossibilities(rows, columns, picture, rowId, 0, 0);
	if (possibilities.empty()) return false; // couldn't deduce due to some previous backtracking

	vector<int> mask = possibilities[0];

	// we build mask
	for (auto& possibility : possibilities) {
		for (int i = 0; i < possibility.size(); i++) {
			if (mask[i] != -1) {
				if (mask[i] == !possibility[i])
					mask[i] = -1;
			}
		}
	}

	for (int i = 0; i < mask.size(); i++) {
		if (mask[i] != -1) {
			Change change;
			std::get<0>(change) = rowId;
			std::get<1>(change) = i;
			std::get<2>(change) = mask[i];
			changes.push_back(change);
		}
	}

	return true;
}

bool deduceColumn(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture, int columnId, vector<Change>& changes) {
	vector< vector<int> > possibilities = generateColumnPossibilities(rows, columns, picture, columnId, 0, 0);
	if (possibilities.empty()) return false; // couldn't deduce due to some previous backtracking

	vector<int> mask = possibilities[0];

	// we build mask
	for (auto& possibility : possibilities) {
		for (int i = 0; i < possibility.size(); i++) {
			if (mask[i] != -1) {
				if (mask[i] == !possibility[i])
					mask[i] = -1;
			}
		}
	}

	for (int i = 0; i < mask.size(); i++) {
		if (mask[i] != -1) {
			Change change;
			std::get<0>(change) = i;
			std::get<1>(change) = columnId;
			std::get<2>(change) = mask[i];
			changes.push_back(change);
		}
	}

	return true;
}

vector< vector<int> > generateRowPossibilities(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture, int rowId, int elem, int index) {
	vector< vector<int> > result;
	int elemSize = rows[rowId][elem];
	int elemNumber = rows[rowId].size();

	for (int i = index; i <= m - elemSize; i++) {
		// check if we can place block of ones starting at index 'i'.
		if (i > 0 && picture[rowId][i - 1] == 1) break; // if previous is 1, we can't start placing here and after

		bool canPlace = true;
		for (int j = i; j < i + elemSize; j++) {
			if (picture[rowId][j] == 0) {
				canPlace = false;
				break;
			}
		}

		if (!canPlace) continue;

		// is it the last element?
		if (elem == elemNumber - 1) { // base case
			// we must check if all fields are zeros
			for (int j = i + elemSize; j < m; j++) {
				if (picture[rowId][j] == 1) {
					canPlace = false;
					break;
				}
			}

			if (canPlace) {
				vector<int> v = vector<int>(i - index, 0); // fill the preblock

				// fill our block with 1s
				for (int j = 0; j < elemSize; j++)
					v.push_back(1);

				// fill the rest with 0s
				for (int j = i + elemSize; j < m; j++)
					v.push_back(0);

				result.push_back(v);
			}
		}
		else { // if not the last element: induction step
			if (i + elemSize >= m || picture[rowId][i + elemSize] == 1) canPlace = false;

			if (canPlace) {
				vector<int> v = vector<int>(i - index, 0); // fill the preblock

				// fill our block with 1s
				for (int j = 0; j < elemSize; j++)
					v.push_back(1);

				v.push_back(0); // add one zero after block

				vector< vector<int> > lastPossibilities = generateRowPossibilities(rows, columns, picture, rowId, elem + 1, i + elemSize + 1);
				for (vector<int> possibility : lastPossibilities) {
					vector<int> r = v;
					r.insert(r.end(), possibility.begin(), possibility.end());
					result.push_back(r);
				}
			}
		}
	}

	return result;
}

vector< vector<int> > generateColumnPossibilities(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture, int columnId, int elem, int index) {
	vector< vector<int> > result;
	int elemSize = columns[columnId][elem];
	int elemNumber = columns[columnId].size();

	for (int i = index; i <= n - elemSize; i++) {
		// check if we can place block of ones starting at index 'i'.
		if (i > 0 && picture[i - 1][columnId] == 1) break; // if previous is 1, we can't start placing here and after

		bool canPlace = true;
		for (int j = i; j < i + elemSize; j++) {
			if (picture[j][columnId] == 0) {
				canPlace = false;
				break;
			}
		}

		if (!canPlace) continue;

		// is it the last element?
		if (elem == elemNumber - 1) { // base case
									  // we must check if all fields are zeros
			for (int j = i + elemSize; j < n; j++) {
				if (picture[j][columnId] == 1) {
					canPlace = false;
					break;
				}
			}

			if (canPlace) {
				vector<int> v = vector<int>(i - index, 0); // fill the preblock

														   // fill our block with 1s
				for (int j = 0; j < elemSize; j++)
					v.push_back(1);

				// fill the rest with 0s
				for (int j = i + elemSize; j < n; j++)
					v.push_back(0);

				result.push_back(v);
			}
		}
		else { // if not the last element: induction step
			if (i + elemSize >= n || picture[i + elemSize][columnId] == 1) canPlace = false;

			if (canPlace) {
				vector<int> v = vector<int>(i - index, 0); // fill the preblock

				// fill our block with 1s
				for (int j = 0; j < elemSize; j++)
					v.push_back(1);

				v.push_back(0); // add one zero after block

				vector< vector<int> > lastPossibilities = generateColumnPossibilities(rows, columns, picture, columnId, elem + 1, i + elemSize + 1);
				for (vector<int> possibility : lastPossibilities) {
					vector<int> r = v;
					r.insert(r.end(), possibility.begin(), possibility.end());
					result.push_back(r);
				}
			}
		}
	}

	return result;
}

void applyChanges(vector< vector<int> >& picture, vector<Change>& changes) {
	for (Change& change : changes) {
		picture[std::get<0>(change)][std::get<1>(change)] = std::get<2>(change);
	}
}

void rollbackChanges(vector< vector<int> >& picture, vector<Change>& changes) {
	for (Change& change : changes) {
		picture[std::get<0>(change)][std::get<1>(change)] = -1;
	}
}

bool deduce(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture, vector<Change>& changes) {
	bool isConflict = false;
	for (int i = 0; i < rows.size(); i++) {
		if (!deduceRow(rows, columns, picture, i, changes)) {
			return false;
		}
	}

	applyChanges(picture, changes); // in the 2nd version we will need to rollback changes!

	for (int j = 0; j < columns.size(); j++) {
		if (!deduceColumn(rows, columns, picture, j, changes)) {
			return false;
		}
	}

	applyChanges(picture, changes);

	return true;
}


void solve(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture) {
	vector<Change> changes;
	if (deduce(rows, columns, picture, changes)) {
		//applyChanges(picture, changes);
	}
}

bool isRowCorrect(vector< vector<int> >& rows, int rowIndex, vector< vector<int> >& picture) {
	vector<int> blocks;
	for (int i = 0; i < m; i++) {
		if (picture[rowIndex][i] == -1) return false;

		if (picture[rowIndex][i] == 1) {
			int count = 1;
			int j;
			for (j = i + 1; j < m; j++) {
				if (picture[rowIndex][j] == -1) return false;

				if (picture[rowIndex][j] == 0) {
					break;
				}

				count++;
			}
			i = j;

			blocks.push_back(count);
		}
	}

	return (blocks == rows[rowIndex]);
}

bool isColumnCorrect(vector< vector<int> >& columns, int columnIndex, vector< vector<int> >& picture) {
	vector<int> blocks;
	for (int i = 0; i < n; i++) {
		if (picture[i][columnIndex] == -1) return false;

		if (picture[i][columnIndex] == 1) {
			int count = 1;
			int j;
			for (j = i + 1; j < n; j++) {
				if (picture[j][columnIndex] == -1) return false;

				if (picture[j][columnIndex] == 0) {
					break;
				}

				count++;
			}
			i = j;

			blocks.push_back(count);
		}
	}

	return (blocks == columns[columnIndex]);
}

bool isPictureCorrect(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture) {
	for (int i = 0; i < n; i++) {
		if (!isRowCorrect(rows, i, picture))
			return false;
	}

	for (int j = 0; j < m; j++) {
		if (!isColumnCorrect(columns, j, picture))
			return false;
	}

	return true;
}

void printPicture(vector< vector<int> > picture) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			if (picture[i][j] == 0) cout << ".";
			else cout << "#";
			//cout << picture[i][j];
		}

		cout << endl;
	}

	//cout << endl;
}

int main()
{
	srand(time(NULL));

	ifstream file;
	file.open("tests.txt");

	double sumTime = 0;
	while (!file.eof()) {
		clock_t begin = clock();

		file >> n >> m;

		vector< vector<int> > rows = vector< vector<int> >(n);
		vector< vector<int> > columns = vector< vector<int> >(m);

		//rows.clear(); columns.clear();

		char line[32];
		file.getline(line, 32);

		for (int i = 0; i < n; i++) {
			vector<int> row = vector<int>();

			file.getline(line, 32);

			char* word = strtok(line, " ");
			while (word != NULL) {
				int number = atoi(word);
				row.push_back(number);

				word = strtok(NULL, " ");
			}

			rows[i] = row;
		}

		for (int i = 0; i < m; i++) {
			vector<int> column = vector<int>();

			char line[32];
			file.getline(line, 32);

			char* word = strtok(line, " ");
			while (word != NULL) {
				int number = atoi(word);
				column.push_back(number);

				word = strtok(NULL, " ");
			}

			columns[i] = column;
		}

		vector< vector<int> > picture = vector< vector<int> >(n);

		for (int i = 0; i < n; i++) {
			picture[i] = vector<int>(m);
			for (int j = 0; j < m; j++) {
				picture[i][j] = -1;
			}
		}

		while (true) {
			solve(rows, columns, picture);
			if (isPictureCorrect(rows, columns, picture)) {
				break;
			}
		}

		//while (!solve(rows, columns, picture)) {
		//	for (int i = 0; i < n; i++) {
		//		for (int j = 0; j < m; j++) {
		//			picture[i][j] = rand() % 2;
		//		}
		//	}

		//	//cout << "new randomized picture" << endl;
		//};

		//Beep(5000, 2000);

		clock_t end = clock();

		printPicture(picture);
		sumTime += double(end - begin);
		cout << "time elapsed: " << double(end - begin) << endl << endl;
	}

	file.close();

	cout << "Total time: " << sumTime << endl;

	system("PAUSE");
	return 0;
}