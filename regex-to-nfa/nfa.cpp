#include "nfa.h"
#include <iostream>
#include <iomanip>
#include <stack>
#include <queue>

NFA::NFA() {
	count = 0;
};

NFA::NFA(std::string post_exp) {
	count = 0;
	makeNFA(post_exp);
}

NFA::~NFA() {

}

void NFA::print() {
	std::cout << " ------------------------------------------" << std::endl;
	std::cout << "|State|     a    |     b    |     e        |" << std::endl;
	std::cout << "|------------------------------------------|" << std::endl;

	std::vector<bool> visited(count, false);
	visited[this->nfa->start->state] = true;
	recursivePrint(this->nfa->start, visited);
	std::cout << " ------------------------------------------" << std::endl;
}

void NFA::recursivePrint(State* here, std::vector<bool>& visited) {
	std::cout << "|";
	if (here->isEnd) {
		std::cout << "*q" << here->state << "  |";
	}
	else {
		std::cout << "q" << here->state << "   |";
	}
	int temp_count = count;
	
	std::cout << "{";
	for (auto iter : here->trans['a']) std::cout << "q" << iter->state << ", ";
	for (int i = here->trans['a'].size(); i < 2; i++) std::cout << "    ";
	std::cout << "}|";

	std::cout << "{";
	for (auto iter : here->trans['b']) std::cout << "q" << iter->state << ", ";
	for (int i = here->trans['b'].size(); i < 2; i++) std::cout << "    ";
	std::cout << "}|";

	std::cout << "{";
	for (auto iter : here->epsilon) std::cout << "q" << iter->state << ", ";
	for (int i = here->epsilon.size(); i < 3; i++) std::cout << "    ";
	std::cout << "}|\n";
	if (here->isEnd) return;

	for (auto iter : here->trans['a']) {
		if (visited[iter->state]) continue;
		visited[iter->state] = true;
		recursivePrint(iter, visited);
	}
	for (auto iter : here->trans['b']) {
		if (visited[iter->state]) continue;
		visited[iter->state] = true;
		recursivePrint(iter, visited);
	}
	for (auto iter : here->epsilon) {
		if (visited[iter->state]) continue;
		visited[iter->state] = true;
		recursivePrint(iter, visited);
	};
}

State* NFA::createState(const bool isEnd) {
	State* ret = new State();
	ret->isEnd = isEnd;
	ret->state = count++;
	return ret;
}

void NFA::addEpsilon(State* from, State* to) {
	from->epsilon.push_back(to);
}

void NFA::addTransition(State* from, State* to, char symbol) {
	from->trans[symbol].push_back(to);
}

Frag* NFA::fromEpsilon() {
	State* start = createState(false);
	State* end = createState(true);
	addEpsilon(start, end);

	Frag* res = new Frag({ start, end });
	return res;
}

Frag* NFA::fromSymbol(const char symbol) {
	State* start = createState(false);
	State* end = createState(true);
	addTransition(start, end, symbol);

	Frag* res = new Frag({ start, end });
	return res;
}

Frag* NFA::concat(Frag* first, Frag* second) {
	addEpsilon(first->end, second->start);
	first->end->isEnd = false;
	first->end = second->end;

	return first;
}
Frag* NFA::merge(Frag* first, Frag* second) {
	State* start = createState(false);
	addEpsilon(start, first->start);
	addEpsilon(start, second->start);

	State* end = createState(true);
	addEpsilon(first->end, end);
	first->end->isEnd = false;
	addEpsilon(second->end, end);
	second->end->isEnd = false;

	Frag* res = new Frag({ start, end });
	return res;
}
Frag* NFA::star(Frag* here) {
	addEpsilon(here->start, here->end);
	addEpsilon(here->end, here->start);

	return here;
}

void NFA::makeNFA(std::string post_exp) {
	std::stack<Frag*> s;
	Frag* right, * left;

	for (char c : post_exp) {
		if ('a' <= c && c <= 'z') {
			s.push(fromSymbol(c));
		}
		else if (c == '.') {
			right = s.top(); s.pop();
			left = s.top(); s.pop();
			s.push(concat(left, right));
		}
		else if (c == '|') {
			right = s.top(); s.pop();
			left = s.top(); s.pop();
			s.push(merge(left, right));
		}
		else if (c == '*') {
			right = s.top(); s.pop();
			s.push(star(right));
		}
	}

	this->nfa = s.top();
}

bool NFA::accept(std::string input) {
	std::queue<std::pair<Frag*, std::string>> q;

	Frag* temp = new Frag();
	temp->start = NULL;
	temp->end = this->nfa->start;

	q.push(make_pair(temp, input));
	while (!q.empty()) {
		Frag* hereState = q.front().first;
		std::string hereExp = q.front().second; q.pop();
		if (hereExp.empty() && hereState->end->isEnd) return true;

		if (!hereExp.empty()) {
			char here = hereExp[0];
			for (auto iter : hereState->end->trans[here]) {

				Frag* next = new Frag();
				next->start = hereState->end;
				next->end = iter;

				q.push(make_pair(next, hereExp.substr(1)));
			}
		}
		for (auto iter : hereState->end->epsilon) {
			if (hereState->start != NULL && iter->state == hereState->start->state) continue;

			Frag* next = new Frag();
			next->start = hereState->end;
			next->end = iter;

			q.push(make_pair(next, hereExp));
		}
	}
	return false;
}