#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <unordered_map>

using namespace std;

class support {

	public:
	enum class fit {NONE, LEFT, RIGHT, TOP, BOTTOM};
	enum class flip {NONE, HORIZONTAL, VERTICAL};
	enum class pixel {WHITE, BLACK, MONSTER};
	enum class rotation {NONE, LEFT, RIGHT, HALF};
};

class tile_t {

	public:
	tile_t () {}

	tile_t (vector<string> tileImage) {

		vector<bool> pixels;
		pixels.reserve(tileImage.size());

		for (string &line: tileImage) {
			pixels.clear();
			for (char pixel: line)
				pixels.push_back(pixel == '#');
			image.push_back(pixels);
		}

		image.shrink_to_fit();
	}
	
	private:
	vector<vector<bool>> image;
	bool processed = false;
	vector<int> adjacents;

	public:
	int getSideLength () { return image.size(); }

	vector<vector<bool>> & getImage () { return image; }

	void changeOrientation (support::rotation rotation, support::flip flip) {

		bool tmp;
		int size = image.size();

		switch (rotation) {

			case support::rotation::NONE:
				break;

			case support::rotation::LEFT:
				for (int row = 0; row < size / 2; ++ row)
					for (int column = row; column < size - row - 1; ++ column) {
						tmp = image[row][column];
						image[row][column] = image[column][size - row - 1];
						image[column][size - row - 1] = image[size - row - 1][size - column - 1];
						image[size - row - 1][size - column - 1] = image[size - column - 1][row];
						image[size - column - 1][row] = tmp;
					}
				break;

			case support::rotation::RIGHT:
				for (int row = 0; row < size / 2; ++ row)
					for (int column = row; column < size - row - 1; ++ column) {
						tmp = image[row][column];
						image[row][column] = image[size - column - 1][row];
						image[size - column - 1][row] = image[size - row - 1][size - column - 1];
						image[size - row - 1][size - column - 1] = image[column][size - row - 1];
						image[column][size - row - 1] = tmp;
					}
				break;

			case support::rotation::HALF:
				for (int row = 0; row < size; ++ row)
					for (int column = 0; column < size / 2; ++ column) {
						tmp = image[row][column];
						image[row][column] = image[size - row - 1][size - column - 1];
						image[size - row - 1][size - column - 1] = tmp;
					}
				break;
		}

		switch (flip) {

			case support::flip::NONE:
				break;

			case support::flip::HORIZONTAL:
				for (int row = 0; row < size; ++ row)
					for (int column = 0; column < size / 2; ++ column) {
						tmp = image[row][column];
						image[row][column] = image[row][size - column - 1];
						image[row][size - column - 1] = tmp;
					}
				break;

			case support::flip::VERTICAL:
				for (int row = 0; row < size / 2; ++ row)
					for (int column = 0; column < size; ++ column) {
						tmp = image[row][column];
						image[row][column] = image[size - row - 1][column];
						image[size - row - 1][column] = tmp;
					}
				break;
		}
	}

	vector<vector<bool>> getEdges () {

		int size = image.size();
		vector<bool> edge;
		vector<vector<bool>> edges;

		edge.reserve(size);

		for (int index = 0; index < size; ++ index)
			edge.push_back(image[index][0]);

		edges.push_back(edge);
		edge.clear();

		for (int index = 0; index < size; ++ index)
			edge.push_back(image[index][size - 1]);

		edges.push_back(edge);
		edges.push_back(image[0]);
		edges.push_back(image[size - 1]);

		return edges;
	}

	support::fit getFit (tile_t &adjacent) {

		bool found = false;
		vector<vector<bool>> originalEdges = getEdges();
		vector<vector<bool>> adjacentEdges = adjacent.getEdges();
		vector<vector<bool>> inverseEdges = adjacentEdges;

		for (vector<bool> &edge: inverseEdges)
			reverse(edge.begin(), edge.end());

		for (int index = 0; index < 4; ++ index) {

			for (vector<bool> &adjacent: adjacentEdges)
				if (originalEdges[index] == adjacent) {
					found = true;
					break;
				}

			if (!found)
				for (vector<bool> &inverse: inverseEdges)
					if (originalEdges[index] == inverse) {
						found = true;
						break;
					}

			if (found)
				return static_cast<support::fit>(index + 1);
		}

		return support::fit::NONE;
	}

