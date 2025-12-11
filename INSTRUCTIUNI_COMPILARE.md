# Instrucțiuni de Compilare și Rulare

## Structura Proiectului

```
BunTema2/
├── CMakeLists.txt           # Configurare CMake pentru build
├── README.md                # Documentație generală
├── EXPLICATII.md            # Explicații detaliate despre Items 10, 11, 12
├── INSTRUCTIUNI_COMPILARE.md # Acest fișier
├── build.bat                # Script build pentru Windows
├── build.sh                 # Script build pentru Linux/Mac
├── .gitignore              # Git ignore patterns
│
├── include/                 # Header files (.hpp)
│   ├── Person.hpp          # Clasa de bază
│   ├── Employee.hpp        # Clasa derivată
│   └── Widget.hpp          # Demonstrație Item 11
│
└── src/                     # Implementation files (.cpp)
    ├── main.cpp            # Program principal
    ├── Person.cpp          # Implementare Person
    ├── Employee.cpp        # Implementare Employee
    └── Widget.cpp          # Implementare Widget
```

---

## Metode de Compilare

### Metodă 1: Folosind CMake (Recomandat)

#### Windows cu Visual Studio

```powershell
# 1. Creare director build
mkdir build
cd build

# 2. Configurare CMake (generează Visual Studio solution)
cmake ..

# 3. Compilare
cmake --build . --config Release

# 4. Rulare
.\Release\EffectiveCppDemo.exe

# SAU pentru Debug
cmake --build . --config Debug
.\Debug\EffectiveCppDemo.exe
```

#### Windows cu MinGW

```powershell
# 1. Creare director build
mkdir build
cd build

# 2. Configurare CMake cu MinGW
cmake -G "MinGW Makefiles" ..

# 3. Compilare
cmake --build .

# 4. Rulare
.\EffectiveCppDemo.exe
```

#### Linux / macOS

```bash
# 1. Creare director build
mkdir build
cd build

# 2. Configurare CMake
cmake ..

# 3. Compilare
make

# 4. Rulare
./EffectiveCppDemo
```

---

### Metodă 2: Folosind Script-urile Build

#### Windows

```powershell
# Dublu-click pe build.bat SAU
.\build.bat
```

Script-ul va:
- Verifica dacă g++ este instalat
- Compila proiectul
- Rula automat programul dacă compilarea reușește

#### Linux / macOS

```bash
# Fă script-ul executabil (doar prima dată)
chmod +x build.sh

# Rulează script-ul
./build.sh
```

---

### Metodă 3: Compilare Manuală cu g++

#### Toate platformele (dacă ai g++ instalat)

```bash
# Compilare directă
g++ -std=c++11 -I./include -o EffectiveCppDemo src/main.cpp src/Person.cpp src/Employee.cpp src/Widget.cpp

# Rulare
# Windows:
.\EffectiveCppDemo.exe

# Linux/Mac:
./EffectiveCppDemo
```

#### Cu debugging symbols

```bash
g++ -std=c++11 -g -I./include -o EffectiveCppDemo src/main.cpp src/Person.cpp src/Employee.cpp src/Widget.cpp
```

#### Cu optimizări

```bash
g++ -std=c++11 -O2 -I./include -o EffectiveCppDemo src/main.cpp src/Person.cpp src/Employee.cpp src/Widget.cpp
```

#### Cu warnings extensive

```bash
g++ -std=c++11 -Wall -Wextra -Wpedantic -I./include -o EffectiveCppDemo src/main.cpp src/Person.cpp src/Employee.cpp src/Widget.cpp
```

---

### Metodă 4: Visual Studio Code

#### 1. Instalează extensiile necesare:
- C/C++ (Microsoft)
- CMake Tools

#### 2. Deschide folderul în VS Code

#### 3. Configurare CMake:
- Apasă `Ctrl+Shift+P`
- Selectează "CMake: Configure"
- Alege compiler-ul dorit

#### 4. Build:
- Apasă `Ctrl+Shift+P`
- Selectează "CMake: Build"
- SAU apasă F7

#### 5. Run:
- Apasă `Ctrl+Shift+P`
- Selectează "CMake: Run Without Debugging"
- SAU apasă Shift+F5

---

### Metodă 5: Visual Studio (Windows)

#### 1. Generare Visual Studio Solution:

```powershell
mkdir build
cd build
cmake -G "Visual Studio 17 2022" ..
# Pentru Visual Studio 2019: cmake -G "Visual Studio 16 2019" ..
```

#### 2. Deschide `EffectiveCppDemo.sln` în Visual Studio

#### 3. Build:
- Apasă F7 sau
- Build → Build Solution

#### 4. Run:
- Apasă F5 (cu debugging) sau
- Ctrl+F5 (fără debugging)

---

## Cerințe Software

### Minim Necesar

- **Compiler C++**: g++ sau MSVC cu suport C++11
- **CMake** (opțional): versiunea 3.10 sau mai nouă

### Instalare Compilator

#### Windows

**Opțiunea 1: MinGW**
```powershell
# Descarcă de la: https://sourceforge.net/projects/mingw/
# Instalează și adaugă la PATH: C:\MinGW\bin
```

