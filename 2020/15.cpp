#include <iostream>
#include <fstream>
#include <vector>

#define PART_1 2020
#define PART_2 30000000

using namespace std;

class list_t {
	private:
	vector<int32_t> list;
	int32_t lastNumber;

	public:
	void inizialize () {
		list.reserve(PART_2);
		for (int32_t index = 0; index < PART_2; ++ index)
			list[index] = -1;
	}

	void insert (int number, int32_t position) {
		list[number] = position;
		lastNumber = number;
	}

	int32_t last () { return lastNumber; }

	void process (int32_t position) {
		if (list[lastNumber] == -1) {
			list[lastNumber] = position - 1;
			lastNumber = 0;
		} else {
			int32_t tmp = lastNumber;
			lastNumber = position - 1 - list[lastNumber];
			list[tmp] = position - 1;
		}
	}
};

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #15       |" << endl
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

	list_t list;
	int partLength[2] = {PART_1, PART_2};
	string line, tmpNumber;
	int32_t index = 0;

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	input >> line;

	list.inizialize();

	while (line.find(",") != string::npos) {
		tmpNumber = line.substr(0, line.find(","));
		line = line.substr(tmpNumber.size()+1, line.size()-1);
		list.insert(stoi(tmpNumber), index);
		++ index;
	}

	list.insert(stoi(line), index);
	++ index;

	input.close();
	
	cout << "Processing input..." << endl;

	// Part 1
	do {
		list.process(index);
		++ index;
	} while (index < PART_1);
	result[0] = list.last();

	// Part 2
	do {
		list.process(index);
		++ index;
	} while (index < PART_2);
	result[1] = list.last();

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}