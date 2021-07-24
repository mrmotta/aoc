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
		<< "|       Day #02       |" << endl
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

	typedef struct {		// Data structure
		int min, max;
		char letter;
		string password;
	} policy_t;

	vector<policy_t> list;	// List of policies
	policy_t tmpPolicy;		// Temporary policy
	string tmp;				// Temporary string for reading
	int letterNumber;		// Temporary number of letters in password

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	while (!input.eof()) {
		input >> tmp;
		tmpPolicy.min = stoll(tmp.substr(0, tmp.find("-")));
		tmpPolicy.max = stoll(tmp.substr(tmp.find("-")+1, tmp.size()));
		input >> tmp;
		tmpPolicy.letter = tmp[0];
		input >> tmpPolicy.password;
		list.push_back(tmpPolicy);
	}

	input.close();
	
	cout << "Processing input..." << endl;

	for (policy_t &currentPolicy: list) {
		// Counting occurrences of the letter
		letterNumber = count(currentPolicy.password.begin(), currentPolicy.password.end(), currentPolicy.letter);
		if (letterNumber >= currentPolicy.min && letterNumber <= currentPolicy.max)
			++ result[0];
		// Checking if only one of the two positions given has the specified letter
		if (currentPolicy.password[currentPolicy.min-1] != currentPolicy.password[currentPolicy.max-1] && (currentPolicy.password[currentPolicy.min-1] == currentPolicy.letter || currentPolicy.password[currentPolicy.max-1] == currentPolicy.letter))
			++ result[1];
	}

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}