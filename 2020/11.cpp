#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

class cell_t {

	public:
	cell_t (char cell) {
		if (cell == '.') {
			seat = false;
			occupied = false;
		} else {
			seat = true;
			if (cell == '#')
				occupied = true;
			else
				occupied = false;
		}
	}

	private:					// Cell data
	bool seat;
	bool occupied;

	public:
	void changeState () { occupied = !occupied; }

	// Getters
	bool isSeat () { return seat; }

	bool isOccupied () { return occupied; }
};

int64_t countOccupied ();
bool process_p1 ();
int countOccupied_p1 (int row, int column);
bool process_p2 ();
int countOccupied_p2 (int row, int column);

vector< vector<cell_t> > list;
vector< vector<cell_t> > tmpList;
vector< vector<cell_t> > originalList;

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #11       |" << endl
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

	string tmpString;				// Temporary string used to parse the input
	vector<cell_t> tmpLine;			// Temporary line used to parse the input
	// int occupiedSeats;					// Counts the number of occupied seats

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	while (input >> tmpString) {
		for (char cell: tmpString)
			tmpLine.push_back(cell_t(cell));
		originalList.push_back(tmpLine);
		tmpLine.clear();
	}

	input.close();

	list = originalList;
	tmpList = originalList;
	
	cout << "Processing input..." << endl;

	// Part 1
	// Processing seats
	while (process_p1());

	// Counting the occupied seats at the end
	result[0] = countOccupied();

	// Resetting the enviroment for part 2
	list = originalList;
	tmpList = originalList;

	// Part 2
	// Processing seats
	while (process_p2());

	// Counting the occupied seats at the end
	result[1] = countOccupied();

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}

int64_t countOccupied () {
	int64_t result = 0;
	for (int row = 0; row < list.size(); ++ row)
		for (int column = 0; column < list[row].size(); ++ column)
			if (list[row][column].isOccupied())
				++ result;
	return result;
}

bool process_p1 () {
	int occupiedSeats;
	bool changed = false;
	for (int row = 0; row < list.size(); ++ row)
		for (int column = 0; column < list[row].size(); ++ column)
			if (list[row][column].isSeat()) {
				occupiedSeats = countOccupied_p1(row, column);
				if ((!list[row][column].isOccupied() && occupiedSeats == 0) || (list[row][column].isOccupied() && occupiedSeats >= 4)) {
					tmpList[row][column].changeState();
					changed = true;
				}
			}
	list = tmpList;
	return changed;
}

int countOccupied_p1 (int row, int column) {
	int count = 0;
	// checking if it's the first row
	if (row > 0) {
		if (column > 0 && list[row-1][column-1].isOccupied())
			++ count;
		if (list[row-1][column].isOccupied())
			++ count;
		if (column < list[row-1].size()-1 && list[row-1][column+1].isOccupied())
			++ count;
	}
	// Processing the same row
	if (column > 0 && list[row][column-1].isOccupied())
		++ count;
	if (column < list[row].size()-1 && list[row][column+1].isOccupied())
		++ count;
	// Checking if it's the last row
	if (row < list.size()-1) {
		if (column > 0 && list[row+1][column-1].isOccupied())
			++ count;
		if (list[row+1][column].isOccupied())
			++ count;
		if (column < list[row+1].size()-1 && list[row+1][column+1].isOccupied())
			++ count;
	}
	return count;
}

bool process_p2 () {
	int occupiedSeats;
	bool changed = false;
	for (int row = 0; row < list.size(); ++ row)
		for (int column = 0; column < list[row].size(); ++ column)
			if (list[row][column].isSeat()) {
				occupiedSeats = countOccupied_p2(row, column);
				if ((!list[row][column].isOccupied() && occupiedSeats == 0) || (list[row][column].isOccupied() && occupiedSeats >= 5)) {
					tmpList[row][column].changeState();
					changed = true;
				}
			}
	list = tmpList;
	return changed;
}

int countOccupied_p2 (int row, int column) {
	int count = 0;
	// Checking column
	for (int i = row-1; i >= 0; -- i)
		if (list[i][column].isSeat()) {
			if (list[i][column].isOccupied())
				++ count;
			break;
		}
	for (int i = row+1; i < list.size(); ++ i)
		if (list[i][column].isSeat()) {
			if (list[i][column].isOccupied())
				++ count;
			break;
		}
	// Checking row
	for (int j = column-1; j >= 0; -- j)
		if (list[row][j].isSeat()) {
			if (list[row][j].isOccupied())
				++ count;
			break;
		}
	for (int j = column+1; j < list[row].size(); ++ j)
		if (list[row][j].isSeat()) {
			if (list[row][j].isOccupied())
				++ count;
			break;
		}
	// Cheching diagonals
	for (int i = row-1, j = column-1; i >= 0 && j >= 0; -- i, -- j)
		if (list[i][j].isSeat()) {
			if (list[i][j].isOccupied())
				++ count;
			break;
		}
	for (int i = row-1, j = column+1; i >= 0 && j < list[row].size(); -- i, ++ j)
		if (list[i][j].isSeat()) {
			if (list[i][j].isOccupied())
				++ count;
			break;
		}
	for (int i = row+1, j = column-1; i < list.size() && j >= 0; ++ i, -- j)
		if (list[i][j].isSeat()) {
			if (list[i][j].isOccupied())
				++ count;
			break;
		}
	for (int i = row+1, j = column+1; i < list.size() && j < list[row].size(); ++ i, ++ j)
		if (list[i][j].isSeat()) {
			if (list[i][j].isOccupied())
				++ count;
			break;
		}
	return count;
}