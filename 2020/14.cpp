#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

typedef enum {ZERO, ONE, X} bitmask_t;

class mask_t {
	private:
	bitmask_t mask[36];			// The mask itself

	public:						// Mask setters: whole mask or specific position
	void setMask (string value) {
		for (int index = 0; index < 36; ++ index)
			switch (value[index]) {
				case '0': mask[index] = ZERO; break;
				case '1': mask[index] = ONE; break;
				default: mask[index] = X;
			}
	}
	void setMask (int index, bitmask_t value) { mask[index] = value; }

	public:						// Mask getter
	bitmask_t getMask (int index) { return mask[index]; }

	public:						// Converts an array of bits into an integer
	int64_t toInt () {
		int64_t multiplier = 1;
		int64_t number = 0;
		for (int index = 35; index >= 0; -- index) {
			if (mask[index] == ONE)
				number += multiplier;
			multiplier = multiplier << 1;
		}
		return number;
	}
};

vector<mask_t> maskList;

class instruction_t {
	private:
	int64_t address;			// Memory address
	int64_t value;				// Value to store in memory
	int mask;					// Mask number, according the the order it appeared

	public:						// Setters
	void setAddress (string input) { address = stoll(input.substr(input.find("[")+1, input.size()-1)); }
	void setAddress (mask_t newAddress) { address = newAddress.toInt(); }
	void setValue (string input) { value = stoll(input); }
	void setMask (int index) { mask = index; }

	public:						// Getters
	int64_t getAddress () { return address; }
	int64_t getValue () { return value; }
	int getMask () { return mask; }

	public:						// Mix the value with the mask and gets the result
	int64_t processValue () {
		mask_t binaryValue = valueToMask();
		mask_t original = binaryValue;
		for (int index = 0; index < 36; ++ index)
			if (maskList[mask].getMask(index) != X)
				binaryValue.setMask(index, maskList[mask].getMask(index));
		return binaryValue.toInt();
	}

	public:						// Converts the value into an array of bits
	mask_t valueToMask() {
		mask_t resultingMask;
		int64_t currentValue = value;
		for (int index = 35; index >= 0; -- index) {
			if (currentValue % 2 == 0)
				resultingMask.setMask(index, ZERO);
			else
				resultingMask.setMask(index, ONE);
			currentValue = currentValue >> 1;
		}
		return resultingMask;
	}

	public:						// Converts the memory address into an array of bits
	mask_t addressToMask() {
		mask_t resultingMask;
		int64_t currentValue = address;
		for (int index = 35; index >= 0; -- index) {
			if (currentValue % 2 == 0)
				resultingMask.setMask(index, ZERO);
			else
				resultingMask.setMask(index, ONE);
			currentValue = currentValue >> 1;
		}
		return resultingMask;
	}
};

vector<instruction_t> originalInstructionList;
vector<instruction_t> sortedInstructionList;
vector<instruction_t> fullInstructionList;

void generate (instruction_t &instruction, mask_t &address, int maskIndex);

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #14       |" << endl
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
	instruction_t tmpInstruction;	// Temporary instruction used to parse the input
	mask_t tmpMask;					// Temporary mask used to parse the input and for part 2
	int maskNumber = -1;			// Simply to know which mask I'm considering

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	while (input >> tmpString) {
		// Checking if it's a mask
		if (tmpString == "mask") {
			// Discarding the '=' symbol
			input >> tmpString;
			// Getting the mask and inserting it
			input >> tmpString;
			tmpMask.setMask(tmpString);
			maskList.push_back(tmpMask);
			++ maskNumber;
		} else {
			// So this is a memory address
			tmpInstruction.setAddress(tmpString);
			// Discarding the '=' symbol
			input >> tmpString;
			// Getting the value and inserting it
			input >> tmpString;
			tmpInstruction.setValue(tmpString);
			// Setting the reference to the mask
			tmpInstruction.setMask(maskNumber);
			originalInstructionList.push_back(tmpInstruction);
		}
	}

	input.close();

	// This is done because for the second part I need the original list
	sortedInstructionList = originalInstructionList;
	
	cout << "Processing input..." << endl;

	// Part 1
	// Sorting the list to make the rest of the process easier
	stable_sort(sortedInstructionList.begin(), sortedInstructionList.end(),
		[] (instruction_t instruction1, instruction_t instruction2) {
			return instruction1.getAddress() < instruction2.getAddress();
		});
	
	// Processing the instructions
	for (int index = 0; index < sortedInstructionList.size(); ++ index) {
		// Getting the last one
		while (index < sortedInstructionList.size()-1 && sortedInstructionList[index].getAddress() == sortedInstructionList[index+1].getAddress())
			++ index;
		// Calculating the partial result
		result[0] += sortedInstructionList[index].processValue();
	}

	// Part 2
	// For each one I process the mask and generate all the possibilities
	for (int index = 0; index < originalInstructionList.size(); ++ index) {
		// Converting the value into a mask
		tmpMask = originalInstructionList[index].addressToMask();
		// Processing the mask according to the rules
		for (int maskIndex = 0; maskIndex < 36; ++ maskIndex) {
			if (maskList[originalInstructionList[index].getMask()].getMask(maskIndex) == ONE)
				tmpMask.setMask(maskIndex, ONE);
			else if (maskList[originalInstructionList[index].getMask()].getMask(maskIndex) == X)
				tmpMask.setMask(maskIndex, X);
		}
		// Generating all the addresses
		generate(originalInstructionList[index], tmpMask, 0);
	}

	// Sorting the list so that it will be easier to get the result
	stable_sort(fullInstructionList.begin(), fullInstructionList.end(),
		[] (instruction_t instruction1, instruction_t instruction2) {
			return instruction1.getAddress() < instruction2.getAddress();
		});

	// Calculating the result
	for (int index = 0; index < fullInstructionList.size(); ++ index) {
		// Getting the last one
		while (index < fullInstructionList.size()-1 && fullInstructionList[index].getAddress() == fullInstructionList[index+1].getAddress())
			++ index;
		// Calculating the partial result
		result[1] += fullInstructionList[index].getValue();
	}

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}

void generate (instruction_t &instruction, mask_t &address, int maskIndex) {
	// Looking for the first X in the address
	while (maskIndex < 36 && maskList[instruction.getMask()].getMask(maskIndex) != X)
		++ maskIndex;
	// If none is found, I've finished
	if (maskIndex == 36) {
		instruction.setAddress(address);
		fullInstructionList.push_back(instruction);
		return;
	}
	// Otherwise, let's do some recursion with both possibilities
	address.setMask(maskIndex, ZERO);
	generate(instruction, address, maskIndex+1);
	address.setMask(maskIndex, ONE);
	generate(instruction, address, maskIndex+1);
}