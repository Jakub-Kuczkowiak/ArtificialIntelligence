#include <iostream>
#include <fstream>
#include <vector>
#include <time.h>
#include <tuple>
#include <queue>
#include <unordered_map>

/*
Futher optimizations that could be done:
	- Use prority queue when doing backtracking at (i, j). Motivation for it is that some fields are more complex and
	  therefore lead to faster success/failure.
*/

#pragma warning(disable: 4996) // to be able to use strtok

using namespace std;

enum ChangeType {
	ROW,
	COLUMN
};

struct pairHash : public std::unary_function<pair<int, int>, std::size_t>
{
	std::size_t operator()(const pair<int, int>& p) const
	{
		return p.first + 10007 * p.second;
	}
};

typedef tuple<int, int, int> Change;
typedef pair<ChangeType, int> DeduceRequest;
typedef unordered_map<pair<int, int>, bool, pairHash> Dictionary;

void printPicture(vector< vector<int> > picture) {
	for (int i = 0; i < picture.size(); i++) {
		for (int j = 0; j < picture[i].size(); j++) {
			if (picture[i][j] == 0) cout << ".";
			else if (picture[i][j] == 1) cout << "#";
			else cout << "?";
		}

		cout << endl;
	}
}

// prefix[i].first = t[0] + t[1] + ... + t[i - 1] where prefix[i].first counts number of zeros
// prefix[i].second = t[0] + t[1] + ... + t[i - 1] where prefix[i].second counts number of ones
pair<vector<int>, vector<int>> prefix_row(vector< vector<int> >& picture, int rowIndex) {
	pair<vector<int>, vector<int>> prefix = pair<vector<int>, vector<int>>(vector<int>(picture[rowIndex].size() + 1), vector<int>(picture[rowIndex].size() + 1));

	prefix.first[0] = prefix.second[0] = 0;

	for (int i = 1; i <= picture[rowIndex].size(); i++) {
		prefix.first[i] = prefix.first[i - 1] + (picture[rowIndex][i - 1] == 0 ? 1 : 0);
		prefix.second[i] = prefix.second[i - 1] + (picture[rowIndex][i - 1] == 1 ? 1 : 0);
	}

	return prefix;
}

pair<vector<int>, vector<int>> prefix_column(vector< vector<int> >& picture, int columnIndex) {
	pair<vector<int>, vector<int>> prefix = pair<vector<int>, vector<int>>(vector<int>(picture.size() + 1), vector<int>(picture.size() + 1));

	prefix.first[0] = prefix.second[0] = 0;

	for (int i = 1; i <= picture.size(); i++) {
		prefix.first[i] = prefix.first[i - 1] + (picture[i - 1][columnIndex] == 0 ? 1 : 0);
		prefix.second[i] = prefix.second[i - 1] + (picture[i - 1][columnIndex] == 1 ? 1 : 0);
	}

	return prefix;
}

__inline int areZeros(pair<vector<int>, vector<int>>& prefix, int i, int j) // ends of out interval
{
	return (prefix.first[j + 1] - prefix.first[i]) > 0;
}

__inline int areOnes(pair<vector<int>, vector<int>>& prefix, int i, int j) // ends of out interval
{
	return (prefix.second[j + 1] - prefix.second[i]) > 0;
}

