#include "../include/Base.hpp"
#include <iostream>
#include <cstdlib>
#include <ctime>

int main(void)
{
	std::srand(static_cast<unsigned int>(std::time(NULL)));

	std::cout << "=== Identify Real Type Test ===" << std::endl;
	std::cout << std::endl;

	for (int i = 0; i < 9; ++i)
	{
		Base *p = generate();

		std::cout << "Run " << (i + 1) << ":" << std::endl;
		std::cout << "  identify(pointer)  : ";
		identify(p);
		std::cout << "  identify(reference): ";
		identify(*p);

		delete p;
	}

	std::cout << std::endl;

	// ── Edge case: explicit types ─────────────────────────────────────────
	std::cout << "=== Explicit types ===" << std::endl;

	Base *a = new A();
	Base *b = new B();
	Base *c = new C();

	std::cout << "A pointer  : "; identify(a);
	std::cout << "A reference: "; identify(*a);

	std::cout << "B pointer  : "; identify(b);
	std::cout << "B reference: "; identify(*b);

	std::cout << "C pointer  : "; identify(c);
	std::cout << "C reference: "; identify(*c);

	delete a;
	delete b;
	delete c;

	return 0;
}
