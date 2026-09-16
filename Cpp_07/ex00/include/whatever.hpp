#ifndef WHATEVER_HPP
# define WHATEVER_HPP

// swap: swaps values of two parameters, returns nothing
template <typename T>
void swap(T &a, T &b)
{
    T tmp = a;
    a = b;
    b = tmp;
}

// min: returns the smallest. If equal, returns the SECOND parameter.
// Uses only operator< for consistency.
template <typename T>
T const &min(T const &a, T const &b)
{
    return (a < b ? a : b);
}

// max: returns the greatest. If equal, returns the SECOND parameter.
// Uses only operator< for consistency.
template <typename T>
T const &max(T const &a, T const &b)
{
    return (b < a ? a : b);
}

#endif