	support::fit fitAndTransform (tile_t &adjacent, support::fit fit) {

		vector<vector<bool>> originalEdges = getEdges();
		vector<vector<bool>> adjacentEdges = adjacent.getEdges();
		vector<vector<bool>> inverseEdges = adjacentEdges;

		for (vector<bool> &edge: inverseEdges)
			reverse(edge.begin(), edge.end());

		if (fit == support::fit::RIGHT) {

			// Checking left side of the adjacent (right) tile
			if (originalEdges[1] == adjacentEdges[0]) return support::fit::RIGHT;
			if (originalEdges[1] == inverseEdges[0]) { adjacent.changeOrientation(support::rotation::NONE, support::flip::VERTICAL); return support::fit::RIGHT; }

			// Checking right side of the adjacent (right) tile
			if (originalEdges[1] == adjacentEdges[1]) { adjacent.changeOrientation(support::rotation::NONE, support::flip::HORIZONTAL); return support::fit::RIGHT; }
			if (originalEdges[1] == inverseEdges[1]) { adjacent.changeOrientation(support::rotation::HALF, support::flip::NONE); return support::fit::RIGHT; }

			// Checking top side of the adjacent (right) tile
			if (originalEdges[1] == adjacentEdges[2]) { adjacent.changeOrientation(support::rotation::LEFT, support::flip::VERTICAL); return support::fit::RIGHT; }
			if (originalEdges[1] == inverseEdges[2]) { adjacent.changeOrientation(support::rotation::LEFT, support::flip::NONE); return support::fit::RIGHT; }

			// Checking bottom side of the adjacent (right) tile
			if (originalEdges[1] == adjacentEdges[3]) { adjacent.changeOrientation(support::rotation::RIGHT, support::flip::NONE); return support::fit::RIGHT; }
			if (originalEdges[1] == inverseEdges[3]) { adjacent.changeOrientation(support::rotation::RIGHT, support::flip::VERTICAL); return support::fit::RIGHT; }	// With support::rotation::LEFT, '20 2.txt' is fixed, but not the rest

		} else {

			// Checking left side of the adjacent (bottom) tile
			if (originalEdges[3] == adjacentEdges[0]) { adjacent.changeOrientation(support::rotation::RIGHT, support::flip::HORIZONTAL); return support::fit::BOTTOM; }
			if (originalEdges[3] == inverseEdges[0]) { adjacent.changeOrientation(support::rotation::RIGHT, support::flip::NONE); return support::fit::BOTTOM; }

			// Checking right side of the adjacent (bottom) tile
			if (originalEdges[3] == adjacentEdges[1]) { adjacent.changeOrientation(support::rotation::LEFT, support::flip::NONE); return support::fit::BOTTOM; }
			if (originalEdges[3] == inverseEdges[1]) { adjacent.changeOrientation(support::rotation::LEFT, support::flip::HORIZONTAL); return support::fit::BOTTOM; }

			// Checking top side of the adjacent (bottom) tile
			if (originalEdges[3] == adjacentEdges[2]) return support::fit::BOTTOM;
			if (originalEdges[3] == inverseEdges[2]) { adjacent.changeOrientation(support::rotation::NONE, support::flip::HORIZONTAL); return support::fit::BOTTOM; }

			// Checking bottom side of the adjacent (bottom) tile
			if (originalEdges[3] == adjacentEdges[3]) { adjacent.changeOrientation(support::rotation::NONE, support::flip::VERTICAL); return support::fit::BOTTOM; }
			if (originalEdges[3] == inverseEdges[3]) { adjacent.changeOrientation(support::rotation::HALF, support::flip::NONE); return support::fit::BOTTOM; }

		}

		return support::fit::NONE;
	}

	bool getProcessed () { return processed; }

