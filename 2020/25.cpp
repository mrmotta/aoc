#include <iostream>
#include <fstream>

#define STARTING_PUBLIC_KEY 1
#define DEFAULT_SUBJECT_NUMBER 7
#define MOD 20201227

using namespace std;

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #25       |" << endl
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

	int64_t result = 0;

	int64_t cardPublicKey;
	int64_t doorPublicKey;
	int cardLoopSize;
	int64_t publicKey;
	int64_t subjectNumber;

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	input >> cardPublicKey;
	input >> doorPublicKey;

	input.close();

	cout << "Processing input..." << endl;

	// Getting the card's loop size
	publicKey = STARTING_PUBLIC_KEY;
	subjectNumber = DEFAULT_SUBJECT_NUMBER;
	for (cardLoopSize = 0; publicKey != cardPublicKey; ++ cardLoopSize)
		publicKey = (publicKey * subjectNumber) % MOD;

	// Calculating the encryption key using the door's public key and the card's loop size
	publicKey = STARTING_PUBLIC_KEY;
	subjectNumber = doorPublicKey;
	for (int index = 0; index < cardLoopSize; ++ index)
		publicKey = (publicKey * subjectNumber) % MOD;
	result = publicKey;

	cout << endl;
	cout << "Result: " << result << endl << endl;

	return 0;
}