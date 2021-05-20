#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef enum {ACC, JMP, NOP} operation_t;

class instruction_t {
	public:
	operation_t operation;		// The operation type
	int argument;				// The argument itself
	bool executed;				// Useful to know if I've already executed it

	public:
	void insert (string op, string arg) {
		setOperation(op);
		setArgument(arg);
		executed = false;
	}

	private:					// Setters
	void setOperation (string op)  {
		if (op == "acc")
			operation = ACC;
		else if (op == "jmp")
			operation = JMP;
		else
			operation = NOP;
	}
	void setArgument (string arg) { argument = stoll(arg); }
};

bool fix (int index, int64_t &result, bool change);

vector<instruction_t> list;

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #08       |" << endl
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

	instruction_t tmpInstruction;	// Temporary instruction used for parsing
	string tmpOperation, tmpArgument;
	int index;						// Used for simulating the instructions in part 1

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	while (input >> tmpOperation) {
		input >> tmpArgument;
		tmpInstruction.insert(tmpOperation, tmpArgument);
		list.push_back(tmpInstruction);
	}

	input.close();
	
	cout << "Processing input..." << endl;

	// Part 1
	index = 0;
	while (index < list.size() && !list[index].executed) {
		// Marking as executed so that I will find out the re-execution easily
		list[index].executed = true;
		// Adjusting the index according to the instruction type
		if (list[index].operation == JMP)
			index += list[index].argument;
		else {
			if (list[index].operation == ACC)
				result[0] += list[index].argument;
			++ index;
		}
	}

	// Resetting the environment for the second part
	for (instruction_t &instruction: list)
		instruction.executed = false;
	
	// Part 2
	fix(0, result[1], true);

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}

bool fix (int index, int64_t &result, bool change) {

	// Basic cases
	if (index == list.size())
		return true;
	if (index < 0 || index > list.size())
		return false;
	if (list[index].executed)
		return false;

	int start = index;

	list[index].executed = true;

	// Adjusting index according to the operations
	if (list[index].operation == JMP) {
		if (change) {
			// If possible, trying to change a JMP into a NOP and checking the result
			list[index].operation = NOP;
			if (fix(index+1, result, false))
				return true;
			list[index].operation = JMP;
		}
		index += list[index].argument;
	} else if (list[index].operation == NOP && list[index].argument != 0) {
		if (change) {
			// If possible, trying to change a NOP into a JMP and checking the result
			list[index].operation = JMP;
			if (fix(index+list[index].argument, result, false))
				return true;
			list[index].operation = NOP;
		}
		++ index;
	} else
		++ index;

	// Otherwise let's go on processing the next position and eventually I calculate also the result
	if (fix(index, result, change)) {
		if (list[index].operation == ACC)
			result += list[index].argument;
		return true;
	}

	// A kind of soft reset
	list[start].executed = false;
	return false;
}