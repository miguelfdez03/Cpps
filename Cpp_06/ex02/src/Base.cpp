#include "../include/Base.hpp"

#include <iostream>
#include <cstdlib>
#include <ctime>

// ── Base destructor ──────────────────────────────────────────────────────────
Base::~Base() {}

// ── generate() ──────────────────────────────────────────────────────────────
// Randomly instantiates A, B, or C and returns it as Base*
Base *generate(void)
{
	int r = std::rand() % 3;
	if (r == 0)
		return new A();
	else if (r == 1)
		return new B();
	else
		return new C();
}

// ── identify(Base* p) ───────────────────────────────────────────────────────
// Uses dynamic_cast to pointer: returns NULL on failure
void identify(Base *p)
{
	if (dynamic_cast<A *>(p))
		std::cout << "A" << std::endl;
	else if (dynamic_cast<B *>(p))
		std::cout << "B" << std::endl;
	else if (dynamic_cast<C *>(p))
		std::cout << "C" << std::endl;
	else
		std::cout << "Unknown type" << std::endl;
}

// ── identify(Base& p) ───────────────────────────────────────────────────────
// Uses dynamic_cast to reference: throws std::bad_cast on failure
// NOTE: using a pointer inside this function is forbidden
void identify(Base &p)
{
	try
	{
		(void)dynamic_cast<A &>(p);
		std::cout << "A" << std::endl;
		return;
	}
	catch (std::exception &) {}

	try
	{
		(void)dynamic_cast<B &>(p);
		std::cout << "B" << std::endl;
		return;
	}
	catch (std::exception &) {}

	try
	{
		(void)dynamic_cast<C &>(p);
		std::cout << "C" << std::endl;
		return;
	}
	catch (std::exception &) {}

	std::cout << "Unknown type" << std::endl;
}
