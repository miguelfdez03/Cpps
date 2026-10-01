# 🗺️ CPP Module 08 — Contenedores, Iteradores y Algoritmos STL

> Este módulo trata sobre **contenedores templados, iteradores y algoritmos STL**.

---

## 📚 Conceptos Clave

### 1. ¿Qué es un Template?
Un template es un "molde" que permite escribir código genérico que funciona con **cualquier tipo**.

```cpp
// Sin template: una función para cada tipo
int buscar_int(std::vector<int> v, int val);
float buscar_float(std::vector<float> v, float val);

// Con template: UNA función para TODOS los tipos
template <typename T>
typename T::iterator easyfind(T &container, int value);
```

El compilador genera la versión específica cuando la usas:
- `easyfind(mi_vector, 42)` → genera versión para `std::vector<int>`
- `easyfind(mi_lista, 42)` → genera versión para `std::list<int>`

### 2. ¿Qué es la STL?
La **Standard Template Library** tiene 3 pilares:

| Pilar | Qué es | Ejemplos |
|-------|--------|----------|
| **Contenedores** | Estructuras que guardan datos | `vector`, `list`, `deque`, `stack` |
| **Iteradores** | "Punteros inteligentes" para recorrer contenedores | `begin()`, `end()`, `++it` |
| **Algoritmos** | Funciones que operan sobre contenedores via iteradores | `std::find`, `std::sort`, `std::min_element` |

### 3. ¿Qué es un Iterador?
Un iterador es como un **puntero** que apunta a un elemento dentro de un contenedor.

```
vector: [10, 20, 30, 40, 50]
         ^               ^    ^
       begin()          |   end()  ← apunta DESPUÉS del último
                        |
                   un iterador "it"
```

- `*it` → te da el valor al que apunta (como un puntero)
- `++it` → avanza al siguiente elemento
- `it == end()` → significa que ya recorriste todo

### 4. Forma Canónica Ortodoxa (OCF)
Toda clase debe tener estos 4 elementos:

```cpp
class MiClase {
    MiClase();                              // Constructor por defecto
    MiClase(const MiClase &src);            // Constructor de copia
    ~MiClase();                             // Destructor
    MiClase &operator=(const MiClase &rhs); // Operador de asignación
};
```

---

## 🟢 Ex00: Easy Find

> 📂 Archivos: [`ex00/include/easyfind.hpp`](ex00/include/easyfind.hpp) · [`ex00/src/main.cpp`](ex00/src/main.cpp)

### ¿Qué hace?
Una función template que **busca un número dentro de cualquier contenedor** de enteros.

```
                    easyfind(contenedor, 42)
                           │
                           ▼
              ┌─────────────────────────┐
              │  std::find(begin, end,  │  ← Algoritmo STL
              │           42)           │
              └──────────┬──────────────┘
                         │
                 ┌───────┴────────┐
                 │                │
            Encontrado      No encontrado
                 │                │
          devuelve iterator   throw exception
```

### Código explicado línea a línea:

```cpp
template <typename T>                        // T puede ser vector, list, deque...
typename T::iterator                         // Devuelve un iterador del contenedor
easyfind(T &container, int value)            // Recibe el contenedor y el int a buscar
{
    typename T::iterator it = std::find(     // std::find es un ALGORITMO STL
        container.begin(),                   // Desde el principio
        container.end(),                     // Hasta el final
        value);                              // Busca este valor

    if (it == container.end())               // Si llegó al final = no lo encontró
        throw std::runtime_error("...");     // Lanza excepción

    return it;                               // Si lo encontró, devuelve el iterador
}
```

### ⚠️ Punto CLAVE de la evaluación:
> **DEBE usar `std::find`** (algoritmo STL). Si haces un bucle manual con iteradores, **está MAL** según el scale.

### ¿Por qué `typename` delante de `T::iterator`?
Porque el compilador no sabe si `T::iterator` es un tipo o una variable. Con `typename` le dices: "esto es un tipo".

### ¿Por qué la implementación está en el `.hpp`?
Porque es una función template. Los templates deben estar en el header porque el compilador necesita ver la implementación completa para poder instanciarlos con cada tipo.

---

## 🟡 Ex01: Span

> 📂 Archivos: [`ex01/include/Span.hpp`](ex01/include/Span.hpp) · [`ex01/src/Span.cpp`](ex01/src/Span.cpp) · [`ex01/src/main.cpp`](ex01/src/main.cpp)

