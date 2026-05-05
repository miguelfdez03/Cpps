#include "../include/Animal.hpp"
#include "../include/Cat.hpp"
#include "../include/Dog.hpp"
#include "../include/WrongAnimal.hpp"
#include "../include/WrongCat.hpp"

int	main(void)
{
	std::cout << "=== Testing correct polymorphism ===" << std::endl;
	const Animal *meta = new Animal();
	const Animal *j = new Dog();
	const Animal *i = new Cat();

	std::cout << "\n=== Getting types ===" << std::endl;
	std::cout << j->getType() << " " << std::endl;
	std::cout << i->getType() << " " << std::endl;

	std::cout << "\n=== Making sounds ===" << std::endl;
	i->makeSound(); // Will output the cat sound!
	j->makeSound(); // Will output the dog sound!
	meta->makeSound();

	std::cout << "\n=== Deleting animals ===" << std::endl;
	delete meta;
	delete j;
	delete i;

	std::cout << "\n\n=== Testing WRONG polymorphism ===" << std::endl;
	const WrongAnimal *wrongMeta = new WrongAnimal();
	const WrongAnimal *wrongCat = new WrongCat();

	std::cout << "\n=== Getting types ===" << std::endl;
	std::cout << wrongCat->getType() << " " << std::endl;

	std::cout << "\n=== Making sounds (WRONG behavior) ===" << std::endl;
	wrongCat->makeSound(); // Will output WrongAnimal sound (not polymorphic!)
	wrongMeta->makeSound();

	std::cout << "\n=== Deleting wrong animals ===" << std::endl;
	delete wrongMeta;
	delete wrongCat;

	std::cout << "\n\n=== Additional tests ===" << std::endl;

	// Test stack allocation
	std::cout << "\n--- Stack allocation test ---" << std::endl;
	Dog dog;
	Cat cat;
	dog.makeSound();
	cat.makeSound();

	// Test copy constructor
	std::cout << "\n--- Copy constructor test ---" << std::endl;
	Dog dog2(dog);
	dog2.makeSound();

	// Test assignment operator
	std::cout << "\n--- Assignment operator test ---" << std::endl;
	Cat cat2;
	cat2 = cat;
	cat2.makeSound();

	std::cout << "\n--- End of tests ---" << std::endl;
	return (0);
}