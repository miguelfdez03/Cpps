# 🗺️ Hoja de Ruta - CPP Module 04

## 📋 Índice
1. [Introducción](#introducción)
2. [Conceptos Teóricos Fundamentales](#conceptos-teóricos-fundamentales)
3. [Requisitos Generales](#requisitos-generales)
4. [Ejercicio 00: Polimorfismo](#ejercicio-00-polimorfismo)
5. [Ejercicio 01: Brain - Gestión de Memoria](#ejercicio-01-brain---gestión-de-memoria)
6. [Ejercicio 02: Clase Abstracta](#ejercicio-02-clase-abstracta)
7. [Ejercicio 03: Interfaces](#ejercicio-03-interfaces)
8. [Checklist Final](#checklist-final)
9. [Recursos Adicionales](#recursos-adicionales)

---

## 🎯 Introducción

El **Módulo 04** de C++ se centra en tres conceptos fundamentales de la Programación Orientada a Objetos:
- **Polimorfismo de Subtipo** (Subtype Polymorphism)
- **Clases Abstractas** (Abstract Classes)
- **Interfaces** (Interfaces via pure abstract classes)

### Objetivos del Módulo
✅ Comprender y aplicar polimorfismo mediante herencia  
✅ Utilizar funciones virtuales correctamente  
✅ Gestionar memoria con punteros en jerarquías de clases  
✅ Implementar destructores virtuales  
✅ Crear clases abstractas e interfaces  
✅ Realizar copias profundas (deep copy)  

---

## 📚 Conceptos Teóricos Fundamentales

### 1. Polimorfismo
El **polimorfismo** permite que objetos de diferentes clases derivadas sean tratados como objetos de una clase base común.

```cpp
Animal* animal = new Dog(); // Dog es tratado como Animal
animal->makeSound(); // Llamará al método de Dog, no de Animal
```

### 2. Funciones Virtuales
Palabra clave `virtual` permite que el método de la clase derivada sobrescriba el de la clase base.

```cpp
class Animal {
public:
    virtual void makeSound() const; // Función virtual
};

class Dog : public Animal {
public:
    void makeSound() const; // Override (sobrescribe la función virtual)
};
```

**⚠️ IMPORTANTE:** Sin `virtual`, se produce "static binding" y se llamará siempre al método de la clase base.

### 3. Destructores Virtuales
**CRÍTICO** para evitar memory leaks cuando se elimina un objeto derivado a través de un puntero a la clase base.

```cpp
class Animal {
public:
    virtual ~Animal(); // Destructor virtual
};
```

**Con destructor virtual:**
```cpp
Animal* dog = new Dog();
delete dog; // ✅ Llama ~Dog() y luego ~Animal()
```

**Sin destructor virtual:**
```cpp
Animal* dog = new Dog();
delete dog; // ❌ Solo llama ~Animal(), memory leak!
```

### 4. Clases Abstractas
Una clase con **al menos una función virtual pura** es abstracta y no puede ser instanciada.

```cpp
class Animal {
public:
    virtual void makeSound() const = 0; // Función virtual pura
};

Animal animal; // ❌ ERROR: No se puede instanciar
```

### 5. Orthodox Canonical Form (OCF)
Todas las clases deben implementar:
```cpp
class MyClass {
public:
    MyClass();                              // Constructor por defecto
    MyClass(const MyClass& other);          // Constructor de copia
    MyClass& operator=(const MyClass& rhs); // Operador de asignación
    ~MyClass();                             // Destructor
};
```

### 6. Deep Copy vs Shallow Copy

**Shallow Copy (INCORRECTO para punteros):**
```cpp
Dog::Dog(const Dog& other) {
    this->brain = other.brain; // ❌ Ambos comparten el mismo Brain
}
```

**Deep Copy (CORRECTO):**
```cpp
Dog::Dog(const Dog& other) {
    this->brain = new Brain(*other.brain); // ✅ Cada uno tiene su propio Brain
}
```

---

## ⚙️ Requisitos Generales

### Compilación
```bash
c++ -Wall -Wextra -Werror -std=c++98 *.cpp -o program
```

### Estructura de Directorios
```
cpp04/
├── ex00/
│   ├── Makefile
│   ├── main.cpp
│   ├── Animal.hpp
│   ├── Animal.cpp
│   ├── Dog.hpp
│   ├── Dog.cpp
│   ├── Cat.hpp
│   ├── Cat.cpp
│   ├── WrongAnimal.hpp
│   ├── WrongAnimal.cpp
│   ├── WrongCat.hpp
│   └── WrongCat.cpp
├── ex01/
├── ex02/
└── ex03/
```

### Normas Obligatorias
- ✅ Usar C++98 estándar
- ✅ Nombres de clases en UpperCamelCase
- ✅ Orthodox Canonical Form en todas las clases
- ✅ Include guards en todos los headers
- ✅ Destructores virtuales cuando hay herencia
- ❌ Prohibido: `using namespace`, `*printf()`, `*alloc()`, `free()`
- ❌ Prohibido STL (hasta módulos 08-09)

---

## 🏋️ Ejercicio 00: Polimorfismo

### 📝 Objetivo
Implementar polimorfismo básico con funciones virtuales y demostrar la diferencia con el binding estático.

### 🎯 Conceptos Clave
- Herencia simple
- Funciones virtuales (`virtual`)
- Binding dinámico vs estático
- Destructores virtuales

### 🏗️ Clases a Implementar

#### 1. Clase Base: Animal
```cpp
// Animal.hpp
#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <iostream>
#include <string>

class Animal {
protected:
    std::string type;

public:
    Animal();
    Animal(const Animal& other);
    Animal& operator=(const Animal& rhs);
    virtual ~Animal(); // ⚠️ VIRTUAL porque hay clases derivadas

    virtual void makeSound() const; // ⚠️ VIRTUAL para polimorfismo
    std::string getType() const;
};

#endif
```

```cpp
// Animal.cpp
#include "Animal.hpp"

Animal::Animal() : type("Animal") {
    std::cout << "Animal constructor called" << std::endl;
}

Animal::Animal(const Animal& other) : type(other.type) {
    std::cout << "Animal copy constructor called" << std::endl;
}

Animal& Animal::operator=(const Animal& rhs) {
    std::cout << "Animal assignment operator called" << std::endl;
    if (this != &rhs)
        this->type = rhs.type;
    return *this;
}

Animal::~Animal() {
    std::cout << "Animal destructor called" << std::endl;
}

void Animal::makeSound() const {
    std::cout << "* Generic animal sound *" << std::endl;
}

std::string Animal::getType() const {
    return this->type;
}
```

#### 2. Clase Derivada: Dog
```cpp
// Dog.hpp
#ifndef DOG_HPP
#define DOG_HPP

#include "Animal.hpp"

class Dog : public Animal {
public:
    Dog();
    Dog(const Dog& other);
    Dog& operator=(const Dog& rhs);
    ~Dog();

    void makeSound() const; // Override (sobrescribe la función virtual)
};

#endif
```

```cpp
// Dog.cpp
#include "Dog.hpp"

Dog::Dog() {
    this->type = "Dog"; // Asigna tipo específico
    std::cout << "Dog constructor called" << std::endl;
}

Dog::Dog(const Dog& other) : Animal(other) {
    std::cout << "Dog copy constructor called" << std::endl;
}

Dog& Dog::operator=(const Dog& rhs) {
    std::cout << "Dog assignment operator called" << std::endl;
    if (this != &rhs)
        Animal::operator=(rhs);
    return *this;
}

Dog::~Dog() {
    std::cout << "Dog destructor called" << std::endl;
}

void Dog::makeSound() const {
    std::cout << "Woof! Woof!" << std::endl;
}
```

#### 3. Clase Derivada: Cat
```cpp
// Cat.hpp
#ifndef CAT_HPP
#define CAT_HPP

#include "Animal.hpp"

class Cat : public Animal {
public:
    Cat();
    Cat(const Cat& other);
    Cat& operator=(const Cat& rhs);
    ~Cat();

    void makeSound() const;
};

#endif
```

```cpp
// Cat.cpp
#include "Cat.hpp"

Cat::Cat() {
    this->type = "Cat";
    std::cout << "Cat constructor called" << std::endl;
}

Cat::Cat(const Cat& other) : Animal(other) {
    std::cout << "Cat copy constructor called" << std::endl;
}

Cat& Cat::operator=(const Cat& rhs) {
    std::cout << "Cat assignment operator called" << std::endl;
    if (this != &rhs)
        Animal::operator=(rhs);
    return *this;
}

Cat::~Cat() {
    std::cout << "Cat destructor called" << std::endl;
}

void Cat::makeSound() const {
    std::cout << "Meow! Meow!" << std::endl;
}
```

### 🔴 Clases "Wrong" (Para Demostrar Error Común)

#### WrongAnimal (Sin virtual)
```cpp
// WrongAnimal.hpp
#ifndef WRONGANIMAL_HPP
#define WRONGANIMAL_HPP

#include <iostream>
#include <string>

class WrongAnimal {
protected:
    std::string type;

public:
    WrongAnimal();
    WrongAnimal(const WrongAnimal& other);
    WrongAnimal& operator=(const WrongAnimal& rhs);
    ~WrongAnimal(); // ⚠️ NO es virtual

    void makeSound() const; // ⚠️ NO es virtual - static binding
    std::string getType() const;
};

#endif
```

#### WrongCat
```cpp
// WrongCat.hpp
#ifndef WRONGCAT_HPP
#define WRONGCAT_HPP

#include "WrongAnimal.hpp"

class WrongCat : public WrongAnimal {
public:
    WrongCat();
    WrongCat(const WrongCat& other);
    WrongCat& operator=(const WrongCat& rhs);
    ~WrongCat();

    void makeSound() const; // NO sobrescribe (sin virtual en base)
};

#endif
```

### 🧪 Main de Prueba
```cpp
// main.cpp
#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include "WrongAnimal.hpp"
#include "WrongCat.hpp"

int main() {
    std::cout << "=== CORRECT BEHAVIOR WITH VIRTUAL ===" << std::endl;
    const Animal* meta = new Animal();
    const Animal* j = new Dog();
    const Animal* i = new Cat();
    
    std::cout << "\nType: " << j->getType() << std::endl;
    std::cout << "Type: " << i->getType() << std::endl;
    
    std::cout << "\nSounds:" << std::endl;
    i->makeSound(); // ✅ Output: Meow! Meow!
    j->makeSound(); // ✅ Output: Woof! Woof!
    meta->makeSound(); // Output: * Generic animal sound *
    
    std::cout << "\nDeleting animals:" << std::endl;
    delete i;
    delete j;
    delete meta;
    
    std::cout << "\n=== WRONG BEHAVIOR WITHOUT VIRTUAL ===" << std::endl;
    const WrongAnimal* wrong = new WrongCat();
    
    std::cout << "Type: " << wrong->getType() << std::endl;
    wrong->makeSound(); // ❌ Output: WrongAnimal sound (not WrongCat!)
    
    std::cout << "\nDeleting wrong animal:" << std::endl;
    delete wrong;
    
    std::cout << "\n=== TESTING COPY ===" << std::endl;
    Dog originalDog;
    Dog copiedDog(originalDog);
    
    return 0;
}
```

### 📋 Makefile
```makefile
NAME = animals

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = main.cpp Animal.cpp Dog.cpp Cat.cpp WrongAnimal.cpp WrongCat.cpp
OBJS = $(SRCS:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
```

### ⚠️ Errores Comunes
1. **Olvidar `virtual` en makeSound()** → No habrá polimorfismo
2. **Olvidar `virtual` en el destructor** → Memory leaks
3. **No inicializar `type` en constructores derivados**
4. **Olvidar llamar al constructor base** en copy constructor

### ✅ Checklist Ex00
- [ ] Animal con atributo protegido `type`
- [ ] Dog y Cat heredan de Animal
- [ ] `makeSound()` es virtual en Animal
- [ ] Destructor virtual en Animal
- [ ] Dog y Cat establecen su tipo correctamente
- [ ] WrongAnimal y WrongCat sin virtual
- [ ] Tests demuestran diferencia virtual/no-virtual
- [ ] Orthodox Canonical Form en todas las clases
- [ ] No memory leaks (valgrind)
- [ ] Makefile funcional

---

## 🧠 Ejercicio 01: Brain - Gestión de Memoria

### 📝 Objetivo
Implementar gestión dinámica de memoria con composición y garantizar deep copies.

### 🎯 Conceptos Clave
- Composición de objetos
- Gestión de memoria con `new`/`delete`
- Deep copy vs shallow copy
- Destructores en cascada
- Memory leak detection

### 🏗️ Clases a Implementar

#### 1. Clase Brain
```cpp
// Brain.hpp
#ifndef BRAIN_HPP
#define BRAIN_HPP

#include <iostream>
#include <string>

class Brain {
private:
    std::string ideas[100]; // Array de 100 ideas

public:
    Brain();
    Brain(const Brain& other);
    Brain& operator=(const Brain& rhs);
    ~Brain();

    // Métodos auxiliares para testing
    void setIdea(int index, const std::string& idea);
    std::string getIdea(int index) const;
};

#endif
```

```cpp
// Brain.cpp
#include "Brain.hpp"

Brain::Brain() {
    std::cout << "Brain constructor called" << std::endl;
    for (int i = 0; i < 100; i++)
        ideas[i] = "";
}

Brain::Brain(const Brain& other) {
    std::cout << "Brain copy constructor called" << std::endl;
    for (int i = 0; i < 100; i++)
        this->ideas[i] = other.ideas[i];
}

Brain& Brain::operator=(const Brain& rhs) {
    std::cout << "Brain assignment operator called" << std::endl;
    if (this != &rhs) {
        for (int i = 0; i < 100; i++)
            this->ideas[i] = rhs.ideas[i];
    }
    return *this;
}

Brain::~Brain() {
    std::cout << "Brain destructor called" << std::endl;
}

void Brain::setIdea(int index, const std::string& idea) {
    if (index >= 0 && index < 100)
        ideas[index] = idea;
}

std::string Brain::getIdea(int index) const {
    if (index >= 0 && index < 100)
        return ideas[index];
    return "";
}
```

#### 2. Dog Modificado (con Brain)
```cpp
// Dog.hpp
#ifndef DOG_HPP
#define DOG_HPP

#include "Animal.hpp"
#include "Brain.hpp"

class Dog : public Animal {
private:
    Brain* brain; // ⚠️ Puntero a Brain

public:
    Dog();
    Dog(const Dog& other);
    Dog& operator=(const Dog& rhs);
    ~Dog();

    void makeSound() const;
    Brain* getBrain() const; // Para testing
};

#endif
```

```cpp
// Dog.cpp
#include "Dog.hpp"

Dog::Dog() : brain(new Brain()) { // ⚠️ Crea Brain con new
    this->type = "Dog";
    std::cout << "Dog constructor called" << std::endl;
}

Dog::Dog(const Dog& other) : Animal(other), brain(new Brain(*other.brain)) {
    // ⚠️ DEEP COPY: Crea nuevo Brain copiando el contenido
    std::cout << "Dog copy constructor called" << std::endl;
}

Dog& Dog::operator=(const Dog& rhs) {
    std::cout << "Dog assignment operator called" << std::endl;
    if (this != &rhs) {
        Animal::operator=(rhs);
        delete this->brain; // ⚠️ Borra el Brain anterior
        this->brain = new Brain(*rhs.brain); // ⚠️ Crea nuevo Brain
    }
    return *this;
}

Dog::~Dog() {
    delete this->brain; // ⚠️ Libera memoria del Brain
    std::cout << "Dog destructor called" << std::endl;
}

void Dog::makeSound() const {
    std::cout << "Woof! Woof!" << std::endl;
}

Brain* Dog::getBrain() const {
    return this->brain;
}
```

#### 3. Cat Modificado (con Brain)
```cpp
// Cat.hpp
#ifndef CAT_HPP
#define CAT_HPP

#include "Animal.hpp"
#include "Brain.hpp"

class Cat : public Animal {
private:
    Brain* brain;

public:
    Cat();
    Cat(const Cat& other);
    Cat& operator=(const Cat& rhs);
    ~Cat();

    void makeSound() const;
    Brain* getBrain() const;
};

#endif
```

```cpp
// Cat.cpp
#include "Cat.hpp"

Cat::Cat() : brain(new Brain()) {
    this->type = "Cat";
    std::cout << "Cat constructor called" << std::endl;
}

Cat::Cat(const Cat& other) : Animal(other), brain(new Brain(*other.brain)) {
    std::cout << "Cat copy constructor called" << std::endl;
}

Cat& Cat::operator=(const Cat& rhs) {
    std::cout << "Cat assignment operator called" << std::endl;
    if (this != &rhs) {
        Animal::operator=(rhs);
        delete this->brain;
        this->brain = new Brain(*rhs.brain);
    }
    return *this;
}

Cat::~Cat() {
    delete this->brain;
    std::cout << "Cat destructor called" << std::endl;
}

void Cat::makeSound() const {
    std::cout << "Meow! Meow!" << std::endl;
}

Brain* Cat::getBrain() const {
    return this->brain;
}
```

### 🧪 Main de Prueba Completo
```cpp
// main.cpp
#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"
#include <iostream>

int main() {
    std::cout << "=== TEST 1: BASIC CONSTRUCTION ===" << std::endl;
    const Animal* j = new Dog();
    const Animal* i = new Cat();
    
    delete j;
    delete i;
    
    std::cout << "\n=== TEST 2: ARRAY OF ANIMALS ===" << std::endl;
    const int arraySize = 10;
    Animal* animals[arraySize];
    
    // Llenar mitad con Dogs, mitad con Cats
    std::cout << "Creating array..." << std::endl;
    for (int idx = 0; idx < arraySize; idx++) {
        if (idx < arraySize / 2)
            animals[idx] = new Dog();
        else
            animals[idx] = new Cat();
    }
    
    std::cout << "\nMaking sounds..." << std::endl;
    for (int idx = 0; idx < arraySize; idx++) {
        animals[idx]->makeSound();
    }
    
    std::cout << "\nDeleting array..." << std::endl;
    for (int idx = 0; idx < arraySize; idx++) {
        delete animals[idx]; // ✅ Debe llamar destructores correctos
    }
    
    std::cout << "\n=== TEST 3: DEEP COPY ===" << std::endl;
    Dog basic;
    {
        Dog tmp = basic; // Copy constructor
        std::cout << "Basic brain address: " << basic.getBrain() << std::endl;
        std::cout << "Tmp brain address: " << tmp.getBrain() << std::endl;
        // ⚠️ Las direcciones DEBEN ser diferentes (deep copy)
    }
    
    std::cout << "\n=== TEST 4: ASSIGNMENT OPERATOR ===" << std::endl;
    Dog dog1;
    Dog dog2;
    dog2 = dog1; // Assignment operator
    std::cout << "Dog1 brain: " << dog1.getBrain() << std::endl;
    std::cout << "Dog2 brain: " << dog2.getBrain() << std::endl;
    // ⚠️ Las direcciones DEBEN ser diferentes
    
    std::cout << "\n=== TEST 5: BRAIN IDEAS ===" << std::endl;
    Dog smartDog;
    smartDog.getBrain()->setIdea(0, "I want food");
    smartDog.getBrain()->setIdea(1, "I want to play");
    
    Dog copyDog(smartDog);
    std::cout << "Original idea 0: " << smartDog.getBrain()->getIdea(0) << std::endl;
    std::cout << "Copy idea 0: " << copyDog.getBrain()->getIdea(0) << std::endl;
    
    // Modificar copia no debe afectar original
    copyDog.getBrain()->setIdea(0, "Different idea");
    std::cout << "After modification:" << std::endl;
    std::cout << "Original idea 0: " << smartDog.getBrain()->getIdea(0) << std::endl;
    std::cout << "Copy idea 0: " << copyDog.getBrain()->getIdea(0) << std::endl;
    
    return 0;
}
```

### 🔍 Testing con Valgrind
```bash
# Compilar
make

# Ejecutar con valgrind para detectar memory leaks
valgrind --leak-check=full --show-leak-kinds=all ./brain

# Salida esperada:
# ERROR SUMMARY: 0 errors from 0 contexts
# All heap blocks were freed -- no leaks are possible
```

### ⚠️ Errores Comunes
1. **Shallow copy:** Copiar puntero en vez de crear nuevo objeto
   ```cpp
   // ❌ INCORRECTO
   Dog::Dog(const Dog& other) : brain(other.brain) { }
   
   // ✅ CORRECTO
   Dog::Dog(const Dog& other) : brain(new Brain(*other.brain)) { }
   ```

2. **Olvidar delete brain en destructor** → Memory leak

3. **No borrar brain anterior en operator=** → Memory leak

4. **Olvidar self-assignment check:**
   ```cpp
   if (this != &rhs) { // ⚠️ NECESARIO
       delete this->brain;
       this->brain = new Brain(*rhs.brain);
   }
   ```

5. **No hacer delete[] sino delete** (aunque aquí usamos delete)

### ✅ Checklist Ex01
- [ ] Brain con array de 100 strings
- [ ] Dog y Cat tienen puntero privado Brain*
- [ ] Constructor crea Brain con `new`
- [ ] Destructor hace `delete` del Brain
- [ ] Copy constructor hace deep copy
- [ ] Assignment operator hace deep copy
- [ ] Array de Animals mitad Dogs mitad Cats
- [ ] Destructores se llaman en orden correcto
- [ ] Tests demuestran deep copy funcional
- [ ] Valgrind: 0 leaks, 0 errors
- [ ] Makefile actualizado

---

## 🚫 Ejercicio 02: Clase Abstracta

### 📝 Objetivo
Hacer que Animal sea una clase abstracta (no instanciable) manteniendo toda la funcionalidad.

### 🎯 Conceptos Clave
- Funciones virtuales puras (`= 0`)
- Clases abstractas
- No se pueden instanciar clases abstractas
- Las clases derivadas DEBEN implementar funciones puras

### 🔧 Modificaciones Necesarias

#### Animal → AAnimal (Opcional renombrar)
```cpp
// Animal.hpp (o AAnimal.hpp)
#ifndef ANIMAL_HPP
#define ANIMAL_HPP

#include <iostream>
#include <string>

class Animal {
protected:
    std::string type;

public:
    Animal();
    Animal(const Animal& other);
    Animal& operator=(const Animal& rhs);
    virtual ~Animal();

    virtual void makeSound() const = 0; // ⚠️ FUNCIÓN VIRTUAL PURA
    std::string getType() const;
};

#endif
```

```cpp
// Animal.cpp
#include "Animal.hpp"

Animal::Animal() : type("Animal") {
    std::cout << "Animal constructor called" << std::endl;
}

Animal::Animal(const Animal& other) : type(other.type) {
    std::cout << "Animal copy constructor called" << std::endl;
}

Animal& Animal::operator=(const Animal& rhs) {
    std::cout << "Animal assignment operator called" << std::endl;
    if (this != &rhs)
        this->type = rhs.type;
    return *this;
}

Animal::~Animal() {
    std::cout << "Animal destructor called" << std::endl;
}

// ⚠️ NO implementar makeSound() aquí, o hacerlo así:
// void Animal::makeSound() const = 0; // En C++98 no es necesario

std::string Animal::getType() const {
    return this->type;
}
```

### 🧪 Main de Prueba
```cpp
// main.cpp
#include "Animal.hpp"
#include "Dog.hpp"
#include "Cat.hpp"

int main() {
    std::cout << "=== TEST 1: CANNOT INSTANTIATE ANIMAL ===" << std::endl;
    // Animal meta; // ❌ ERROR de compilación (esperado)
    // Animal* meta = new Animal(); // ❌ ERROR de compilación (esperado)
    std::cout << "Animal class cannot be instantiated ✓" << std::endl;
    
    std::cout << "\n=== TEST 2: CAN INSTANTIATE DERIVED CLASSES ===" << std::endl;
    const Animal* j = new Dog(); // ✅ OK
    const Animal* i = new Cat(); // ✅ OK
    
    std::cout << j->getType() << std::endl;
    std::cout << i->getType() << std::endl;
    
    j->makeSound(); // ✅ Woof!
    i->makeSound(); // ✅ Meow!
    
    delete j;
    delete i;
    
    std::cout << "\n=== TEST 3: ARRAY OF ANIMALS ===" << std::endl;
    const int size = 6;
    Animal* animals[size];
    
    for (int idx = 0; idx < size; idx++) {
        if (idx < size / 2)
            animals[idx] = new Dog();
        else
            animals[idx] = new Cat();
    }
    
    std::cout << "\nMaking sounds:" << std::endl;
    for (int idx = 0; idx < size; idx++) {
        std::cout << animals[idx]->getType() << ": ";
        animals[idx]->makeSound();
    }
    
    std::cout << "\nDeleting animals:" << std::endl;
    for (int idx = 0; idx < size; idx++) {
        delete animals[idx];
    }
    
    return 0;
}
```

### 📊 Diferencias con Ex01

| Aspecto | Ex01 | Ex02 |
|---------|------|------|
| Animal | Instanciable | No instanciable |
| makeSound() | Implementado | Virtual puro (= 0) |
| Dog/Cat | Funcionan igual | DEBEN implementar makeSound() |
| Tests | Incluyen Animal directo | No pueden crear Animal |

### ⚠️ Errores Comunes
1. **No hacer makeSound() = 0** → Animal sigue siendo instanciable
2. **Olvidar implementar makeSound() en Dog/Cat** → Error de compilación
3. **Intentar instanciar Animal en tests** → Error de compilación (esperado)

### ✅ Checklist Ex02
- [ ] makeSound() es virtual pura en Animal
- [ ] Animal no puede ser instanciada
- [ ] Dog implementa makeSound()
- [ ] Cat implementa makeSound()
- [ ] Todo lo demás funciona igual que ex01
- [ ] Array de Animals funciona
- [ ] No memory leaks
- [ ] Tests comprueban que Animal no es instanciable

---

## 🔌 Ejercicio 03: Interfaces

### 📝 Objetivo
Implementar un sistema de Materias (hechizos) usando interfaces (clases abstractas puras) y demostrar composición compleja.

### 🎯 Conceptos Clave
- Interfaces (pure abstract classes)
- Sistema de inventario
- Gestión compleja de memoria
- Clonación de objetos
- Factory pattern (MateriaSource)

### 🏗️ Arquitectura del Sistema

```
IMateriaSource (interface)
     ↑
     |
MateriaSource (concrete)

AMateria (abstract)
     ↑
     ├── Ice (concrete)
     └── Cure (concrete)

ICharacter (interface)
     ↑
     |
Character (concrete)
```

### 📋 Implementación Paso a Paso

#### 1. Clase Abstracta AMateria
```cpp
// AMateria.hpp
#ifndef AMATERIA_HPP
#define AMATERIA_HPP

#include <iostream>
#include <string>

class ICharacter; // Forward declaration

class AMateria {
protected:
    std::string type;

public:
    AMateria();
    AMateria(std::string const& type);
    AMateria(const AMateria& other);
    AMateria& operator=(const AMateria& rhs);
    virtual ~AMateria();

    std::string const& getType() const;
    
    virtual AMateria* clone() const = 0; // ⚠️ Virtual pura
    virtual void use(ICharacter& target); // ⚠️ Virtual (puede sobrescribirse)
};

#endif
```

```cpp
// AMateria.cpp
#include "AMateria.hpp"
#include "ICharacter.hpp"

AMateria::AMateria() : type("unknown") {
}

AMateria::AMateria(std::string const& type) : type(type) {
}

AMateria::AMateria(const AMateria& other) : type(other.type) {
    // ⚠️ NO copiar tipo según subject
}

AMateria& AMateria::operator=(const AMateria& rhs) {
    if (this != &rhs) {
        // ⚠️ NO copiar tipo según subject
        // "copying the type doesn't make sense"
    }
    return *this;
}

AMateria::~AMateria() {
}

std::string const& AMateria::getType() const {
    return this->type;
}

void AMateria::use(ICharacter& target) {
    (void)target; // Default: hacer nada
}
```

#### 2. Materias Concretas: Ice y Cure
```cpp
// Ice.hpp
#ifndef ICE_HPP
#define ICE_HPP

#include "AMateria.hpp"

class Ice : public AMateria {
public:
    Ice();
    Ice(const Ice& other);
    Ice& operator=(const Ice& rhs);
    ~Ice();

    AMateria* clone() const; // ⚠️ Devuelve new Ice
    void use(ICharacter& target);
};

#endif
```

```cpp
// Ice.cpp
#include "Ice.hpp"
#include "ICharacter.hpp"

Ice::Ice() : AMateria("ice") {
}

Ice::Ice(const Ice& other) : AMateria(other) {
}

Ice& Ice::operator=(const Ice& rhs) {
    if (this != &rhs)
        AMateria::operator=(rhs);
    return *this;
}

Ice::~Ice() {
}

AMateria* Ice::clone() const {
    return new Ice(*this); // ⚠️ Crea copia nueva
}

void Ice::use(ICharacter& target) {
    std::cout << "* shoots an ice bolt at " << target.getName() << " *" << std::endl;
}
```

```cpp
// Cure.hpp
#ifndef CURE_HPP
#define CURE_HPP

#include "AMateria.hpp"

class Cure : public AMateria {
public:
    Cure();
    Cure(const Cure& other);
    Cure& operator=(const Cure& rhs);
    ~Cure();

    AMateria* clone() const;
    void use(ICharacter& target);
};

#endif
```

```cpp
// Cure.cpp
#include "Cure.hpp"
#include "ICharacter.hpp"

Cure::Cure() : AMateria("cure") {
}

Cure::Cure(const Cure& other) : AMateria(other) {
}

Cure& Cure::operator=(const Cure& rhs) {
    if (this != &rhs)
        AMateria::operator=(rhs);
    return *this;
}

Cure::~Cure() {
}

AMateria* Cure::clone() const {
    return new Cure(*this);
}

void Cure::use(ICharacter& target) {
    std::cout << "* heals " << target.getName() << "'s wounds *" << std::endl;
}
```

#### 3. Interface ICharacter
```cpp
// ICharacter.hpp
#ifndef ICHARACTER_HPP
#define ICHARACTER_HPP

#include <string>
class AMateria;

class ICharacter {
public:
    virtual ~ICharacter() {}
    virtual std::string const& getName() const = 0;
    virtual void equip(AMateria* m) = 0;
    virtual void unequip(int idx) = 0;
    virtual void use(int idx, ICharacter& target) = 0;
};

#endif
```

#### 4. Clase Character
```cpp
// Character.hpp
#ifndef CHARACTER_HPP
#define CHARACTER_HPP

#include "ICharacter.hpp"
#include "AMateria.hpp"

class Character : public ICharacter {
private:
    std::string name;
    AMateria* inventory[4]; // ⚠️ 4 slots

public:
    Character();
    Character(std::string const& name);
    Character(const Character& other);
    Character& operator=(const Character& rhs);
    ~Character();

    std::string const& getName() const;
    void equip(AMateria* m);
    void unequip(int idx);
    void use(int idx, ICharacter& target);
};

#endif
```

```cpp
// Character.cpp
#include "Character.hpp"

Character::Character() : name("unnamed") {
    for (int i = 0; i < 4; i++)
        inventory[i] = NULL;
}

Character::Character(std::string const& name) : name(name) {
    for (int i = 0; i < 4; i++)
        inventory[i] = NULL;
}

Character::Character(const Character& other) : name(other.name) {
    for (int i = 0; i < 4; i++) {
        if (other.inventory[i])
            this->inventory[i] = other.inventory[i]->clone(); // ⚠️ Deep copy
        else
            this->inventory[i] = NULL;
    }
}

Character& Character::operator=(const Character& rhs) {
    if (this != &rhs) {
        this->name = rhs.name;
        // ⚠️ Borrar inventario anterior
        for (int i = 0; i < 4; i++) {
            if (this->inventory[i]) {
                delete this->inventory[i];
                this->inventory[i] = NULL;
            }
        }
        // ⚠️ Copiar nuevo inventario
        for (int i = 0; i < 4; i++) {
            if (rhs.inventory[i])
                this->inventory[i] = rhs.inventory[i]->clone();
            else
                this->inventory[i] = NULL;
        }
    }
    return *this;
}

Character::~Character() {
    for (int i = 0; i < 4; i++) {
        if (inventory[i])
            delete inventory[i];
    }
}

std::string const& Character::getName() const {
    return this->name;
}

void Character::equip(AMateria* m) {
    if (!m)
        return;
    
    // Buscar primer slot vacío
    for (int i = 0; i < 4; i++) {
        if (!inventory[i]) {
            inventory[i] = m;
            return;
        }
    }
    // Inventario lleno - no hacer nada (no delete m!)
}

void Character::unequip(int idx) {
    if (idx >= 0 && idx < 4 && inventory[idx]) {
        inventory[idx] = NULL; // ⚠️ NO delete, solo quitar referencia
    }
}

void Character::use(int idx, ICharacter& target) {
    if (idx >= 0 && idx < 4 && inventory[idx]) {
        inventory[idx]->use(target);
    }
}
```

#### 5. Interface IMateriaSource
```cpp
// IMateriaSource.hpp
#ifndef IMATERIASOURCE_HPP
#define IMATERIASOURCE_HPP

#include <string>
class AMateria;

class IMateriaSource {
public:
    virtual ~IMateriaSource() {}
    virtual void learnMateria(AMateria* m) = 0;
    virtual AMateria* createMateria(std::string const& type) = 0;
};

#endif
```

#### 6. Clase MateriaSource
```cpp
// MateriaSource.hpp
#ifndef MATERIASOURCE_HPP
#define MATERIASOURCE_HPP

#include "IMateriaSource.hpp"
#include "AMateria.hpp"

class MateriaSource : public IMateriaSource {
private:
    AMateria* templates[4]; // ⚠️ 4 templates máximo

public:
    MateriaSource();
    MateriaSource(const MateriaSource& other);
    MateriaSource& operator=(const MateriaSource& rhs);
    ~MateriaSource();

    void learnMateria(AMateria* m);
    AMateria* createMateria(std::string const& type);
};

#endif
```

```cpp
// MateriaSource.cpp
#include "MateriaSource.hpp"

MateriaSource::MateriaSource() {
    for (int i = 0; i < 4; i++)
        templates[i] = NULL;
}

MateriaSource::MateriaSource(const MateriaSource& other) {
    for (int i = 0; i < 4; i++) {
        if (other.templates[i])
            this->templates[i] = other.templates[i]->clone();
        else
            this->templates[i] = NULL;
    }
}

MateriaSource& MateriaSource::operator=(const MateriaSource& rhs) {
    if (this != &rhs) {
        for (int i = 0; i < 4; i++) {
            if (this->templates[i]) {
                delete this->templates[i];
                this->templates[i] = NULL;
            }
        }
        for (int i = 0; i < 4; i++) {
            if (rhs.templates[i])
                this->templates[i] = rhs.templates[i]->clone();
            else
                this->templates[i] = NULL;
        }
    }
    return *this;
}

MateriaSource::~MateriaSource() {
    for (int i = 0; i < 4; i++) {
        if (templates[i])
            delete templates[i];
    }
}

void MateriaSource::learnMateria(AMateria* m) {
    if (!m)
        return;
    
    // Buscar primer slot vacío
    for (int i = 0; i < 4; i++) {
        if (!templates[i]) {
            templates[i] = m->clone(); // ⚠️ Clonar, no almacenar directamente
            return;
        }
    }
    // Si está lleno, no hacer nada
}

AMateria* MateriaSource::createMateria(std::string const& type) {
    for (int i = 0; i < 4; i++) {
        if (templates[i] && templates[i]->getType() == type)
            return templates[i]->clone(); // ⚠️ Devolver clon
    }
    return NULL; // Tipo desconocido
}
```

### 🧪 Main del Subject
```cpp
// main.cpp
#include "IMateriaSource.hpp"
#include "MateriaSource.hpp"
#include "ICharacter.hpp"
#include "Character.hpp"
#include "AMateria.hpp"
#include "Ice.hpp"
#include "Cure.hpp"

int main() {
    std::cout << "=== SUBJECT TEST ===" << std::endl;
    IMateriaSource* src = new MateriaSource();
    src->learnMateria(new Ice());
    src->learnMateria(new Cure());
    
    ICharacter* me = new Character("me");
    
    AMateria* tmp;
    tmp = src->createMateria("ice");
    me->equip(tmp);
    tmp = src->createMateria("cure");
    me->equip(tmp);
    
    ICharacter* bob = new Character("bob");
    
    me->use(0, *bob); // * shoots an ice bolt at bob *
    me->use(1, *bob); // * heals bob's wounds *
    
    delete bob;
    delete me;
    delete src;
    
    return 0;
}
```

### 🧪 Tests Adicionales Recomendados
```cpp
void testDeepCopy() {
    std::cout << "\n=== TEST: DEEP COPY CHARACTER ===" << std::endl;
    Character* original = new Character("original");
    original->equip(new Ice());
    original->equip(new Cure());
    
    Character* copy = new Character(*original);
    
    std::cout << "Original Character: " << original->getName() << std::endl;
    std::cout << "Copy Character: " << copy->getName() << std::endl;
    
    Character bob("bob");
    original->use(0, bob);
    copy->use(0, bob);
    
    delete original;
    delete copy;
}

void testUnequip() {
    std::cout << "\n=== TEST: UNEQUIP ===" << std::endl;
    Character* hero = new Character("hero");
    AMateria* ice = new Ice();
    AMateria* cure = new Cure();
    
    hero->equip(ice);
    hero->equip(cure);
    
    Character enemy("enemy");
    hero->use(0, enemy); // Should work
    hero->use(1, enemy); // Should work
    
    hero->unequip(0); // ⚠️ ice ya no está en inventario
    hero->use(0, enemy); // No debería hacer nada
    
    delete ice; // ⚠️ Debemos manejar la memoria nosotros
    delete cure;
    delete hero;
}

void testFullInventory() {
    std::cout << "\n=== TEST: FULL INVENTORY ===" << std::endl;
    Character* mage = new Character("mage");
    
    mage->equip(new Ice());
    mage->equip(new Cure());
    mage->equip(new Ice());
    mage->equip(new Cure());
    
    AMateria* extra = new Ice();
    mage->equip(extra); // Inventario lleno - no se equipa
    
    // ⚠️ extra nunca se equipó, debemos borrarlo
    delete extra;
    delete mage;
}

int main() {
    // Subject test
    // ... código del subject ...
    
    testDeepCopy();
    testUnequip();
    testFullInventory();
    
    system("leaks materia"); // En macOS
    return 0;
}
```

### 📋 Makefile Completo
```makefile
NAME = materia

CXX = c++
CXXFLAGS = -Wall -Wextra -Werror -std=c++98

SRCS = main.cpp \
       AMateria.cpp \
       Ice.cpp \
       Cure.cpp \
       Character.cpp \
       MateriaSource.cpp

OBJS = $(SRCS:.cpp=.o)

HEADERS = AMateria.hpp \
          Ice.hpp \
          Cure.hpp \
          ICharacter.hpp \
          Character.hpp \
          IMateriaSource.hpp \
          MateriaSource.hpp

all: $(NAME)

$(NAME): $(OBJS)
	$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)

%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(OBJS)

fclean: clean
	rm -f $(NAME)

re: fclean all

.PHONY: all clean fclean re
```

### ⚠️ Errores Comunes y Críticos

1. **Memory leaks con unequip:**
   ```cpp
   // ❌ INCORRECTO
   void Character::unequip(int idx) {
       if (inventory[idx])
           delete inventory[idx]; // NO! Según subject: NO delete
   }
   
   // ✅ CORRECTO
   void Character::unequip(int idx) {
       if (inventory[idx])
           inventory[idx] = NULL; // Solo desreferencia
   }
   ```

2. **learnMateria debe clonar:**
   ```cpp
   // ❌ INCORRECTO
   void MateriaSource::learnMateria(AMateria* m) {
       templates[i] = m; // NO! Quedamos con puntero externo
   }
   
   // ✅ CORRECTO
   void MateriaSource::learnMateria(AMateria* m) {
       templates[i] = m->clone(); // Crear copia propia
   }
   ```

3. **Copiar tipo en AMateria:**
   ```cpp
   // Según subject: "copying the type doesn't make sense"
   AMateria& AMateria::operator=(const AMateria& rhs) {
       // NO copiar this->type = rhs.type;
       return *this;
   }
   ```

4. **Forward declarations:**
   ```cpp
   // AMateria.hpp necesita:
   class ICharacter; // Forward declaration
   
   // Incluir ICharacter.hpp solo en .cpp
   ```

### ✅ Checklist Ex03
- [ ] AMateria es abstracta (clone() = 0)
- [ ] Ice y Cure implementan clone() y use()
- [ ] ICharacter es interfaz pura
- [ ] Character tiene inventario de 4 slots
- [ ] Character::equip() encuentra primer slot vacío
- [ ] Character::unequip() NO hace delete
- [ ] Character deep copy funcional
- [ ] IMateriaSource es interfaz pura
- [ ] MateriaSource aprende hasta 4 materias
- [ ] learnMateria clona la materia recibida
- [ ] createMateria devuelve clone o NULL
- [ ] Test del subject compila y ejecuta correctamente
- [ ] Salida coincide: "* shoots an ice bolt at bob *"
- [ ] Valgrind: 0 leaks (gestionar memoria de unequip!)
- [ ] Todos los headers con include guards
- [ ] Forward declarations donde corresponda

---

## ✅ Checklist Final del Módulo

### General
- [ ] Todos los ejercicios compilan con `-Wall -Wextra -Werror -std=c++98`
- [ ] Makefiles funcionan (all, clean, fclean, re)
- [ ] No hay memory leaks (valgrind / leaks)
- [ ] Orthodox Canonical Form en todas las clases
- [ ] Include guards en todos los .hpp
- [ ] Destructores virtuales donde corresponde

### Por Ejercicio
#### Ex00
- [ ] Polimorfismo funciona con virtual
- [ ] WrongAnimal demuestra problema sin virtual
- [ ] Mensajes de constructores/destructores

#### Ex01
- [ ] Deep copy implementada correctamente
- [ ] Array de Animals funciona
- [ ] Brain se destruye correctamente

#### Ex02
- [ ] Animal no es instanciable
- [ ] Todo lo demás funciona igual

#### Ex03
- [ ] Sistema de Materias completo
- [ ] Interfaces correctamente implementadas
- [ ] Gestión de memoria compleja correcta
- [ ] unequip() NO hace delete

### Tests
- [ ] Tests más completos que los del subject
- [ ] Tests de deep copy
- [ ] Tests de edge cases (inventario lleno, índices inválidos, etc.)
- [ ] Tests de gestión de memoria

---

## 📚 Recursos Adicionales

### Conceptos para Estudiar
1. **Virtual functions y vtables**
   - Cómo funciona el dispatch dinámico
   - Overhead de funciones virtuales

2. **Abstract classes vs Interfaces**
   - Diferencias conceptuales
   - Cuándo usar cada uno

3. **RAII (Resource Acquisition Is Initialization)**
   - Gestión automática de recursos
   - Smart pointers (C++11, solo para conocimiento)

4. **Design Patterns**
   - Factory Pattern (MateriaSource)
   - Prototype Pattern (clone())

### Comandos Útiles

#### Compilación con warnings extras
```bash
c++ -Wall -Wextra -Werror -std=c++98 -Wshadow -Wconversion *.cpp
```

#### Valgrind completo
```bash
valgrind --leak-check=full \
         --show-leak-kinds=all \
         --track-origins=yes \
         --verbose \
         ./program
```

#### Leaks en macOS
```bash
leaks --atExit -- ./program
```

### Debugging Tips

1. **Imprimir direcciones para verificar deep copy:**
   ```cpp
   std::cout << "Address: " << (void*)this->brain << std::endl;
   ```

2. **Verificar orden de destructores:**
   ```cpp
   ~ClassName() {
       std::cout << "Destroying: " << this << std::endl;
   }
   ```

3. **Debugger (gdb/lldb):**
   ```bash
   c++ -g -Wall -Wextra -Werror *.cpp
   gdb ./a.out
   # o
   lldb ./a.out
   ```

### Errores de Compilación Comunes

1. **Circular dependencies:**
   - Solución: Forward declarations

2. **Virtual function not defined:**
   - Solución: Implementar todas las virtuales puras en derivadas

3. **Cannot instantiate abstract class:**
   - Esperado si Animal es abstracta en ex02/ex03

### Preguntas de Evaluación

Prepárate para responder:
- ¿Qué es polimorfismo?
- ¿Para qué sirve `virtual`?
- ¿Por qué el destructor debe ser virtual?
- ¿Qué es una clase abstracta?
- ¿Diferencia entre deep y shallow copy?
- ¿Cuándo se llama copy constructor vs assignment operator?
- ¿Qué es una vtable?
- ¿Por qué unequip() no hace delete?

---

## 🎓 Consejos Finales

1. **Implementa incrementalmente:** No intentes hacer todo de golpe. Haz ex00, comprueba que funciona, luego pasa a ex01.

2. **Valgrind es tu amigo:** Ejecuta valgrind constantemente. Un memory leak pequeño en ex01 será un nightmare en ex03.

3. **Tests, tests, tests:** Los tests del subject son mínimos. Crea tests que rompan tu código.

4. **Comentarios en el código:** Documenta decisiones importantes, especialmente en gestión de memoria.

5. **No uses auto, nullptr, unique_ptr:** Son de C++11. Recuerda: estás en C++98.

6. **Revisa dos veces:**
   - ✅ Self-assignment checks
   - ✅ NULL checks antes de delete
   - ✅ Destructores virtuales
   - ✅ Forward declarations

7. **Ex03 es opcional pero recomendable:** Es el ejercicio más complejo pero también el más instructivo.

---

## 🚀 Orden de Implementación Recomendado

### Ex00
1. Animal (base abstracta o no)
2. Dog
3. Cat
4. WrongAnimal
5. WrongCat
6. main con tests
7. Makefile

### Ex01
1. Copiar ex00
2. Brain
3. Modificar Dog (añadir Brain*)
4. Modificar Cat (añadir Brain*)
5. main con tests de array y deep copy
6. Valgrind testing

### Ex02
1. Copiar ex01
2. Hacer Animal abstracta (makeSound() = 0)
3. Ajustar tests (eliminar instanciación directa de Animal)
4. Verificar que todo lo demás funciona

### Ex03
1. Empezar desde cero (o base mínima)
2. AMateria (abstracta)
3. Ice y Cure
4. ICharacter (interface)
5. Character (inventario básico)
6. IMateriaSource (interface)
7. MateriaSource
8. main del subject
9. Tests adicionales
10. Gestión de memoria de unequip

---

**¡Éxito con tu CPP04!** 🎉

Recuerda: La clave está en entender los conceptos, no solo en hacer que compile. Tómate tu tiempo para comprender por qué cada cosa funciona como funciona.

**Happy coding!** 💻
