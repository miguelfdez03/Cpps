#include <iostream>
#include <cstdlib>
#include <ctime>
#include "../include/Array.hpp"

#define MAX_VAL 750

int main(int, char**)
{
    // --- Test del subject ---
    Array<int> numbers(MAX_VAL);
    int* mirror = new int[MAX_VAL];
    srand(time(NULL));
    for (int i = 0; i < MAX_VAL; i++)
    {
        const int value = rand();
        numbers[i] = value;
        mirror[i] = value;
    }
    // SCOPE: prueba que copia y asignacion son independientes del original
    {
        Array<int> tmp = numbers;
        Array<int> test(tmp);
    }

    for (int i = 0; i < MAX_VAL; i++)
    {
        if (mirror[i] != numbers[i])
        {
            std::cerr << "didn't save the same value!!" << std::endl;
            return 1;
        }
    }
    // Índice negativo → excepción
    try
    {
        numbers[-2] = 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
    // Índice fuera de rango por arriba → excepción
    try
    {
        numbers[MAX_VAL] = 0;
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }

    for (int i = 0; i < MAX_VAL; i++)
    {
        numbers[i] = rand();
    }
    delete[] mirror;

    // --- Tests extra para el evaluador ---

    // Test constructor por defecto
    std::cout << "\n--- Default constructor ---" << std::endl;
    Array<int> empty;
    std::cout << "size = " << empty.size() << std::endl;
    try { empty[0] = 1; }
    catch (std::exception &e) { std::cerr << e.what() << '\n'; }

    // Test con strings
    std::cout << "\n--- String array ---" << std::endl;
    Array<std::string> strs(3);
    strs[0] = "hello";
    strs[1] = "42";
    strs[2] = "world";
    for (unsigned int i = 0; i < strs.size(); i++)
        std::cout << strs[i] << std::endl;

    // Test deep copy: modificar copia no afecta original
    std::cout << "\n--- Deep copy test ---" << std::endl;
    Array<int> original(3);
    original[0] = 1; original[1] = 2; original[2] = 3;
    Array<int> copy(original);
    copy[0] = 99;
    std::cout << "original[0] = " << original[0] << " (should be 1)" << std::endl;
    std::cout << "copy[0]     = " << copy[0]     << " (should be 99)" << std::endl;

    // Test asignación: modificar asignado no afecta original
    std::cout << "\n--- Assignment test ---" << std::endl;
    Array<int> assigned;
    assigned = original;
    assigned[1] = 88;
    std::cout << "original[1] = " << original[1] << " (should be 2)" << std::endl;
    std::cout << "assigned[1] = " << assigned[1] << " (should be 88)" << std::endl;

    // Test const array (solo lectura)
    std::cout << "\n--- Const array test ---" << std::endl;
    Array<int> const constArr(original);
    std::cout << "constArr[2] = " << constArr[2] << std::endl;
    try { constArr[-1]; }
    catch (std::exception &e) { std::cerr << e.what() << '\n'; }

    return 0;
}