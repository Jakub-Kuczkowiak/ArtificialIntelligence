#include <iostream>
#include <fstream>
#include <string>
#include <unordered_map>

using namespace std;

class Node
{
public:
	unordered_map<char, Node*> children;
	bool isFullWord;

	Node() {
		children = unordered_map<char, Node*>();
		isFullWord = false;
	}
};

void AddWord(Node* node, string word, int index)
{
	if (index == word.length()) {
		node->isFullWord = true;
	}

	if (index < word.length()) {
		unordered_map<char, Node*>::const_iterator iter = node->children.find(word[index]);
		if (iter == node->children.end()) {
			// not found
			Node* newNode = new Node();
			node->children[word[index]] = newNode;
			AddWord(newNode, word, index + 1);
		}
		else {
			// found
			AddWord(iter->second, word, index + 1);
		}
	}
}

Node root;
vector< pair<int, string> > dp;

// For correctness we assume that maximizeQuerry maximies output for querries of length smaller than querry.size() - index
pair<int, string> maximizeQuerry(Node* node, string querry, int index) {
	// we check if we counted it already using dp
	if (dp[index].first != -2) {
		return dp[index];
	}

	// base case: where length is == 0
	if (index == querry.size()) {
		dp[index] = pair<int, string>(0, "");
		return dp[index];
	}

	int indexCopy = index;
	
	string word = "";
	int max = -1;
	string maxString = "";
	while (true) {
		if (node->isFullWord) {
			pair<int, string> p = maximizeQuerry(&root, querry, index);
			dp[index] = p;

			int p1 = p.first;
			if (p1 != -1) {
				p1 += word.length()*word.length();

				if (p1 > max) {
					max = p1;
					maxString = word + " " + p.second;
				}
			}
		}

		if (index == querry.size()) { // no more tree to serach
			dp[indexCopy] = pair<int, string>(max, maxString);
			return dp[indexCopy];
		}

		// we find proper path
		unordered_map<char, Node*>::const_iterator iter = node->children.find(querry[index]);
		if (iter == node->children.end()) { // no such children, so we end searching.
			dp[indexCopy] = pair<int, string>(max, maxString);
			return dp[indexCopy];
		}

		node = iter->second;
		index++;
		word += iter->first;
	}
	
	dp[indexCopy] = pair<int, string>(max, maxString);
	return dp[indexCopy];
}

int main()
{
	ifstream file;
	file.open("words_for_ai1.txt");

	root = Node();

	while (!file.eof()) {
		char word[255];
		file.getline(word, 255);
		AddWord(&root, word, 0);
	}

	file.close();
	root.isFullWord = false;
	
	ifstream tadeuszFile;
	tadeuszFile.open("pan_tadeusz_utf8.txt");

	ofstream answer;
	answer.open("answer.txt");
	answer.clear();

	while (!tadeuszFile.eof()) {
		string querry;
		tadeuszFile >> querry;

		dp = vector<pair<int, string>>(querry.size() + 1);
		for (int i = 0; i < querry.size(); i++)
			dp[i].first = -2; // means not_initialized

		pair<int, string> result = maximizeQuerry(&root, querry, 0);

		answer.write(result.second.c_str(), result.second.length());
		answer << '\n';
		dp.clear();
	}

	tadeuszFile.close();
	answer.close();

	//dp = vector<pair<int, string>>(querry.size() + 1);
	//for (int i = 0; i < querry.size(); i++)
	//	dp[i].first = -2; // means not_initialized

	//pair<int, string> result = maximizeQuerry(&root, querry, 0);
	//cout << result.first << endl;
	//cout << result.second << endl;

	system("PAUSE");

	return 0;
}