	void setProcessed () { processed = true; }

	void addAdjacent (int adjacentId) { adjacents.push_back(adjacentId); }

	vector<int> getAdjacents () { return adjacents; }

	bool isAdjacent (int adjacentID) { return find(adjacents.begin(), adjacents.end(), adjacentID) != adjacents.end(); }
};

class monster_t {

	public:
	monster_t () {}

	monster_t (vector<string> image) {
		vertical = image.size();
		horizontal = image.front().size();
		for (int row = 0; row < vertical; ++ row)
			for (int column = 0; column < horizontal; ++ column)
				if (image[row][column] == '#')
					monster.push_back(pair<int, int> (row, column));
		monster.shrink_to_fit();
	}

	private:
	int vertical;
	int horizontal;
	vector<pair<int, int>> monster;

	public:
	int verticalSize () { return vertical; }

	int horizontalSize () { return horizontal; }

	vector<pair<int, int>> & getPixels () { return monster; }
};

class image_t {

	public:
	image_t () {}

	image_t (vector<vector<support::pixel>> newImage) { image = newImage; }

	image_t (unordered_map<int, tile_t> &tiles, int *tilesMatrix, int sideLength) {

		int size = tiles.begin()->second.getSideLength();
		vector<support::pixel> pixels;
		vector<vector<bool>> tile;
		pixels.reserve(sideLength * (size - 2));

		// Filling the pixel line
		for (int index = 0; index < sideLength * (size - 2); ++ index)
			pixels.push_back(support::pixel::WHITE);

		// Creating the whole matrix
		for (int index = 0; index < sideLength * (size - 2); ++ index)
			image.push_back(pixels);

		// Filling the matrix
		for (int row = 0; row < sideLength; ++ row)
			for (int column = 0; column < sideLength; ++ column) {
				tile = tiles[tilesMatrix[row * sideLength + column]].getImage();
				for (int tileRow = 1; tileRow < size - 1; ++ tileRow)
					for (int tileColumn = 1; tileColumn < size - 1; ++ tileColumn)
						if (tile[tileRow][tileColumn])
							image[row * (size - 2) + tileRow - 1][column * (size - 2) + tileColumn - 1] = support::pixel::BLACK;
			}
	}

	private:
	vector<vector<support::pixel>> image;

	public:
	void changeOrientation (vector<vector<support::pixel>> newImage) { image = newImage; }

	vector<vector<support::pixel>> changeOrientation (support::rotation rotation, support::flip flip) {

		support::pixel tmp;
		int size = image.size();
		vector<vector<support::pixel>> newImage = image;

		switch (rotation) {

			case support::rotation::NONE:
				break;

			case support::rotation::LEFT:
				for (int row = 0; row < size / 2; ++ row)
					for (int column = row; column < size - row - 1; ++ column) {
						tmp = newImage[row][column];
						newImage[row][column] = newImage[column][size - row - 1];
						newImage[column][size - row - 1] = newImage[size - row - 1][size - column - 1];
						newImage[size - row - 1][size - column - 1] = newImage[size - column - 1][row];
						newImage[size - column - 1][row] = tmp;
					}
				break;

			case support::rotation::RIGHT:
				for (int row = 0; row < size / 2; ++ row)
					for (int column = row; column < size - row - 1; ++ column) {
						tmp = newImage[row][column];
						newImage[row][column] = newImage[size - column - 1][row];
						newImage[size - column - 1][row] = newImage[size - row - 1][size - column - 1];
						newImage[size - row - 1][size - column - 1] = newImage[column][size - row - 1];
						newImage[column][size - row - 1] = tmp;
					}
				break;

			case support::rotation::HALF:
				for (int row = 0; row < size; ++ row)
					for (int column = 0; column < size / 2; ++ column) {
						tmp = newImage[row][column];
						newImage[row][column] = newImage[size - row - 1][size - column - 1];
						newImage[size - row - 1][size - column - 1] = tmp;
					}
				break;
		}

		switch (flip) {

			case support::flip::NONE:
				break;

			case support::flip::HORIZONTAL:
				for (int row = 0; row < size; ++ row)
					for (int column = 0; column < size / 2; ++ column) {
						tmp = newImage[row][column];
						newImage[row][column] = newImage[row][size - column - 1];
						newImage[row][size - column - 1] = tmp;
					}
				break;

			case support::flip::VERTICAL:
				for (int row = 0; row < size / 2; ++ row)
					for (int column = 0; column < size; ++ column) {
						tmp = newImage[row][column];
						newImage[row][column] = newImage[size - row - 1][column];
						newImage[size - row - 1][column] = tmp;
					}
				break;
		}

		return newImage;
	}

