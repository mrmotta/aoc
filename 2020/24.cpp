#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_set>

#define ITERATIONS 100

using namespace std;

class movements_t {
	
	public:
	enum direction_t {EAST, WEST, NORTH_EST, NORTH_WEST, SOUTH_EAST, SOUTH_WEST};

	private:
	vector<direction_t> movements;

	public:
	movements_t () {}

	movements_t (string line) {
		for (int index = 0; index < line.size(); ++ index) {
			switch (line[index]) {
				case 'e': movements.push_back(EAST); break;
				case 's':
					++ index;
					if (line[index] == 'e')
						movements.push_back(SOUTH_EAST);
					else
						movements.push_back(SOUTH_WEST);
					break;
				case 'w': movements.push_back(WEST); break;
				case 'n':
					++ index;
					if (line[index] == 'e')
						movements.push_back(NORTH_EST);
					else
						movements.push_back(NORTH_WEST);
					break;
			}
		}
		movements.shrink_to_fit();
	}

	public:
	vector<direction_t> & getMovements () { return movements; }
};

class coordinates_t {

	public:
	coordinates_t () {}

	coordinates_t (int x, int y) {
		this->x = x;
		this->y = y;
	}

	public:
	int x = 0;
	int y = 0;

	inline bool operator == (const coordinates_t &coordinates) const {
		return x == coordinates.x && y == coordinates.y;
	}

	public:
	struct Hash {
		size_t operator() (const coordinates_t &coordinates) const {
			hash<int64_t> hash;
			return hash(((int64_t) coordinates.x << 32) + (int64_t) coordinates.y);
		}
	};

	coordinates_t getNeighbour (movements_t::direction_t direction) {

		coordinates_t coordinates;
		coordinates.x = this->x;
		coordinates.y = this->y;

		switch (direction) {
			case movements_t::EAST: ++ coordinates.x; break;
			case movements_t::WEST: -- coordinates.x; break;
			case movements_t::NORTH_EST: ++ coordinates.x; ++ coordinates.y; break;
			case movements_t::NORTH_WEST: ++ coordinates.y; break;
			case movements_t::SOUTH_EAST: -- coordinates.y; break;
			case movements_t::SOUTH_WEST: -- coordinates.x; -- coordinates.y; break;
		}

		return coordinates;
	}

	int countNeighbours (unordered_set<coordinates_t, Hash> &blackTiles) {

		int count = 0;

		for (int index = 0; index < 6; ++ index)
			if (blackTiles.find(this->getNeighbour(static_cast<movements_t::direction_t>(index))) != blackTiles.end())
				++ count;

		return count;
	}
};

class limits_t {

	public:
	coordinates_t min;
	coordinates_t max;

	public:
	void edit (coordinates_t coordinates) {
		if (coordinates.x < min.x)
			min.x = coordinates.x;
		else if (coordinates.x > max.x)
			max.x = coordinates.x;
		if (coordinates.y < min.y)
			min.y = coordinates.y;
		else if (coordinates.y > max.y)
			max.y = coordinates.y;
	}

	void expand () {
		-- min.x;
		-- min.y;
		++ max.x;
		++ max.y;
	}
};

coordinates_t getLocation (movements_t &movements, limits_t &limits);

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #24       |" << endl
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
	vector<movements_t> tiles;
	coordinates_t location;
	limits_t limits;
	int neighbours;
	unordered_set<coordinates_t, coordinates_t::Hash> blackTiles;
	unordered_set<coordinates_t, coordinates_t::Hash> tmpBlackTiles;

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	while (input >> line)
		tiles.push_back(movements_t(line));

	input.close();

	tiles.shrink_to_fit();

	cout << "Processing input..." << endl;

	// Part 1

	for (movements_t &movements: tiles) {
		location = getLocation(movements, limits);
		if (blackTiles.find(location) == blackTiles.end())
			blackTiles.insert(location);
		else
			blackTiles.erase(location);
	}

	result[0] = blackTiles.size();

	// Part 2

	tmpBlackTiles = blackTiles;

	for (int index = 0; index < ITERATIONS; ++ index) {

		limits.expand();

		for (int x = limits.min.x; x <= limits.max.x; ++ x)
			for (int y = limits.min.y; y <= limits.max.y; ++ y) {

				location = coordinates_t(x, y);
				neighbours = location.countNeighbours(blackTiles);

				if (blackTiles.find(location) != blackTiles.end()) {
					if (neighbours == 0 || neighbours > 2)
						tmpBlackTiles.erase(location);
				} else if (neighbours == 2)
					tmpBlackTiles.insert(location);
			}

		blackTiles = tmpBlackTiles;
	}

	result[1] = blackTiles.size();

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}

coordinates_t getLocation (movements_t &movements, limits_t &limits) {

	coordinates_t coordinates;

	for (movements_t::direction_t direction: movements.getMovements())
		coordinates = coordinates.getNeighbour(direction);

	limits.edit(coordinates);

	return coordinates;
}