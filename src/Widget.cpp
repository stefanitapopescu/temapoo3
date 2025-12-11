#include "Widget.hpp"
#include <iostream>

// Constructor
Widget::Widget(int id, const std::string& dataStr)
    : data(new std::string(dataStr)), id(id) {
    std::cout << "Widget constructor called for ID: " << id << std::endl;
}

// Copy constructor
Widget::Widget(const Widget& other)
    : data(new std::string(*other.data)), id(other.id) {
    std::cout << "Widget copy constructor called for ID: " << id << std::endl;
}

// Assignment operator - demonstreaza importanta verificarii self-assignment
// Item 10: Return reference to *this
// Item 11: Handle assignment to self
Widget& Widget::operator=(const Widget& rhs) {
    std::cout << "Widget assignment operator called" << std::endl;
    
    // Item 11: CRITICAL - Check for self-assignment
    // Fara aceasta verificare:
    // 1. delete data; ar sterge si rhs.data (daca this == &rhs)
    // 2. apoi new std::string(*rhs.data) ar incerca sa acceseze memorie stearsa
    // 3. rezultat: undefined behavior, crash potential
    
    if (this == &rhs) {
        std::cout << "  -> Self-assignment detected! Avoiding undefined behavior." << std::endl;
        return *this;  // Item 10: return *this
    }
    
    std::cout << "  -> Different objects, proceeding with assignment" << std::endl;
    
    // Safe to proceed
    id = rhs.id;
    delete data;
    data = new std::string(*rhs.data);
    
    // Item 10: Return reference to *this pentru chaining
    return *this;
}

// Alternative: Exception-safe assignment (copy-and-swap idiom)
Widget& Widget::operatorAssignmentSafe(const Widget& rhs) {
    std::cout << "Widget safe assignment operator called" << std::endl;
    
    // Aceasta tehnica este si exception-safe si handleaza self-assignment automat
    // Nu mai este nevoie de verificare explicita pentru self-assignment
    
    // Create temporary copy
    std::string* temp = new std::string(*rhs.data);
    
    // If above succeeds (no exception), now modify this object
    delete data;
    data = temp;
    id = rhs.id;
    
    return *this;
}

// Destructor
Widget::~Widget() {
    std::cout << "Widget destructor called for ID: " << id << std::endl;
    delete data;
}

// Display
void Widget::display() const {
    std::cout << "Widget ID: " << id << ", Data: " 
              << (data ? *data : "N/A") << std::endl;
}

// Getters
int Widget::getId() const {
    return id;
}

std::string Widget::getData() const {
    return data ? *data : "";
}


