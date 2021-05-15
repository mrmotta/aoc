#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

int getID (string ticket);

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #05       |" << endl
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

	vector<int> list;				// Tickets list
	string tmpTicket;				// Temporary value for parsing input
	int min, max;					// Useful for the processing part

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	while (input >> tmpTicket)
		list.push_back(getID(tmpTicket));

	input.close();
	sort(list.begin(), list.end());
	
	cout << "Processing input..." << endl;

	result[0] = list[list.size()-1];

	for (int index = 0; index < list.size()-1; ++ index)
		if (list[index] == list[index+1] - 2) {
			result[1] = list[index] + 1;
			break;
		}

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}

// From ticket to seat ID
int getID (string ticket) {
	string rowCode = ticket.substr(0, 7);
	string columnCode = ticket.substr(7, 9);
	int rowNumber = 0;
	int columnNumber = 0;

	for (int index = 0, row = 128; index < 8; ++ index) {
		row /= 2;
		if (rowCode[index] == 'B')
			rowNumber += row;
	}

	for (int index = 0, column = 8; index < 3; index ++) {
		column /= 2;
		if (columnCode[index] == 'R')
			columnNumber += column;
	}

	return rowNumber * 8 + columnNumber;
}