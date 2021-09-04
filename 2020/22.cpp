#include <iostream>
#include <fstream>
#include <queue>
#include <unordered_set>

using namespace std;

int combat (queue<int> deck[]);
int recursiveCombat (queue<int> deck[]);
void editDeck (queue<int> deck[], int winner);
int64_t calculateResult (queue<int> deck[], int winner);

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #22       |" << endl
		<< "+---------------------+" << endl;

	// Checking arguments and opening file

	ifstream input;

	if (argc != 2) {
		cout << endl << "ERROR: you should pass the file path." << endl;
		return 1;
	}

	input.open(argv[1], ios::in);

	if (!input) {
		cout << endl << "ERROR: file not found or unreadable." << endl;
		return 2;
	}

	// Preparing the environment for the execution

	int64_t result[2] = {0};

	int deckNumber = 0;
	queue<int> originalDeck[2];
	queue<int> deck[2];
	string tmpLine;

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	// Fixing initial position for reading the input
	input >> tmpLine;
	input >> tmpLine;

	while (input >> tmpLine) {
		if (tmpLine.front() == 'P') {
			input >> tmpLine;
			input >> tmpLine;
			++ deckNumber;
		}
		originalDeck[deckNumber].push(stoi(tmpLine));
	}

	input.close();

	cout << "Processing input..." << endl;

	// Part 1
	deck[0] = originalDeck[0];
	deck[1] = originalDeck[1];
	result[0] = calculateResult(deck, combat(deck));

	// Part 2
	deck[0] = originalDeck[0];
	deck[1] = originalDeck[1];
	result[1] = calculateResult(deck, recursiveCombat(deck));

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}

int combat (queue<int> deck[]) {

	while (!deck[0].empty() && !deck[1].empty()) {

		if (deck[0].front() > deck[1].front())
			editDeck(deck, 0);
		else
			editDeck(deck, 1);
	}

	if (!deck[0].empty())
		return 0;
	
	return 1;
}

int recursiveCombat (queue<int> deck[]) {

	int64_t hash;
	queue<int> tmpDeck[2];
	queue<int> subDeck[2];
	unordered_set<int64_t> configurations;

	while (!deck[0].empty() && !deck[1].empty()) {

		tmpDeck[0] = deck[0];
		tmpDeck[1] = deck[1];

		hash = calculateResult(tmpDeck, 0) * calculateResult(tmpDeck, 1);

		if (configurations.find(hash) != configurations.end())
			return 0;

		configurations.insert(hash);

		if (deck[0].size() > deck[0].front() && deck[1].size() > deck[1].front()) {

			tmpDeck[0] = deck[0];
			tmpDeck[1] = deck[1];

			tmpDeck[0].pop();
			tmpDeck[1].pop();

			subDeck[0] = queue<int>();
			subDeck[1] = queue<int>();

			for (int index = 0; index < deck[0].front(); ++ index) {
				subDeck[0].push(tmpDeck[0].front());
				tmpDeck[0].pop();		
			}

			for (int index = 0; index < deck[1].front(); ++ index) {
				subDeck[1].push(tmpDeck[1].front());
				tmpDeck[1].pop();
			}

			editDeck(deck, recursiveCombat(subDeck));

		} else {

			if (deck[0].front() > deck[1].front())
				editDeck(deck, 0);
			else
				editDeck(deck, 1);
		}
	}

	if (!deck[0].empty())
		return 0;

	return 1;
}

void editDeck (queue<int> deck[], int winner) {

	if (winner == 0) {
		deck[0].push(deck[0].front());
		deck[0].push(deck[1].front());
	} else {
		deck[1].push(deck[1].front());
		deck[1].push(deck[0].front());
	}

	deck[0].pop();
	deck[1].pop();
}

int64_t calculateResult (queue<int> deck[], int winner) {

	int64_t result = 0;
	
	for (int multiplier = deck[winner].size(); multiplier > 0; -- multiplier) {
		result += deck[winner].front() * multiplier;
		deck[winner].pop();
	}

	return result;
}