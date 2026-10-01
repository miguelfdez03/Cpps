/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   MutantStack.hpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mfernan2 <mfernan2@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2026/09/30 17:00:00 by mfernan2          #+#    #+#             */
/*   Updated: 2026/09/30 17:00:00 by mfernan2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MUTANTSTACK_HPP
# define MUTANTSTACK_HPP

# include <stack>

/*
** MutantStack: inherits from std::stack and exposes iterators.
**
** std::stack uses a protected member 'c' which is the underlying container
** (by default std::deque). We expose its iterators to make std::stack iterable.
**
** All std::stack member functions (push, pop, top, size, empty) are
** inherited automatically.
*/

template <typename T>
class MutantStack : public std::stack<T>
{
	public:
		MutantStack() : std::stack<T>() {}
		MutantStack(const MutantStack &src) : std::stack<T>(src) {}
		~MutantStack() {}

		MutantStack &operator=(const MutantStack &rhs)
		{
			if (this != &rhs)
				std::stack<T>::operator=(rhs);
			return *this;
		}

		// Iterator typedefs from the underlying container
		typedef typename std::stack<T>::container_type::iterator				iterator;
		typedef typename std::stack<T>::container_type::const_iterator			const_iterator;
		typedef typename std::stack<T>::container_type::reverse_iterator			reverse_iterator;
		typedef typename std::stack<T>::container_type::const_reverse_iterator	const_reverse_iterator;

		iterator				begin()			{ return this->c.begin(); }
		iterator				end()			{ return this->c.end(); }
		const_iterator			begin() const	{ return this->c.begin(); }
		const_iterator			end() const		{ return this->c.end(); }

		reverse_iterator		rbegin()		{ return this->c.rbegin(); }
		reverse_iterator		rend()			{ return this->c.rend(); }
		const_reverse_iterator	rbegin() const	{ return this->c.rbegin(); }
		const_reverse_iterator	rend() const	{ return this->c.rend(); }
};

#endif
