#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <Windows.h> // only for sleep

using namespace std;

int n, m;

int get_sum(vector<int>& S, int i, int j) // ends of out interval
{
	if (i > j) return 0;
	return S[j + 1] - S[i];
}

int opt_dist(vector<int>& numbers, unsigned D) {
	// we create a table of prefix sum.
	vector<int> sum = vector<int>(numbers.size() + 1); // there is one element more.
	sum[0] = 0; // sum[i] = t[0] + t[1] + ... + t[i - 1]

	for (int i = 1; i <= numbers.size(); i++) {
		sum[i] = sum[i - 1] + numbers[i - 1];
	}

	// now we iterate through all intervals [0, k], [1, D + 1], [2, D + 2]...
	int min = numbers.size() + 1;
	for (int i = 0; i <= numbers.size() - D; i++) {
		int costOfUnsetting = get_sum(sum, 0, i - 1);
		int costOfSetting = D - get_sum(sum, i, i + D - 1);
		int costOfUnsettings2 = get_sum(sum, i + D, numbers.size() - 1);

		int cost = costOfUnsetting + costOfSetting + costOfUnsettings2;
		if (cost < min)
			min = cost;
	}

	return min;
}

int rowCorrectness(vector<int>& row, int rowIndex, vector< vector<int> >& picture) {
	return opt_dist(picture[rowIndex], row[rowIndex]);
}

int columnCorrectness(vector<int>& column, int columnIndex, vector< vector<int> >& picture) {
	// transform column to vector
	vector<int> columnT = vector<int>(n);
	for (int i = 0; i < n; i++) {
		columnT[i] = picture[i][columnIndex];
	}

	return opt_dist(columnT, column[columnIndex]);
}

bool isPictureCorrect(vector<int>& rows, vector<int>& columns, vector< vector<int> >& picture) {
	for (int i = 0; i < n; i++) {
		if (rowCorrectness(rows, i, picture) > 0)
			return false;
	}

	for (int j = 0; j < m; j++) {
		if (columnCorrectness(columns, j, picture) > 0)
			return false;
	}

	return true;
}

void printPicture(vector< vector<int> > picture) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < m; j++) {
			cout << picture[i][j];
		}

		cout << endl;
	}

	//cout << endl;
}

bool solve(vector<int>& rows, vector<int>& columns, vector< vector<int> >& picture) {
	clock_t begin = clock();

	bool correct = isPictureCorrect(rows, columns, picture);
	int tries = 0;
	while (!correct) {
		// with some probability we change random pixel
		if (rand() % 100 == 0) {
			int i = rand() % rows.size();
			int j = rand() % columns.size();
			picture[i][j] = !picture[i][j];
		}

		// we want random row or column?
		int choice = rand() % 2;
		if (choice == 0) { // row
			int randomRow = rand() % rows.size();
			if (rowCorrectness(rows, randomRow, picture) == 0) {
				// with some probability we destroy
				if (rand() % 100 > 0)
					continue;
			}

			// we choose column such that change of pixel [i, j] will make the best repair
			int maxRepair = 0;
			int maxJ = 0;
			for (int j = 0; j < m; j++) {
				int i = randomRow;

				int rC = rowCorrectness(rows, i, picture); // this one is always the same, move up
				int cC = columnCorrectness(columns, j, picture);

				picture[i][j] = !picture[i][j];

				int rR = rowCorrectness(rows, i, picture);
				int cR = columnCorrectness(columns, j, picture);

				picture[i][j] = !picture[i][j];

				int repair = rC - rR + cC - cR;
				if (repair > maxRepair) {
					maxRepair = repair;
					maxJ = j;
				}
			}

			// we fix that pixel 
			picture[randomRow][maxJ] = !picture[randomRow][maxJ];
		}
		else { // column
			int randomColumn = rand() % columns.size();
			if (columnCorrectness(rows, randomColumn, picture) == 0) {
				// with some probability we destroy
				if (rand() % 100 > 0)
					continue;
			}

			// we choose column such that change of pixel [i, j] will make the best repair
			int maxRepair = 0;
			int maxI = 0;
			for (int i = 0; i < m; i++) {
				int j = randomColumn;

				int rC = rowCorrectness(rows, i, picture); 
				int cC = columnCorrectness(columns, j, picture); // this one is always the same, move up

				picture[i][j] = !picture[i][j];

				int rR = rowCorrectness(rows, i, picture);
				int cR = columnCorrectness(columns, j, picture);

				picture[i][j] = !picture[i][j];

				int repair = rC - rR + cC - cR;
				if (repair > maxRepair) {
					maxRepair = repair;
					maxI = i;
				}
			}

			// we fix that pixel 
			picture[maxI][randomColumn] = !picture[maxI][randomColumn];
		}

		//printPicture(picture);

		correct = isPictureCorrect(rows, columns, picture);

		//Sleep(500);
		//getchar();

		clock_t end = clock();
		double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;

		//if (tries > 5000) return false;
	}

	return true;
}

int main()
{
	srand(time(NULL));
	
	ifstream file;
	file.open("picture.txt");

	double sumTime = 0;
	while (!file.eof()) {
		clock_t begin = clock();

		file >> n >> m;
		vector<int> rows = vector<int>(n);
		vector<int> columns = vector<int>(m);

		rows.clear(); columns.clear();

		for (int i = 0; i < n; i++) {
			int number;
			file >> number;
			rows.push_back(number);
		}

		for (int i = 0; i < m; i++) {
			int number;
			file >> number;
			columns.push_back(number);
		}

		vector< vector<int> > picture = vector< vector<int> >(n);

		for (int i = 0; i < n; i++) {
			picture[i] = vector<int>(m);
			for (int j = 0; j < m; j++) {
				picture[i][j] = rand() % 2;
			}
		}

		while (!solve(rows, columns, picture)) {
			for (int i = 0; i < n; i++) {
				for (int j = 0; j < m; j++) {
					picture[i][j] = rand() % 2;
				}
			}

			cout << "new randomized picture" << endl;
		};

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