### ¿Qué hace?
Una clase que almacena N números enteros y calcula la **distancia más corta y más larga** entre todos ellos.

```
Span sp(5);              ← Puede guardar máximo 5 números
sp.addNumber(6);         ← [6]
sp.addNumber(3);         ← [6, 3]
sp.addNumber(17);        ← [6, 3, 17]
sp.addNumber(9);         ← [6, 3, 17, 9]
sp.addNumber(11);        ← [6, 3, 17, 9, 11]

shortestSpan() = 2       ← distancia entre 9 y 11
longestSpan()  = 14      ← distancia entre 3 y 17
```

### ¿Cómo funciona `shortestSpan()`?

```
Paso 1: Ordenar una copia
   [6, 3, 17, 9, 11]  →  [3, 6, 9, 11, 17]

Paso 2: Calcular diferencias entre vecinos
   6-3=3    9-6=3    11-9=2    17-11=6

Paso 3: El mínimo de esas diferencias
   min(3, 3, 2, 6) = 2  ✓
```

> ⚠️ **NO es simplemente restar los 2 números más bajos.** Eso daría `6-3=3`, que es INCORRECTO. La respuesta correcta es `2` (entre 9 y 11). El evaluador **verificará esto** específicamente.

### ¿Cómo funciona `longestSpan()`?

```
Simplemente: max_element - min_element
   max = 17, min = 3
   17 - 3 = 14  ✓
```

### Algoritmos STL usados:

| Función STL | Dónde se usa | Qué hace |
|-------------|-------------|----------|
| `std::sort()` | `shortestSpan` | Ordena el vector de menor a mayor |
| `std::min_element()` | `longestSpan` | Encuentra el valor más pequeño |
| `std::max_element()` | `longestSpan` | Encuentra el valor más grande |

### Protección contra overflow:

```cpp
// Antes de hacer maxVal - minVal, comprobamos si desbordaría int:
if (minVal < 0 && maxVal > INT_MAX + minVal)
    throw std::overflow_error("Integer overflow");
```

Ejemplo: `INT_MAX - INT_MIN` = 4,294,967,295 que NO cabe en un `int` (máx 2,147,483,647). → Lanza excepción en vez de comportamiento indefinido.

### `addRange()` — La forma mejorada de añadir números

```cpp
// En vez de esto (pesado):
sp.addNumber(1);
sp.addNumber(2);
sp.addNumber(3);

// Puedes hacer esto (práctico):
int arr[] = {1, 2, 3};
sp.addRange(arr, arr + 3);  // Añade todo el rango de golpe
```

Es una función **template** que acepta cualquier tipo de iterador:
```cpp
template <typename InputIterator>
void addRange(InputIterator first, InputIterator last)
{
    while (first != last)      // Mientras no llegue al final
    {
        addNumber(*first);     // Añade el valor al que apunta
        ++first;               // Avanza al siguiente
    }
}
```

---

## 🔴 Ex02: MutantStack

> 📂 Archivos: [`ex02/include/MutantStack.hpp`](ex02/include/MutantStack.hpp) · [`ex02/src/main.cpp`](ex02/src/main.cpp)

### ¿Qué hace?
Crea un `std::stack` que **se puede recorrer con iteradores** (cosa que el `std::stack` normal NO puede hacer).

### ¿Por qué `std::stack` no tiene iteradores?
Porque un stack es LIFO (Last In, First Out) — solo se supone que accedes al tope. Pero nosotros "rompemos" esa regla heredando de él.

### El truco: el miembro protegido `c`

```
std::stack internamente usa un contenedor (por defecto std::deque):

   std::stack<int>
   ┌──────────────────────────────┐
   │  protected:                   │
   │    deque<int> c;              │  ← ¡Este es el contenedor real!
   │                               │
   │  public:                      │
   │    push(), pop(), top(),      │  ← Solo expone estas funciones
   │    size(), empty()            │
   └──────────────────────────────┘

   MutantStack hereda de std::stack y EXPONE los iteradores de "c":

   MutantStack<int>
   ┌──────────────────────────────┐
   │  (hereda todo de std::stack)  │
   │                               │
   │  + begin()  → c.begin()       │  ← ¡NUEVO!
   │  + end()    → c.end()         │  ← ¡NUEVO!
   │  + rbegin() → c.rbegin()      │  ← ¡NUEVO!
   │  + rend()   → c.rend()        │  ← ¡NUEVO!
   └──────────────────────────────┘
```

