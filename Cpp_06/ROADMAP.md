# 🗺️ Roadmap y Arquitectura Conceptual
## C++ Module 06: C++ casts

> **Regla de oro de esta guía:** Todo lo aquí escrito se sustenta únicamente en el subject PDF y la hoja de evaluación oficial de 42evalhub (https://www.42evalhub.com/common/cpp06).

---

## 🧩 Visión General: ¿Qué construyes y por qué?

El módulo 06 te obliga a dominar los **4 tipos de cast de C++**. No es un tema aleatorio: C++ heredó el cast genérico de C (`(tipo)valor`) que es peligroso porque no distingue si lo que estás haciendo tiene sentido. C++ introdujo 4 casts con semánticas precisas y verificables en compilación o en tiempo de ejecución.

La progresión de los 3 ejercicios es deliberada:

```
ex00              ex01                  ex02
  │                 │                    │
static_cast    reinterpret_cast      dynamic_cast
(conversión    (serialización        (identificación
 de tipos)      de punteros)          de tipo real)
```

Cada ejercicio enseña **uno** de estos casts en profundidad, con un contexto práctico concreto.

---

## 📦 Ex00 — ScalarConverter: static_cast

### ¿Qué problema de C++ resuelve?
`static_cast` es el cast de propósito general para conversiones numéricas conocidas en tiempo de compilación. Es seguro (verificado por el compilador), explícito (comunica intención) y eficiente (sin coste en runtime para primitivos).

### Analogía
Tienes una calculadora que recibe un número escrito en papel (string) y te muestra ese mismo valor en 4 pantallas distintas: la pantalla `char`, la `int`, la `float` y la `double`. La calculadora necesita primero **identificar qué tipo de número es** el que has escrito, y luego convertirlo.

### Arquitectura interna de ScalarConverter

```
ScalarConverter                          (no instanciable — constructor privado)
└── static convert(std::string literal)
     ├── detectType()    → char | int | float | double | pseudo-literal
     │
     ├── toChar()        → static_cast<char>
     ├── toInt()         → static_cast<int>
     ├── toFloat()       → static_cast<float>
     └── toDouble()      → static_cast<double>
```

### Los tipos de input posibles

| Tipo detectado | Ejemplos | Regla de detección |
|---|---|---|
| `char` | `'a'`, `'Z'` | Empieza y termina con `'` |
| `int` | `0`, `-42`, `42` | Solo dígitos (y `-` inicial) |
| `float` | `0.0f`, `-4.2f` | Tiene `.` y termina en `f` |
| `double` | `0.0`, `-4.2` | Tiene `.` pero NO termina en `f` |
| Pseudo-literal | `-inff`, `+inff`, `nanf`, `-inf`, `+inf`, `nan` | Literales exactos |

### Formato de salida exacto (del subject)

```
./convert 0
char: Non displayable
int: 0
float: 0.0f
double: 0.0

./convert nan
char: impossible
int: impossible
float: nanf
double: nan

./convert 42.0f
char: '*'
int: 42
float: 42.0f
double: 42.0
```

### Trampas del evaluador

- `float` y `double` con número entero → mostrar `.0f` / `.0`, NO `42f` ni `42`
- `char` no displayable (0–31, 127+) → `"Non displayable"`, no `"impossible"`
- `char` imposible (overflow, nan, inf) → `"impossible"`
- `int` imposible (overflow, nan, inf) → `"impossible"`
- Para floats que son enteros, usar `std::setprecision` con `std::fixed` para forzar el `.0`

### Checks exactos del evaluador (42evalhub)
| Check |
|---|
| Clase con constructor privado y métodos estáticos |
| Se usa `static_cast` para las conversiones |
| Casts implícitos solo para promotion casts |
| El programa funciona correctamente |

---

## 📦 Ex01 — Serializer: reinterpret_cast

### ¿Qué problema de C++ resuelve?
`reinterpret_cast` es el cast de bajo nivel: reinterpreta los bits de un tipo como si fueran de otro tipo. Es el único cast que puede convertir un puntero a entero y viceversa. Se usa en serialización, protocolos de red, FFI, y cualquier caso donde necesitas tratar memoria cruda.

### Arquitectura

```
Serializer                               (no instanciable)
├── static serialize(Data* ptr)   → uintptr_t   [reinterpret_cast]
└── static deserialize(uintptr_t) → Data*       [reinterpret_cast]

Data                                     (estructura con miembros)
└── int value  (u otros campos)
```

### Flujo de verificación en main

```
Data d;
d.value = 42;
uintptr_t raw = Serializer::serialize(&d);    // puntero → entero
Data* ptr     = Serializer::deserialize(raw); // entero  → puntero
assert(ptr == &d);                             // mismo puntero
assert(ptr->value == 42);                     // datos intactos
```

### Checks exactos del evaluador (42evalhub)
| Check |
|---|
| El programa funciona correctamente |
| Clase con constructor privado y métodos estáticos |
| `reinterpret_cast` usado: `Data*` → `uintptr_t` |
| `reinterpret_cast` usado: `uintptr_t` → `Data*` |
| La struct `Data` resultante es usable |

---

## 📦 Ex02 — Identify real type: dynamic_cast

### ¿Qué problema de C++ resuelve?
`dynamic_cast` es el único cast con verificación en **tiempo de ejecución** (runtime). Permite preguntar: *"¿Este puntero Base* apunta realmente a un objeto de tipo A, B o C?"*. Si la respuesta es no, devuelve `NULL` (para punteros) o lanza `std::bad_cast` (para referencias).

**CRÍTICO**: Requiere que la clase base tenga **al menos un método virtual** (la tabla virtual, `vtable`). Sin ella, `dynamic_cast` no funciona. Por eso `Base` tiene un destructor virtual.

### Arquitectura

```
Base                          (destructor virtual — necesario para dynamic_cast)
├── A : public Base           (vacía)
├── B : public Base           (vacía)
└── C : public Base           (vacía)

generate() → Base*            (crea A, B o C aleatoriamente)
identify(Base* p)             (usa dynamic_cast<A*>(p) → NULL check)
identify(Base& p)             (usa dynamic_cast<A&>(p) → try/catch std::bad_cast)
```

### Las dos variantes de identify

**Con puntero** (`identify(Base* p)`):
```cpp
if (dynamic_cast<A*>(p))       std::cout << "A";
else if (dynamic_cast<B*>(p))  std::cout << "B";
else if (dynamic_cast<C*>(p))  std::cout << "C";
```

**Con referencia** (`identify(Base& p)`):
```cpp
try { (void)dynamic_cast<A&>(p); std::cout << "A"; return; }
catch (std::exception&) {}
try { (void)dynamic_cast<B&>(p); std::cout << "B"; return; }
catch (std::exception&) {}
std::cout << "C";
```

### PROHIBIDO: `<typeinfo>` en cualquier fichero

### Checks exactos del evaluador (42evalhub)
| Check |
|---|
| El programa funciona correctamente |
| Se usa `dynamic_cast` para identificar el tipo real |
| `identify(Base* p)` comprueba si el cast devuelve `NULL` |
| `identify(Base& p)` usa `try`/`catch` |
| `<typeinfo>` NO aparece en ningún fichero |

---

## Visión Global — Los 4 casts de C++

| Situación | Cast correcto |
|---|---|
| Convertir `double` → `int` | `static_cast` |
| Convertir `Base*` → `Derived*` (seguro, sabes el tipo) | `static_cast` |
| Convertir puntero → entero y viceversa | `reinterpret_cast` |
| Descubrir si `Base*` es realmente `Derived*` en runtime | `dynamic_cast` |
| Quitar `const` de un puntero | `const_cast` |

---

## Checklist pre-entrega global

| Ítem | Estado |
|---|---|
| Compila con `c++ -Wall -Wextra -Werror -std=c++98` | ☐ |
| Sin `using namespace`, sin `friend` | ☐ |
| Sin `<typeinfo>` en ningún fichero | ☐ |
| `ScalarConverter` no instanciable (constructor privado) | ☐ |
| `Serializer` no instanciable (constructor privado) | ☐ |
| `Data` tiene miembros (no está vacía) | ☐ |
| `static_cast` usado en ex00 | ☐ |
| `reinterpret_cast` usado dos veces en ex01 | ☐ |
| `dynamic_cast` usado en ex02 | ☐ |
| `identify(Base*)` comprueba NULL | ☐ |
| `identify(Base&)` usa try/catch (sin punteros dentro) | ☐ |
| Tests suficientes en cada `main` | ☐ |
| Sin memory leaks (`valgrind --leak-check=full`) | ☐ |
