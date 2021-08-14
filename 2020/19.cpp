#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#define NEW_RULE_11 "11: 42 31 | 42 11 31"

using namespace std;

class rule_t {

	public:
	rule_t (string line) {

		string number;

		id = stoi(line.substr(0, line.find(":")));
		line = line.substr(line.find(":")+2, line.size());

		if (line[0] == '"') {
			letter = line[1];
			return;
		}

		++ rules;

		for (char character: line) {
			if (character >= '0' && character <= '9') {
				number.push_back(character);
			} else {
				if (!number.empty()) {
					if (rules == 1)
						rule1.push_back(stoi(number));
					else
						rule2.push_back(stoi(number));
					number.clear();
				}
				if (character == '|')
					++ rules;
			}
		}
		if (rules == 1)
			rule1.push_back(stoi(number));
		else
			rule2.push_back(stoi(number));
	}

	private:
	int id;
	int rules = 0;
	char letter = '#';
	vector<int> rule1;
	vector<int> rule2;

	public:
	int getId () { return id; }

	bool isLetter () { return letter != '#'; }

	char getLetter () { return letter; }

	vector<int> getFirstRule () { return rule1; }

	bool hasSecondRule () { return rules == 2; }

	vector<int> getSecondRule () { return rule2; }
};

class message_t {

	public:
	message_t (string line) { message = line; }

	private:
	bool valid = false;
	string message;

	public:
	bool isValid (vector<rule_t> &rules, bool loop = false) {

		int result;

		if (!loop)
			result = process(rules);
		else
			result = processLoop(rules);

		if (result == message.size()) {
			valid = true;
			return true;
		}

		return false;
	}

	bool isValid () { return valid; }

	private:
	int process (vector<rule_t> &rules, int messageIndex = 0, int ruleIndex = 0) {
		
		int nextIndex = messageIndex;

		if (messageIndex == message.size())
			return -1;

		if (rules[ruleIndex].isLetter()) {
			if (message[messageIndex] == rules[ruleIndex].getLetter())
				return messageIndex + 1;
			return -1;
		}

		for (int nextRule: rules[ruleIndex].getFirstRule()) {
			nextIndex = process(rules, nextIndex, nextRule);
			if (nextIndex == -1)
				break;
		}

		if (nextIndex != -1)
			return nextIndex;

		if (!rules[ruleIndex].hasSecondRule())
			return -1;
		
		nextIndex = messageIndex;
		
		for (int nextRule: rules[ruleIndex].getSecondRule()) {
			nextIndex = process(rules, nextIndex, nextRule);
			if (nextIndex == -1)
				break;
		}

		return nextIndex;
	}

	int processLoop (vector<rule_t> &rules) { return processLoop(rules, 0, true); }

	int processLoop (vector<rule_t> &rules, int messageIndex, bool first = false) {

		int nextIndex = process(rules, messageIndex, 8);

		if (nextIndex == -1)
			return -1;

		nextIndex = processLoop(rules, nextIndex);

		if (nextIndex != -1)
			return nextIndex;

		nextIndex = process(rules, messageIndex, 11);

		if (nextIndex == message.size() && !first)
			return nextIndex;

		return -1;
	}
};

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #19       |" << endl
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

	vector<rule_t> rules;
	vector<message_t> messages;
	string tmpLine;

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	// Getting rules
	while (getline(input, tmpLine, '\n')) {
		if (tmpLine[tmpLine.size()-1] == '\r')
			tmpLine = tmpLine.substr(0, tmpLine.size()-1);
		if (tmpLine.empty())
			break;
		rules.push_back(rule_t(tmpLine));
	}

	// Getting messages
	while (getline(input, tmpLine, '\n')) {
		if (tmpLine[tmpLine.size()-1] == '\r')
			tmpLine = tmpLine.substr(0, tmpLine.size()-1);
		messages.push_back(message_t(tmpLine));
	}

	input.close();

	sort(rules.begin(), rules.end(),
		[] (rule_t rule1, rule_t rule2) {
			return rule1.getId() < rule2.getId();
		});

	cout << "Processing input..." << endl;

	// Part 1
	for (message_t &message: messages)
		if (message.isValid(rules))
			++ result[0];

	// Preparation for Part 2
	for (int index = 0; index < messages.size(); ++ index)
		if (messages[index].isValid()) {
			messages.erase(messages.begin() + index);
			-- index;
		}

	result[1] = result[0];

	rules[11] = rule_t(NEW_RULE_11);

	// Part 2
	for (message_t &message: messages)
		if (message.isValid(rules, true))
			++ result[1];

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}
