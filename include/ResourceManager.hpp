#ifndef RESOURCE_MANAGER_HPP
#define RESOURCE_MANAGER_HPP

#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>

/**
 * ============================================================================
 * ITEM 13: Use objects to manage resources (RAII - Resource Acquisition Is Initialization)
 * ============================================================================
 * 
 * Principiul RAII: Resursele sunt achizitionate in constructor si eliberate in destructor.
 * Acest lucru garanteaza ca resursele sunt eliberate chiar daca apar exceptii.
 * 
 * Exemple de resurse: memorie, file handles, mutex locks, conexiuni de retea, etc.
 */

// ============================================================================
// Exemplu 1: FileHandle - RAII pentru file descriptors
// ============================================================================
class FileHandle {
private:
    std::fstream file;
    std::string filename;
    bool isOpen;

public:
    // Constructor - achizitioneaza resursa (deschide fisierul)
    explicit FileHandle(const std::string& fname, std::ios::openmode mode = std::ios::in | std::ios::out) 
        : filename(fname), isOpen(false) {
        std::cout << "[FileHandle] Deschidere fisier: " << fname << std::endl;
        file.open(fname, mode);
        if (file.is_open()) {
            isOpen = true;
            std::cout << "[FileHandle] Fisier deschis cu succes!" << std::endl;
        } else {
            std::cout << "[FileHandle] EROARE: Nu s-a putut deschide fisierul!" << std::endl;
        }
    }
    
    // Destructor - elibereaza resursa (inchide fisierul)
    ~FileHandle() {
        if (isOpen && file.is_open()) {
            file.close();
            std::cout << "[FileHandle] Fisier inchis automat: " << filename << std::endl;
        }
    }
    
    // Item 14: Disable copying (fisierul nu poate fi copiat)
    FileHandle(const FileHandle&) = delete;
    FileHandle& operator=(const FileHandle&) = delete;
    
    // Permite move semantics (C++11)
    FileHandle(FileHandle&& other) noexcept 
        : file(std::move(other.file)), filename(std::move(other.filename)), isOpen(other.isOpen) {
        other.isOpen = false;
    }
    
    FileHandle& operator=(FileHandle&& other) noexcept {
        if (this != &other) {
            if (isOpen && file.is_open()) {
                file.close();
            }
            file = std::move(other.file);
            filename = std::move(other.filename);
            isOpen = other.isOpen;
            other.isOpen = false;
        }
        return *this;
    }
    
    // Metode pentru operatii cu fisierul
    void write(const std::string& data) {
        if (isOpen && file.is_open()) {
            file << data;
            std::cout << "[FileHandle] Scris: " << data << std::endl;
        }
    }
    
    std::string readLine() {
        std::string line;
        if (isOpen && file.is_open() && std::getline(file, line)) {
            return line;
        }
        return "";
    }
    
    std::string readAll() {
        std::string content;
        std::string line;
        if (isOpen && file.is_open()) {
            file.seekg(0);  // Reset to beginning
            while (std::getline(file, line)) {
                content += line + "\n";
            }
        }
        return content;
    }
    
    bool good() const { return isOpen && file.is_open(); }
    
    std::fstream& getStream() { return file; }
};

// ============================================================================
// Exemplu 2: MemoryBlock - RAII pentru memorie alocata dinamic
// ============================================================================
class MemoryBlock {
private:
    int* data;
    size_t size;

public:
    // Constructor - aloca memorie
    explicit MemoryBlock(size_t sz) : size(sz) {
        std::cout << "[MemoryBlock] Alocare " << sz << " integers" << std::endl;
        data = new int[sz];
        for (size_t i = 0; i < sz; ++i) {
            data[i] = 0;
        }
    }
    
    // Destructor - elibereaza memorie
    ~MemoryBlock() {
        std::cout << "[MemoryBlock] Eliberare memorie (" << size << " integers)" << std::endl;
        delete[] data;
    }
    
    // Item 14: Deep copy (comportament de copiere pentru resurse)
    MemoryBlock(const MemoryBlock& other) : size(other.size) {
        std::cout << "[MemoryBlock] Copy constructor - deep copy" << std::endl;
        data = new int[size];
        for (size_t i = 0; i < size; ++i) {
            data[i] = other.data[i];
        }
    }
    
    MemoryBlock& operator=(const MemoryBlock& rhs) {
        std::cout << "[MemoryBlock] Copy assignment - deep copy" << std::endl;
        if (this != &rhs) {
            delete[] data;
            size = rhs.size;
            data = new int[size];
            for (size_t i = 0; i < size; ++i) {
                data[i] = rhs.data[i];
            }
        }
        return *this;
    }
    
    // Move semantics
    MemoryBlock(MemoryBlock&& other) noexcept : data(other.data), size(other.size) {
        std::cout << "[MemoryBlock] Move constructor" << std::endl;
        other.data = nullptr;
        other.size = 0;
    }
    
    MemoryBlock& operator=(MemoryBlock&& other) noexcept {
        std::cout << "[MemoryBlock] Move assignment" << std::endl;
        if (this != &other) {
            delete[] data;
            data = other.data;
            size = other.size;
            other.data = nullptr;
            other.size = 0;
        }
        return *this;
    }
    
