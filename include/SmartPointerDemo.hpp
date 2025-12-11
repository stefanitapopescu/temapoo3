#ifndef SMART_POINTER_DEMO_HPP
#define SMART_POINTER_DEMO_HPP

#include <iostream>
#include <memory>
#include <string>
#include <vector>

/**
 * ============================================================================
 * ITEM 13 & 14: Smart Pointers - unique_ptr si shared_ptr
 * ============================================================================
 * 
 * Smart pointers sunt implementari RAII pentru managementul memoriei:
 * - unique_ptr: ownership exclusiv (un singur proprietar)
 * - shared_ptr: ownership partajat (mai multi proprietari)
 * - weak_ptr: referinta non-owning la shared_ptr
 */

// ============================================================================
// Clasa Resource pentru demonstratii
// ============================================================================
class Resource {
private:
    std::string name;
    int* data;
    size_t size;

public:
    explicit Resource(const std::string& n, size_t sz = 10) 
        : name(n), size(sz) {
        std::cout << "[Resource] Creare: " << name << " (size=" << sz << ")" << std::endl;
        data = new int[sz];
        for (size_t i = 0; i < sz; ++i) {
            data[i] = static_cast<int>(i);
        }
    }
    
    ~Resource() {
        std::cout << "[Resource] Distrugere: " << name << std::endl;
        delete[] data;
    }
    
    // Disable copying pentru a forta folosirea smart pointers
    Resource(const Resource&) = delete;
    Resource& operator=(const Resource&) = delete;
    
    void doWork() const {
        std::cout << "[Resource] " << name << " lucreaza..." << std::endl;
    }
    
    std::string getName() const { return name; }
};

// ============================================================================
// UNIQUE_PTR - Ownership Exclusiv
// ============================================================================
namespace UniquePointerDemo {

    // Functie care creeaza si returneaza un unique_ptr (transfer de ownership)
    inline std::unique_ptr<Resource> createResource(const std::string& name) {
        std::cout << "\n[Factory] Creare resursa: " << name << std::endl;
        return std::make_unique<Resource>(name, 5);
    }
    
    // Functie care primeste ownership (prin move)
    inline void takeOwnership(std::unique_ptr<Resource> res) {
        std::cout << "[takeOwnership] Am primit ownership pentru: " << res->getName() << std::endl;
        res->doWork();
        std::cout << "[takeOwnership] La iesire, resursa va fi distrusa" << std::endl;
    }
    
    // Functie care foloseste resursa fara a prelua ownership
    inline void useResource(Resource* res) {
        if (res) {
            std::cout << "[useResource] Folosesc resursa (fara ownership): " << res->getName() << std::endl;
            res->doWork();
        }
    }
    