// generatePossibilitiesMask Invariant:
// if it is possible to generate at least possibility at state [elem, index], it sets dp[elem, index] = true and
// build proper mask for all [elem, index] and also sets single mask for singular prefix;
// otherwise it sets dp[elem, index] = false
// time complexity: O(elemMax * index * rowSize) <= O(size * size * size) = O(size^3).
bool generateRowPossibilitiesMask(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture,
	int rowId, int elem, int index, Dictionary& dp, pair<vector<int>, vector<int>>& prefix, pair<vector<int>, vector<int>>& mask)
{
	auto v = dp.find(pair<int, int>(elem, index));
	if (v != dp.end()) return v->second;

	bool result = false;
	int elemSize = rows[rowId][elem];
	int elemNumber = rows[rowId].size();

	for (int i = index; i <= columns.size() - elemSize; i++) {
		// check if we can place block of ones starting at index 'i'.
		if (i > 0 && picture[rowId][i - 1] == 1) break; // if previous is 1, we can't start placing here and after

		if (areZeros(prefix, i, i + elemSize - 1)) continue;

		// is it the last element?
		if (elem == elemNumber - 1) { // base case
			// we must check if there are ones in the end
			if (areOnes(prefix, i + elemSize, columns.size() - 1)) continue;

			for (int j = index; j < i; j++) // mark preblock as zeros
				mask.first[j] = 1;

			for (int j = i; j < i + elemSize; j++) // mark middle of mask for suffix as ones
				mask.second[j] = 1;

			for (int j = i + elemSize; j < columns.size(); j++) // mark rest with zeros possible
				mask.first[j] = 1;

			dp[pair<int, int>(elem, index)] = true;
			result = true;
		}
		else { // if not the last element: induction step
			if (i + elemSize >= columns.size() || picture[rowId][i + elemSize] == 1) continue;

			// check if we can generate at least one possibility for suffix starting at i + elemSize + 1 and if positive
			// mark the rest of mask starting at i + elemSize + 1 (due to induction hypothesis)
			if (!generateRowPossibilitiesMask(rows, columns, picture, rowId, elem + 1, i + elemSize + 1, dp, prefix, mask)) continue;

			for (int j = index; j < i; j++) // fill the prefix of mask with zeros
				mask.first[j] = 1;

			for (int j = i; j < i + elemSize; j++) // mark middle of mask for suffix as ones
				mask.second[j] = 1;

			mask.first[i + elemSize] = 1; // mark zero after block

			dp[pair<int, int>(elem, index)] = true;
			result = true;
		}
	}

	dp[pair<int, int>(elem, index)] = result;
	return result;
}

bool generateColumnPossibilitiesMask(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture,
	int columnId, int elem, int index, Dictionary& dp, pair<vector<int>, vector<int>>& prefix, pair<vector<int>, vector<int>>& mask) 
{
	auto v = dp.find(pair<int, int>(elem, index));
	if (v != dp.end()) return v->second;

	bool result = false;
	int elemSize = columns[columnId][elem];
	int elemNumber = columns[columnId].size();

	for (int i = index; i <= rows.size() - elemSize; i++) {
		// check if we can place block of ones starting at index 'i'.
		if (i > 0 && picture[i - 1][columnId] == 1) break; // if previous is 1, we can't start placing here and after

		if (areZeros(prefix, i, i + elemSize - 1)) continue;

		// is it the last element?
		if (elem == elemNumber - 1) { // base case
			// we must check if there are ones in the end
			if (areOnes(prefix, i + elemSize, rows.size() - 1)) continue;

			for (int j = index; j < i; j++) // mark preblock as zeros
				mask.first[j] = 1;

			for (int j = i; j < i + elemSize; j++) // mark middle of mask for suffix as ones
				mask.second[j] = 1;

			for (int j = i + elemSize; j < rows.size(); j++) // mark rest with zeros possible
				mask.first[j] = 1;

			dp[pair<int, int>(elem, index)] = true;
			result = true;
		}
		else { // if not the last element: induction step
			if (i + elemSize >= rows.size() || picture[i + elemSize][columnId] == 1) continue;

			// check if we can generate at least one possibility for suffix starting at i + elemSize + 1
			// mark the rest of mask starting at i + elemSize + 1 (due to induction hypothesis)
			if (!generateColumnPossibilitiesMask(rows, columns, picture, columnId, elem + 1, i + elemSize + 1, dp, prefix, mask)) continue;

			for (int j = index; j < i; j++) // fill the prefix of mask with zeros
				mask.first[j] = 1;

			for (int j = i; j < i + elemSize; j++) // mark middle of mask for suffix as ones
				mask.second[j] = 1;

			mask.first[i + elemSize] = 1; // mark zero after block

			result = true;
			dp[pair<int, int>(elem, index)] = true;
		}
	}

	dp[pair<int, int>(elem, index)] = result;
	return result;
}

