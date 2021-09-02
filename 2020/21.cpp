#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class food_t {
	
	public:
	food_t (string line, unordered_set<string> &ingredientsList) {

		string tmp;

		// Getting ingredients
		while (line.front() != '(') {
			tmp = line.substr(0, line.find(" "));
			ingredients.insert(tmp);
			ingredientsList.insert(tmp);
			line = line.substr(line.find(" ")+1, line.size());
		}
		
		// Getting allergens
		line = line.substr(10, line.size()-11);
		while (line.find(",") != string::npos) {
			allergens.insert(line.substr(0, line.find(",")));
			line = line.substr(line.find(",")+2, line.size());
		}
		allergens.insert(line);
	}

	private:
	unordered_set<string> allergens;
	unordered_set<string> ingredients;

	public:
	unordered_set<string> & getAllergens () { return allergens; }

	unordered_set<string> & getIngredients () { return ingredients; }
};

void prepare (vector<food_t> &foods, unordered_map<string, unordered_set<string>> &allergens, unordered_set<string> &ingredientsWithAllergens);
vector<string> getSortedList (unordered_map<string, unordered_set<string>> &allergens);

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #21       |" << endl
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

	int64_t result1 = 0;
    string result2;

	vector<food_t> foods;
	unordered_map<string, unordered_set<string>> allergens;
	unordered_set<string> ingredientsWithAllergens;
	string tmpLine;

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	while (getline(input, tmpLine, '\n')) {
		if (tmpLine[tmpLine.size()-1] == '\r')
			tmpLine = tmpLine.substr(0, tmpLine.size()-1);
		foods.push_back(food_t(tmpLine, ingredientsWithAllergens));
	}

	input.close();

	cout << "Processing input..." << endl;

	// Preparation
	prepare(foods, allergens, ingredientsWithAllergens);

	// Part 1
	for (const string &ingredient: ingredientsWithAllergens)
		for (food_t &food: foods)
			if (food.getIngredients().find(ingredient) != food.getIngredients().end())
				++ result1;
	
	// Part 2
	for (const string &ingredient: getSortedList(allergens)) {
		result2.append(ingredient);
		result2.push_back(',');
	}
	result2 = result2.substr(0, result2.size()-1);

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result1 << endl;
	cout << "- Part 2: " << result2 << endl << endl;

	return 0;
}

void prepare (vector<food_t> &foods, unordered_map<string, unordered_set<string>> &allergens, unordered_set<string> &ingredientsWithAllergens) {

	bool end = false;
	vector<string> toRemove;

	// Intersection between possible ingredients that can contain allergens
	for (food_t &food: foods) {
		for (const string &allergen: food.getAllergens()) {
			if (allergens.find(allergen) != allergens.end()) {
				toRemove.clear();
				for (const string &ingredient: allergens[allergen]) {
					ingredientsWithAllergens.insert(ingredient);
					if (food.getIngredients().find(ingredient) == food.getIngredients().end())
						toRemove.push_back(ingredient);
				}
				for (string &element: toRemove)
					allergens[allergen].erase(element);
			} else
				allergens[allergen] = food.getIngredients();
		}
	}

	// Getting pairs ingredient-allergen
	while (!end) {
		end = true;
		for (unordered_map<string, unordered_set<string>>::iterator allergen = allergens.begin(); allergen != allergens.end(); ++ allergen)
			if (allergen->second.size() == 1) {
				for (unordered_map<string, unordered_set<string>>::iterator other = allergens.begin(); other != allergens.end(); ++ other) {
					if (other == allergen)
						continue;
					other->second.erase(*allergen->second.begin());
				}
			} else
				end = false;
	}

	// Removing ingredients
	for (const pair<string, unordered_set<string>> &allergen: allergens)
		ingredientsWithAllergens.erase(*allergen.second.begin());
}

vector<string> getSortedList (unordered_map<string, unordered_set<string>> &allergens) {

	vector<string> sortedList;
	map<string, string> sortedMap;

	// Sorting elements by allergen
	for (const pair<string, unordered_set<string>> &allergen: allergens)
		sortedMap.insert(pair<string, string> (allergen.first, *allergen.second.begin()));

	// Extracting only the ingredients
	for (const pair<string, string> &allergen: sortedMap)
		sortedList.push_back(allergen.second);

	return sortedList;
}