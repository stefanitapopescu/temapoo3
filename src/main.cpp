#include "Person.hpp"
#include "Employee.hpp"
#include "Widget.hpp"
#include "ResourceManager.hpp"
#include "SmartPointerDemo.hpp"
#include "ThreadingDemo.hpp"
#include <iostream>

void separator(const std::string& title) {
    std::cout << "\n" << std::string(60, '=') << std::endl;
    std::cout << "  " << title << std::endl;
    std::cout << std::string(60, '=') << "\n" << std::endl;
}

void demonstrateItem10() {
    separator("ITEM 10: Assignment operators return a reference to *this");
    
    std::cout << "Aceasta permite chaining de assignment-uri:\n" << std::endl;
    
    Person p1("Alice", 30, "123 Main St");
    Person p2("Bob", 25, "456 Oak Ave");
    Person p3("Charlie", 35, "789 Pine Rd");
    
    std::cout << "\nExecutam: p1 = p2 = p3;" << std::endl;
    std::cout << "(assignment-ul se evalueaza de la dreapta la stanga)\n" << std::endl;
    
    p1 = p2 = p3;  // Functioneaza pentru ca operator= returneaza *this
    
    std::cout << "\nDupa assignment chain:" << std::endl;
    p1.display();
    p2.display();
    p3.display();
    
    std::cout << "\nFara 'return *this', nu am putea face chaining!" << std::endl;
}

void demonstrateItem11() {
    separator("ITEM 11: Handle assignment to self in operator=");
    
    std::cout << "Self-assignment apare cand un obiect este asignat lui insusi.\n" << std::endl;
    
    Widget w1(1, "Important Data");
    w1.display();
    
    std::cout << "\nIncercam self-assignment: w1 = w1;" << std::endl;
    w1 = w1;  // Fara verificare, ar putea cauza probleme!
    
    std::cout << "\nDupa self-assignment (obiectul ramane valid):" << std::endl;
    w1.display();
    
    std::cout << "\n--- De ce este important? ---" << std::endl;
    std::cout << "Fara verificarea 'if (this == &rhs)', in operator=:" << std::endl;
    std::cout << "1. delete data;  // sterge data (care este si rhs.data!)" << std::endl;
    std::cout << "2. data = new std::string(*rhs.data);  // acceseaza memorie stearsa!" << std::endl;
    std::cout << "3. Rezultat: UNDEFINED BEHAVIOR, crash potential!\n" << std::endl;
    
    // Demonstratie cu obiecte diferite
    std::cout << "\nAssignment normal (obiecte diferite):" << std::endl;
    Widget w2(2, "Other Data");
    std::cout << "Inainte:" << std::endl;
    w1.display();
    w2.display();
    
    std::cout << "\nExecutam: w1 = w2;" << std::endl;
    w1 = w2;
    
    std::cout << "\nDupa:" << std::endl;
    w1.display();
    w2.display();
}

void demonstrateItem12() {
    separator("ITEM 12: Copy all parts of an object");
    
    std::cout << "Partea 1: Copy constructor trebuie sa copieze TOATE membrii\n" << std::endl;
    
    Person p1("David", 40, "321 Elm St");
    std::cout << "Original person:" << std::endl;
    p1.display();
    
    std::cout << "\nCreare copie folosind copy constructor:" << std::endl;
    Person p2(p1);  // Copy constructor
    
    std::cout << "\nCopia:" << std::endl;
    p2.display();
    
    std::cout << "\nModificam copia..." << std::endl;
    p2.setName("David Jr.");
    p2.setAge(20);
    
    std::cout << "\nOriginal (nemodificat):" << std::endl;
    p1.display();
    std::cout << "\nCopia (modificata):" << std::endl;
    p2.display();
    
    separator("ITEM 12: Copy ALL parts in DERIVED classes");
    
    std::cout << "Partea 2: In clase derivate, trebuie sa copiem si partea de baza!\n" << std::endl;
    
    Employee e1("Emma", 28, "555 Broadway", "EMP001", 75000.0, "Engineering");
    std::cout << "Original employee:" << std::endl;
    e1.display();
    
    std::cout << "\nCreare copie folosind copy constructor:" << std::endl;
    Employee e2(e1);  // TREBUIE sa copieze si partea Person!
    
    std::cout << "\nCopia:" << std::endl;
    e2.display();
    
    std::cout << "\nModificam copia..." << std::endl;
    e2.setName("Emma Smith");
    e2.setSalary(80000.0);
    
    std::cout << "\nOriginal (nemodificat):" << std::endl;
    e1.display();
    std::cout << "\nCopia (modificata):" << std::endl;
    e2.display();
    
    std::cout << "\n--- Greseala comuna ---" << std::endl;
    std::cout << "O greseala frecventa este sa uitam de partea din clasa de baza:" << std::endl;
    std::cout << "  Employee(const Employee& other)" << std::endl;
    std::cout << "    : employeeId(other.employeeId),  // Copiem membrii derivati" << std::endl;
    std::cout << "      salary(other.salary)            // DAR uitam de Person!" << std::endl;
    std::cout << "\nCorect:" << std::endl;
    std::cout << "  Employee(const Employee& other)" << std::endl;
    std::cout << "    : Person(other),                  // ESSENTIAL!" << std::endl;
    std::cout << "      employeeId(other.employeeId)," << std::endl;
    std::cout << "      salary(other.salary)" << std::endl;
}