    int& operator[](size_t index) {
        if (index >= size) throw std::out_of_range("Index out of bounds");
        return data[index];
    }
    
    size_t getSize() const { return size; }
};

// ============================================================================
// Exemplu 3: DatabaseConnection - RAII pentru conexiuni
// ============================================================================
class DatabaseConnection {
private:
    std::string connectionString;
    bool connected;
    int connectionId;
    static int nextId;

public:
    explicit DatabaseConnection(const std::string& connStr) 
        : connectionString(connStr), connected(false), connectionId(nextId++) {
        std::cout << "[DB " << connectionId << "] Conectare la: " << connStr << std::endl;
        // Simulam conectarea
        connected = true;
        std::cout << "[DB " << connectionId << "] Conectat cu succes!" << std::endl;
    }
    
    ~DatabaseConnection() {
        if (connected) {
            std::cout << "[DB " << connectionId << "] Deconectare automata" << std::endl;
            connected = false;
        }
    }
    
    // Item 14: Disable copying (conexiunea nu poate fi copiata)
    DatabaseConnection(const DatabaseConnection&) = delete;
    DatabaseConnection& operator=(const DatabaseConnection&) = delete;
    
    // Move semantics
    DatabaseConnection(DatabaseConnection&& other) noexcept 
        : connectionString(std::move(other.connectionString)), 
          connected(other.connected), connectionId(other.connectionId) {
        other.connected = false;
    }
    
    void executeQuery(const std::string& query) {
        if (connected) {
            std::cout << "[DB " << connectionId << "] Executare: " << query << std::endl;
        }
    }
    
    bool isConnected() const { return connected; }
};

// Initializare static member
inline int DatabaseConnection::nextId = 1;

// ============================================================================
// Functii demonstrative pentru Item 13
// ============================================================================
inline void demonstrateRAII_FileHandle() {
    std::cout << "\n--- RAII cu FileHandle ---\n" << std::endl;
    
    {
        std::cout << "Intram in scope..." << std::endl;
        FileHandle file("test_raii.txt", std::ios::out | std::ios::trunc);
        
        if (file.good()) {
            file.write("Linia 1: RAII in actiune!\n");
            file.write("Linia 2: Fisierul se va inchide automat.\n");
        }
        
        std::cout << "Iesim din scope..." << std::endl;
    } // FileHandle destructor apelat automat aici!
    
    std::cout << "\nFisierul a fost inchis automat la iesirea din scope!\n" << std::endl;
}

inline void demonstrateRAII_MemoryBlock() {
    std::cout << "\n--- RAII cu MemoryBlock ---\n" << std::endl;
    
    {
        std::cout << "Intram in scope..." << std::endl;
        MemoryBlock block(5);
        
        for (size_t i = 0; i < block.getSize(); ++i) {
            block[i] = static_cast<int>(i * 10);
        }
        
        std::cout << "Valori: ";
        for (size_t i = 0; i < block.getSize(); ++i) {
            std::cout << block[i] << " ";
        }
        std::cout << std::endl;
        
        std::cout << "Iesim din scope..." << std::endl;
    } // MemoryBlock destructor apelat automat!
    
    std::cout << "\nMemoria a fost eliberata automat!\n" << std::endl;
}

inline void demonstrateRAII_ExceptionSafety() {
    std::cout << "\n--- RAII: Exception Safety ---\n" << std::endl;
    
    try {
        std::cout << "Intram in try block..." << std::endl;
        MemoryBlock block(3);
        DatabaseConnection db("localhost:5432/mydb");
        
        block[0] = 100;
        db.executeQuery("SELECT * FROM users");
        
        std::cout << "Simulam o exceptie..." << std::endl;
        throw std::runtime_error("Eroare simulata!");
        
        // Acest cod nu se executa
        block[1] = 200;
        
    } catch (const std::exception& e) {
        std::cout << "\nExceptie prinsa: " << e.what() << std::endl;
        std::cout << "DAR resursele au fost eliberate automat de destructori!\n" << std::endl;
    }
}

inline void demonstrateItem13() {
    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "  ITEM 13: Use objects to manage resources (RAII)\n";
    std::cout << "============================================================\n";
    
    std::cout << "\nPrincipiul RAII:" << std::endl;
    std::cout << "- Resursa este achizitionata in CONSTRUCTOR" << std::endl;
    std::cout << "- Resursa este eliberata in DESTRUCTOR" << std::endl;
    std::cout << "- Garanteaza eliberarea chiar si la exceptii!\n" << std::endl;
    
    demonstrateRAII_FileHandle();
    demonstrateRAII_MemoryBlock();
    demonstrateRAII_ExceptionSafety();
    
    std::cout << "\n--- Avantajele RAII ---" << std::endl;
    std::cout << "1. Nu poti uita sa eliberezi resursa" << std::endl;
    std::cout << "2. Exception-safe: resursa e eliberata si la exceptii" << std::endl;
    std::cout << "3. Cod mai curat, fara cleanup manual" << std::endl;
    std::cout << "4. Ownership clar al resurselor\n" << std::endl;
}

#endif // RESOURCE_MANAGER_HPP
