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
		<< "|       Day #03       |" << endl
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

	vector< vector<bool> > terrain;			// Terrain structure
	string line;							// Temporary string for reading file
	vector<bool> tmp;						// Temporary vector for parsing file lines (true if it's a tree '#')
	int currentPoint[5] = {0};				// Starting point for every required paths
	const int offset[] = {1, 3, 5, 7, 1};	// All the column offsets for every required paths
	int treesCount[5] = {0};				// Count of the trees founded on each path

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	while (input >> line) {
		tmp.clear();
		for (char item: line)
			if (item == '#')
				tmp.push_back(true);
			else
				tmp.push_back(false);
		terrain.push_back(tmp);
	}

	input.close();
	
	cout << "Processing input..." << endl;

	for (int row = 0, max, index, length = terrain[0].size(); row < terrain.size(); ++ row) {
		// I consider the last path only if I'm on an odd line ('cause I have to skip one line each turn in the last case)
		if (row % 2 == 0)
			max = 5;
		else
			max = 4;
		// If I hit tree along some path I consider it and increment the columns by their path offset and adjust in case of overflow
		for (index = 0; index < max; ++ index) {
			if (terrain[row][currentPoint[index]])
				++ treesCount[index];
			currentPoint[index] += offset[index];
			if (currentPoint[index] >= length)
				currentPoint[index] %= length;
		}
		// cout << row << ": " << treesCount[0] << " " << treesCount[1] << " " << treesCount[2] << " " << treesCount[3] << " " << treesCount[4] << " " << endl;
	}

	result[0] = treesCount[1];
	result[1] = treesCount[0] * treesCount[1] * treesCount[2] * treesCount[3] * treesCount[4];

	cout << treesCount[0] << " " << treesCount[1] << " " << treesCount[2] << " " << treesCount[3] << " " << treesCount[4] << " " << endl;

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

}