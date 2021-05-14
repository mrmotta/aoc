#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #01       |" << endl
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
	bool foundResult[] = {false, false};
	vector<int> list;			// The list of all the numbers read from the file
	int tmpNumber;				// Only a temporary value used when reading

	cout << endl;
	cout << "Reading and parsing file content..." << endl;

	while (input >> tmpNumber)
		list.push_back(tmpNumber);

	input.close();
	
	cout << "Processing input..." << endl;

	// Bruteforce method: checking every possible combination of two and three (order doesn't matter) untill I find the right one

	for (int index1 = 0; index1 < list.size(); ++ index1)
		for (int index2 = index1 + 1; index2 < list.size(); ++ index2) {
			if (!foundResult[0] && list[index1] + list[index2] == 2020) {
				result[0] = list[index1] * list[index2];
				foundResult[0] = true;
			}
			if (!foundResult[1])
				for (int index3 = index2 + 1; index3 < list.size(); ++ index3)
					if (list[index1] + list[index2] + list[index3] == 2020) {
						result[1] = list[index1] * list[index2] * list[index3];
						foundResult[1] = true;
					}
			if (foundResult[0] && foundResult[1])
				goto print;
		}

	print:
	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}