    inline void demonstrate() {
        std::cout << "\n";
        std::cout << "============================================================\n";
        std::cout << "  UNIQUE_PTR - Ownership Exclusiv\n";
        std::cout << "============================================================\n";
        
        std::cout << "\n--- Exemplu 1: Creare si utilizare de baza ---\n" << std::endl;
        {
            std::unique_ptr<Resource> ptr1 = std::make_unique<Resource>("Resursa_A", 3);
            ptr1->doWork();
            
            std::cout << "\nLa iesirea din scope, resursa e distrusa automat..." << std::endl;
        }
        std::cout << "Resursa a fost distrusa!\n" << std::endl;
        
        std::cout << "\n--- Exemplu 2: Transfer de ownership (move) ---\n" << std::endl;
        {
            std::unique_ptr<Resource> ptr1 = std::make_unique<Resource>("Resursa_B", 2);
            std::cout << "ptr1 detine resursa" << std::endl;
            
            // EROARE: unique_ptr nu poate fi copiat!
            // std::unique_ptr<Resource> ptr2 = ptr1;  // COMPILE ERROR!
            
            // Corect: folosim std::move pentru transfer
            std::unique_ptr<Resource> ptr2 = std::move(ptr1);
            std::cout << "Dupa move: ptr1 este " << (ptr1 ? "valid" : "null") << std::endl;
            std::cout << "Dupa move: ptr2 este " << (ptr2 ? "valid" : "null") << std::endl;
            
            if (ptr2) {
                ptr2->doWork();
            }
        }
        
        std::cout << "\n--- Exemplu 3: Factory pattern cu unique_ptr ---\n" << std::endl;
        {
            auto resource = createResource("Resursa_Factory");
            resource->doWork();
            
            std::cout << "\nTransfer ownership catre functie..." << std::endl;
            takeOwnership(std::move(resource));
            
            std::cout << "Dupa transfer: resource este " << (resource ? "valid" : "null") << std::endl;
        }
        
        std::cout << "\n--- Exemplu 4: Folosire fara transfer de ownership ---\n" << std::endl;
        {
            auto resource = std::make_unique<Resource>("Resursa_Borrowed", 4);
            
            // Pasam raw pointer pentru folosire temporara
            useResource(resource.get());
            
            std::cout << "Resursa inca exista: " << resource->getName() << std::endl;
        }
        
        std::cout << "\n--- Avantaje unique_ptr ---" << std::endl;
        std::cout << "1. Zero overhead fata de raw pointer" << std::endl;
        std::cout << "2. Ownership clar si explicit" << std::endl;
        std::cout << "3. Nu poate fi copiat accidental" << std::endl;
        std::cout << "4. Eliberare automata la iesirea din scope" << std::endl;
    }
}

// ============================================================================
// SHARED_PTR - Ownership Partajat
// ============================================================================
namespace SharedPointerDemo {

    class SharedResource {
    private:
        std::string name;
        
    public:
        explicit SharedResource(const std::string& n) : name(n) {
            std::cout << "[SharedResource] Creare: " << name << std::endl;
        }
        
        ~SharedResource() {
            std::cout << "[SharedResource] Distrugere: " << name << std::endl;
        }
        
        void use() const {
            std::cout << "[SharedResource] Folosire: " << name << std::endl;
        }
        
        std::string getName() const { return name; }
    };
    