### Código clave explicado:

```cpp
// Los tipos de iterador vienen del contenedor subyacente (deque)
typedef typename std::stack<T>::container_type::iterator  iterator;

// begin() y end() simplemente llaman a los del deque interno
iterator begin() { return this->c.begin(); }
iterator end()   { return this->c.end(); }
```

- `this->c` → accede al `deque` protegido del `stack` padre
- `container_type` → es un typedef dentro de `std::stack` que te dice qué contenedor usa (en este caso `deque<T>`)

### ¿Por qué `this->c` y no solo `c`?
Porque en clases template que heredan, el compilador necesita `this->` para acceder a miembros del padre. Sin él, no sabe que `c` viene de la clase base.

### Tipos de iteradores implementados:

| Tipo | Para qué |
|------|----------|
| `iterator` | Recorrer de principio a fin |
| `const_iterator` | Recorrer sin poder modificar |
| `reverse_iterator` | Recorrer de fin a principio |
| `const_reverse_iterator` | Recorrer al revés sin modificar |

---

## 🎯 Preguntas típicas del evaluador

### Generales

**P: ¿Qué es un template?**
> Es un mecanismo de C++ para escribir código genérico. Defines la lógica una vez y funciona con cualquier tipo. El compilador genera la versión específica cuando instancias el template.

**P: ¿Qué diferencia hay entre un contenedor, un iterador y un algoritmo?**
> Un **contenedor** guarda datos (vector, list). Un **iterador** es como un puntero que apunta a elementos dentro del contenedor. Un **algoritmo** es una función que opera sobre contenedores usando iteradores (find, sort).

### Ex00

**P: ¿Por qué usas `std::find` y no un bucle manual?**
> Porque el subject pide usar algoritmos STL. `std::find` es el algoritmo estándar para buscar un elemento en un rango.

**P: ¿Qué pasa si el contenedor está vacío?**
> `std::find` devuelve `end()` directamente, y nuestra función lanza la excepción.

### Ex01

**P: ¿Por qué shortestSpan no es simplemente restar los 2 más bajos?**
> Porque el span más corto puede estar entre cualquier par de números. Por ejemplo con `{3, 6, 9, 11, 17}`, los dos más bajos dan `6-3=3`, pero el span más corto es `11-9=2`.

**P: ¿Por qué ordenas antes de calcular?**
> Porque al ordenar, el span más corto siempre está entre dos elementos **consecutivos**. Sin ordenar habría que comparar todos los pares posibles (O(n²)), con ordenar es O(n log n).

**P: ¿Cómo funciona `addRange`?**
> Es una función template que acepta dos iteradores (inicio y fin de un rango). Recorre el rango llamando a `addNumber()` para cada elemento.

### Ex02

**P: ¿Qué es `this->c`?**
> Es el contenedor subyacente del `std::stack` (por defecto un `std::deque`). Es un miembro `protected`, así que desde la clase hija podemos acceder a él.

**P: ¿Por qué heredas con `public`?**
> Para que todas las funciones públicas de `std::stack` (`push`, `pop`, `top`, `size`, `empty`) sean accesibles desde `MutantStack`.

**P: ¿La línea `std::stack<int> s(mstack)` funciona?**
> Sí. `MutantStack` hereda de `std::stack`, así que se puede construir un `std::stack` a partir de un `MutantStack` (slicing).

---

## 🧠 Diagrama resumen

```
CPP Module 08: Contenedores, Iteradores, Algoritmos
│
├── Ex00: easyfind
│   ├── Concepto: Función TEMPLATE
│   ├── STL usado: std::find (ALGORITMO)
│   └── Contenedores probados: vector, list, deque
│
├── Ex01: Span
│   ├── Concepto: Clase con OCF + algoritmos STL
│   ├── STL usado: std::sort, std::min_element, std::max_element
│   ├── addRange: template con ITERADORES
│   └── Protección: overflow de int → exception
│
└── Ex02: MutantStack
    ├── Concepto: HERENCIA de std::stack
    ├── Truco: acceder a this->c (contenedor protegido)
    ├── Añade: iterator, const_iterator, reverse_iterator
    └── Prueba: misma salida que std::list
```
