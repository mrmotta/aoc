#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class field_t {

	private:
	typedef struct {
		int min;
		int max;
	} range_t;

	public:
	string type;
	range_t firstRange;
	range_t secondRange;

	public:
	void initialize (string line) {
		type = line.substr(0, line.find(":"));
		line = line.substr(type.size()+2, line.size());
		firstRange.min = stoi(line.substr(0, line.find("-")));
		line = line.substr(line.find("-")+1, line.size());
		firstRange.max = stoi(line.substr(0, line.find(" ")));
		line = line.substr(line.find(" ")+4, line.size());
		secondRange.min = stoi(line.substr(0, line.find("-")));
		line = line.substr(line.find("-")+1, line.size());
		secondRange.max = stoi(line);
	}
};

class ticket_t {

	private:
	vector<int> fields;
	bool valid;

	public:
	void initialize (string line) {
		valid = true;
		while (line.find(",") != string::npos) {
			fields.push_back(stoi(line.substr(0, line.find(","))));
			line = line.substr(line.find(",")+1, line.size());
		};
		fields.push_back(stoi(line));
	}

	bool isValid () { return valid; }

	void clear () { fields.clear(); }
	
	int checkValidity (vector<field_t> &ticketFields) {
		bool correct;
		for (int field: fields) {
			correct = false;
			for (field_t &ticketField: ticketFields)
				if ((field >= ticketField.firstRange.min && field <= ticketField.firstRange.max) || (field >= ticketField.secondRange.min && field <= ticketField.secondRange.max)) {
					correct = true;
					break;
				}
			if (!correct) {
				valid = false;
				return field;
			}
		}
		return 0;
	}

	bool fits (int index, field_t field) {
		return (fields[index] >= field.firstRange.min && fields[index] <= field.firstRange.max) || (fields[index] >= field.secondRange.min && fields[index] <= field.secondRange.max);
	}

	int getFiled (int index) { return fields[index]; }
};

class match_t {

	private:
	string field;
	vector<int> matches;

	public:
	string getField () { return field; }

	void setField (string newField) { field = newField; }

	vector<int> getMatches () { return matches; }

	void append (int match) { matches.push_back(match); }

	int first () { return matches.front(); }

	int length () { return matches.size(); }

	bool contains (int item) { return binary_search(matches.begin(), matches.end(), item); }

	void remove (int item) { matches.erase(lower_bound(matches.begin(), matches.end(), item)); }

	void reset (int index) {
		matches.clear();
		matches.push_back(index);
	}
};

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #16       |" << endl
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

	vector<field_t> fields;
	field_t tmpField;
	vector<ticket_t> tickets;
	ticket_t myTicket;
	ticket_t tmpTicket;
	vector<match_t> matches;
	match_t tmpMatch;
	string tmpLine;
	bool valid;
	bool end = false;

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	// Getting all the fields
	while (getline(input, tmpLine, '\n')) {
		if (tmpLine[tmpLine.size()-1] == '\r')
			tmpLine = tmpLine.substr(0, tmpLine.size()-1);
		if (tmpLine.empty())
			break;
		tmpField.initialize(tmpLine);
		fields.push_back(tmpField);
	}

	// Getting my ticket
	getline(input, tmpLine, '\n');
	getline(input, tmpLine, '\n');
	if (tmpLine[tmpLine.size()-1] == '\r')
		tmpLine = tmpLine.substr(0, tmpLine.size()-1);
	myTicket.initialize(tmpLine);

	// Getting all the other tickets 
	getline(input, tmpLine, '\n');
	getline(input, tmpLine, '\n');
	while (getline(input, tmpLine, '\n')) {
		if (tmpLine[tmpLine.size()-1] == '\r')
			tmpLine = tmpLine.substr(0, tmpLine.size()-1);
		tmpTicket.initialize(tmpLine);
		tickets.push_back(tmpTicket);
		tmpTicket.clear();
	}

	input.close();
	
	cout << "Processing input..." << endl;

	// Part 1
	for (ticket_t &ticket: tickets)
		result[0] += ticket.checkValidity(fields);
	
	// Part 2: preparing the environment
	for (field_t &field: fields)
		for (int index = 0; index < fields.size(); ++ index) {
			valid = true;
			for (ticket_t &ticket: tickets)
				if (ticket.isValid() && !ticket.fits(index, field)) {
					valid = false;
					break;
				}
			if (valid) {
				bool found = false;
				for (match_t &element: matches)
					if (element.getField() == field.type) {
						element.append(index);
						found = true;
						break;
					}
				if (!found) {
					tmpMatch.reset(index);
					tmpMatch.setField(field.type);
					matches.push_back(tmpMatch);
				}
			}
		}

	// Part 2: finding out which position corresponds to which field
	while (!end) {
		end = true;
		for (match_t &match: matches) {
			if (match.length() == 1) {
				for (match_t &match2: matches)
					if (match2.getField() != match.getField() && match2.contains(match.first()))
						match2.remove(match.first());
			} else {
				end = false;
				for (int index: match.getMatches()) {
					valid = true;
					for (match_t &match2: matches)
						if (match2.getField() != match.getField() && match2.contains(index)) {
							valid = false;
							break;
						}
					if (valid) {
						match.reset(index);
						for (match_t &match2: matches)
							if (match2.getField() != match.getField() && match2.contains(index))
								match2.remove(match.first());
					}
				}
			}
		}
	}

	// Part 2: calculating the result
	result[1] = 1;
	for (match_t &match: matches)
		if (match.getField().find("departure") != string::npos)
			result[1] *= myTicket.getFiled(match.first());

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}