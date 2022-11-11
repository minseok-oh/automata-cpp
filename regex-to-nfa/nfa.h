#include <string>
#include <vector>
#include <map>

struct State {
	bool isEnd;
	int state;
	std::map<char, std::vector<State*>> trans;
	std::vector<State*> epsilon;
};

struct Frag {
	State* start, * end;
};

class NFA {
private:
	Frag* nfa;
	int count;
public:
	NFA();
	NFA(std::string post_exp);
	~NFA();

	void print();
	void recursivePrint(State* here, std::vector<bool>& visited);

	State* createState(const bool isEnd);
	void addEpsilon(State* from, State* to);
	void addTransition(State* from, State* to, char symbol);
	Frag* fromEpsilon();
	Frag* fromSymbol(const char symbol);

	Frag* concat(Frag* first, Frag* second);
	Frag* merge(Frag* first, Frag* second);
	Frag* star(Frag* here);
	void makeNFA(std::string post_exp);

	bool accept(std::string input);
};