#include <iostream>
#include <vector>

using namespace std;

int get_sum(vector<int>& S, int i, int j) // ends of out interval
{
	return S[j + 1] - S[i];
}

int opt_dist(vector<int> numbers, unsigned D) {
	// we create a table of prefix sum.
	vector<int> sum = vector<int>(numbers.size() + 1); // there is one element more.
	sum[0] = 0; // sum[i] = t[0] + t[1] + ... + t[i - 1]

	for (int i = 1; i <= numbers.size(); i++) {
		sum[i] = sum[i - 1] + numbers[i - 1];
	}

	// now we iterate through all intervals [0, k], [1, D + 1], [2, D + 2]...
	int min = numbers.size() + 1;
	for (int i = 0; i < numbers.size() - D; i++) {
		int costOfUnsetting = get_sum(sum, 0, i - 1);
		int costOfSetting = D - get_sum(sum, i, i + D - 1);
		int costOfUnsettings2 = get_sum(sum, i + D, numbers.size() - 1);

		int cost = costOfUnsetting + costOfSetting + costOfUnsettings2;
		if (cost < min)
			min = cost;
	}

	return min;
}

int main()
{
	vector<int> numbers = { 1, 0, 1, 1, 0, 0, 1, 1, 0, 0};
	int result = opt_dist(numbers, 5);

	for (int i = 0; i < numbers.size(); i++)
		cout << "D = " << i << ": " << opt_dist(numbers, i) << endl;

	//cout << result;

	system("PAUSE");

	return 0;
}