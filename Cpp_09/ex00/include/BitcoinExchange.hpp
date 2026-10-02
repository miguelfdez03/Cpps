#ifndef BITCOINEXCHANGE_HPP
# define BITCOINEXCHANGE_HPP

# include <map>
# include <string>
# include <cstdlib>

class BitcoinExchange
{
public:
	// Orthodox Canonical Form
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	~BitcoinExchange();

	// Core methods
	void loadDatabase(const std::string &dbFile);
	void processInput(const std::string &inputFile) const;

private:
	// std::map chosen: sorted by key (date string), O(log n) lookup,
	// upper_bound gives us the first key AFTER a date, perfect for
	// "find closest earlier date".
	std::map<std::string, float> _database;

	// Helper validators
	bool        isValidDate(const std::string &date) const;
	bool        isValidValue(const std::string &valueStr, float &out) const;
	std::string findRate(const std::string &date) const;
};

#endif // BITCOINEXCHANGE_HPP
