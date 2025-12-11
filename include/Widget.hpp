#ifndef WIDGET_HPP
#define WIDGET_HPP

#include <string>

/**
 * Clasa Widget demonstreaza in mod explicit:
 * Item 11: Handle assignment to self in operator=
 * 
 * Aceasta clasa arata probleme care pot aparea fara verificarea self-assignment
 */
class Widget {
private:
    std::string* data;
    int id;

public:
    // Constructor
    Widget(int id = 0, const std::string& dataStr = "");
    
    // Copy constructor
    Widget(const Widget& other);
    
    // Assignment operator - demonstreaza importanta verificarii self-assignment
    Widget& operator=(const Widget& rhs);
    
    // Alternative safe assignment (exception-safe)
    Widget& operatorAssignmentSafe(const Widget& rhs);
    
    // Destructor
    ~Widget();
    
    // Display
    void display() const;
    
    // Getters
    int getId() const;
    std::string getData() const;
};

#endif // WIDGET_HPP


