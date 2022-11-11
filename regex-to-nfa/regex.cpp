#include "regex.h"
#include <iostream>
#include <stack>

RegEx::RegEx(const std::string& exp) {
	setExp(exp);
}

short opPriority(const char c) {
	switch (c) {
	case '|':
		return 0;
	case '.':
		return 1;
	case '*':
		return 0;
	default:
		return -1;
	}
}

void RegEx::setExp(const std::string& exp) {
	this->_in_exp = exp;

	this->_post_exp.clear();
	std::stack<char> s;
	for (char c : exp) {
		if (c == ' ') continue;
		else if (c == 'a' || c == 'b') this->_post_exp += c;
		else if (c == ')') {
			while (s.top() != '(') {
				this->_post_exp += s.top();
				s.pop();
			}
			s.pop();
		}
		else if (c == '(') {
			s.push(c);
		}
		else {
			while (!s.empty() && s.top() != '(' && opPriority(s.top()) < opPriority(c)) {
				this->_post_exp += s.top();
				s.pop();
			}
			s.push(c);
		}
	}
	while (!s.empty()) {
		this->_post_exp += s.top();
		s.pop();
	}
}

std::string RegEx::getInExp() {
	return this->_in_exp;
}

std::string RegEx::getPostExp() {
	return this->_post_exp;
}

