#include <iostream>
#include <vector>
#include <time.h>
#include <algorithm>
#include <unordered_map>

using namespace std;

enum Color {
	Clubs = 0, // trefl
	Diamonds = 1, // karo
	Hearts = 2, // kier
	Spades = 3, // wino (pik)
};

enum Uklad {
	Poker = 8,
	Kareta = 7,
	Full = 6,
	Kolor = 5,
	Strit = 4,
	Trojka = 3,
	DwiePary = 2,
	Para = 1,
	WysokaKarta = 0
};

class Card
{
public:
	char symbol;
	Color color;

	Card(char symbol, Color color) {
		this->symbol = symbol;
		this->color = color;
	}
};

vector<Card> random_blotkarz() {
	vector<Card> cards = vector<Card>();

	for (int i = 0; i < 5; i++) {
		// random color
		Color color;
		char symbol;

		int clr = rand() % 4;
		if (clr == 0)  color = Clubs;
		else if (clr == 1) color = Diamonds;
		else if (clr == 2) color = Hearts;
		else if (clr == 3) color = Spades;

		// random symbol
		clr = 2 + rand() % 9;
		symbol = '0' + clr;

		bool bShouldAdd = true;
		for (auto it : cards) {
			if (it.color == color && it.symbol == symbol) {
				bShouldAdd = false;
				break;
			}
		}

		if (!bShouldAdd) {
			i--;
			continue;
		}

		cards.push_back(Card(symbol, color));
	}

	return cards;
}

vector<Card> random_figurant() {
	vector<Card> cards = vector<Card>();

	for (int i = 0; i < 5; i++) {
		// random color
		Color color;
		char symbol;

		int clr = rand() % 4;
		if (clr == 0)  color = Clubs;
		else if (clr == 1) color = Diamonds;
		else if (clr == 2) color = Hearts;
		else if (clr == 3) color = Spades;

		// random symbol
		clr = rand() % 4;
		if (clr == 0) symbol = 'W';
		else if (clr == 1) symbol = 'D';
		else if (clr == 2) symbol = 'K';
		else if (clr == 3) symbol = 'A';

		bool bShouldAdd = true;
		for (auto it : cards) {
			if (it.color == color && it.symbol == symbol) {
				bShouldAdd = false;
				break;
			}
		}

		if (!bShouldAdd) {
			i--;
			continue;
		}

		cards.push_back(Card(symbol, color));
	}

	return cards;
}

bool sortBySymbol(Card a, Card b) { return a.symbol < b.symbol; }

Uklad ukladBlotkarz(vector<Card>& blotkarz) {
	unordered_map<char, int> symbolMap = unordered_map<char, int>();
	unordered_map<Color, int> colorMap = unordered_map<Color, int>();
	for (int i = '2'; i <= '10'; i++) {
		symbolMap[i] = 0;
	}

	colorMap[Color::Clubs] = 0;
	colorMap[Color::Diamonds] = 0;
	colorMap[Color::Hearts] = 0;
	colorMap[Color::Spades] = 0;

	for (int i = 0; i < 5; i++) {
		symbolMap[blotkarz[i].symbol]++;
		colorMap[blotkarz[i].color]++;
	}

	sort(blotkarz.begin(), blotkarz.end(), sortBySymbol);

	// is it poker?
	if (symbolMap[blotkarz[0].color] == 5) { // 5 kart w tym samym kolorze
		if (blotkarz[0].symbol == blotkarz[1].symbol - 1 && blotkarz[1].symbol == blotkarz[2].symbol - 1 &&
			blotkarz[2].symbol == blotkarz[3].symbol - 1 && blotkarz[3].symbol == blotkarz[4].symbol - 1) {
			return Poker;
		}
		else {
			return Kolor; // to juz musi byc kolor i nie moze byc kareta czy full!
		}
	}

	// czy kareta?
	if (symbolMap[blotkarz[0].symbol] == 4 || symbolMap[blotkarz[1].symbol] == 4) {
		return Kareta;
	}

	// czy full
	if (symbolMap[blotkarz[0].symbol] == 3 && symbolMap[blotkarz[4].symbol] == 2) {
		return Full;
	}

	// czy strit
	if (blotkarz[0].symbol == blotkarz[1].symbol - 1 && blotkarz[1].symbol == blotkarz[2].symbol - 1 &&
		blotkarz[2].symbol == blotkarz[3].symbol - 1 && blotkarz[3].symbol == blotkarz[4].symbol - 1)
		return Strit;

	// czy trojka
	if (symbolMap[blotkarz[0].symbol] == 3 || symbolMap[blotkarz[4].symbol] == 3)
		return Trojka;

	// czy dwie pary
	int pairs = 0;
	for (auto it : symbolMap) {
		if (it.second == 2) {
			pairs++;
		}
	}

	if (pairs == 2) return DwiePary;
	if (pairs == 1) return Para;

	return WysokaKarta;
}

Uklad ukladFigurant(vector<Card>& figurant) {
	unordered_map<char, int> symbolMap = unordered_map<char, int>();
	unordered_map<Color, int> colorMap = unordered_map<Color, int>();
	for (int i = '2'; i <= '10'; i++) {
		symbolMap[i] = 0;
	}

	colorMap[Color::Clubs] = 0;
	colorMap[Color::Diamonds] = 0;
	colorMap[Color::Hearts] = 0;
	colorMap[Color::Spades] = 0;

	for (int i = 0; i < 5; i++) {
		symbolMap[figurant[i].symbol]++;
		colorMap[figurant[i].color]++;
	}

	sort(figurant.begin(), figurant.end(), sortBySymbol);

	// czy kareta?
	if (symbolMap[figurant[0].symbol] == 4 || symbolMap[figurant[1].symbol] == 4) {
		return Kareta;
	}

	// czy full
	if (symbolMap[figurant[0].symbol] == 3 && symbolMap[figurant[4].symbol] == 2) {
		return Full;
	}

	// czy kolor?
	if (symbolMap[figurant[0].color] == 5) { // 5 kart w tym samym kolorze
		return Kolor;
	}

	// czy trojka
	if (symbolMap[figurant[0].symbol] == 3 || symbolMap[figurant[4].symbol] == 3)
		return Trojka;

	// czy dwie pary
	int pairs = 0;
	for (auto it : symbolMap) {
		if (it.second == 2) {
			pairs++;
		}
	}

	if (pairs == 2) return DwiePary;
	if (pairs == 1) return Para;

	return WysokaKarta;
}

int main()
{
	srand(time(NULL));

	cout << "Podaj liczbe eksperymentow: ";

	int n;
	cin >> n;

	int blotkarzWins = 0, figurantWins = 0;
	for (int i = 0; i < n; i++) {
		vector<Card> blotkarz = random_blotkarz();
		vector<Card> figurant = random_figurant();

		if (ukladBlotkarz(blotkarz) > ukladFigurant(figurant)) blotkarzWins++;
		else figurantWins++;
	}

	cout << "Stosunek dla " << n << " gier:" << endl;
	cout << "Blotkarz wygral: " << blotkarzWins << " razy" << endl;
	cout << "Figurant wygral: " << figurantWins << " razy" << endl;

	system("PAUSE");

	return 0;
}

/*
Stosunek dla 1000 gier:
Blotkarz wygral : 101 razy
Figurant wygral : 899 razy
*/