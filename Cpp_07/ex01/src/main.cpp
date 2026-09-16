#include <iostream>
#include <string>
#include "../include/iter.hpp"

// Función template que imprime cualquier tipo
template <typename T>
void print(T const &x)
{
    std::cout << x << std::endl;
}

// Función que incrementa un entero
void increment(int &x)
{
    x += 1;
}

int main(void)
{
    // --- Test 1: array de ints {0,1,2,3,4} → imprime cada elemento ---
    int arr1[] = {0, 1, 2, 3, 4};
    ::iter(arr1, 5, print<int>);

    // --- Test 2: array relleno de 42 × 5 ---
    int arr2[] = {42, 42, 42, 42, 42};
    ::iter(arr2, 5, print<int>);

    // --- Test 3: array const (debe funcionar con la sobrecarga const) ---
    std::cout << "\n--- Const array test ---" << std::endl;
    int const arr3[] = {10, 20, 30};
    ::iter(arr3, 3, print<int>);

    // --- Test 4: función que modifica (incrementa) los elementos ---
    std::cout << "\n--- Increment test ---" << std::endl;
    int arr4[] = {1, 2, 3};
    ::iter(arr4, 3, increment);
    ::iter(arr4, 3, print<int>);

    // --- Test 5: strings ---
    std::cout << "\n--- String test ---" << std::endl;
    std::string sarr[] = {"hello", "42", "world"};
    ::iter(sarr, 3, print<std::string>);

    return 0;
}
