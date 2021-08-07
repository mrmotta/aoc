#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

typedef struct {					// Just to make things a little bit easier
	int number;
	string bag;
} innerBag_t;

class bag_t {

	public:
	bag_t (string line) {

		innerBag_t tmp;

		bag = line.substr(0, line.find(" bags contain"));
		if (line.find("no other bags") != string::npos)
			return;
		
		line = line.substr(line.find(" bags contain")+14, line.size());

		while (line.find(",") != string::npos) {
			tmp.number = atoll(&line[0]);
			tmp.bag = line.substr(2, line.find(" bag")-2);
			line = line.substr(line.find(",")+2, line.size());
			innerBags.push_back(tmp);
		};

		tmp.number = atoll(&line[0]);
		tmp.bag = line.substr(2, line.find(" bag")-2);
		innerBags.push_back(tmp);

		sort(innerBags.begin(), innerBags.end(),
			[] (innerBag_t bag1, innerBag_t bag2) {
				return bag1.bag < bag2.bag;
			});
	}

	public:
	string bag;						// The bag itself
	vector<innerBag_t> innerBags;	// All its inner bags (if any)
};

bag_t getBag (string bagName);
bool shinyGold (bag_t bag);
int64_t countBags (bag_t bag);

vector<bag_t> list;

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #07       |" << endl
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

	string tmpLine;						// Used for line-by-line reading

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	while (getline(input, tmpLine, '\n'))
		list.push_back(bag_t(tmpLine));

	input.close();

	// Just to make everything else a bit easier
	sort(list.begin(), list.end(),
		[] (bag_t bag1, bag_t bag2) -> bool {
			return bag1.bag < bag2.bag;
		});
	
	cout << "Processing input..." << endl;

	for (bag_t bag: list)
		if (shinyGold(bag))
			++ result[0];
	
	result[1] = countBags(getBag("shiny gold"));

	// Otherwise I count the shiny gold bag itself too
	-- result[0];
	-- result[1];

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}

bag_t getBag (string bagName) {				// A kind of binary search
	int min = 0, index, max = list.size();
	while (true) {
		index = (min + max) / 2;
		if (list[index].bag == bagName)
			return list[index];
		if (list[index].bag > bagName)
			max = index-1;
		else
			min = index+1;
	}
}

bool shinyGold (bag_t bag) {				// Returns true if it can contain a shiny gold bag
	if (bag.bag == "shiny gold")
		return true;
	if (bag.innerBags.empty())
		return false;
	for (innerBag_t innerBag: bag.innerBags)
		if (shinyGold(getBag(innerBag.bag)))
			return true;
	return false;
}

int64_t countBags (bag_t bag) {				// Returns the count of bags into the current one
	int64_t count = 1;
	if (bag.innerBags.empty())
		return 1;
	for (innerBag_t innerBag: bag.innerBags)
		count += innerBag.number * countBags(getBag(innerBag.bag));
	return count;
}