	bool findMonster (monster_t &monster) {
		bool found;
		bool correctOrientation = false;
		for (int row = 0; row < image.size() - monster.verticalSize(); ++ row)
			for (int column = 0; column < image.front().size() - monster.horizontalSize(); ++ column) {
				found = true;
				for (pair<int, int> &coordinate: monster.getPixels())
					if (image[row + coordinate.first][column + coordinate.second] == support::pixel::BLACK)
						image[row + coordinate.first][column + coordinate.second] = support::pixel::MONSTER;
					else {
						for (pair<int, int> &processed: monster.getPixels()) {
							if (processed == coordinate)
								break;
							image[row + processed.first][column + processed.second] = support::pixel::BLACK;
						}
						found = false;
						break;
					}
				if (found)
					correctOrientation = true;
			}
		return correctOrientation;
	}

	int countBlack () {
		int count = 0;
		for (int row = 0; row < image.size(); ++ row)
			for (int column = 0; column < image.front().size(); ++ column)
				if (image[row][column] == support::pixel::BLACK)
					++ count;
		return count;
	}
};

int getTopLeftCorner (unordered_map<int, tile_t> &tiles);

void fillMatrix (unordered_map<int, tile_t> &tiles, int *tilesMatrix, int sideLength);

image_t getMonsters (image_t image, monster_t &monster);

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #20       |" << endl
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

	unordered_map<int, tile_t> tiles;
	monster_t monster = monster_t(vector<string> {
		"                  # ",
		"#    ##    ##    ###",
		" #  #  #  #  #  #   "}
	);
	image_t image;
	string tmpLine;
	int tmpId;
	int sideLength;
	int tilesNumber;
	int *tilesMatrix;
	vector<string> tmpImage;

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	while (getline(input, tmpLine, '\n')) {

		tmpImage.clear();

		if (tmpLine[tmpLine.size()-1] == '\r')
			tmpLine = tmpLine.substr(0, tmpLine.size()-1);

		tmpId = stoi(tmpLine.substr(5, 8));

		for (int index = 0; index < 10; ++ index) {
			getline(input, tmpLine, '\n');
			if (tmpLine[tmpLine.size()-1] == '\r')
				tmpLine = tmpLine.substr(0, tmpLine.size()-1);
			tmpImage.push_back(tmpLine);
		}

		tiles[tmpId] = tmpImage;

		getline(input, tmpLine, '\n');
	}

	input.close();

	tilesNumber = tiles.size();
	tilesMatrix = (int *) malloc(sizeof(int) * tilesNumber);
	sideLength = sqrt(tilesNumber);

	cout << "Processing input..." << endl;

	// Part 1
	tilesMatrix[0] = getTopLeftCorner(tiles);
	fillMatrix(tiles, tilesMatrix, sideLength);
	result[0] = tilesMatrix[0] * tilesMatrix[sideLength - 1] * tilesMatrix[tilesNumber - sideLength] * tilesMatrix[tilesNumber - 1];

	// Part 2
	image = getMonsters(image_t(tiles, tilesMatrix, sideLength), monster);
	result[1] = image.countBlack();

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}

