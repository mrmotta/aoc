#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #04       |" << endl
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

	class passport {

		public:							// Passport fields
		int byr, iyr, eyr, hgt;
		bool hgtCm;
		string hcl, ecl, pid, cid;

		public:							// Initializer
		void intialize (void) {
			byr = -1;
			iyr = -1;
			eyr = -1;
			hgt = -1;
			hgtCm = false;
			hcl = "";
			ecl = "";
			pid = "";
			cid = "";
		}

		public:							// General setter
		void insertValue (string key, string value) {
			if (key == "byr") setByr(value);
			else if (key == "iyr") setIyr(value);
			else if (key == "eyr") setEyr(value);
			else if (key == "hgt") setHgt(value);
			else if (key == "hcl") hcl = value;
			else if (key == "ecl") ecl = value;
			else if (key == "pid") pid = value;
			else cid = value;
		}

		private:						// Specific setters
		void setByr (string value) { byr = atoi(value.c_str()); }
		void setIyr (string value) { iyr = atoi(value.c_str()); }
		void setEyr (string value) { eyr = atoi(value.c_str()); }
		void setHgt (string value) {
			hgt = atoi(value.substr(0, value.size()-2).c_str());
			if (value.find("cm") != string::npos)
				hgtCm = true;
			else
				hgtCm = false;
		}

		public:							// Utilities
		bool hasAllRequiredFields() { return byr != -1 && iyr != -1 && eyr != -1 && hgt != -1 && !hcl.empty() && !ecl.empty() && !pid.empty(); }
		bool isValid() {
			if (byr < 1920 || byr > 2002) return false;
			if (iyr < 2010 || iyr > 2020) return false;
			if (eyr < 2020 || eyr > 2030) return false;
			if (hgtCm && (hgt < 150 || hgt > 193)) return false;
			if (!hgtCm && (hgt < 59 || hgt > 76)) return false;
			if (!checkHcl()) return false;
			if (!checkEcl()) return false;
			if (!checkPid()) return false;
			return true;
		}

		private:
		bool checkHcl () {
			if (hcl[0] != '#') return false;
			for (char character: hcl.substr(1, hcl.size()-1))
				if ((character < '0' || character > '9') && (character < 'a' || character > 'f')) return false;
			return true;
		}
		char colorList[7][4] = {"amb", "blu", "brn", "gry", "grn", "hzl", "oth"};
		bool checkEcl () {
			for (int index = 0; index < 7; ++ index)
				if (ecl == colorList[index]) return true;
			return false;
		}
		bool checkPid () {
			if (pid.size() != 9) return false;
			for (char number: pid)
				if (number < '0' || number > '9') return false;
			return true;
		}
	};
	vector<passport> list;				// Passport list
	passport tmpPassport;				// Temporary passport

	string tmpString;					// It has multiple purposes during the input process...
	istringstream tmpIStringStream;		// like this one

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	// Initializing passport
	tmpPassport.intialize();

	while (getline(input, tmpString)) {
		// Checking if it's time to add it to the list or not
		if (tmpString.size() > 1) {		// Unfortunately std::string::empty doesn't always work
			// Figuring out what I've found
			tmpIStringStream = (istringstream) tmpString;
			while (tmpIStringStream >> tmpString)
				tmpPassport.insertValue(tmpString.substr(0, tmpString.find(":")), tmpString.substr(tmpString.find(":")+1, tmpString.size()-1));
		} else {
			list.push_back(tmpPassport);
			// Reinitialising passport
			tmpPassport.intialize();
		}
	}
	
	// Don't forget the last one :)
	list.push_back(tmpPassport);

	input.close();
	
	cout << "Processing input..." << endl;

	for (auto &tmpPassport: list)
		if (tmpPassport.hasAllRequiredFields()) {
			++ result[0];
			if (tmpPassport.isValid())
				++ result[1];
		}

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}