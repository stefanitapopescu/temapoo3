# Effective C++ Demo - Items 10, 11, 12, 13, 14 + Threading

Acest proiect demonstrează principii fundamentale din cartea "Effective C++" de Scott Meyers, plus exemple de multithreading cu sincronizare.

## Principiile Demonstrate

### Item 10: Have assignment operators return a reference to *this

**Descriere**: Operatorii de assignment ar trebui să returneze o referință la `*this` pentru a permite chaining-ul de assignment-uri.

**Exemplu**:
```cpp
Person& Person::operator=(const Person& rhs) {
    // ... assignment logic ...
    return *this;  // Permite: a = b = c;
}
```

**Beneficii**:
- Permite chaining: `a = b = c;`
- Urmează convențiile tipurilor built-in
- Comportament consistent și așteptat

### Item 11: Handle assignment to self in operator=

**Descriere**: Operatorul de assignment trebuie să verifice și să gestioneze cazul în care un obiect este asignat lui însuși.

**Exemplu**:
```cpp
Person& Person::operator=(const Person& rhs) {
    if (this == &rhs) {  // Verificare self-assignment
        return *this;
    }
    // ... rest of assignment logic ...
    return *this;
}
```

**Probleme fără verificare**:
- Ștergerea resurselor care sunt apoi accesate
- Undefined behavior
- Potențiale crash-uri

### Item 12: Copy all parts of an object

**Descriere**: Copy constructor-ul și operatorul de assignment trebuie să copieze TOATE membrii obiectului, inclusiv partea din clasa de bază în cazul claselor derivate.

**Exemple**:

**Clasa simplă**:
```cpp
Person::Person(const Person& other)
    : name(other.name),           // Copiază TOȚI membrii
      age(other.age),
      address(new std::string(*other.address)) {  // Deep copy
}
```

**Clasă derivată (IMPORTANT)**:
```cpp
Employee::Employee(const Employee& other)
    : Person(other),              // ESENȚIAL: Copiază partea de bază!
      employeeId(other.employeeId),
      salary(other.salary),
      department(new std::string(*other.department)) {
}
```

### Item 13: Use objects to manage resources (RAII)

**Descriere**: Resursele (memorie, file handles, conexiuni) trebuie gestionate prin obiecte care le achiziționează în constructor și le eliberează în destructor.

**Exemplu**:
```cpp
class FileHandle {
    std::fstream file;
public:
    FileHandle(const std::string& fname) {
        file.open(fname);  // Achiziție în constructor
    }
    ~FileHandle() {
        file.close();      // Eliberare în destructor
    }
};
```

**Beneficii**:
- Nu poți uita să eliberezi resursa
- Exception-safe: resursa e eliberată chiar și la excepții
- Cod mai curat, fără cleanup manual

### Item 14: Think carefully about copying behavior in resource-managing classes

**Descriere**: Când o clasă gestionează resurse, trebuie să decizi cum se comportă la copiere.

**Opțiuni**:
1. **Prohibit copying** (ca `unique_ptr`):
```cpp
FileHandle(const FileHandle&) = delete;
FileHandle& operator=(const FileHandle&) = delete;
```

2. **Reference counting** (ca `shared_ptr`):
```cpp
std::shared_ptr<Resource> ptr1 = std::make_shared<Resource>();
std::shared_ptr<Resource> ptr2 = ptr1;  // Ambele arată la aceeași resursă
```

3. **Deep copy**: Fiecare obiect are propria copie a resursei

4. **Transfer ownership** (move semantics):
```cpp
std::unique_ptr<Resource> ptr2 = std::move(ptr1);  // ptr1 devine null
```

### Smart Pointers (unique_ptr, shared_ptr)

**unique_ptr** - Ownership exclusiv:
```cpp
std::unique_ptr<Resource> ptr = std::make_unique<Resource>();
// ptr nu poate fi copiat, doar mutat
std::unique_ptr<Resource> ptr2 = std::move(ptr);
```

**shared_ptr** - Ownership partajat:
```cpp
std::shared_ptr<Resource> ptr1 = std::make_shared<Resource>();
std::shared_ptr<Resource> ptr2 = ptr1;  // use_count = 2
// Resursa e distrusă când ultimul shared_ptr iese din scope
```

### Threading cu Mutex (Sincronizare)

**Problema FĂRĂ sincronizare** (race conditions):
```cpp
// Thread 1 și Thread 2 accesează counter simultan
counter++;  // Race condition! Rezultat imprevizibil
```

**Soluția cu MUTEX**:
```cpp
std::mutex mtx;
void increment() {
    std::lock_guard<std::mutex> lock(mtx);  // RAII lock
    counter++;  // Acces exclusiv, sigur
}  // Lock eliberat automat
```

**Producer-Consumer cu file descriptor**:
```cpp
std::mutex fileMutex;

// Thread Writer
{
    std::lock_guard<std::mutex> lock(fileMutex);
    file << "data";
}

// Thread Reader
{
    std::lock_guard<std::mutex> lock(fileMutex);
    std::getline(file, line);
}
```

