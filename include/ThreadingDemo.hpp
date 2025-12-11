#ifndef THREADING_DEMO_HPP
#define THREADING_DEMO_HPP

#include <iostream>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <vector>
#include <chrono>
#include <sstream>
#include <atomic>

/**
 * ============================================================================
 * THREADING DEMO: Sincronizare cu Mutex vs Fara Sincronizare
 * ============================================================================
 * 
 * Demonstreaza:
 * 1. Probleme care apar FARA sincronizare (race conditions)
 * 2. Solutia cu MUTEX pentru sincronizare corecta
 * 3. RAII pentru mutex (std::lock_guard, std::unique_lock)
 * 4. Exemplu cu file descriptor - un thread scrie, altul citeste
 */

// ============================================================================
// Clasa pentru managementul fisierului cu RAII
// ============================================================================
class ThreadSafeFile {
private:
    std::fstream file;
    std::string filename;
    mutable std::mutex fileMutex;  // Mutex pentru sincronizare
    bool isOpen;

public:
    explicit ThreadSafeFile(const std::string& fname) 
        : filename(fname), isOpen(false) {
        // Cream fisierul daca nu exista
        file.open(fname, std::ios::out | std::ios::trunc);
        if (file.is_open()) {
            file.close();
        }
        // Redeschidem pentru read/write
        file.open(fname, std::ios::in | std::ios::out);
        isOpen = file.is_open();
        std::cout << "[ThreadSafeFile] Fisier deschis: " << fname << std::endl;
    }
    
    ~ThreadSafeFile() {
        if (isOpen) {
            file.close();
            std::cout << "[ThreadSafeFile] Fisier inchis: " << filename << std::endl;
        }
    }
    
    // Disable copying
    ThreadSafeFile(const ThreadSafeFile&) = delete;
    ThreadSafeFile& operator=(const ThreadSafeFile&) = delete;
    
    // Scriere SINCRONIZATA cu mutex
    void writeSync(const std::string& data, int threadId) {
        std::lock_guard<std::mutex> lock(fileMutex);  // RAII lock
        
        if (isOpen && file.is_open()) {
            file.seekp(0, std::ios::end);
            file << "[Thread " << threadId << "] " << data << "\n";
            file.flush();
            std::cout << "[SYNC Write] Thread " << threadId << ": " << data << std::endl;
        }
    }
    
    // Citire SINCRONIZATA cu mutex
    std::string readSync(int threadId) {
        std::lock_guard<std::mutex> lock(fileMutex);  // RAII lock
        
        std::string content;
        if (isOpen && file.is_open()) {
            file.seekg(0);
            std::string line;
            while (std::getline(file, line)) {
                content += line + "\n";
            }
            file.clear();  // Clear EOF flag
            std::cout << "[SYNC Read] Thread " << threadId << " a citit " 
                      << content.length() << " caractere" << std::endl;
        }
        return content;
    }
    
    // Scriere NESINCRONIZATA (pentru demonstratie probleme)
    void writeUnsafe(const std::string& data, int threadId) {
        // FARA LOCK - poate cauza race conditions!
        if (isOpen && file.is_open()) {
            file.seekp(0, std::ios::end);
            file << "[Thread " << threadId << "] " << data << "\n";
            file.flush();
            std::cout << "[UNSAFE Write] Thread " << threadId << ": " << data << std::endl;
        }
    }
    
    // Citire NESINCRONIZATA
    std::string readUnsafe(int threadId) {
        // FARA LOCK - poate cauza race conditions!
        std::string content;
        if (isOpen && file.is_open()) {
            file.seekg(0);
            std::string line;
            while (std::getline(file, line)) {
                content += line + "\n";
            }
            file.clear();
            std::cout << "[UNSAFE Read] Thread " << threadId << " a citit " 
                      << content.length() << " caractere" << std::endl;
        }
        return content;
    }
    
    std::mutex& getMutex() { return fileMutex; }
};

// ============================================================================
// Exemplu simplu: Counter cu si fara sincronizare
// ============================================================================
class Counter {
private:
    int unsafeValue;
    std::atomic<int> atomicValue;
    int mutexValue;
    std::mutex counterMutex;

public:
    Counter() : unsafeValue(0), atomicValue(0), mutexValue(0) {}
    
