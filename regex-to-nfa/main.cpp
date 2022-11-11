#include <iostream>
#include "regex.h"
#include "nfa.h"

int main() {
	RegEx& exp = RegEx::getInstance();
	std::string reg;
	std::cout << "Enter Regular Expression >> ";
	std::cin >> reg;
	exp.setExp(reg);

	NFA temp = NFA();
	temp.makeNFA(exp.getPostExp());
	temp.print();

	while (1) {
		std::string input;
		std::cout << "Enter string >> ";
		std::cin >> input;

		bool cal = temp.accept(input);
		std::cout << "ANSWER: ";
		if (cal) std::cout << "TRUE\n";
		else std::cout << "FALSE\n";
	}
	return 0;
}