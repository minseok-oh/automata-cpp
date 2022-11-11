#include <string>

class RegEx {
private:
	std::string _in_exp;
	std::string _post_exp;

	RegEx() {};
	RegEx(const std::string& exp);
	~RegEx() {};
public:
	void setExp(const std::string& exp);
	std::string getInExp();
	std::string getPostExp();

	static RegEx& getInstance() {
		static RegEx exp;
		return exp;
	}
};