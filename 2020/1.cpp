#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main (int argc, char *argv[]) {

	cout << endl
		<< "+----------------+" << endl
		<< "| Advent of Code |" << endl
		<< "+----------------+" << endl
		<< "|     Day 01     |" << endl
		<< "+----------------+" << endl;

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

	int64_t results[2] = {0};
	vector<int> list;			// The list of all the numbers read from the file
	int tmpNumber;				// Only a temporary value used when reading

	cout << endl;
	cout << "Reading and parsing file content..." << endl;

	while (input >> tmpNumber)
		list.push_back(tmpNumber);

	input.close();
	
	cout << "Processing input..." << endl;

	// Bruteforce method O(n^2): checking every possible combination of two (order doesn't matter) untill I find the right one

	for (int index1 = 0; index1 < list.size(); index1 ++)
		for (int index2 = index1 + 1; index2 < list.size(); index2 ++)
			if (list[index1] + list[index2] == 2020) {
				results[0] = list[index1] * list[index2];
				break;
			}

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << results[0] << endl;

	// Bruteforce method O(n^3): checking every possible combination of three (order doesn't matter) untill I find the right one
	
	for (int index1 = 0; index1 < list.size(); index1 ++)
		for (int index2 = index1 + 1; index2 < list.size(); index2 ++)
			for (int index3 = index2 + 1; index3 < list.size(); index3 ++)
				if (list[index1] + list[index2] + list[index3] == 2020) {
					results[1] = list[index1] * list[index2] * list[index3];
					break;
				}

	cout << "- Part 2: " << results[1] << endl << endl;

	return 0;
}