**Opțiunea 2: Visual Studio**
```
# Descarcă Visual Studio Community (gratuit)
# https://visualstudio.microsoft.com/downloads/
# Instalează "Desktop development with C++"
```

#### Linux (Ubuntu/Debian)

```bash
sudo apt update
sudo apt install build-essential cmake
```

#### Linux (Fedora/RHEL)

```bash
sudo dnf install gcc-c++ cmake
```

#### macOS

```bash
# Instalează Xcode Command Line Tools
xcode-select --install

# SAU instalează Homebrew și apoi:
brew install cmake
```

---

## Verificare Instalare

```bash
# Verifică g++
g++ --version

# Verifică CMake
cmake --version

# Verifică compilatorul C++
# Windows (MSVC)
cl

# Linux/Mac
c++ --version
```

---

## Troubleshooting

### Problema: "cmake: command not found"

**Soluție**: 
- Instalează CMake sau
- Folosește compilare manuală cu g++ sau
- Folosește script-urile `build.bat` / `build.sh`

### Problema: "g++: command not found"

**Soluție**:
- Instalează compilator C++ (vezi secțiunea "Instalare Compilator")

### Problema: Erori de linking pe Windows

**Soluție**:
- Asigură-te că folosești același toolchain pentru toate fișierele
- Încearcă să ștergi directorul `build/` și să recompilezi

### Problema: "No CMAKE_CXX_COMPILER could be found"

**Soluție**:
```powershell
# Specifică explicit compilatorul
cmake -DCMAKE_CXX_COMPILER=g++ ..

# SAU pentru Visual Studio
cmake -G "Visual Studio 17 2022" ..
```

### Problema: Erori Unicode/Caractere românești

**Soluție**:
```bash
# Compilează cu suport UTF-8
g++ -std=c++11 -finput-charset=UTF-8 -fexec-charset=UTF-8 -I./include -o EffectiveCppDemo src/*.cpp
```

---

## Output Așteptat

Când rulezi programul, ar trebui să vezi:

```
************************************************************
*                                                          *
*     EFFECTIVE C++ - Items 10, 11, 12                    *
*     Demonstratie Practica                               *
*                                                          *
************************************************************

============================================================
  ITEM 10: Assignment operators return a reference to *this
============================================================

Aceasta permite chaining de assignment-uri:

Person constructor called for: Alice
Person constructor called for: Bob
Person constructor called for: Charlie

Executam: p1 = p2 = p3;
(assignment-ul se evalueaza de la dreapta la stanga)

...

[Demonstrații detaliate pentru fiecare Item]

...

Program finished successfully!
```

---

## Testing

### Test Rapid

```bash
# Compilează
g++ -std=c++11 -I./include -o test src/main.cpp src/Person.cpp src/Employee.cpp src/Widget.cpp

# Rulează și salvează output
./test > output.txt

# Verifică output-ul
cat output.txt
```

### Test cu Valgrind (Linux - memory leaks)

```bash
# Compilează cu debug symbols
g++ -std=c++11 -g -I./include -o test src/main.cpp src/Person.cpp src/Employee.cpp src/Widget.cpp

# Rulează cu Valgrind
valgrind --leak-check=full ./test
```

---

## Curățare Build Files

### Windows (PowerShell)

```powershell
# Șterge directorul build
Remove-Item -Recurse -Force build

# Șterge executable-ul
Remove-Item EffectiveCppDemo.exe -ErrorAction SilentlyContinue
```

### Linux / macOS

```bash
# Șterge directorul build
rm -rf build

# Șterge executable-ul
rm -f EffectiveCppDemo

# Clean all
make clean  # dacă ai folosit make
```

---

## Rebuild Complet

### Windows

```powershell
# Curățare
Remove-Item -Recurse -Force build
mkdir build
cd build

# Rebuild
cmake ..
cmake --build . --config Release
```

### Linux / macOS

```bash
# Curățare și rebuild
rm -rf build
mkdir build
cd build
cmake ..
make
```

---

## Integrare cu IDE-uri

### CLion
1. File → Open → Selectează directorul proiectului
2. CLion va detecta automat CMakeLists.txt
3. Build: Ctrl+F9
4. Run: Shift+F10

### Eclipse CDT
1. File → New → Makefile Project with Existing Code
2. Selectează directorul proiectului
3. Project → Build All
4. Run → Run

### Code::Blocks
1. File → Open → Selectează CMakeLists.txt
2. Code::Blocks va genera workspace
3. Build → Build
4. Build → Run

---

## Resurse Adiționale

- **Documentație Completă**: Vezi `README.md`
- **Explicații Detaliate**: Vezi `EXPLICATII.md`
- **Cod Sursă**: Explorează `include/` și `src/`

---

## Contact și Support

Pentru probleme sau întrebări:
1. Verifică secțiunea "Troubleshooting" mai sus
2. Asigură-te că ai toate cerințele software instalate
3. Verifică că folosești C++11 sau mai nou

---

## Licență

Proiect educațional demonstrativ pentru principiile din "Effective C++" de Scott Meyers.


