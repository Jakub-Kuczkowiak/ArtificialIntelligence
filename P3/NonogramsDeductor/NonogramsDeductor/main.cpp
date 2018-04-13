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
			changes.push_back(change);
		}
	}

	return true;
}

vector< vector<int> > generateRowPossibilities(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture, int rowId, int elem, int index) {
	vector< vector<int> > result;
	int elemSize = rows[rowId][elem];
	int elemNumber = rows[rowId].size();

	for (int i = index; i < m - elemSize; i++) {
		// check if we can place block of ones starting at index 'i'.
		bool canPlace = true;
		for (int j = i; j < i + elemSize; j++) {
			if (picture[rowId][j] == 0) {
				canPlace = false;
				break;
			}
		}

		// is it the last element?
		if (elem == elemNumber - 1) { // base case
			// we must check if all fields are zeros
			for (int j = i + elemSize + 1; j < m; j++) {
				if (picture[rowId][j] == 1) {
					canPlace = false;
					break;
				}
			}

			if (canPlace) {
				vector<int> v = vector<int>(0, index - i); // fill the preblock

				// fill our block with 1s
				for (int j = 0; j < elemSize; j++)
					v.push_back(1);

				// fill the rest with 0s
				for (int j = i + elemSize + 1; j < m; j++)
					v.push_back(0);

				result.push_back(v);
			}
		}
		else { // if not the last element: induction step
			if (canPlace) {
				vector<int> v = vector<int>(0, index - i); // fill the preblock

				// fill our block with 1s
				for (int j = 0; j < elemSize; j++)
					v.push_back(1);

				v.push_back(0); // add one zero after block

				vector< vector<int> > lastPossibilities = generateRowPossibilities(rows, columns, picture, rowId, elem + 1, index + elemSize + 1);
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


void deduce(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture) {

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
			deduce(rows, columns, picture);
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

		//printPicture(picture);
		sumTime += double(end - begin);
		cout << "time elapsed: " << double(end - begin) << endl << endl;
	}

	file.close();

	cout << "Total time: " << sumTime << endl;

	system("PAUSE");
	return 0;
}