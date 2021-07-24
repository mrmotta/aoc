#include <iostream>
#include <fstream>
#include <vector>
#include <cmath>

using namespace std;

class list_t {
	private:					// Data to store
	typedef struct {
		int id;
		int position;
	} bus_t;
	int64_t timestamp;
	vector<bus_t> bus;

	public:						// Setter
	void insert (ifstream &input) {
		string line, busNumber;
		bus_t tmpBus;
		int count = 0;

		input >> line;
		timestamp = stoll(line);
		input >> line;

		while (line.find(",") != string::npos) {
			busNumber = line.substr(0, line.find(","));
			line = line.substr(busNumber.size()+1, line.size());
			if (busNumber != "x") {
				tmpBus.id = stoi(busNumber);
				tmpBus.position = count;
				bus.push_back(tmpBus);
			}
			++ count;
		}

		if (line != "x") {
			tmpBus.id = stoi(line);
			tmpBus.position = count;
			bus.push_back(tmpBus);
		}
	}

	public:						// Getters and utilities
	int64_t getTimestamp () { return timestamp; }
	int size () { return bus.size(); };
	int getId (int index) { return bus[index].id; }
	int getPosition (int index) { return bus[index].position; }
};

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #13       |" << endl
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

	vector<int64_t> arrival;	// List of arrival times immediatly after the timestamp
	int min = 0;				// The minimum one of the previous ones
	int64_t partialResult;		// Partial result for part 2

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	list.insert(input);

	input.close();
	
	cout << "Processing input..." << endl;

	// Part 1
	// Calculating the arrival times immediatly after the timestamp for each bus id
	for (int index = 0; index < list.size(); ++ index)
		arrival.push_back(list.getId(index) * ceil((float) list.getTimestamp() / list.getId(index)));
	
	// Finding the first one to arrive
	for (int index = 1; index < list.size(); ++ index)
		if (arrival[index] < arrival[min])
			min = index;
	
	// Calculating the result
	result[0] = list.getId(min) * (arrival[min] - list.getTimestamp());

	// Part 2
	partialResult = list.getId(0);
	for (int index = 1; index < list.size(); ++ index) {
		while ((result[1] + list.getPosition(index)) % list.getId(index) != 0)
			result[1] += partialResult;
		partialResult *= list.getId(index);
	}

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}