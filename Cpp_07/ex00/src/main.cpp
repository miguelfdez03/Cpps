#include <iostream>
#include <string>
#include "../include/whatever.hpp"

int main(void)
{
    // --- Test del subject (salida exacta requerida) ---
    int a = 2;
    int b = 3;
    ::swap(a, b);
    std::cout << "a = " << a << ", b = " << b << std::endl;
    std::cout << "min(a, b) = " << ::min(a, b) << std::endl;
    std::cout << "max(a, b) = " << ::max(a, b) << std::endl;

    std::string c = "chaine1";
    std::string d = "chaine2";
    ::swap(c, d);
    std::cout << "c = " << c << ", d = " << d << std::endl;
    std::cout << "min(c, d) = " << ::min(c, d) << std::endl;
    std::cout << "max(c, d) = " << ::max(c, d) << std::endl;

    // --- Tests extra: igualdad (debe retornar el SEGUNDO parámetro) ---
    int x = 5;
    int y = 5;
    std::cout << "\n--- Equal values test ---" << std::endl;
    std::cout << "min(5, 5) = " << ::min(x, y) << std::endl; // retorna y (segundo)
    std::cout << "max(5, 5) = " << ::max(x, y) << std::endl; // retorna y (segundo)

    // --- Test con floats ---
    float f1 = 1.5f;
    float f2 = 2.5f;
    ::swap(f1, f2);
    std::cout << "\n--- Float test ---" << std::endl;
    std::cout << "f1 = " << f1 << ", f2 = " << f2 << std::endl;
    std::cout << "min(f1, f2) = " << ::min(f1, f2) << std::endl;
    std::cout << "max(f1, f2) = " << ::max(f1, f2) << std::endl;

    return 0;
}
