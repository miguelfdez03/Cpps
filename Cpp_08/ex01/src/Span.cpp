/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Span.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfernan2 <mfernan2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 17:00:00 by mfernan2          #+#    #+#             */
/*   Updated: 2026/09/30 17:00:00 by mfernan2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Span.hpp"
#include <algorithm>
#include <numeric>
#include <cstdlib>
#include <stdexcept>

Span::Span() : _maxSize(0)
{
}

Span::Span(unsigned int n) : _maxSize(n)
{
}

Span::Span(const Span &src) : _maxSize(src._maxSize), _numbers(src._numbers)
{
}

Span::~Span()
{
}

Span &Span::operator=(const Span &rhs)
{
	if (this != &rhs)
	{
		_maxSize = rhs._maxSize;
		_numbers = rhs._numbers;
	}
	return *this;
}

void Span::addNumber(int number)
{
	if (_numbers.size() >= _maxSize)
		throw std::overflow_error("Span is full: cannot add more numbers");
	_numbers.push_back(number);
}

/*
** shortestSpan:
** Sorts a copy of the stored numbers, then uses std::adjacent_difference
** to compute the differences between consecutive sorted elements.
** The minimum of those differences is the shortest span.
** Note: we cannot simply subtract the two lowest numbers — that only works
** for longestSpan. The shortest span is the minimum gap between any two
** consecutive values in the sorted sequence.
*/
unsigned int Span::shortestSpan() const
{
	if (_numbers.size() < 2)
		throw std::logic_error("Not enough numbers to find a span");

	std::vector<int> sorted(_numbers);
	std::sort(sorted.begin(), sorted.end());

	std::vector<int> diffs(sorted.size());
	std::adjacent_difference(sorted.begin(), sorted.end(), diffs.begin());

	// diffs[0] is sorted[0] itself, skip it; find min from diffs[1] onward
	return static_cast<unsigned int>(
		*std::min_element(diffs.begin() + 1, diffs.end()));
}

/*
** longestSpan:
** The longest span is simply max_element - min_element.
** Uses STL algorithms std::min_element and std::max_element.
*/
unsigned int Span::longestSpan() const
{
	if (_numbers.size() < 2)
		throw std::logic_error("Not enough numbers to find a span");

	int minVal = *std::min_element(_numbers.begin(), _numbers.end());
	int maxVal = *std::max_element(_numbers.begin(), _numbers.end());

	return static_cast<unsigned int>(maxVal - minVal);
}

unsigned int Span::getMaxSize() const
{
	return _maxSize;
}

unsigned int Span::getSize() const
{
	return _numbers.size();
}

const std::vector<int> &Span::getNumbers() const
{
	return _numbers;
}
