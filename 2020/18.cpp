#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

using namespace std;

class expression_t {

	public:
	expression_t () {}

	expression_t (string line) {
		items_t item;
		string tmpString;
		for (int index = 0; index < line.length(); ++ index) {
			if (line[index] == ' ')
				continue;
			item.number = 0;
			switch (line[index]) {
				case '+': item.type = SUM; break;
				case '*': item.type = MULTIPLICATION; break;
				case '(': item.type = OPEN_BRACKET; break;
				case ')': item.type = CLOSED_BRACKET; break;
				default:
					item.type = NUMBER;
					item.number = line[index] - '0';
			}
			expression.push_back(item);
			expression.shrink_to_fit();
		}
	}

	public:
	typedef enum {SUM, MULTIPLICATION, OPEN_BRACKET, CLOSED_BRACKET, NUMBER} types_t;
	typedef enum {SAME_PRECEDENCE, NEW_PRECEDENCE} precedence_t;

	private:
	typedef struct {
		types_t type;
		int number;
	} items_t;
	vector<items_t> expression;

	public:
	expression_t getPrefixNotation (precedence_t precedence) {

		expression_t output;
		vector<items_t> modifiedExpression = expression;
		vector<items_t> stack;

		while (!modifiedExpression.empty()) {

			switch (modifiedExpression.back().type) {

				case SUM:
					stack.push_back(modifiedExpression.back());
					break;

				case MULTIPLICATION:
					if (precedence == NEW_PRECEDENCE)
						while (!stack.empty() && (stack.back().type == SUM || stack.back().type == MULTIPLICATION)) {
							output.expression.push_back(stack.back());
							stack.pop_back();
						}
					stack.push_back(modifiedExpression.back());
					break;

				case OPEN_BRACKET:
					while (stack.back().type != CLOSED_BRACKET) {
						output.expression.push_back(stack.back());
						stack.pop_back();
					}
					stack.pop_back();
					break;

				case CLOSED_BRACKET:
					stack.push_back(modifiedExpression.back());
					break;

				case NUMBER:
					output.expression.push_back(modifiedExpression.back());
					break;
			}

			modifiedExpression.pop_back();
		}

		while (!stack.empty()) {
			output.expression.push_back(stack.back());
			stack.pop_back();
		}

		reverse(output.expression.begin(), output.expression.end());
		
		return output;
	}

	int64_t calculate () {

		vector<int64_t> stack;
		int64_t tmp;

		stack.push_back(expression.back().number);
		expression.pop_back();

		while (!expression.empty()) {

			if (expression.back().type != NUMBER) {

				tmp = stack.back();
				stack.pop_back();

				switch (expression.back().type) {
					case SUM: tmp += stack.back(); break;
					case MULTIPLICATION: tmp *= stack.back(); break;
					default: return 0;
				}

				stack.back() = tmp;
				
			} else
				stack.push_back(expression.back().number);

			expression.pop_back();
		}

		return stack.front();
	}
};

int main (int argc, char *argv[]) {

	cout << endl
		<< "+---------------------+" << endl
		<< "| Advent of Code 2020 |" << endl
		<< "+---------------------+" << endl
		<< "|       Day #18       |" << endl
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

	vector<expression_t> expressions;
	string tmpLine;

	cout << endl;
	cout << "Reading and parsing input..." << endl;

	while (getline(input, tmpLine, '\n')) {
		if (tmpLine[tmpLine.size()-1] == '\r')
			tmpLine = tmpLine.substr(0, tmpLine.size()-1);
		expressions.push_back(expression_t(tmpLine));
	}

	input.close();

	cout << "Processing input..." << endl;

	for (expression_t &expression: expressions) {
		result[0] += expression.getPrefixNotation(expression_t::SAME_PRECEDENCE).calculate();
		result[1] += expression.getPrefixNotation(expression_t::NEW_PRECEDENCE).calculate();
	}

	cout << endl;
	cout << "Results:" << endl;
	cout << "- Part 1: " << result[0] << endl;
	cout << "- Part 2: " << result[1] << endl << endl;

	return 0;
}