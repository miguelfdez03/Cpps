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

#include "../include/easyfind.hpp"
#include <iostream>
#include <vector>
#include <list>
#include <deque>

int main()
{
	std::cout << "===== TEST 1: std::vector =====" << std::endl;
	{
		std::vector<int> vec;
		vec.push_back(1);
		vec.push_back(42);
		vec.push_back(3);
		vec.push_back(7);
		vec.push_back(99);

		try
		{
			std::vector<int>::iterator it = easyfind(vec, 42);
			std::cout << "Found value: " << *it << std::endl;
		}
		catch (const std::exception &e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		try
		{
			std::vector<int>::iterator it = easyfind(vec, 100);
			std::cout << "Found value: " << *it << std::endl;
		}
		catch (const std::exception &e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}
	}

	std::cout << std::endl << "===== TEST 2: std::list =====" << std::endl;
	{
		std::list<int> lst;
		lst.push_back(10);
		lst.push_back(20);
		lst.push_back(30);
		lst.push_back(40);
		lst.push_back(50);

		try
		{
			std::list<int>::iterator it = easyfind(lst, 30);
			std::cout << "Found value: " << *it << std::endl;
		}
		catch (const std::exception &e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		try
		{
			std::list<int>::iterator it = easyfind(lst, 0);
			std::cout << "Found value: " << *it << std::endl;
		}
		catch (const std::exception &e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}
	}

	std::cout << std::endl << "===== TEST 3: std::deque =====" << std::endl;
	{
		std::deque<int> deq;
		deq.push_back(100);
		deq.push_back(200);
		deq.push_back(300);

		try
		{
			std::deque<int>::iterator it = easyfind(deq, 200);
			std::cout << "Found value: " << *it << std::endl;
		}
		catch (const std::exception &e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}

		try
		{
			std::deque<int>::iterator it = easyfind(deq, 999);
			std::cout << "Found value: " << *it << std::endl;
		}
		catch (const std::exception &e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}
	}

	std::cout << std::endl << "===== TEST 4: Empty container =====" << std::endl;
	{
		std::vector<int> empty;

		try
		{
			std::vector<int>::iterator it = easyfind(empty, 1);
			std::cout << "Found value: " << *it << std::endl;
		}
		catch (const std::exception &e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}
	}

	std::cout << std::endl << "===== TEST 5: First occurrence =====" << std::endl;
	{
		std::vector<int> vec;
		vec.push_back(5);
		vec.push_back(5);
		vec.push_back(5);

		try
		{
			std::vector<int>::iterator it = easyfind(vec, 5);
			std::cout << "Found value: " << *it
					  << " at position: " << (it - vec.begin()) << std::endl;
		}
		catch (const std::exception &e)
		{
			std::cout << "Exception: " << e.what() << std::endl;
		}
	}

	return 0;
}
