#ifndef PERSON_H
#define PERSON_H


#include <iostream>
#include <string>


struct Person {
    std::string surname;
    std::string name;
    std::string patronymic;
    unsigned int emp_id;

    void inputFromConsole();

    friend std::ostream &operator<<(std::ostream &os, const Person &p);

    friend std::istream &operator>>(std::istream &is, Person &p);
};


#endif
