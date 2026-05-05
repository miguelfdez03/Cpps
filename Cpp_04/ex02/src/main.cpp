#include "../include/Animal.hpp"
#include "../include/Cat.hpp"
#include "../include/Dog.hpp"
#include <iostream>

int main()
{
	std::cout << "=== TEST 1: BASIC CONSTRUCTION ===" << std::endl;
	const Animal* j = new Dog();
	const Animal* i = new Cat();

	delete j;
	delete i;

	std::cout << "\n=== TEST 2: ARRAY OF ANIMALS ===" << std::endl;
	const int arraySize = 4;
	Animal* animals[arraySize];

	std::cout << "Creating array..." << std::endl;
	for (int idx = 0; idx < arraySize; idx++)
	{
		if (idx < arraySize / 2)
			animals[idx] = new Dog();
		else
			animals[idx] = new Cat();
	}

	std::cout << "\nDeleting array..." << std::endl;
	for (int idx = 0; idx < arraySize; idx++)
	{
		delete animals[idx];
	}

	std::cout << "\n=== TEST 3: DEEP COPY ===" << std::endl;
	Dog basic;
	{
		Dog tmp = basic;
	} // tmp is destroyed here, if shallow copy, basic's brain will be deleted too!

	std::cout << "\n=== TEST 4: ASSIGNMENT OPERATOR ===" << std::endl;
	Dog dog1;
	Dog dog2;
	dog2 = dog1;

	std::cout << "End of tests" << std::endl;
	return 0;
}