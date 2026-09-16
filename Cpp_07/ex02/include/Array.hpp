#ifndef ARRAY_HPP
# define ARRAY_HPP

# include <stdexcept>

template <typename T>
class Array
{
    private:
        T            *_data;
        unsigned int  _size;

    public:
        // Constructor por defecto: array vacío
        Array() : _data(new T[0]()), _size(0) {}

        // Constructor con n elementos inicializados por defecto
        Array(unsigned int n) : _data(new T[n]()), _size(n) {}

        // Constructor de copia: deep copy (modif. de uno no afecta al otro)
        Array(const Array &other) : _data(new T[other._size]()), _size(other._size)
        {
            for (unsigned int i = 0; i < _size; i++)
                _data[i] = other._data[i];
        }

        // Operador de asignación: deep copy con self-assignment guard
        Array &operator=(const Array &other)
        {
            if (this != &other)
            {
                delete[] _data;
                _size = other._size;
                _data = new T[_size]();
                for (unsigned int i = 0; i < _size; i++)
                    _data[i] = other._data[i];
            }
            return *this;
        }

        // Destructor
        ~Array() { delete[] _data; }

        // operator[] no-const: permite lectura y escritura con bounds check
        // NOTA: índice es int para detectar valores negativos (el subject prueba [-2])
        T &operator[](int i)
        {
            if (i < 0 || static_cast<unsigned int>(i) >= _size)
                throw std::out_of_range("Array index out of bounds");
            return _data[i];
        }

        // operator[] const: solo lectura (para instancias const)
        T const &operator[](int i) const
        {
            if (i < 0 || static_cast<unsigned int>(i) >= _size)
                throw std::out_of_range("Array index out of bounds");
            return _data[i];
        }

        // size(): retorna el número de elementos, no modifica la instancia
        unsigned int size() const { return _size; }
};

#endif
