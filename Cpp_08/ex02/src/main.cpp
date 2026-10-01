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

#include "../include/MutantStack.hpp"
#include <iostream>
#include <list>

int main()
{
	std::cout << "===== TEST 1: Subject example (MutantStack) =====" << std::endl;
	{
		MutantStack<int> mstack;

		mstack.push(5);
		mstack.push(17);
		std::cout << mstack.top() << std::endl;
		mstack.pop();
		std::cout << mstack.size() << std::endl;
		mstack.push(3);
		mstack.push(5);
		mstack.push(737);
		//[...]
		mstack.push(0);
		MutantStack<int>::iterator it = mstack.begin();
		MutantStack<int>::iterator ite = mstack.end();
		++it;
		--it;
		while (it != ite)
		{
			std::cout << *it << std::endl;
			++it;
		}
		std::stack<int> s(mstack);
	}

	std::cout << std::endl << "===== TEST 2: Same test with std::list =====" << std::endl;
	{
		std::list<int> mstack;

		mstack.push_back(5);
		mstack.push_back(17);
		std::cout << mstack.back() << std::endl;
		mstack.pop_back();
		std::cout << mstack.size() << std::endl;
		mstack.push_back(3);
		mstack.push_back(5);
		mstack.push_back(737);
		//[...]
		mstack.push_back(0);
		std::list<int>::iterator it = mstack.begin();
		std::list<int>::iterator ite = mstack.end();
		++it;
		--it;
		while (it != ite)
		{
			std::cout << *it << std::endl;
			++it;
		}
	}

	std::cout << std::endl << "===== TEST 3: Reverse iterators =====" << std::endl;
	{
		MutantStack<int> mstack;
		mstack.push(1);
		mstack.push(2);
		mstack.push(3);
		mstack.push(4);
		mstack.push(5);

		std::cout << "Forward: ";
		for (MutantStack<int>::iterator it = mstack.begin(); it != mstack.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;

		std::cout << "Reverse: ";
		for (MutantStack<int>::reverse_iterator rit = mstack.rbegin(); rit != mstack.rend(); ++rit)
			std::cout << *rit << " ";
		std::cout << std::endl;
	}

	std::cout << std::endl << "===== TEST 4: Copy constructor =====" << std::endl;
	{
		MutantStack<int> original;
		original.push(10);
		original.push(20);
		original.push(30);

		MutantStack<int> copy(original);
		std::cout << "Original top: " << original.top() << std::endl;
		std::cout << "Copy top: " << copy.top() << std::endl;

		copy.push(40);
		std::cout << "After push on copy - Original size: " << original.size()
				  << ", Copy size: " << copy.size() << std::endl;
	}

	std::cout << std::endl << "===== TEST 5: Assignment operator =====" << std::endl;
	{
		MutantStack<int> s1;
		s1.push(100);
		s1.push(200);

		MutantStack<int> s2;
		s2.push(999);
		s2 = s1;
		std::cout << "s2 top: " << s2.top() << std::endl;
		std::cout << "s2 size: " << s2.size() << std::endl;

		std::cout << "s2 elements: ";
		for (MutantStack<int>::iterator it = s2.begin(); it != s2.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;
	}

	std::cout << std::endl << "===== TEST 6: Empty stack =====" << std::endl;
	{
		MutantStack<int> mstack;
		std::cout << "Empty: " << mstack.empty() << std::endl;
		std::cout << "Size: " << mstack.size() << std::endl;

		MutantStack<int>::iterator it = mstack.begin();
		MutantStack<int>::iterator ite = mstack.end();
		if (it == ite)
			std::cout << "begin == end (stack is empty)" << std::endl;
	}

	std::cout << std::endl << "===== TEST 7: String stack =====" << std::endl;
	{
		MutantStack<std::string> sstack;
		sstack.push("Hello");
		sstack.push("World");
		sstack.push("42");

		for (MutantStack<std::string>::iterator it = sstack.begin(); it != sstack.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;
	}

	std::cout << std::endl << "===== TEST 8: Const iterators =====" << std::endl;
	{
		MutantStack<int> mstack;
		mstack.push(42);
		mstack.push(21);
		mstack.push(84);

		const MutantStack<int> &constRef = mstack;
		std::cout << "Const iteration: ";
		for (MutantStack<int>::const_iterator it = constRef.begin(); it != constRef.end(); ++it)
			std::cout << *it << " ";
		std::cout << std::endl;
	}

	return 0;
}
