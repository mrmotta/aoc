#include <iostream>
#include <fstream>
#include <vector>

using namespace std;

typedef enum {NORTH, EAST, SOUTH, WEST, LEFT, RIGHT, FORWARD} action_t;

class instruction_t {
	public:
	action_t action;			// The action itself
	int value;					// The value related

	public:						// Setter	
	void insert (string instruction) {
		switch (instruction[0]) {
			case 'N': action = NORTH; break;
			case 'E': action = EAST; break;
			case 'S': action = SOUTH; break;
			case 'W': action = WEST; break;
			case 'L': action = LEFT; break;
			case 'R': action = RIGHT; break;
			default: action = FORWARD;
		}
		value = stoi(instruction.substr(1, instruction.size()-1));
	}
};

vector<instruction_t> list;

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #12       |" << endl
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
	instruction_t tmpInstruction;	// Temporary instruction used to parse the 
	action_t facing = EAST;			// Starting facing direction
	int tmpInt;						// Used for rotating the ship
	int64_t position[2] = {0};		// East-West and Noth-South position of the ship
	int64_t waypoint[2] = {10, 1};	// East-West and Noth-South waypoint location

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	while (input >> tmpString) {
		tmpInstruction.insert(tmpString);
		list.push_back(tmpInstruction);
	}

	input.close();
	
	cout << "Processing input..." << endl;

	// Part 1
	// Processing instructions
	for (instruction_t &instruction: list) {
		switch (instruction.action) {
			case NORTH: position[1] += instruction.value; break;
			case EAST: position[0] += instruction.value; break;
			case SOUTH: position[1] -= instruction.value; break;
			case WEST: position[0] -= instruction.value; break;
			case LEFT:
				tmpInt = static_cast<int>(facing) - instruction.value / 90;
				if (tmpInt < 0) tmpInt += 4;
				facing = static_cast<action_t>(tmpInt);
				break;
			case RIGHT:
				tmpInt = static_cast<int>(facing) + instruction.value / 90;
				if (tmpInt > 3) tmpInt -= 4;
				facing = static_cast<action_t>(tmpInt);
				break;
			default:
				switch (facing) {
					case NORTH: position[1] += instruction.value; break;
					case EAST: position[0] += instruction.value; break;
					case SOUTH: position[1] -= instruction.value; break;
					default: position[0] -= instruction.value;
				}
		}
	}

	// Calculating the distance
	if (position[0] < 0) position[0] = -position[0];
	if (position[1] < 0) position[1] = -position[1];
	result[0] = position[0] + position[1];

	// Part 2
	// Environment reset
	position[0] = 0;
	position[1] = 0;

	// Processing instructions
	for (instruction_t &instruction: list) {
		switch (instruction.action) {
			case NORTH: waypoint[1] += instruction.value; break;
			case EAST: waypoint[0] += instruction.value; break;
			case SOUTH: waypoint[1] -= instruction.value; break;
			case WEST: waypoint[0] -= instruction.value; break;
			case FORWARD:
				position[0] += waypoint[0] * instruction.value;
				position[1] += waypoint[1] * instruction.value;
				break;
			default:
				if (instruction.value == 180) {
					waypoint[0] = -waypoint[0];
					waypoint[1] = -waypoint[1];
				} else if ((instruction.action == RIGHT && instruction.value == 90) || (instruction.action == LEFT && instruction.value == 270)) {
					tmpInt = waypoint[1];
					waypoint[1] = -waypoint[0];
					waypoint[0] = tmpInt;
				} else {
					tmpInt = -waypoint[1];
					waypoint[1] = waypoint[0];
					waypoint[0] = tmpInt;
				}
		}
	}

	// Calculating the distance
	if (position[0] < 0) position[0] = -position[0];
	if (position[1] < 0) position[1] = -position[1];
	result[1] = position[0] + position[1];

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}