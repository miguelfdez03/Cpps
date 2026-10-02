# CPP09 — Roadmap de Evaluación y Defensa

> Guía exhaustiva para la peer-evaluation del módulo CPP09 de la escuela 42.

---

## Tabla de contenidos

1. [Resumen de contenedores y justificación](#1-resumen-de-contenedores-y-justificación)
2. [Ex00 — Bitcoin Exchange](#2-ex00--bitcoin-exchange)
3. [Ex01 — Reverse Polish Notation](#3-ex01--reverse-polish-notation)
4. [Ex02 — PmergeMe (Ford-Johnson)](#4-ex02--pmergeme-ford-johnson)
5. [Algoritmo Ford-Johnson paso a paso con ejemplo](#5-algoritmo-ford-johnson-paso-a-paso-con-ejemplo)
6. [Preguntas frecuentes de evaluación](#6-preguntas-frecuentes-de-evaluación)

---

## 1. Resumen de contenedores y justificación

| Ejercicio | Contenedor(es) | Razón principal |
|-----------|---------------|-----------------|
| Ex00 | `std::map<std::string, float>` | Ordenado por clave, búsqueda O(log n), `upper_bound` nativo |
| Ex01 | `std::stack<int>` | LIFO nativo, semántica exacta para RPN |
| Ex02 | `std::vector<int>` + `std::deque<int>` | Array contiguo vs. array segmentado: comparación de rendimiento |

**Regla de oro cumplida:** Ningún contenedor se repite entre ejercicios.

---

## 2. Ex00 — Bitcoin Exchange

### ¿Qué contenedor y por qué?

**`std::map<std::string, float>`**

#### Justificación técnica

| Operación | `std::map` | `std::unordered_map` |
|-----------|-----------|---------------------|
| Inserción | O(log n) | O(1) amortizado |
| Búsqueda exacta | O(log n) | O(1) amortizado |
| **`upper_bound` (clave menor-o-igual)** | **O(log n)** | **No disponible** |
| Iteración ordenada | O(n) | No garantizado |

La operación clave del enunciado es: *"si la fecha no existe, usa la más reciente hacia atrás"*. Esto requiere encontrar el **mayor elemento <= fecha buscada**. `std::map` implementa esto nativamente con `upper_bound`:

```cpp
auto it = _database.upper_bound(date); // primer elemento > date
--it; // el mayor elemento <= date
```

`std::unordered_map` no tiene este método. El `std::map` es la única estructura STL que combina **orden automático** + **búsqueda O(log n)** + **`upper_bound`**.

#### Flujo del programa

```
argv[1] (input file)
    |
    v
main() --> BitcoinExchange::loadDatabase("data.csv")
                |  Lee CSV, valida fechas y valores
                |  Llena _database: map<string, float>
                v
           BitcoinExchange::processInput(file)
                |  Por cada línea "YYYY-MM-DD | valor":
                |  1. Valida formato de fecha
                |  2. Valida valor (0 <= v <= 1000)
                |  3. findRate(date) -> upper_bound trick
                |  4. Imprime date => value = result
                v
           stdout
```

#### Validaciones implementadas

- Fecha formato exacto `YYYY-MM-DD`: longitud 10, guiones en posición 4 y 7.
- Mes válido (1–12), día válido según mes y año bisiesto.
- Valor < 0 → `"Error: not a positive number."`
- Valor > 1000 → `"Error: too large a number."`
- Fecha no encontrada en DB (antes del primer registro) → error.
- Separador `" | "` ausente → `"Error: bad input => ..."`

---

## 3. Ex01 — Reverse Polish Notation

### ¿Qué contenedor y por qué?

**`std::stack<int>`**

#### Justificación técnica

| Operación | `std::stack` | `std::vector` manual |
|-----------|-------------|----------------------|
| `push` / `pop` | O(1) | O(1) amortizado |
| Semántica LIFO | Nativa | Emulada |
| Claridad de código | Alta | Media |

`std::stack` es un *container adaptor* que por defecto usa `std::deque` internamente. Expone exactamente las operaciones necesarias: `push`, `pop`, `top`, `size`, `empty`.

#### Traza del ejemplo del subject

```
Tokens: "8 9 * 9 - 9 - 9 - 4 - 1 +"

8   -> push(8)      stack: [8]
9   -> push(9)      stack: [8, 9]
*   -> b=9, a=8     stack: [72]       (8*9=72)
9   -> push(9)      stack: [72, 9]
-   -> b=9, a=72    stack: [63]       (72-9=63)
9   -> push(9)      stack: [63, 9]
-   -> b=9, a=63    stack: [54]       (63-9=54)
9   -> push(9)      stack: [54, 9]
-   -> b=9, a=54    stack: [45]       (54-9=45)
4   -> push(4)      stack: [45, 4]
-   -> b=4, a=45    stack: [41]       (45-4=41)
1   -> push(1)      stack: [41, 1]
+   -> b=1, a=41    stack: [42]       (41+1=42)

Resultado: 42 ✓
```

#### Errores manejados

- Token no es dígito ni operador → `"Error: invalid token '...'"`
- Menos de 2 operandos al aplicar operador → `"Error: insufficient operands..."`
- División por cero → `"Error: division by zero."`
- Al final, stack.size() > 1 → `"Error: too many operands remaining."`
- Expresión vacía → `"Error: empty expression."`

---

## 4. Ex02 — PmergeMe (Ford-Johnson)

### ¿Qué contenedores y por qué?

**`std::vector<int>`** + **`std::deque<int>`**

#### Comparación de contenedores

| Característica | `std::vector` | `std::deque` |
|---------------|--------------|-------------|
| Memoria | Contigua | Bloques segmentados |
| Acceso aleatorio | O(1) | O(1) |
| Inserción al frente | O(n) | O(1) |
| Inserción al centro | O(n) | O(n) |
| Cache locality | Excelente | Buena |
| Aritmética de iteradores | Si | Si |

Ambos son contenedores de *acceso aleatorio*, necesario para la búsqueda binaria O(log n).

**¿Por qué NO `std::list`?** La búsqueda binaria requiere acceso aleatorio O(1). `std::list` tiene iteradores bidireccionales, no aleatorios → incompatible.

---

## 5. Algoritmo Ford-Johnson paso a paso con ejemplo

### Concepto general

El algoritmo Ford-Johnson (*merge-insert sort*) minimiza el número de comparaciones necesarias para ordenar n elementos. Es el algoritmo óptimo para n <= ~47.

### Ejemplo: ordenar `[3, 5, 9, 7, 4]`

**Entrada:** `3 5 9 7 4`  (n=5, impar → hay un "straggler")

---

#### PASO 1: Separar straggler y formar pares

Como n=5 es impar, el último elemento se aparta:
- **Straggler:** `4`
- **Pares a procesar:** `(3,5)` y `(9,7)`

---

#### PASO 2: Ordenar cada par (mayor primero)

| Par | Comparación | winner | loser |
|-----|------------|--------|-------|
| (3, 5) | 3 < 5 | 5 | 3 |
| (9, 7) | 9 > 7 | 9 | 7 |

- **`larger[]`** (winners): `[5, 9]`
- **`smaller[]`** (pend, losers): `[3, 7]`

---

#### PASO 3: Ordenar recursivamente `larger[] = [5, 9]`

Recursión con n=2:
- Comparación única: 5 < 9 → ya ordenado
- **`larger[]` ordenado:** `[5, 9]`
- Asociación preservada: `pend[0]=3 ↔ larger[0]=5`, `pend[1]=7 ↔ larger[1]=9`

---

#### PASO 4: Construir la cadena principal (main chain)

**Main chain inicial** = `[5, 9]`

```
posición:    0    1
main:       [5,   9]
pend:       [3,   7]
partnerPos: [0,   1]
```

`partnerPos[i]` = posición actual en main del partner de `pend[i]`.

---

#### PASO 5: Secuencia de Jacobsthal para 2 elementos pend

```
J(0)=0, J(1)=1, J(2)=1, J(3)=3...

Grupos formados:
  Grupo 1: [J(0)+1 .. J(1)]  = indices {0}   -> insertar pend[0]
  Grupo 2: [J(1)+1 .. J(3)]  = indices {2,1} -> insertar pend[1] (cap en n=2)

Orden final: [0, 1]
```

---

#### PASO 6: Insertar pend en orden Jacobsthal

**Insertar `pend[0] = 3`:**
- Partner en `partnerPos[0] = 0` → buscar en `[main.begin(), main.begin()+1)` = `[5]`
- 3 < 5 → insertar antes de 5
- main: `[3, 5, 9]`
- partnerPos actualizado (todos los que estaban en posición >= 0 se incrementan):
  `partnerPos = [1, 2]`

**Insertar `pend[1] = 7`:**
- Partner en `partnerPos[1] = 2` → buscar en `[main.begin(), main.begin()+3)` = `[3, 5, 9]`
- Binary search: 5 < 7 < 9 → insertar entre 5 y 9 (en posición 2)
- main: `[3, 5, 7, 9]`
- partnerPos actualizado: `partnerPos = [1, 3]`

---

#### PASO 7: Insertar straggler `4`

Búsqueda binaria en `[3, 5, 7, 9]` para 4:
- 3 < 4 < 5 → insertar en posición 1

**Resultado final:** `[3, 4, 5, 7, 9]` ✓

---

### ¿Por qué Jacobsthal minimiza comparaciones?

Cada vez que insertamos `pend[k]`, la búsqueda binaria se acota a `[begin, partnerPos[k]+1)`.
El número de comparaciones para buscar en un rango de tamaño `m` es `ceil(log2(m+1))`.

El orden de Jacobsthal agrupa los pend elements de forma que el tamaño del rango de búsqueda más grande dentro de cada grupo es siempre una potencia de 2. Esto garantiza el número mínimo de comparaciones en el peor caso:

| n | Ford-Johnson | Merge sort | Insertion sort |
|---|-------------|-----------|---------------|
| 5 | 7 compar. | 8 | 10 |
| 11 | 19 compar. | 22 | 55 |
| 21 | 38 compar. | 42 | 210 |

---

## 6. Preguntas frecuentes de evaluación

### Generales

**P: ¿Cumples la Forma Canónica Ortodoxa?**
R: Sí. Todas las clases (BitcoinExchange, RPN, PmergeMe) tienen:
- Constructor por defecto
- Constructor de copia
- Operador de asignación
- Destructor

**P: ¿Usas funciones de C prohibidas?**
R: No. Uso únicamente la STL: `std::ifstream`, `std::istringstream`, `std::map`, `std::stack`, `std::vector`, `std::deque`. Para conversión de strings uso `std::atof`/`std::atol` que son del header `<cstdlib>`, permitidos.

**P: ¿Hay fugas de memoria?**
R: No. No uso `new`/`delete`. Todos los contenedores STL gestionan su propia memoria automáticamente.

---

### Ex00

**P: ¿Por qué `upper_bound` y no `lower_bound`?**
R: `lower_bound(date)` devuelve el primer elemento >= date. Si la fecha exacta no existe, apunta a la siguiente fecha (posterior). `upper_bound(date)` devuelve el primer elemento ESTRICTAMENTE mayor que date, y decrementando obtenemos el mayor <= date. Eso es exactamente lo que necesitamos.

**P: ¿Validas años bisiestos?**
R: Sí. En `isValidDate()`: `leap = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0)`. Si bisiesto, febrero tiene 29 días.

**P: ¿Qué pasa si `data.csv` no existe?**
R: `std::runtime_error("Error: could not open database file: ...")` → el programa termina con exit code 1.

---

### Ex01

**P: ¿Qué pasa si doy `./RPN "3 +"`?**
R: Error: insufficient operands for operator '+'. (solo hay 1 operando en la pila cuando se intenta hacer la operación).

**P: ¿Por qué no procesáis números de múltiples dígitos?**
R: El subject lo prohíbe explícitamente: "you must handle tokens which are less than 10".

**P: ¿Qué token es inválido?**
R: Cualquier string que no sea un único dígito (0-9) o un único operador (+, -, *, /).

---

### Ex02

**P: ¿Por qué no haces simplemente un std::sort?**
R: El subject exige el algoritmo Ford-Johnson específicamente. `std::sort` usa introsort (quicksort + heapsort + insertion sort), no Ford-Johnson.

**P: ¿Por qué la complejidad de Ford-Johnson es mejor?**
R: Ford-Johnson tiene O(n log n) comparaciones como merge sort, pero la constante es mejor para n pequeños porque aprovecha la información de los pares para acotar el rango de búsqueda binaria.

**P: ¿Cómo mantienes la asociación larger[i] ↔ smaller[i] tras la recursión?**
R: Los pares se almacenan como `std::pair<int,int>`. Extraigo los `.first` (larger) en un array, los ordeno recursivamente, y luego re-asocio cada valor ordenado con su pareja original usando un array de flags `matched[]` para evitar asignar el mismo par dos veces (importante con duplicados).

**P: ¿Cómo mides el tiempo con precisión?**
R: `std::clock()` de `<ctime>`, convertido a microsegundos: `(double)(end - start) / CLOCKS_PER_SEC * 1e6`.

**P: ¿Qué pasa si doy un número negativo?**
R: El carácter '-' es detectado como inválido en el parser → `"Error: invalid character in '-5'."`.

---

## Árbol de archivos del proyecto

```
Cpp_09/
├── roadmap.md            <- este archivo
│
├── ex00/
│   ├── Makefile
│   ├── data.csv
│   ├── input.txt
│   ├── include/
│   │   └── BitcoinExchange.hpp
│   └── src/
│       ├── main.cpp
│       └── BitcoinExchange.cpp
│
├── ex01/
│   ├── Makefile
│   ├── include/
│   │   └── RPN.hpp
│   └── src/
│       ├── main.cpp
│       └── RPN.cpp
│
└── ex02/
    ├── Makefile
    ├── include/
    │   └── PmergeMe.hpp
    └── src/
        ├── main.cpp
        └── PmergeMe.cpp
```

---

*CPP09 — Escuela 42 — Generado con Antigravity*