void demonstrateAllTogether() {
    separator("Demonstratie combinata: Items 10, 11, 12");
    
    std::cout << "Cream 3 employees si demonstram toate conceptele:\n" << std::endl;
    
    Employee e1("John", 35, "100 Tech Blvd", "E001", 90000, "IT");
    Employee e2("Sarah", 30, "200 Business St", "E002", 85000, "HR");
    Employee e3("Mike", 40, "300 Admin Ave", "E003", 95000, "Finance");
    
    std::cout << "Initial state:" << std::endl;
    e1.display();
    e2.display();
    e3.display();
    
    // Item 10: Chaining
    std::cout << "\n--- ITEM 10: Assignment chaining ---" << std::endl;
    std::cout << "Executam: e1 = e2 = e3;\n" << std::endl;
    e1 = e2 = e3;  // Functioneaza datorita return *this
    
    std::cout << "Dupa chaining (toti au aceleasi valori):" << std::endl;
    e1.display();
    e2.display();
    
    // Item 11: Self-assignment
    std::cout << "\n--- ITEM 11: Self-assignment ---" << std::endl;
    std::cout << "Executam: e1 = e1;\n" << std::endl;
    e1 = e1;  // Protejat de verificarea self-assignment
    
    std::cout << "\nObiectul ramane valid:" << std::endl;
    e1.display();
    
    // Item 12: Deep copy
    std::cout << "\n--- ITEM 12: Deep copy (toate partile) ---" << std::endl;
    Employee e4(e3);  // Copy constructor - copiaza TOTUL
    std::cout << "Copie completa creata:" << std::endl;
    e4.display();
    
    std::cout << "\nModificam copia..." << std::endl;
    e4.setName("Mike Clone");
    e4.setEmployeeId("E004");
    e4.setSalary(100000);
    
    std::cout << "\nOriginal (nemodificat):" << std::endl;
    e3.display();
    std::cout << "\nCopia (modificata):" << std::endl;
    e4.display();
}

void showMenu() {
    std::cout << "\n";
    std::cout << "************************************************************\n";
    std::cout << "*                                                          *\n";
    std::cout << "*     EFFECTIVE C++ - Items 10, 11, 12, 13, 14            *\n";
    std::cout << "*     + Threading Demo (Mutex sincronizare)               *\n";
    std::cout << "*                                                          *\n";
    std::cout << "************************************************************\n";
    std::cout << "\nSelecteaza ce vrei sa vezi:\n" << std::endl;
    std::cout << "1. Item 10: Assignment operators return *this" << std::endl;
    std::cout << "2. Item 11: Handle self-assignment" << std::endl;
    std::cout << "3. Item 12: Copy all parts of an object" << std::endl;
    std::cout << "4. Item 13: RAII - Use objects to manage resources" << std::endl;
    std::cout << "5. Item 14: Smart Pointers (unique_ptr, shared_ptr)" << std::endl;
    std::cout << "6. Threading: Mutex sincronizare (din poza)" << std::endl;
    std::cout << "7. Ruleaza TOATE demonstratiile" << std::endl;
    std::cout << "0. Exit" << std::endl;
    std::cout << "\nOptiune: ";
}

