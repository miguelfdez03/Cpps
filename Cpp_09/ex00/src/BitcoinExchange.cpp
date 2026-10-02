#include "../include/BitcoinExchange.hpp"
#include <iostream>
#include <fstream>
#include <stdexcept>
#include <cstdlib>

// ─── Orthodox Canonical Form ────────────────────────────────────────────────

BitcoinExchange::BitcoinExchange() {}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
	: _database(other._database) {}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		_database = other._database;
	return *this;
}

BitcoinExchange::~BitcoinExchange() {}

// ─── Database Loader ─────────────────────────────────────────────────────────

void BitcoinExchange::loadDatabase(const std::string &dbFile)
{
	std::ifstream file(dbFile.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open database file: " + dbFile);

	std::string line;
	// Skip header line
	if (!std::getline(file, line))
		throw std::runtime_error("Error: database file is empty.");

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;

		std::size_t commaPos = line.find(',');
		if (commaPos == std::string::npos)
		{
			std::cerr << "Warning: malformed database line (no comma): " << line << std::endl;
			continue;
		}

		std::string date  = line.substr(0, commaPos);
		std::string value = line.substr(commaPos + 1);

		// Trim whitespace from date and value
		std::size_t start = date.find_first_not_of(" \t\r\n");
		std::size_t end   = date.find_last_not_of(" \t\r\n");
		if (start == std::string::npos)
			continue;
		date = date.substr(start, end - start + 1);

		start = value.find_first_not_of(" \t\r\n");
		end   = value.find_last_not_of(" \t\r\n");
		if (start == std::string::npos)
			continue;
		value = value.substr(start, end - start + 1);

		if (!isValidDate(date))
		{
			std::cerr << "Warning: invalid date in database: " << date << std::endl;
			continue;
		}

		float rate = static_cast<float>(std::atof(value.c_str()));
		_database[date] = rate;
	}

	if (_database.empty())
		throw std::runtime_error("Error: database is empty or all entries are invalid.");
}

// ─── Input Processor ─────────────────────────────────────────────────────────

void BitcoinExchange::processInput(const std::string &inputFile) const
{
	std::ifstream file(inputFile.c_str());
	if (!file.is_open())
		throw std::runtime_error("Error: could not open input file: " + inputFile);

	std::string line;
	// Skip header line
	if (!std::getline(file, line))
	{
		std::cerr << "Error: input file is empty." << std::endl;
		return;
	}

	while (std::getline(file, line))
	{
		if (line.empty())
			continue;

		// Expect format: "YYYY-MM-DD | value"
		std::size_t pipePos = line.find(" | ");
		if (pipePos == std::string::npos)
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}

		std::string date      = line.substr(0, pipePos);
		std::string valueStr  = line.substr(pipePos + 3);

		// Trim whitespace
		std::size_t start = date.find_first_not_of(" \t\r\n");
		std::size_t end   = date.find_last_not_of(" \t\r\n");
		if (start == std::string::npos)
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}
		date = date.substr(start, end - start + 1);

		start = valueStr.find_first_not_of(" \t\r\n");
		end   = valueStr.find_last_not_of(" \t\r\n");
		if (start == std::string::npos)
		{
			std::cerr << "Error: bad input => " << line << std::endl;
			continue;
		}
		valueStr = valueStr.substr(start, end - start + 1);

		// Validate date
		if (!isValidDate(date))
		{
			std::cerr << "Error: bad input => " << date << std::endl;
			continue;
		}

		// Validate value
		float value = 0.0f;
		if (!isValidValue(valueStr, value))
			continue; // error message already printed inside isValidValue

		// Find matching rate
		std::string rateKey = findRate(date);
		if (rateKey.empty())
		{
			std::cerr << "Error: no exchange rate found for date " << date << std::endl;
			continue;
		}

		float rate   = _database.at(rateKey);
		float result = value * rate;

		std::cout << date << " => " << value << " = " << result << std::endl;
	}
}

// ─── Date Validator ──────────────────────────────────────────────────────────

bool BitcoinExchange::isValidDate(const std::string &date) const
{
	// Format must be YYYY-MM-DD (exactly 10 chars)
	if (date.size() != 10)
		return false;
	if (date[4] != '-' || date[7] != '-')
		return false;

	for (int i = 0; i < 10; ++i)
	{
		if (i == 4 || i == 7)
			continue;
		if (date[i] < '0' || date[i] > '9')
			return false;
	}

	int year  = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day   = std::atoi(date.substr(8, 2).c_str());

	if (month < 1 || month > 12)
		return false;
	if (day < 1 || day > 31)
		return false;

	// Days per month validation
	int daysInMonth[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	// Leap year check
	bool leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
	if (leap)
		daysInMonth[1] = 29;

	if (day > daysInMonth[month - 1])
		return false;

	return true;
}

// ─── Value Validator ─────────────────────────────────────────────────────────

bool BitcoinExchange::isValidValue(const std::string &valueStr, float &out) const
{
	// Check for valid numeric characters (digits, dot, optional leading minus)
	if (valueStr.empty())
	{
		std::cerr << "Error: bad input => (empty value)" << std::endl;
		return false;
	}

	// Must start with digit or '-'
	std::size_t start = 0;
	if (valueStr[0] == '-')
		start = 1;

	// Rest must be digits or one dot
	int dotCount = 0;
	for (std::size_t i = start; i < valueStr.size(); ++i)
	{
		if (valueStr[i] == '.')
		{
			++dotCount;
			if (dotCount > 1)
			{
				std::cerr << "Error: bad input => " << valueStr << std::endl;
				return false;
			}
		}
		else if (valueStr[i] < '0' || valueStr[i] > '9')
		{
			std::cerr << "Error: bad input => " << valueStr << std::endl;
			return false;
		}
	}

	out = static_cast<float>(std::atof(valueStr.c_str()));

	if (out < 0.0f)
	{
		std::cerr << "Error: not a positive number." << std::endl;
		return false;
	}
	if (out > 1000.0f)
	{
		std::cerr << "Error: too large a number." << std::endl;
		return false;
	}

	return true;
}

// ─── Rate Finder (lower_bound trick) ─────────────────────────────────────────

std::string BitcoinExchange::findRate(const std::string &date) const
{
	if (_database.empty())
		return "";

	// upper_bound gives iterator to first element STRICTLY GREATER than date
	std::map<std::string, float>::const_iterator it = _database.upper_bound(date);

	// If it points to the beginning, no date <= given date exists
	if (it == _database.begin())
		return "";

	// Step back one: that's the largest key <= date
	--it;
	return it->first;
}
