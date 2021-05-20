#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #10       |" << endl
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

	class adaptor_t {
		public:
		int output;					// The number itself
		int64_t cost;				// The cost used for part 2

		public:						// General setter
		void set (string number) {
			output = stoi(number);
			cost = 0;
		}
	};

	vector<adaptor_t> list;

	string tmpNumber;				// Temporary string used to parse the input
	adaptor_t tmpAdaptor;			// Temporary adaptor used to parse the input
	int differences[2] = {0};		// Used for part 1

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	// Inserting the starting point (charging outlet)
	tmpAdaptor.output = 0;
	tmpAdaptor.cost = 0;
	list.push_back(tmpAdaptor);

	// Reading all the other adapters
	while (input >> tmpNumber) {
		tmpAdaptor.set(tmpNumber);
		list.push_back(tmpAdaptor);
	}

	input.close();

	sort(list.begin(), list.end(),
		[] (adaptor_t adaptor1, adaptor_t adaptor2) {
			return adaptor1.output < adaptor2.output;
		});
	
	cout << "Processing input..." << endl;

	// Part 1
	// Checking every case
	for (int index = 0; index < list.size(); ++ index)
		if (list[index+1].output == list[index].output+1)
			++ differences[0];
		else
			++ differences[1];

	// Calculating the final result for part 1
	result[0] = differences[0] * differences[1];

	// Part 2
	// Calculating the costs (from the back)
	list[list.size()-1].cost = 1;
	for (int index = list.size()-2; index >= 0; -- index) {
		for (int i = index+1; i < list.size() && list[i].output <= list[index].output+3; ++ i)
			list[index].cost += list[i].cost;
	}

	// Getting the result
	result[1] = list[0].cost;

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}