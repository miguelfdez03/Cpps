#ifndef RPN_HPP
# define RPN_HPP

# include <stack>
# include <string>
# include <iostream>
# include <sstream>
# include <stdexcept>
# include <cstdlib>

class RPN
{
public:
	// Orthodox Canonical Form
	RPN();
	RPN(const RPN &other);
	RPN &operator=(const RPN &other);
	~RPN();

	// Core method: evaluates an RPN expression string, returns the result
	// Throws std::runtime_error on invalid input or division by zero
	int evaluate(const std::string &expression) const;

private:
	bool isOperator(char c) const;
	bool isDigit(char c) const;
};

#endif // RPN_HPP
