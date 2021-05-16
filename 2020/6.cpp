#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <algorithm>

using namespace std;

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #06       |" << endl
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

	class groupAnswers_t {
		private:						// Answers set
		vector<string> group;

		public:							// Getter and reset
		vector<string> getGroup () { return group; }
		void clear () { group.clear(); }

		public:							// Add and sort algorithms
		void addAnswers (string answers) {
			sort(answers.begin(), answers.end());
			group.push_back(answers);
		}
		void sortGroup () {
			sort(group.begin(), group.end(),
				[] (string votes1, string votes2) -> bool {
					return votes1.size() < votes2.size();
				});
		}

		public:							// Votes counting
		int anyone () {
			int sum = 0;
			bool totalVotes[26] = {false};
			for (string person: group)
				for (char vote: person)
					totalVotes[vote - 'a'] = true;
			for (bool vote: totalVotes)
				if (vote) ++ sum;
			return sum;
		}
		int everyone () {
			// If there's only one person in the group it's easy
			if (group.size() == 1)
				return group[0].size();
			
			// Otherwise let's do something else
			int sum = 0;
			string referencePerson = group[0];
			for (char referenceVote: referencePerson)
				if (checkPresence(referenceVote))
					++ sum;
			
			return sum;
		}

		private:						// Simply checking if everyone has that vote
		bool checkPresence (char referenceVote) {
			for (int index = 1; index < group.size(); ++ index)
				if (!binary_search(group[index].begin(), group[index].end(), referenceVote))
					return false;
			return true;
		}
	};
	vector<groupAnswers_t> list;		// List of grous answers

	groupAnswers_t tmpGroup;			// Temporary customs declaration form
	string tmpAnswers;					// Used for line-by-line reading

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	while (getline(input, tmpAnswers, '\n')) {
		if (tmpAnswers[tmpAnswers.size()-1] == '\r')
			tmpAnswers = tmpAnswers.substr(0, tmpAnswers.size()-1);
		// Checking if it's time to add it to the list or not
		if (!tmpAnswers.empty())
			tmpGroup.addAnswers(tmpAnswers);
		else {
			tmpGroup.sortGroup();
			list.push_back(tmpGroup);
			tmpGroup.clear();
		}
	}

	// Don't forget the last one :)
	tmpGroup.sortGroup();
	list.push_back(tmpGroup);

	input.close();
	
	cout << "Processing input..." << endl;

	// Summing all the counts of votes
	for (groupAnswers_t group: list) {
		result[0] += group.anyone();
		result[1] += group.everyone();
	}

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}