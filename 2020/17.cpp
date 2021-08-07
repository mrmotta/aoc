#include <iostream>
#include <fstream>
#include <unordered_set>

#define ROUNDS 6
#define ACTIVE '#'

using namespace std;

class dimensions_t {

	private:
	typedef struct {
		int min;
		int max;
	} coordinate_t;

	public:
	coordinate_t x;
	coordinate_t y;
	coordinate_t z;
	coordinate_t w;

	public:
	void initialize (int linesNumber, int lineLength) {
		x.min = 0; x.max = linesNumber + 1;
		y.min = 0; y.max = lineLength + 1;
		z.min = 0; z.max = 2;
		w.min = 0; w.max = 2;
	}

	void increase () {
		++ x.max;
		++ y.max;
		++ z.max;
		++ w.max;
	}

	void decrease () {
		-- x.min;
		-- y.min;
		-- z.min;
		-- w.min;
	}
};

class map4d_t {

	private:
	struct Hash {
		size_t operator () (const tuple<int, int, int, int> &element) const {
			auto [x, y, z, w] = element;
			return hash<string>()(to_string(x) + "-" + to_string(y) + "-" + to_string(z) + "-" + to_string(w));
		}
	};
	unordered_set <tuple<int, int, int, int>, struct Hash> map;

	public:
	void setActive (int x, int y) { setActive(x, y, 0, 0); }

	void setActive (int x, int y, int z, int w) { map.insert(make_tuple(x, y, z, w)); }

	void setInactive (int x, int y, int z, int w) { map.erase(make_tuple(x, y, z, w)); }

	bool isActive (int x, int y, int z, int w) { return map.find(make_tuple(x, y, z, w)) != map.end(); }

	int process (int x, int y, int z, int w) {
		int neighbors = 0;
		for (int delta_x = -1; delta_x <= 1; ++ delta_x)
			for (int delta_y = -1; delta_y <= 1; ++ delta_y)
				for (int delta_z = -1; delta_z <= 1; ++ delta_z)
					for (int delta_w = -1; delta_w <= 1; ++ delta_w)
						if ((delta_x != 0 || delta_y != 0 || delta_z != 0 || delta_w != 0) && isActive(x + delta_x, y + delta_y, z + delta_z, w + delta_w))
							++ neighbors;
		return neighbors;
	}

	int countActive () { return map.size(); }
};

class map3d_t : public map4d_t {

	public:
	void setActive (int x, int y) { map4d_t::setActive(x, y); }

	void setActive (int x, int y, int z) { map4d_t::setActive(x, y, z, 0); }

	void setInactive (int x, int y, int z) { map4d_t::setInactive(x, y, z, 0); }

	bool isActive (int x, int y, int z) { return map4d_t::isActive(x, y, z, 0); }

	int process (int x, int y, int z) {
		int neighbors = 0;
		for (int delta_x = -1; delta_x <= 1; ++ delta_x)
			for (int delta_y = -1; delta_y <= 1; ++ delta_y)
				for (int delta_z = -1; delta_z <= 1; ++ delta_z)
						if ((delta_x != 0 || delta_y != 0 || delta_z != 0) && isActive(x + delta_x, y + delta_y, z + delta_z))
							++ neighbors;
		return neighbors;
	}
};

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #17       |" << endl
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

	dimensions_t dimensions;
	map3d_t map3d;
	map3d_t oldMap3d;
	map4d_t map4d;
	map4d_t oldMap4d;
	int neighbors;

	string tmpLine;
	int linesNumber = 0;

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	for (int x = 0; getline(input, tmpLine, '\n'); ++ x, ++ linesNumber) {
		if (tmpLine[tmpLine.size()-1] == '\r')
			tmpLine = tmpLine.substr(0, tmpLine.size()-1);
		for (int y = 0; y < tmpLine.length(); ++ y)
			if (tmpLine[y] == ACTIVE) {
				map3d.setActive(x, y);
				map4d.setActive(x, y);
			}
	}

	input.close();

	dimensions.initialize(linesNumber, tmpLine.length());

	cout << "Processing input..." << endl;

	// Part 1
	oldMap3d = map3d;
	for (int index = 0; index < ROUNDS; ++ index) {
		dimensions.decrease();
		for (int x = dimensions.x.min; x < dimensions.x.max; ++ x)
			for (int y = dimensions.y.min; y < dimensions.y.max; ++ y)
				for (int z = dimensions.z.min; z < dimensions.z.max; ++ z) {
					neighbors = oldMap3d.process(x, y, z);
					if (oldMap3d.isActive(x, y, z)) {
						if (neighbors != 2 && neighbors != 3)
							map3d.setInactive(x, y, z);
					} else {
						if (neighbors == 3)
							map3d.setActive(x, y, z);
					}
				}
		dimensions.increase();
		oldMap3d = map3d;
	}

	result[0] = map3d.countActive();

	// Part 2
	dimensions.initialize(linesNumber, tmpLine.length());
	oldMap4d = map4d;
	for (int index = 0; index < ROUNDS; ++ index) {
		dimensions.decrease();
		for (int x = dimensions.x.min; x < dimensions.x.max; ++ x)
			for (int y = dimensions.y.min; y < dimensions.y.max; ++ y)
				for (int z = dimensions.z.min; z < dimensions.z.max; ++ z)
					for (int w = dimensions.w.min; w < dimensions.w.max; ++ w) {
						neighbors = oldMap4d.process(x, y, z, w);
						if (oldMap4d.isActive(x, y, z, w)) {
							if (neighbors != 2 && neighbors != 3)
								map4d.setInactive(x, y, z, w);
						} else {
							if (neighbors == 3)
								map4d.setActive(x, y, z, w);
						}
					}
		dimensions.increase();
		oldMap4d = map4d;
	}

	result[1] = map4d.countActive();

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}