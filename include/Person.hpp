#ifndef PERSON_HPP
#define PERSON_HPP

#include <string>

/**
 * Clasa Person demonstreaza Item 10, 11, 12 din Effective C++
 * 
 * Item 10: Assignment operators return a reference to *this
 * Item 11: Handle assignment to self in operator=
 * Item 12: Copy all parts of an object
 */
class Person {
private:
    std::string name;
    int age;
    std::string* address;  // pointer pentru a demonstra deep copy

public:
    // Constructor
    Person(const std::string& name = "", int age = 0, const std::string& addr = "");
    
    // Copy constructor - Item 12: Copy all parts
    Person(const Person& other);
    
    // Item 10 & 11: Assignment operator returns reference to *this
    // si handleaza assignment to self
    Person& operator=(const Person& rhs);
    
    // Destructor
    ~Person();
    
    // Getters
    std::string getName() const;
    int getAge() const;
    std::string getAddress() const;
    
    // Setters
    void setName(const std::string& name);
    void setAge(int age);
    void setAddress(const std::string& addr);
    
    // Display
    void display() const;
};

#endif // PERSON_HPP