bool deduceRow(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture, int rowId, vector<Change>& changes) {
	Dictionary dp;
	pair<vector<int>, vector<int>> prefix = prefix_row(picture, rowId);
	pair<vector<int>, vector<int>> mask = pair<vector<int>, vector<int>>(vector<int>(columns.size()), vector<int>(columns.size()));
	if (!generateRowPossibilitiesMask(rows, columns, picture, rowId, 0, 0, dp, prefix, mask))
		return false; // couldn't deduce due to some previous backtracking

	// explore mask
	for (int i = 0; i < mask.first.size(); i++) {
		if (picture[rowId][i] == -1) {
			if (mask.first[i] == 1 && mask.second[i] == 0) {
				Change change;
				std::get<0>(change) = rowId;
				std::get<1>(change) = i;
				std::get<2>(change) = 0;
				changes.push_back(change);
			}
			else if (mask.first[i] == 0 && mask.second[i] == 1) {
				Change change;
				std::get<0>(change) = rowId;
				std::get<1>(change) = i;
				std::get<2>(change) = 1;
				changes.push_back(change);
			}
		}
	}

	return true;
}

bool deduceColumn(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture, int columnId, vector<Change>& changes) {
	Dictionary dp;
	pair<vector<int>, vector<int>> prefix = prefix_column(picture, columnId);
	pair<vector<int>, vector<int>> mask = pair<vector<int>, vector<int>>(vector<int>(rows.size()), vector<int>(rows.size()));
	if (!generateColumnPossibilitiesMask(rows, columns, picture, columnId, 0, 0, dp, prefix, mask))
		return false; // couldn't deduce due to some previous backtracking

	// explore mask
	for (int i = 0; i < mask.first.size(); i++) {
		if (picture[i][columnId] == -1) {
			if (mask.first[i] == 1 && mask.second[i] == 0) {
				Change change;
				std::get<0>(change) = i;
				std::get<1>(change) = columnId;
				std::get<2>(change) = 0;
				changes.push_back(change);
			}
			else if (mask.first[i] == 0 && mask.second[i] == 1) {
				Change change;
				std::get<0>(change) = i;
				std::get<1>(change) = columnId;
				std::get<2>(change) = 1;
				changes.push_back(change);
			}
		}
	}

	return true;
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

bool deduce(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture, vector<Change>& changes, queue<DeduceRequest> q) {
	while (!q.empty())
	{
		DeduceRequest request = q.front();
		q.pop();

		vector<Change> localChanges;
		if (request.first == ROW) {
			if (!deduceRow(rows, columns, picture, request.second, localChanges)) {
				return false;
			}

			for (Change change : localChanges) {
				DeduceRequest req = DeduceRequest(COLUMN, std::get<1>(change));
				q.push(req);
			}
		}
		else {
			if (!deduceColumn(rows, columns, picture, request.second, localChanges)) {
				return false;

			}

			for (Change change : localChanges) {
				DeduceRequest req = DeduceRequest(ROW, std::get<0>(change));
				q.push(req);
			}
		}

		applyChanges(picture, localChanges);
		changes.insert(changes.end(), localChanges.begin(), localChanges.end());
	}

	// Use it to see progress.
	/*static long long count = 0;
	count++;
	if (count % 10000 == 0) {
		printPicture(picture);
		cout << endl;
	}*/

	return true;
}

bool backtrack(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture, int i, int j) {
	if (i == rows.size() - 1 && j == columns.size() - 1) return true;

	int nextI = i, nextJ = j + 1;
	if (j == columns.size() - 1) { // we check here if it is the end of row
		nextI = i + 1;
		nextJ = 0;
	}

	if (picture[i][j] == -1) {
		queue<DeduceRequest> q;
		q.push(DeduceRequest(ROW, i));
		q.push(DeduceRequest(COLUMN, j));

		vector<Change> zeroChanges;
		picture[i][j] = 0;
		bool zeroDeduce = deduce(rows, columns, picture, zeroChanges, q);
		rollbackChanges(picture, zeroChanges);

		vector<Change> oneChanges;
		picture[i][j] = 1;
		bool oneDeduce = deduce(rows, columns, picture, oneChanges, q);

		if (!zeroDeduce && !oneDeduce) { // both placements were incorrect
			rollbackChanges(picture, oneChanges);
		}
		else if (!zeroDeduce) { // only putting one has given optimistic result
			if (backtrack(rows, columns, picture, nextI, nextJ))
				return true;

			rollbackChanges(picture, oneChanges);
		}
		else if (!oneDeduce) { // only putting zero has given optimistic result
			rollbackChanges(picture, oneChanges);
			applyChanges(picture, zeroChanges);
			picture[i][j] = 0;
			if (backtrack(rows, columns, picture, nextI, nextJ))
				return true;

			rollbackChanges(picture, zeroChanges);
		}
		else { // both placements have given optimistic result
			// we don't cut any solutions but we prefer to backtrack more optimistic case first
			if (oneChanges.size() >= zeroChanges.size()) { 
				if (backtrack(rows, columns, picture, nextI, nextJ))
					return true;

				rollbackChanges(picture, oneChanges);
				applyChanges(picture, zeroChanges);
				picture[i][j] = 0;
				if (backtrack(rows, columns, picture, nextI, nextJ))
					return true;

				rollbackChanges(picture, zeroChanges);
			}
			else {
				rollbackChanges(picture, oneChanges);
				applyChanges(picture, zeroChanges);
				picture[i][j] = 0;
				if (backtrack(rows, columns, picture, nextI, nextJ))
					return true;

				rollbackChanges(picture, zeroChanges);
				applyChanges(picture, oneChanges);
				picture[i][j] = 1;
				if (backtrack(rows, columns, picture, nextI, nextJ))
					return true;

				rollbackChanges(picture, oneChanges);
			}
		}

		picture[i][j] = -1;
		return false;
	}
	else
	{
		return backtrack(rows, columns, picture, nextI, nextJ);
	}
}

bool solve(vector< vector<int> >& rows, vector< vector<int> >& columns, vector< vector<int> >& picture) {
	queue<DeduceRequest> q;
	for (int i = 0; i < rows.size(); i++)
		q.push(DeduceRequest(ROW, i));

	for (int j = 0; j < columns.size(); j++)
		q.push(DeduceRequest(COLUMN, j));

	vector<Change> changes;
	if (!deduce(rows, columns, picture, changes, q)) {
		rollbackChanges(picture, changes);
		return false;
	}

	return backtrack(rows, columns, picture, 0, 0);
}

bool isRowCorrect(vector< vector<int> >& rows, vector< vector<int> >& columns, int rowIndex, vector< vector<int> >& picture) {
	vector<int> blocks;
	for (int i = 0; i < columns.size(); i++) {
		if (picture[rowIndex][i] == -1) return false;

		if (picture[rowIndex][i] == 1) {
			int count = 1;
			int j;
			for (j = i + 1; j < columns.size(); j++) {
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

bool isColumnCorrect(vector< vector<int> >& rows, vector< vector<int> >& columns, int columnIndex, vector< vector<int> >& picture) {
	vector<int> blocks;
	for (int i = 0; i < rows.size(); i++) {
		if (picture[i][columnIndex] == -1) return false;

		if (picture[i][columnIndex] == 1) {
			int count = 1;
			int j;
			for (j = i + 1; j < rows.size(); j++) {
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
	for (int i = 0; i < rows.size(); i++) {
		if (!isRowCorrect(rows, columns, i, picture))
			return false;
	}

	for (int j = 0; j < columns.size(); j++) {
		if (!isColumnCorrect(rows, columns, j, picture))
			return false;
	}

	return true;
}

int main()
{
	ifstream file;
	file.open("tests.txt");

	double sumTime = 0;
	while (!file.eof()) {
		clock_t begin = clock();

		int n, m;
		file >> n >> m;

		vector< vector<int> > rows = vector< vector<int> >(n);
		vector< vector<int> > columns = vector< vector<int> >(m);

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

		if (!solve(rows, columns, picture)) {
			cout << "incorrect picture description" << endl;
			continue;
		}

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