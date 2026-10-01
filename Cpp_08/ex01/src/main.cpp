/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.cpp                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfernan2 <mfernan2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 17:00:00 by mfernan2          #+#    #+#             */
/*   Updated: 2026/09/30 17:00:00 by mfernan2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/Span.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main()
{
	std::cout << "===== TEST 1: Subject example =====" << std::endl;
	{
		Span sp = Span(5);
		sp.addNumber(6);
		sp.addNumber(3);
		sp.addNumber(17);
		sp.addNumber(9);
		sp.addNumber(11);
		std::cout << sp.shortestSpan() << std::endl;
		std::cout << sp.longestSpan() << std::endl;
		// Expected: 2 and 14
	}

	std::cout << std::endl << "===== TEST 2: Overflow exception =====" << std::endl;
	{
		Span sp(3);
		try
		{
			sp.addNumber(1);
			sp.addNumber(2);
			sp.addNumber(3);
			sp.addNumber(4); // Should throw
		}
		catch (const std::exception &e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}
	}

	std::cout << std::endl << "===== TEST 3: Not enough elements =====" << std::endl;
	{
		Span sp(5);
		try
		{
			sp.shortestSpan(); // Should throw (empty)
		}
		catch (const std::exception &e)
		{
			std::cout << "Exception (empty): " << e.what() << std::endl;
		}

		sp.addNumber(42);
		try
		{
			sp.longestSpan(); // Should throw (only 1 element)
		}
		catch (const std::exception &e)
		{
			std::cout << "Exception (1 elem): " << e.what() << std::endl;
		}
	}

	std::cout << std::endl << "===== TEST 4: Range add with iterators =====" << std::endl;
	{
		int arr[] = {5, 3, 17, 9, 11};
		Span sp(5);
		sp.addRange(arr, arr + 5);
		std::cout << "Shortest span: " << sp.shortestSpan() << std::endl;
		std::cout << "Longest span: " << sp.longestSpan() << std::endl;
		// Same as subject: 2 and 14
	}

	std::cout << std::endl << "===== TEST 5: Range add overflow =====" << std::endl;
	{
		int arr[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
		Span sp(5);
		try
		{
			sp.addRange(arr, arr + 10); // Should throw after 5
		}
		catch (const std::exception &e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
			std::cout << "Size: " << sp.getSize() << std::endl;
		}
	}

	std::cout << std::endl << "===== TEST 6: 10000 numbers =====" << std::endl;
	{
		Span sp(10000);
		std::srand(std::time(NULL));
		for (int i = 0; i < 10000; i++)
			sp.addNumber(std::rand());
		std::cout << "Shortest span: " << sp.shortestSpan() << std::endl;
		std::cout << "Longest span: " << sp.longestSpan() << std::endl;
	}

	std::cout << std::endl << "===== TEST 7: 100000 numbers =====" << std::endl;
	{
		Span sp(100000);
		for (int i = 0; i < 100000; i++)
			sp.addNumber(i);
		std::cout << "Shortest span: " << sp.shortestSpan() << std::endl;
		std::cout << "Longest span: " << sp.longestSpan() << std::endl;
		// Expected: 1 and 99999
	}

	std::cout << std::endl << "===== TEST 8: Negative numbers =====" << std::endl;
	{
		Span sp(5);
		sp.addNumber(-10);
		sp.addNumber(-3);
		sp.addNumber(0);
		sp.addNumber(5);
		sp.addNumber(20);
		std::cout << "Shortest span: " << sp.shortestSpan() << std::endl;
		std::cout << "Longest span: " << sp.longestSpan() << std::endl;
		// Sorted: -10 -3 0 5 20 => diffs: 7 3 5 15 => shortest=3, longest=30
	}

	std::cout << std::endl << "===== TEST 9: Copy constructor =====" << std::endl;
	{
		Span sp1(5);
		sp1.addNumber(1);
		sp1.addNumber(100);
		Span sp2(sp1);
		std::cout << "sp2 shortest: " << sp2.shortestSpan() << std::endl;
		std::cout << "sp2 longest: " << sp2.longestSpan() << std::endl;
		// Expected: 99 and 99
	}

	std::cout << std::endl << "===== TEST 10: Assignment operator =====" << std::endl;
	{
		Span sp1(5);
		sp1.addNumber(10);
		sp1.addNumber(20);
		sp1.addNumber(30);

		Span sp2(10);
		sp2 = sp1;
		std::cout << "sp2 shortest: " << sp2.shortestSpan() << std::endl;
		std::cout << "sp2 longest: " << sp2.longestSpan() << std::endl;
		// Expected: 10 and 20
	}

	return 0;
}
