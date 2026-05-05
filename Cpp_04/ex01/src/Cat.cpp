#include "../include/Cat.hpp"

Cat::Cat() : brain(new Brain())
{
	this->type = "Cat";
	std::cout << "Cat default constructor called" << std::endl;
}

Cat::Cat(const Cat &other) : Animal(other), brain(new Brain(*other.brain))
{
	std::cout << "Cat copy constructor called" << std::endl;
}

Cat &Cat::operator=(const Cat &other)
{
	std::cout << "Cat copy assignment operator called" << std::endl;
	if (this != &other)
	{
		Animal::operator=(other);
		delete this->brain;
		this->brain = new Brain(*other.brain);
	}
	return (*this);
}

Cat::~Cat()
{
	delete this->brain;
	std::cout << "Cat destructor called" << std::endl;
}

void Cat::makeSound() const
{
	std::cout << "Meow! Meow!" << std::endl;
}