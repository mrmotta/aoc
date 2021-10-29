#include <iostream>
#include <fstream>
#include <vector>

#define PART_1_ITEMS 9
#define PART_1_ITERATIONS 100
#define PART_2_ITEMS 1000000
#define PART_2_ITERATIONS 10000000

using namespace std;

typedef struct element {
	long int number;
	struct element *previous;
	struct element *next;
} item_t;

item_t * prepareList(string line, vector<item_t *> &positions);
void addItems(item_t *list, vector<item_t *> &positions);
void cleanList(item_t *list);
void process (item_t *list, vector<item_t *> &positions, const long int loopIterations, const long int maxPosition);

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #23       |" << endl
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
	string line;
	string tmpResult;
	vector<item_t *> positions(PART_2_ITEMS);
	item_t *list = NULL;
	item_t *current;

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	input >> line;
	input.close();

	cout << "Processing input..." << endl;

	// Part 1

	list = prepareList(line, positions);

	process(list, positions, PART_1_ITERATIONS, PART_1_ITEMS - 1);

	current = positions[0]->next;
	for (int index = 0; index < PART_1_ITEMS - 1; ++ index) {
		tmpResult.push_back(current->number + '0');
		current = current->next;
	}
	result[0] = stoll(tmpResult);

	// Part 2

	cleanList(list);
	list = prepareList(line, positions);
	current = list;
	addItems(list, positions);

	process(list, positions, PART_2_ITERATIONS, PART_2_ITEMS - 1);

	result[1] = (unsigned long long int) positions[0]->next->number * positions[0]->next->next->number;

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}

item_t * prepareList(string line, vector<item_t *> &positions) {

	item_t *list = NULL;
	item_t *current;

	for (char number: line) {
		if (list == NULL) {
			list = new item_t();
			current = list;
		} else {
			current->next = new item_t();
			current->next->previous = current;
			current = current->next;
		}
		current->number = number - '0';
		positions[current->number - 1] = current;
		current->next = list;
		list->previous = current;
	}

	return list;
}

void addItems(item_t *list, vector<item_t *> &positions) {

	item_t *current = list->previous;

	for (long number = PART_1_ITEMS + 1; number <= PART_2_ITEMS; ++ number) {
		current->next = new item_t();
		current->next->previous = current;
		current = current->next;
		current->number = number;
		positions[current->number - 1] = current;
		current->next = list;
		list->previous = current;
	}
}

void cleanList(item_t *list) {

	item_t *cursor = list;
	item_t *tmp;

	list->previous->next = NULL;

	while (cursor != NULL) {
		tmp = cursor;
		cursor = cursor->next;
		delete tmp;
	}
}

void process (item_t *list, vector<item_t *> &positions, const long int loopIterations, const long int maxPosition) {

	long int position;
	item_t *tmpList[2];
	item_t *current = list;
	item_t *tmp;

	for (int index = 0; index < loopIterations; ++ index) {

		// Picking up cups
		tmpList[0] = current->next;
		tmpList[1] = tmpList[0]->next->next;
		current->next = tmpList[1]->next;
		tmpList[1]->next->previous = current;

		// Calculating position where to put cups
		tmp = current;
		do {
			position = tmp->number - 2;
			if (position == -1)
				position = maxPosition;
			tmp = positions[position];
		} while (tmpList[0] == tmp || tmpList[0]->next == tmp || tmpList[1] == tmp);

		// Putting back cups
		tmpList[1]->next = tmp->next;
		tmp->next->previous = tmpList[1];
		tmpList[0]->previous = tmp;
		tmp->next = tmpList[0];

		current = current->next;
	}
}