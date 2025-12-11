#include "Employee.hpp"
#include <iostream>

// Constructor
Employee::Employee(const std::string& name, int age, const std::string& addr,
                   const std::string& empId, double sal, const std::string& dept)
    : Person(name, age, addr),  // Initialize base class
      employeeId(empId),
      salary(sal),
      department(new std::string(dept)) {
    std::cout << "Employee constructor called for: " << empId << std::endl;
}

// Copy constructor
// Item 12: IMPORTANT - trebuie sa copiem si partea din clasa de baza!
Employee::Employee(const Employee& other)
    : Person(other),  // Copy base class part - ESSENTIAL!
      employeeId(other.employeeId),
      salary(other.salary),
      department(new std::string(*other.department)) {
    std::cout << "Employee copy constructor called for: " << employeeId << std::endl;
}

// Assignment operator
// Item 10: Return reference to *this
// Item 11: Handle self-assignment
// Item 12: Copy ALL parts (including base class part!)
Employee& Employee::operator=(const Employee& rhs) {
    std::cout << "Employee assignment operator called" << std::endl;
    
    // Item 11: Check for self-assignment
    if (this == &rhs) {
        std::cout << "  -> Self-assignment detected, returning *this" << std::endl;
        return *this;
    }
    
    // Item 12: CRITICAL - Assign base class part first!
    // O greseala comuna este sa uitam de partea din clasa de baza
    Person::operator=(rhs);
    
    // Item 12: Now copy derived class members
    employeeId = rhs.employeeId;
    salary = rhs.salary;
    
    // Deep copy
    delete department;
    department = new std::string(*rhs.department);
    
    // Item 10: Return reference to *this
    return *this;
}

// Destructor
Employee::~Employee() {
    std::cout << "Employee destructor called for: " << employeeId << std::endl;
    delete department;
}

// Getters
std::string Employee::getEmployeeId() const {
    return employeeId;
}

double Employee::getSalary() const {
    return salary;
}

std::string Employee::getDepartment() const {
    return department ? *department : "";
}

// Setters
void Employee::setEmployeeId(const std::string& empId) {
    this->employeeId = empId;
}

void Employee::setSalary(double sal) {
    this->salary = sal;
}

void Employee::setDepartment(const std::string& dept) {
    if (department) {
        *department = dept;
    } else {
        department = new std::string(dept);
    }
}

// Display
void Employee::display() const {
    std::cout << "=== Employee Information ===" << std::endl;
    std::cout << "  Name: " << getName() << std::endl;
    std::cout << "  Age: " << getAge() << std::endl;
    std::cout << "  Address: " << getAddress() << std::endl;
    std::cout << "  Employee ID: " << employeeId << std::endl;
    std::cout << "  Salary: $" << salary << std::endl;
    std::cout << "  Department: " << (department ? *department : "N/A") << std::endl;
    std::cout << "============================" << std::endl;
}


