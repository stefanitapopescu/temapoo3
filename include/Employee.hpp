#ifndef EMPLOYEE_HPP
#define EMPLOYEE_HPP

#include "Person.hpp"
#include <string>

/**
 * Clasa Employee mosteneste Person si demonstreaza:
 * Item 12: Copy all parts of an object (inclusiv in clase derivate)
 */
class Employee : public Person {
private:
    std::string employeeId;
    double salary;
    std::string* department;  // pointer pentru deep copy

public:
    // Constructor
    Employee(const std::string& name = "", int age = 0, 
             const std::string& addr = "", const std::string& empId = "",
             double sal = 0.0, const std::string& dept = "");
    
    // Copy constructor - Item 12: trebuie sa copieze si partea din clasa de baza
    Employee(const Employee& other);
    
    // Assignment operator - Item 10, 11, 12
    Employee& operator=(const Employee& rhs);
    
    // Destructor
    ~Employee();
    
    // Getters
    std::string getEmployeeId() const;
    double getSalary() const;
    std::string getDepartment() const;
    
    // Setters
    void setEmployeeId(const std::string& empId);
    void setSalary(double sal);
    void setDepartment(const std::string& dept);
    
    // Display
    void display() const;
};

#endif // EMPLOYEE_HPP