    inline void demonstrate() {
        std::cout << "\n";
        std::cout << "============================================================\n";
        std::cout << "  SHARED_PTR - Ownership Partajat\n";
        std::cout << "============================================================\n";
        
        std::cout << "\n--- Exemplu 1: Mai multi proprietari ---\n" << std::endl;
        {
            std::shared_ptr<SharedResource> ptr1 = std::make_shared<SharedResource>("Shared_A");
            std::cout << "ptr1 use_count: " << ptr1.use_count() << std::endl;
            
            {
                std::shared_ptr<SharedResource> ptr2 = ptr1;  // Copiere OK!
                std::cout << "Dupa copiere in ptr2:" << std::endl;
                std::cout << "  ptr1 use_count: " << ptr1.use_count() << std::endl;
                std::cout << "  ptr2 use_count: " << ptr2.use_count() << std::endl;
                
                {
                    std::shared_ptr<SharedResource> ptr3 = ptr1;
                    std::cout << "Dupa copiere in ptr3:" << std::endl;
                    std::cout << "  use_count: " << ptr1.use_count() << std::endl;
                    
                    ptr3->use();
                    std::cout << "ptr3 iese din scope..." << std::endl;
                }
                
                std::cout << "Dupa distrugere ptr3: use_count = " << ptr1.use_count() << std::endl;
                std::cout << "ptr2 iese din scope..." << std::endl;
            }
            
            std::cout << "Dupa distrugere ptr2: use_count = " << ptr1.use_count() << std::endl;
            std::cout << "ptr1 iese din scope..." << std::endl;
        }
        std::cout << "Resursa distrusa cand ultimul shared_ptr a iesit din scope!\n" << std::endl;
        
        std::cout << "\n--- Exemplu 2: shared_ptr in containere ---\n" << std::endl;
        {
            std::vector<std::shared_ptr<SharedResource>> resources;
            
            auto res1 = std::make_shared<SharedResource>("Container_Res1");
            auto res2 = std::make_shared<SharedResource>("Container_Res2");
            
            resources.push_back(res1);
            resources.push_back(res2);
            resources.push_back(res1);  // res1 apare de 2 ori!
            
            std::cout << "res1 use_count: " << res1.use_count() << " (original + 2 in vector)" << std::endl;
            std::cout << "res2 use_count: " << res2.use_count() << " (original + 1 in vector)" << std::endl;
            
            std::cout << "\nIterare prin vector:" << std::endl;
            for (const auto& r : resources) {
                r->use();
            }
            
            std::cout << "\nVector se sterge..." << std::endl;
            resources.clear();
            
            std::cout << "Dupa clear: res1 use_count = " << res1.use_count() << std::endl;
            std::cout << "Resursele originale inca exista!\n" << std::endl;
        }
        
        std::cout << "\n--- Exemplu 3: weak_ptr (referinta non-owning) ---\n" << std::endl;
        {
            std::weak_ptr<SharedResource> weakPtr;
            
            {
                auto sharedPtr = std::make_shared<SharedResource>("Weak_Test");
                weakPtr = sharedPtr;
                
                std::cout << "In scope: weak_ptr expired? " << (weakPtr.expired() ? "DA" : "NU") << std::endl;
                
                if (auto locked = weakPtr.lock()) {
                    std::cout << "weak_ptr locked cu succes!" << std::endl;
                    locked->use();
                }
                
                std::cout << "sharedPtr iese din scope..." << std::endl;
            }
            
            std::cout << "Dupa scope: weak_ptr expired? " << (weakPtr.expired() ? "DA" : "NU") << std::endl;
            
            if (auto locked = weakPtr.lock()) {
                locked->use();  // Nu se executa
            } else {
                std::cout << "Nu se poate accesa - resursa a fost distrusa!" << std::endl;
            }
        }
        
        std::cout << "\n--- Cand folosim shared_ptr vs unique_ptr ---" << std::endl;
        std::cout << "unique_ptr: Ownership clar, un singur proprietar" << std::endl;
        std::cout << "shared_ptr: Mai multi proprietari, lifetime partajat" << std::endl;
        std::cout << "weak_ptr: Observer pattern, evita circular references" << std::endl;
    }
}

// ============================================================================
// ITEM 14: Copying behavior in resource-managing classes
// ============================================================================
namespace CopyingBehaviorDemo {

    // Optiunea 1: Prohibit copying (ca unique_ptr)
    class NoCopyResource {
    private:
        int* data;
    public:
        NoCopyResource() : data(new int(42)) {
            std::cout << "[NoCopy] Creare" << std::endl;
        }
        ~NoCopyResource() {
            std::cout << "[NoCopy] Distrugere" << std::endl;
            delete data;
        }
        
        // Prohibit copying
        NoCopyResource(const NoCopyResource&) = delete;
        NoCopyResource& operator=(const NoCopyResource&) = delete;
        
        // Allow moving
        NoCopyResource(NoCopyResource&& other) noexcept : data(other.data) {
            other.data = nullptr;
        }
    };
    
    // Optiunea 2: Reference counting (ca shared_ptr)
    class RefCountedResource {
    private:
        struct Data {
            int value;
            int refCount;
            Data(int v) : value(v), refCount(1) {}
        };
        Data* data;
        
    public:
        RefCountedResource(int val = 0) : data(new Data(val)) {
            std::cout << "[RefCounted] Creare (refCount=1)" << std::endl;
        }
        
        RefCountedResource(const RefCountedResource& other) : data(other.data) {
            ++data->refCount;
            std::cout << "[RefCounted] Copiere (refCount=" << data->refCount << ")" << std::endl;
        }
        
        RefCountedResource& operator=(const RefCountedResource& rhs) {
            if (this != &rhs) {
                // Decrementam vechiul refCount
                if (--data->refCount == 0) {
                    std::cout << "[RefCounted] Eliberare vechi data" << std::endl;
                    delete data;
                }
                // Copiem noul data
                data = rhs.data;
                ++data->refCount;
                std::cout << "[RefCounted] Assignment (refCount=" << data->refCount << ")" << std::endl;
            }
            return *this;
        }
        
