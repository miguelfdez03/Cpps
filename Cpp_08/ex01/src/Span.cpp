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
#include <stdexcept>
#include <climits>

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
** Sorts a copy of the stored numbers, then computes the differences
** between consecutive sorted elements to find the minimum span.
** Throws std::overflow_error if any adjacent difference overflows int.
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

	int minSpan = INT_MAX;
	for (std::vector<int>::size_type i = 1; i < sorted.size(); ++i)
	{
		if (sorted[i - 1] < 0 && sorted[i] > INT_MAX + sorted[i - 1])
			throw std::overflow_error("Integer overflow in span computation");
		int diff = sorted[i] - sorted[i - 1];
		if (diff < minSpan)
			minSpan = diff;
	}
	return static_cast<unsigned int>(minSpan);
}

/*
** longestSpan:
** The longest span is simply max_element - min_element.
** Uses STL algorithms std::min_element and std::max_element.
** Throws std::overflow_error if the difference overflows int.
*/
unsigned int Span::longestSpan() const
{
	if (_numbers.size() < 2)
		throw std::logic_error("Not enough numbers to find a span");

	int minVal = *std::min_element(_numbers.begin(), _numbers.end());
	int maxVal = *std::max_element(_numbers.begin(), _numbers.end());

	if (minVal < 0 && maxVal > INT_MAX + minVal)
		throw std::overflow_error("Integer overflow in span computation");
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