## Structura Proiectului

```
BunTema2/
├── CMakeLists.txt              # Configurare CMake (C++17, threading)
├── README.md                   # Această documentație
├── build.bat                   # Script build pentru Windows
├── include/                    # Header files (.hpp)
│   ├── Person.hpp              # Item 10, 11, 12
│   ├── Employee.hpp            # Item 12 pentru ierarhii
│   ├── Widget.hpp              # Item 11 explicit
│   ├── ResourceManager.hpp     # Item 13: RAII (FileHandle, MemoryBlock, etc.)
│   ├── SmartPointerDemo.hpp    # Item 14: unique_ptr, shared_ptr, weak_ptr
│   └── ThreadingDemo.hpp       # Threading: mutex, sincronizare, producer-consumer
└── src/                        # Implementation files (.cpp)
    ├── main.cpp                # Program principal cu meniu interactiv
    ├── Person.cpp              # Implementare Person
    ├── Employee.cpp            # Implementare Employee
    └── Widget.cpp              # Implementare Widget
```

## Compilare și Rulare

### Windows (PowerShell/CMD)

```bash
# Creare director build
mkdir build
cd build

# Configurare CMake
cmake ..

# Compilare
cmake --build .

# Rulare (Debug)
.\Debug\EffectiveCppDemo.exe

# SAU (Release)
.\Release\EffectiveCppDemo.exe
```

### Linux/MacOS

```bash
# Creare director build
mkdir build
cd build

# Configurare CMake
cmake ..

# Compilare
make

# Rulare
./EffectiveCppDemo
```

### Alternative: Compilare cu g++ (fără CMake)

```bash
g++ -std=c++17 -I./include -pthread src/*.cpp -o EffectiveCppDemo
./EffectiveCppDemo
```

### Windows cu build.bat

```bash
.\build.bat
```

## Demonstrații în Program

Programul include demonstrații detaliate pentru:

1. **Item 10**: Chaining de assignment-uri (`a = b = c`)
2. **Item 11**: Self-assignment și probleme potențiale
3. **Item 12**: Copy constructor pentru clase simple și derivate
4. **Item 13**: RAII cu FileHandle, MemoryBlock, DatabaseConnection
5. **Item 14**: Smart pointers (unique_ptr, shared_ptr, weak_ptr)
6. **Threading**: 
   - Exemplu FĂRĂ sincronizare (race conditions)
   - Exemplu CU mutex (sincronizare corectă)
   - RAII pentru mutex (lock_guard, unique_lock)
   - Producer-Consumer: un thread scrie, altul citește din fișier

## Exemple de Output

Programul va afișa:
- Constructor/destructor calls pentru a urmări crearea/distrugerea obiectelor
- Demonstrații ale chaining-ului de assignment
- Verificări de self-assignment
- Demonstrații ale copy constructor-ului
- Deep copy vs shallow copy

## Concepte C++ Importante

### Deep Copy vs Shallow Copy

- **Shallow Copy**: Copiază valorile pointerilor (ambele obiecte arată la aceeași memorie)
- **Deep Copy**: Creează copii noi ale resurselor (fiecare obiect are propria memorie)

Acest proiect folosește **deep copy** pentru pointeri:

```cpp
// Deep copy
address = new std::string(*other.address);  // Alocă memorie nouă
```

### Copy Constructor vs Assignment Operator

- **Copy Constructor**: Inițializează un obiect NOU cu valorile altui obiect
  ```cpp
  Person p2(p1);  // Copy constructor
  ```

- **Assignment Operator**: Asignează valori unui obiect EXISTENT
  ```cpp
  p2 = p1;  // Assignment operator
  ```

### Exception Safety

Programul include și o versiune exception-safe a assignment operator-ului în clasa `Widget`:

```cpp
Widget& operatorAssignmentSafe(const Widget& rhs);
```

Această versiune folosește tehnica "copy-and-swap" și nu necesită verificare explicită pentru self-assignment.

## Concluzie

Aceste principii sunt fundamentale pentru scrierea de cod C++ robust și corect:

- **Item 10**: Asigură comportament consistent (chaining)
- **Item 11**: Previne bug-uri subtile și crash-uri (self-assignment)
- **Item 12**: Garantează că obiectele sunt copiate complet și corect
- **Item 13**: RAII - gestionare automată a resurselor, exception-safe
- **Item 14**: Comportament de copiere adecvat pentru clase cu resurse

**Threading cu Mutex**:
- Previne race conditions când mai multe thread-uri accesează aceleași resurse
- `std::lock_guard` oferă RAII pentru mutex (eliberare automată)
- Producer-Consumer pattern pentru comunicare între thread-uri

Urmărirea acestor principii este esențială pentru:
- Prevenirea bug-urilor și memory leaks
- Cod mai sigur și mai predictibil
- Exception safety
- Comportament corect în aplicații multi-threaded

## Autor

Proiect demonstrativ pentru Effective C++ Items 10, 11, 12, 13, 14 + Threading