        ~RefCountedResource() {
            if (--data->refCount == 0) {
                std::cout << "[RefCounted] Distrugere finala (refCount=0)" << std::endl;
                delete data;
            } else {
                std::cout << "[RefCounted] Decrement (refCount=" << data->refCount << ")" << std::endl;
            }
        }
        
        int getValue() const { return data->value; }
        int getRefCount() const { return data->refCount; }
    };
    
    // Optiunea 3: Deep copy
    class DeepCopyResource {
    private:
        int* data;
        size_t size;
        
    public:
        DeepCopyResource(size_t sz = 5) : size(sz) {
            data = new int[sz];
            for (size_t i = 0; i < sz; ++i) data[i] = static_cast<int>(i);
            std::cout << "[DeepCopy] Creare (size=" << sz << ")" << std::endl;
        }
        
        DeepCopyResource(const DeepCopyResource& other) : size(other.size) {
            data = new int[size];
            for (size_t i = 0; i < size; ++i) data[i] = other.data[i];
            std::cout << "[DeepCopy] Deep copy (size=" << size << ")" << std::endl;
        }
        
        DeepCopyResource& operator=(const DeepCopyResource& rhs) {
            if (this != &rhs) {
                delete[] data;
                size = rhs.size;
                data = new int[size];
                for (size_t i = 0; i < size; ++i) data[i] = rhs.data[i];
                std::cout << "[DeepCopy] Deep copy assignment" << std::endl;
            }
            return *this;
        }
        
        ~DeepCopyResource() {
            std::cout << "[DeepCopy] Distrugere" << std::endl;
            delete[] data;
        }
    };
    
    inline void demonstrate() {
        std::cout << "\n";
        std::cout << "============================================================\n";
        std::cout << "  ITEM 14: Copying behavior in resource-managing classes\n";
        std::cout << "============================================================\n";
        
        std::cout << "\nOptiuni pentru comportamentul de copiere:\n" << std::endl;
        
        std::cout << "--- Optiunea 1: Prohibit copying ---\n" << std::endl;
        {
            NoCopyResource r1;
            // NoCopyResource r2 = r1;  // COMPILE ERROR!
            NoCopyResource r2 = std::move(r1);  // OK - move
            std::cout << "Move functioneaza, copy nu!\n" << std::endl;
        }
        
        std::cout << "--- Optiunea 2: Reference counting ---\n" << std::endl;
        {
            RefCountedResource r1(100);
            std::cout << "r1 refCount: " << r1.getRefCount() << std::endl;
            
            RefCountedResource r2 = r1;
            std::cout << "Dupa copiere, r1 refCount: " << r1.getRefCount() << std::endl;
            
            {
                RefCountedResource r3 = r1;
                std::cout << "Dupa alta copiere, refCount: " << r1.getRefCount() << std::endl;
            }
            std::cout << "Dupa distrugere r3, refCount: " << r1.getRefCount() << std::endl;
        }
        
        std::cout << "\n--- Optiunea 3: Deep copy ---\n" << std::endl;
        {
            DeepCopyResource r1(3);
            DeepCopyResource r2 = r1;  // Copie completa
            std::cout << "Fiecare obiect are propria copie a datelor!\n" << std::endl;
        }
        
        std::cout << "\n--- Rezumat Item 14 ---" << std::endl;
        std::cout << "1. Prohibit copying: pentru resurse unice (file handles, mutex)" << std::endl;
        std::cout << "2. Reference counting: pentru resurse partajate" << std::endl;
        std::cout << "3. Deep copy: cand fiecare obiect trebuie sa aiba propria copie" << std::endl;
        std::cout << "4. Transfer ownership: move semantics pentru eficienta" << std::endl;
    }
}

// ============================================================================
// Functie principala de demonstratie
// ============================================================================
inline void demonstrateSmartPointers() {
    UniquePointerDemo::demonstrate();
    SharedPointerDemo::demonstrate();
    CopyingBehaviorDemo::demonstrate();
}

#endif // SMART_POINTER_DEMO_HPP
