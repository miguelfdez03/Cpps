#ifndef ITER_HPP
# define ITER_HPP
# include <cstddef>

// iter for non-const arrays: func can modify elements
template <typename T, typename F>
void iter(T *array, size_t const length, F func)
{
    for (size_t i = 0; i < length; i++)
        func(array[i]);
}

// iter for const arrays: func receives const references
template <typename T, typename F>
void iter(T const *array, size_t const length, F func)
{
    for (size_t i = 0; i < length; i++)
        func(array[i]);
}

#endif
