/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   Span.hpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfernan2 <mfernan2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 17:00:00 by mfernan2          #+#    #+#             */
/*   Updated: 2026/09/30 17:00:00 by mfernan2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SPAN_HPP
# define SPAN_HPP

# include <vector>
# include <climits>

class Span
{
	private:
		unsigned int		_maxSize;
		std::vector<int>	_numbers;

	public:
		Span();
		Span(unsigned int n);
		Span(const Span &src);
		~Span();
		Span &operator=(const Span &rhs);

		void	addNumber(int number);

		/*
		** Range-based add: adds all elements in the iterator range [first, last)
		** to the Span. More practical than calling addNumber() repeatedly.
		*/
		template <typename InputIterator>
		void	addRange(InputIterator first, InputIterator last)
		{
			while (first != last)
			{
				addNumber(*first);
				++first;
			}
		}

		unsigned int	shortestSpan() const;
		unsigned int	longestSpan() const;

		// Getters
		unsigned int			getMaxSize() const;
		unsigned int			getSize() const;
		const std::vector<int>	&getNumbers() const;
};

#endif