int main() {
    std::cout << "\n";
    std::cout << "************************************************************\n";
    std::cout << "*                                                          *\n";
    std::cout << "*     EFFECTIVE C++ - Items 10, 11, 12, 13, 14            *\n";
    std::cout << "*     + Threading Demo (Mutex sincronizare)               *\n";
    std::cout << "*     Demonstratie Practica                               *\n";
    std::cout << "*                                                          *\n";
    std::cout << "************************************************************\n";
    
    int choice = 7;  // Default: ruleaza toate
    
    std::cout << "\nVrei meniu interactiv? (1=Da, 0=Nu, ruleaza toate): ";
    int interactive = 0;
    std::cin >> interactive;
    
    if (interactive == 1) {
        while (true) {
            showMenu();
            std::cin >> choice;
            
            if (choice == 0) break;
            
            try {
                switch (choice) {
                    case 1:
                        demonstrateItem10();
                        break;
                    case 2:
                        demonstrateItem11();
                        break;
                    case 3:
                        demonstrateItem12();
                        break;
                    case 4:
                        demonstrateItem13();  // RAII
                        break;
                    case 5:
                        demonstrateSmartPointers();  // unique_ptr, shared_ptr
                        break;
                    case 6:
                        demonstrateThreading();  // Mutex, threading
                        break;
                    case 7:
                        demonstrateItem10();
                        demonstrateItem11();
                        demonstrateItem12();
                        demonstrateAllTogether();
                        demonstrateItem13();
                        demonstrateSmartPointers();
                        demonstrateThreading();
                        break;
                    default:
                        std::cout << "Optiune invalida!" << std::endl;
                }
            } catch (const std::exception& e) {
                std::cerr << "Exception: " << e.what() << std::endl;
            }
            
            std::cout << "\nApasa Enter pentru a continua...";
            std::cin.ignore();
            std::cin.get();
        }
    } else {
        // Ruleaza toate demonstratiile
        try {
            // Items 10, 11, 12 (existente)
            demonstrateItem10();
            demonstrateItem11();
            demonstrateItem12();
            demonstrateAllTogether();
            
            // Item 13: RAII
            demonstrateItem13();
            
            // Item 14: Smart Pointers (unique_ptr, shared_ptr)
            demonstrateSmartPointers();
            
            // Threading Demo (din poza: threaduri sincronizate si desincronizate)
            demonstrateThreading();
            
            // Summary
            separator("SUMMARY - TOATE ITEMURILE");
            
            std::cout << "Item 10: Assignment operators return reference to *this" << std::endl;
            std::cout << "  -> Permite chaining: a = b = c;\n" << std::endl;
            
            std::cout << "Item 11: Handle assignment to self in operator=" << std::endl;
            std::cout << "  -> Verifica: if (this == &rhs) return *this;\n" << std::endl;
            
            std::cout << "Item 12: Copy all parts of an object" << std::endl;
            std::cout << "  -> In clase derivate, apeleaza explicit copy din baza\n" << std::endl;
            
            std::cout << "Item 13: Use objects to manage resources (RAII)" << std::endl;
            std::cout << "  -> Resursa achizitionata in constructor" << std::endl;
            std::cout << "  -> Resursa eliberata in destructor" << std::endl;
            std::cout << "  -> Exception-safe!\n" << std::endl;
            
            std::cout << "Item 14: Copying behavior in resource-managing classes" << std::endl;
            std::cout << "  -> unique_ptr: ownership exclusiv, nu se poate copia" << std::endl;
            std::cout << "  -> shared_ptr: ownership partajat, reference counting" << std::endl;
            std::cout << "  -> Optiuni: prohibit copy, deep copy, reference counting\n" << std::endl;
            
            std::cout << "Threading cu Mutex:" << std::endl;
            std::cout << "  -> FARA sincronizare: race conditions, date corupte" << std::endl;
            std::cout << "  -> CU mutex: operatii atomice, date consistente" << std::endl;
            std::cout << "  -> std::lock_guard: RAII pentru mutex" << std::endl;
            std::cout << "  -> Producer-Consumer: un thread scrie, altul citeste\n" << std::endl;
            
        } catch (const std::exception& e) {
            std::cerr << "Exception: " << e.what() << std::endl;
            return 1;
        }
    }
    
    std::cout << "\nProgram finished successfully!" << std::endl;
    return 0;
}