    // Incrementare NESIGURA (race condition)
    void incrementUnsafe() {
        int temp = unsafeValue;
        // Simulam o operatie care ia timp
        std::this_thread::sleep_for(std::chrono::microseconds(1));
        unsafeValue = temp + 1;
    }
    
    // Incrementare cu ATOMIC (lock-free)
    void incrementAtomic() {
        atomicValue++;
    }
    
    // Incrementare cu MUTEX
    void incrementMutex() {
        std::lock_guard<std::mutex> lock(counterMutex);
        mutexValue++;
    }
    
    int getUnsafe() const { return unsafeValue; }
    int getAtomic() const { return atomicValue; }
    int getMutex() const { return mutexValue; }
};

// ============================================================================
// Demonstratie: FARA sincronizare (probleme)
// ============================================================================
inline void demonstrateWithoutSync() {
    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "  EXEMPLU FARA SINCRONIZARE (Race Conditions)\n";
    std::cout << "============================================================\n";
    
    std::cout << "\n--- Counter fara sincronizare ---\n" << std::endl;
    
    Counter counter;
    const int numIncrements = 1000;
    
    auto incrementTask = [&counter, numIncrements]() {
        for (int i = 0; i < numIncrements; ++i) {
            counter.incrementUnsafe();
        }
    };
    
    std::cout << "Pornim 2 thread-uri, fiecare incrementeaza de " << numIncrements << " ori" << std::endl;
    std::cout << "Valoare asteptata: " << (2 * numIncrements) << std::endl;
    
    std::thread t1(incrementTask);
    std::thread t2(incrementTask);
    
    t1.join();
    t2.join();
    
    std::cout << "\nValoare obtinuta (UNSAFE): " << counter.getUnsafe() << std::endl;
    std::cout << "PROBLEMA: Valoarea este probabil MAI MICA decat " << (2 * numIncrements) << "!" << std::endl;
    std::cout << "Cauza: Race condition - thread-urile suprascriu valorile celuilalt\n" << std::endl;
    
    // Demonstratie cu fisier
    std::cout << "\n--- Scriere in fisier fara sincronizare ---\n" << std::endl;
    
    {
        ThreadSafeFile file("unsafe_demo.txt");
        
        auto writerTask = [&file](int threadId) {
            for (int i = 0; i < 3; ++i) {
                std::stringstream ss;
                ss << "Mesaj " << i << " de la thread " << threadId;
                file.writeUnsafe(ss.str(), threadId);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        };
        
        auto readerTask = [&file](int threadId) {
            for (int i = 0; i < 2; ++i) {
                std::this_thread::sleep_for(std::chrono::milliseconds(15));
                file.readUnsafe(threadId);
            }
        };
        
        std::thread writer1(writerTask, 1);
        std::thread writer2(writerTask, 2);
        std::thread reader(readerTask, 3);
        
        writer1.join();
        writer2.join();
        reader.join();
        
        std::cout << "\nPROBLEME POSIBILE:" << std::endl;
        std::cout << "- Date corupte sau incomplete" << std::endl;
        std::cout << "- Mesaje intercalate incorect" << std::endl;
        std::cout << "- Citiri inconsistente" << std::endl;
    }
}

// ============================================================================
// Demonstratie: CU sincronizare (corect)
// ============================================================================
inline void demonstrateWithSync() {
    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "  EXEMPLU CU SINCRONIZARE (Mutex)\n";
    std::cout << "============================================================\n";
    
    std::cout << "\n--- Counter cu sincronizare ---\n" << std::endl;
    
    Counter counter;
    const int numIncrements = 1000;
    
    // Test cu atomic
    auto atomicTask = [&counter, numIncrements]() {
        for (int i = 0; i < numIncrements; ++i) {
            counter.incrementAtomic();
        }
    };
    
    // Test cu mutex
    auto mutexTask = [&counter, numIncrements]() {
        for (int i = 0; i < numIncrements; ++i) {
            counter.incrementMutex();
        }
    };
    
    std::cout << "Test cu std::atomic:" << std::endl;
    std::thread t1(atomicTask);
    std::thread t2(atomicTask);
    t1.join();
    t2.join();
    std::cout << "Valoare ATOMIC: " << counter.getAtomic() << " (asteptat: " << (2 * numIncrements) << ")" << std::endl;
    
    std::cout << "\nTest cu std::mutex:" << std::endl;
    std::thread t3(mutexTask);
    std::thread t4(mutexTask);
    t3.join();
    t4.join();
    std::cout << "Valoare MUTEX: " << counter.getMutex() << " (asteptat: " << (2 * numIncrements) << ")" << std::endl;
    
    std::cout << "\nAmbele metode dau rezultatul CORECT!\n" << std::endl;
    
    // Demonstratie cu fisier sincronizat
    std::cout << "\n--- Scriere/Citire in fisier CU sincronizare ---\n" << std::endl;
    
    {
        ThreadSafeFile file("sync_demo.txt");
        
        auto writerTask = [&file](int threadId) {
            for (int i = 0; i < 3; ++i) {
                std::stringstream ss;
                ss << "Mesaj " << i << " de la thread " << threadId;
                file.writeSync(ss.str(), threadId);
                std::this_thread::sleep_for(std::chrono::milliseconds(10));
            }
        };
        
        auto readerTask = [&file](int threadId) {
            for (int i = 0; i < 2; ++i) {
                std::this_thread::sleep_for(std::chrono::milliseconds(25));
                std::string content = file.readSync(threadId);
            }
        };
        
        std::thread writer1(writerTask, 1);
        std::thread writer2(writerTask, 2);
        std::thread reader(readerTask, 3);
        
        writer1.join();
        writer2.join();
        reader.join();
        
        std::cout << "\nCu MUTEX:" << std::endl;
        std::cout << "- Fiecare operatie este atomica" << std::endl;
        std::cout << "- Nu exista date corupte" << std::endl;
        std::cout << "- Citirile sunt consistente" << std::endl;
    }
}

// ============================================================================
// Demonstratie: RAII pentru Mutex (lock_guard, unique_lock)
// ============================================================================
inline void demonstrateMutexRAII() {
    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "  RAII pentru Mutex: lock_guard si unique_lock\n";
    std::cout << "============================================================\n";
    
    std::mutex mtx;
    int sharedData = 0;
    
    std::cout << "\n--- std::lock_guard (simplu, RAII) ---\n" << std::endl;
    {
        std::cout << "Intram in scope si luam lock-ul..." << std::endl;
        std::lock_guard<std::mutex> lock(mtx);
        
        sharedData = 42;
        std::cout << "Am modificat sharedData = " << sharedData << std::endl;
        std::cout << "Lock-ul se elibereaza automat la iesirea din scope" << std::endl;
    }
    std::cout << "Lock eliberat!\n" << std::endl;
    
    std::cout << "\n--- std::unique_lock (flexibil) ---\n" << std::endl;
    {
        std::unique_lock<std::mutex> lock(mtx);
        std::cout << "Lock luat" << std::endl;
        
        sharedData = 100;
        
        lock.unlock();
        std::cout << "Lock eliberat manual (pentru operatii care nu necesita lock)" << std::endl;
        
        // Operatii fara lock...
        std::cout << "Facem operatii fara lock..." << std::endl;
        
        lock.lock();
        std::cout << "Lock reluat" << std::endl;
        
        sharedData = 200;
        std::cout << "sharedData = " << sharedData << std::endl;
    }
    
    std::cout << "\n--- Avantaje RAII pentru mutex ---" << std::endl;
    std::cout << "1. Lock-ul se elibereaza automat (chiar si la exceptii)" << std::endl;
    std::cout << "2. Nu poti uita sa faci unlock" << std::endl;
    std::cout << "3. Exception-safe" << std::endl;
    std::cout << "4. Cod mai curat si mai sigur" << std::endl;
}

// ============================================================================
// Exemplu complet: Producer-Consumer cu fisier
// ============================================================================
inline void demonstrateProducerConsumer() {
    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "  Producer-Consumer: Un thread scrie, altul citeste\n";
    std::cout << "============================================================\n";
    
    std::mutex fileMutex;
    std::string sharedFilename = "producer_consumer.txt";
    std::atomic<bool> done(false);
    std::atomic<int> messagesWritten(0);
    
    // Cream fisierul
    {
        std::ofstream file(sharedFilename, std::ios::trunc);
        file << "=== Log Start ===\n";
    }
    
    // Producer thread - scrie in fisier
    auto producer = [&]() {
        std::cout << "[Producer] Pornit" << std::endl;
        
        for (int i = 1; i <= 5; ++i) {
            {
                std::lock_guard<std::mutex> lock(fileMutex);
                
                std::ofstream file(sharedFilename, std::ios::app);
                if (file.is_open()) {
                    file << "Mesaj #" << i << " - timestamp: " 
                         << std::chrono::system_clock::now().time_since_epoch().count() << "\n";
                    file.flush();
                    std::cout << "[Producer] Scris mesaj #" << i << std::endl;
                    messagesWritten++;
                }
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
        
        done = true;
        std::cout << "[Producer] Terminat" << std::endl;
    };
    
    // Consumer thread - citeste din fisier
    auto consumer = [&]() {
        std::cout << "[Consumer] Pornit" << std::endl;
        int lastCount = 0;
        
        while (!done || messagesWritten > lastCount) {
            {
                std::lock_guard<std::mutex> lock(fileMutex);
                
                std::ifstream file(sharedFilename);
                if (file.is_open()) {
                    std::string content((std::istreambuf_iterator<char>(file)),
                                        std::istreambuf_iterator<char>());
                    
                    int currentCount = messagesWritten;
                    if (currentCount > lastCount) {
                        std::cout << "[Consumer] Citit " << content.length() 
                                  << " bytes, " << currentCount << " mesaje" << std::endl;
                        lastCount = currentCount;
                    }
                }
            }
            
            std::this_thread::sleep_for(std::chrono::milliseconds(150));
        }
        
        // Citire finala
        {
            std::lock_guard<std::mutex> lock(fileMutex);
            std::ifstream file(sharedFilename);
            std::string content((std::istreambuf_iterator<char>(file)),
                                std::istreambuf_iterator<char>());
            std::cout << "\n[Consumer] Continut final:\n" << content << std::endl;
        }
        
        std::cout << "[Consumer] Terminat" << std::endl;
    };
    
    std::thread producerThread(producer);
    std::thread consumerThread(consumer);
    
    producerThread.join();
    consumerThread.join();
    
    std::cout << "\nProducer-Consumer finalizat cu succes!" << std::endl;
    std::cout << "Mutex-ul a asigurat ca operatiile sunt atomice." << std::endl;
}

// ============================================================================
// Functie principala de demonstratie
// ============================================================================
inline void demonstrateThreading() {
    std::cout << "\n";
    std::cout << "************************************************************\n";
    std::cout << "*                                                          *\n";
    std::cout << "*     THREADING: Sincronizare cu Mutex                    *\n";
    std::cout << "*     (Ilustrare din poza: threaduri sincronizate         *\n";
    std::cout << "*      si desincronizate care manageuiesc aceleasi        *\n";
    std::cout << "*      resurse)                                           *\n";
    std::cout << "*                                                          *\n";
    std::cout << "************************************************************\n";
    
    std::cout << "\nVom demonstra:" << std::endl;
    std::cout << "1. Ce se intampla FARA sincronizare (race conditions)" << std::endl;
    std::cout << "2. Cum rezolvam cu MUTEX (sincronizare corecta)" << std::endl;
    std::cout << "3. RAII pentru mutex (lock_guard, unique_lock)" << std::endl;
    std::cout << "4. Producer-Consumer: un thread scrie, altul citeste\n" << std::endl;
    
    demonstrateWithoutSync();
    demonstrateWithSync();
    demonstrateMutexRAII();
    demonstrateProducerConsumer();
    
    std::cout << "\n";
    std::cout << "============================================================\n";
    std::cout << "  REZUMAT THREADING\n";
    std::cout << "============================================================\n";
    std::cout << "\nProbleme FARA sincronizare:" << std::endl;
    std::cout << "- Race conditions: rezultate imprevizibile" << std::endl;
    std::cout << "- Date corupte" << std::endl;
    std::cout << "- Comportament nedeterminist" << std::endl;
    
    std::cout << "\nSolutii pentru sincronizare:" << std::endl;
    std::cout << "- std::mutex + std::lock_guard (RAII)" << std::endl;
    std::cout << "- std::atomic pentru operatii simple" << std::endl;
    std::cout << "- std::unique_lock pentru control mai fin" << std::endl;
    
    std::cout << "\nBest practices:" << std::endl;
    std::cout << "- Foloseste RAII pentru lock-uri (lock_guard)" << std::endl;
    std::cout << "- Minimizeaza timpul cat tii lock-ul" << std::endl;
    std::cout << "- Evita deadlock-uri (ordinea lock-urilor)" << std::endl;
    std::cout << "- Prefera std::atomic pentru operatii simple" << std::endl;
}

#endif // THREADING_DEMO_HPP
