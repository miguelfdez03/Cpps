#include "../include/ScalarConverter.hpp"

#include <iostream>
#include <sstream>
#include <climits>
#include <cmath>
#include <cerrno>
#include <cstdlib>
#include <limits>

// ─── helpers ────────────────────────────────────────────────────────────────

static bool isCharLiteral(const std::string &s)
{
	return s.length() == 3 && s[0] == '\'' && s[2] == '\'';
}

static bool isPseudoLiteral(const std::string &s)
{
	return (s == "-inff" || s == "+inff" || s == "nanf" ||
	        s == "-inf"  || s == "+inf"  || s == "nan");
}

static bool isIntLiteral(const std::string &s)
{
	if (s.empty())
		return false;
	size_t start = 0;
	if (s[0] == '-' || s[0] == '+')
		start = 1;
	if (start == s.length())
		return false;
	for (size_t i = start; i < s.length(); ++i)
		if (!std::isdigit(s[i]))
			return false;
	return true;
}

static bool isFloatLiteral(const std::string &s)
{
	if (s.empty() || s[s.length() - 1] != 'f')
		return false;
	std::string body = s.substr(0, s.length() - 1); // strip 'f'
	bool hasDot = false;
	size_t start = 0;
	if (!body.empty() && (body[0] == '-' || body[0] == '+'))
		start = 1;
	if (start == body.length())
		return false;
	for (size_t i = start; i < body.length(); ++i)
	{
		if (body[i] == '.' && !hasDot)
			hasDot = true;
		else if (!std::isdigit(body[i]))
			return false;
	}
	return hasDot;
}

static bool isDoubleLiteral(const std::string &s)
{
	bool hasDot = false;
	size_t start = 0;
	if (!s.empty() && (s[0] == '-' || s[0] == '+'))
		start = 1;
	if (start == s.length())
		return false;
	for (size_t i = start; i < s.length(); ++i)
	{
		if (s[i] == '.' && !hasDot)
			hasDot = true;
		else if (!std::isdigit(s[i]))
			return false;
	}
	return hasDot;
}

// ─── print helpers ──────────────────────────────────────────────────────────

static void printChar(double d)
{
	if (std::isnan(d) || std::isinf(d) || d < 0 || d > 127)
		std::cout << "char: impossible" << std::endl;
	else if (!std::isprint(static_cast<int>(d)))
		std::cout << "char: Non displayable" << std::endl;
	else
		std::cout << "char: '" << static_cast<char>(d) << "'" << std::endl;
}

static void printInt(double d)
{
	if (std::isnan(d) || std::isinf(d) ||
	    d > static_cast<double>(INT_MAX) || d < static_cast<double>(INT_MIN))
		std::cout << "int: impossible" << std::endl;
	else
		std::cout << "int: " << static_cast<int>(d) << std::endl;
}

static void printFloat(double d)
{
	float f = static_cast<float>(d);
	if (std::isinf(f))
		std::cout << "float: " << (f > 0 ? "+inff" : "-inff") << std::endl;
	else if (std::isnan(f))
		std::cout << "float: nanf" << std::endl;
	else
	{
		// Force at least one decimal place
		std::ostringstream oss;
		oss << f;
		std::string s = oss.str();
		bool hasDot = false;
		for (size_t i = 0; i < s.length(); ++i)
			if (s[i] == '.')
				hasDot = true;
		if (!hasDot)
			s += ".0";
		std::cout << "float: " << s << "f" << std::endl;
	}
}

static void printDouble(double d)
{
	if (std::isinf(d))
		std::cout << "double: " << (d > 0 ? "+inf" : "-inf") << std::endl;
	else if (std::isnan(d))
		std::cout << "double: nan" << std::endl;
	else
	{
		std::ostringstream oss;
		oss << d;
		std::string s = oss.str();
		bool hasDot = false;
		for (size_t i = 0; i < s.length(); ++i)
			if (s[i] == '.')
				hasDot = true;
		if (!hasDot)
			s += ".0";
		std::cout << "double: " << s << std::endl;
	}
}

static void printAll(double d)
{
	printChar(d);
	printInt(d);
	printFloat(d);
	printDouble(d);
}

// ─── convert ────────────────────────────────────────────────────────────────

void ScalarConverter::convert(const std::string &literal)
{
	// ── pseudo-literals ─────────────────────────────────────────────────────
	if (isPseudoLiteral(literal))
	{
		double d;
		if (literal == "nan" || literal == "nanf")
			d = std::numeric_limits<double>::quiet_NaN();
		else if (literal == "+inf" || literal == "+inff")
			d = std::numeric_limits<double>::infinity();
		else
			d = -std::numeric_limits<double>::infinity();
		std::cout << "char: impossible" << std::endl;
		std::cout << "int: impossible" << std::endl;
		printFloat(d);
		printDouble(d);
		return;
	}

	// ── char literal ────────────────────────────────────────────────────────
	if (isCharLiteral(literal))
	{
		char c = literal[1];
		double d = static_cast<double>(c);
		printAll(d);
		return;
	}

	// ── int literal ─────────────────────────────────────────────────────────
	if (isIntLiteral(literal))
	{
		errno = 0;
		long l = std::strtol(literal.c_str(), NULL, 10);
		if (errno == ERANGE || l > INT_MAX || l < INT_MIN)
		{
			std::cout << "char: impossible" << std::endl;
			std::cout << "int: impossible" << std::endl;
			std::cout << "float: impossible" << std::endl;
			std::cout << "double: impossible" << std::endl;
			return;
		}
		double d = static_cast<double>(l);
		printAll(d);
		return;
	}

	// ── float literal ───────────────────────────────────────────────────────
	if (isFloatLiteral(literal))
	{
		float f = static_cast<float>(std::strtod(literal.c_str(), NULL));
		double d = static_cast<double>(f);
		printAll(d);
		return;
	}

	// ── double literal ──────────────────────────────────────────────────────
	if (isDoubleLiteral(literal))
	{
		double d = std::strtod(literal.c_str(), NULL);
		printAll(d);
		return;
	}

	// ── unknown type ────────────────────────────────────────────────────────
	std::cerr << "Error: unrecognized literal type." << std::endl;
}
