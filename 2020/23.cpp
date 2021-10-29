#include <iostream>
#include <fstream>
#include <vector>

#define PART_1_ITEMS 9
#define PART_1_ITERATIONS 100
#define PART_2_ITEMS 1000000
#define PART_2_ITERATIONS 10000000

using namespace std;

void prepareList(vector<unsigned long int> &list, string line, const int part);
void process (vector<unsigned long int> &list, int start, const long int loopIterations, const long int maxPosition);

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #23       |" << endl
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
	string line;
	vector<unsigned long int> list(PART_2_ITEMS);
	int current;
	int start;

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	input >> line;
	start = line[0] - '1';
	input.close();

	cout << "Processing input..." << endl;

	// Part 1

	prepareList(list, line, 1);

	process(list, start, PART_1_ITERATIONS, PART_1_ITEMS - 1);

	current = list[0];
	for (int index = 1; index < PART_1_ITEMS; ++ index) {
		result[0] = result[0] * 10 + current + 1;
		current = list[current];
	}

	// Part 2

	prepareList(list, line, 2);

	current = list[0];
	for (int index = 1; index < PART_1_ITEMS; ++ index) {
		result[1] = result[1] * 10 + current + 1;
		current = list[current];
	}

	process(list, start, PART_2_ITERATIONS, PART_2_ITEMS - 1);

	result[1] = (unsigned long long int) (list[0] + 1) * (list[list[0]] + 1);

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}

void prepareList(vector<unsigned long int> &list, string line, const int part) {

	int start = line[0] - '1';
	long int current = start;

	for (int index = 1; index < line.length(); ++ index) {
		list[current] = line[index] - '1';
		current = list[current];
	}

	if (part == 2) {
		for (long number = PART_1_ITEMS; number < PART_2_ITEMS; ++ number) {
			list[current] = number;
			current = number;
		}
	}

	list[current] = start;
}

void process (vector<unsigned long int> &list, int start, const long int loopIterations, const long int maxPosition) {

	long int position;
	long int current = start;
	long int tmpList;

	for (int index = 0; index < loopIterations; ++ index) {

		// Picking up cups
		tmpList = list[current];
		list[current] = list[list[list[list[current]]]];

		// Calculating position where to put cups
		position = current;
		do {
			-- position;
			if (position == -1)
				position = maxPosition;
		} while (tmpList == position || list[tmpList] == position || list[list[tmpList]] == position);

		// Putting back cups
		list[list[list[tmpList]]] = list[position];
		list[position] = tmpList;

		// Moving to next cup
		current = list[current];
	}
}