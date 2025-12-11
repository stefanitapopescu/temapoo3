#include "Person.hpp"
#include <iostream>

// Constructor
Person::Person(const std::string& name, int age, const std::string& addr)
    : name(name), age(age), address(new std::string(addr)) {
    std::cout << "Person constructor called for: " << name << std::endl;
}

// Copy constructor - Item 12: Copy ALL parts of an object
Person::Person(const Person& other)
    : name(other.name), age(other.age), address(new std::string(*other.address)) {
    std::cout << "Person copy constructor called for: " << name << std::endl;
}

// Assignment operator
// Item 10: Return a reference to *this
// Item 11: Handle assignment to self
// Item 12: Copy all parts
Person& Person::operator=(const Person& rhs) {
    std::cout << "Person assignment operator called" << std::endl;
    
    // Item 11: Check for self-assignment
    // Fara aceasta verificare, ar putea aparea probleme:
    // - delete address ar sterge si rhs.address (daca this == &rhs)
    // - apoi incercam sa copiem din memorie stearsa
    if (this == &rhs) {
        std::cout << "  -> Self-assignment detected, returning *this" << std::endl;
        return *this;  // Item 10: return reference to *this
    }
    
    // Item 12: Copy ALL members
    name = rhs.name;
    age = rhs.age;
    
    // Deep copy pentru pointeri
    delete address;
    address = new std::string(*rhs.address);
    
    // Item 10: Return a reference to *this
    // Acest lucru permite chaining: a = b = c;
    return *this;
}

// Destructor
Person::~Person() {
    std::cout << "Person destructor called for: " << name << std::endl;
    delete address;
}

// Getters
std::string Person::getName() const {
    return name;
}

int Person::getAge() const {
    return age;
}

std::string Person::getAddress() const {
    return address ? *address : "";
}

// Setters
void Person::setName(const std::string& name) {
    this->name = name;
}

void Person::setAge(int age) {
    this->age = age;
}

void Person::setAddress(const std::string& addr) {
    if (address) {
        *address = addr;
    } else {
        address = new std::string(addr);
    }
}

// Display
void Person::display() const {
    std::cout << "Person: " << name << ", Age: " << age 
              << ", Address: " << (address ? *address : "N/A") << std::endl;
}