int getTopLeftCorner (unordered_map<int, tile_t> &tiles) {

	int tlCorner = 0;
	support::fit fit;
	vector<support::fit> fits;
	fits.reserve(4);

	for (unordered_map<int, tile_t>::iterator currentTile = tiles.begin(); currentTile != tiles.end(); ++ currentTile) {

		fits.clear();

		for (unordered_map<int, tile_t>::iterator otherTile = tiles.begin(); otherTile != tiles.end() && fits.size() < 4; ++ otherTile) {

			if (otherTile == currentTile)
				continue;

			fit = currentTile->second.getFit(otherTile->second);

			if (fit != support::fit::NONE) {
				fits.push_back(fit);
				if (!otherTile->second.isAdjacent(currentTile->first)) {
					currentTile->second.addAdjacent(otherTile->first);
					otherTile->second.addAdjacent(currentTile->first);
				}
			}
		}

		if (fits.size() == 2 && tlCorner == 0) {
			if (fits[0] == support::fit::TOP || fits[1] == support::fit::TOP) {
				if (fits[0] == support::fit::LEFT || fits[1] == support::fit::LEFT)
					currentTile->second.changeOrientation(support::rotation::HALF, support::flip::NONE);
				else
					currentTile->second.changeOrientation(support::rotation::RIGHT, support::flip::NONE);
			} else if (fits[0] == support::fit::LEFT || fits[1] == support::fit::LEFT)
				currentTile->second.changeOrientation(support::rotation::LEFT, support::flip::NONE);
			tlCorner = currentTile->first;
		}
	}

	return tlCorner;
}

void fillMatrix (unordered_map<int, tile_t> &tiles, int *tilesMatrix, int sideLength) {

	support::fit fit;
	vector<int> adjacents;

	// Processing first line
	for (int index = 0; index < sideLength - 1; ++ index) {
		tiles[tilesMatrix[index]].setProcessed();
		for (int adjacent: tiles[tilesMatrix[index]].getAdjacents()) {
			if (tiles[adjacent].getProcessed())
				continue;
			if (tiles[tilesMatrix[index]].fitAndTransform(tiles[adjacent], support::fit::RIGHT) != support::fit::NONE)
				tilesMatrix[index + 1] = adjacent;
			else if (tiles[tilesMatrix[index]].fitAndTransform(tiles[adjacent], support::fit::BOTTOM) != support::fit::NONE)
				tilesMatrix[sideLength + index] = adjacent;
		}
	}
	tiles[tilesMatrix[sideLength - 1]].setProcessed();

	// Getting last element of second line
	for (int adjacent: tiles[tilesMatrix[sideLength + sideLength - 2]].getAdjacents()) {
		if (tiles[adjacent].getProcessed())
			continue;
		if (tiles[tilesMatrix[sideLength + sideLength - 2]].fitAndTransform(tiles[adjacent], support::fit::RIGHT) != support::fit::NONE)
			tilesMatrix[sideLength + sideLength - 1] = adjacent;
	}

	// Processing all the remaining lines
	for (int row = 1; row < sideLength - 1; ++ row)
		for (int column = 0; column < sideLength; ++ column) {
			tiles[tilesMatrix[row * sideLength + column]].setProcessed();
			for (int adjacent: tiles[tilesMatrix[row * sideLength + column]].getAdjacents()) {
				if (tiles[adjacent].getProcessed())
					continue;
				if (tiles[tilesMatrix[row * sideLength + column]].fitAndTransform(tiles[adjacent], support::fit::BOTTOM) != support::fit::NONE)
					tilesMatrix[(row + 1) * sideLength + column] = adjacent;
			}
		}
	
	// Setting as processed the last line
	for (int index = 0; index < sideLength; ++ index)
		tiles[tilesMatrix[(sideLength - 1) * sideLength + index]].setProcessed();
}

image_t getMonsters (image_t image, monster_t &monster) {
	bool found = false;
	image_t tmpImage = image;
	for (int rotation = 0; rotation < 4 && !found; ++ rotation)
		for (int flip = 0; flip < 3 && !found; ++ flip) {
			tmpImage = image.changeOrientation(static_cast<support::rotation>(rotation), static_cast<support::flip>(flip));
			if (tmpImage.findMonster(monster)) {
				found = true;
				image = tmpImage;
			}
		}
	return image;
}