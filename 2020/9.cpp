#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

vector<int64_t> list;

bool isNotValid (int index);

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #09       |" << endl
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

	string tmp;						// Temporary string used to parse the input
	int64_t count;					// Temporary variable used to sum numbers for part 2
	int64_t min, max;				// Min and max of the interval found in part 2

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	while (input >> tmp)
		list.push_back(stoll(tmp));

	input.close();
	
	cout << "Processing input..." << endl;

	// Part 1
	for (int index = 25; index < list.size(); ++ index)
		if (isNotValid(index)) {
			result[0] = list[index];
			break;
		}
	
	// Part 2
	for (int first = 0, last; first < list.size()-1; ++ first) {
		count = 0;
		// Checking if this is the starting of the requested part
		for (last = first; count < result[0]; ++ last)
			count += list[last];
		if (count == result[0]) {
			// Let's find min and max in this interval
			min = list[first];
			max = list[first];
			for (int index = first; index < last; ++ index) {
				if (list[index] < min)
					min = list[index];
				else if (list[index] > max)
					max = list[index];
			}
			result[1] = min + max;
			break;
		}
	}

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}

bool isNotValid (int index) {
	for (int i = index-25; i < index-1; ++ i)
		for (int j = i+1; j < index; ++ j)
			if (list[i] + list[j] == list[index])
				return false;
	return true;
}