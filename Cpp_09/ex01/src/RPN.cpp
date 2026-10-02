#include "../include/RPN.hpp"
#include <stack>
#include <sstream>
#include <stdexcept>

// ─── Orthodox Canonical Form ────────────────────────────────────────────────

RPN::RPN() {}

RPN::RPN(const RPN &other)
{
	// stack has no meaningful state to copy between evaluations;
	// evaluate() creates a local stack each time.
	(void)other;
}

RPN &RPN::operator=(const RPN &other)
{
	(void)other;
	return *this;
}

RPN::~RPN() {}

// ─── Helpers ─────────────────────────────────────────────────────────────────

bool RPN::isOperator(char c) const
{
	return (c == '+' || c == '-' || c == '*' || c == '/');
}

bool RPN::isDigit(char c) const
{
	return (c >= '0' && c <= '9');
}

// ─── Evaluator ───────────────────────────────────────────────────────────────

int RPN::evaluate(const std::string &expression) const
{
	// std::stack: LIFO container, ideal for RPN processing.
	// Each token is either a single-digit operand (pushed) or an operator
	// (pops two operands, computes, pushes result).
	std::stack<int> stack;

	std::istringstream iss(expression);
	std::string token;

	while (iss >> token)
	{
		if (token.size() == 1 && isDigit(token[0]))
		{
			// Single digit operand (subject states numbers < 10)
			stack.push(token[0] - '0');
		}
		else if (token.size() == 1 && isOperator(token[0]))
		{
			if (stack.size() < 2)
				throw std::runtime_error("Error: insufficient operands for operator '" + token + "'.");

			int b = stack.top(); stack.pop();
			int a = stack.top(); stack.pop();

			int result = 0;
			switch (token[0])
			{
				case '+': result = a + b; break;
				case '-': result = a - b; break;
				case '*': result = a * b; break;
				case '/':
					if (b == 0)
						throw std::runtime_error("Error: division by zero.");
					result = a / b;
					break;
			}
			stack.push(result);
		}
		else
		{
			throw std::runtime_error("Error: invalid token '" + token + "'.");
		}
	}

	if (stack.empty())
		throw std::runtime_error("Error: empty expression.");
	if (stack.size() != 1)
		throw std::runtime_error("Error: too many operands remaining.");

	return stack.top();
}
