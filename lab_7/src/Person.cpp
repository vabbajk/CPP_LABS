#include "../include/Person.h"
#include <iostream>
#include <limits>
#include "../include/Header.h"

using namespace std;


void Person::inputFromConsole() {
    cout << "Введите фамилию: ";
    cin >> surname;
    cout << "Введите имя: ";
    cin >> name;
    cout << "Введите отчество: ";
    cin >> patronymic;
    cout << "Введите номер сотрудника (целое): ";
    int id = secureInputMethod(0, INT_MAX);
    while (id == INT_MIN) {
        cout << "\nВведите номер сотрудника (целое): ";
        id = secureInputMethod(0, INT_MAX);
    }
    emp_id = static_cast<unsigned int>(id);
}


ostream &operator<<(ostream &os, const Person &p) {

    os << p.surname << ' ' << p.name << ' ' << p.patronymic << ' ' << p.emp_id << '\n';
    return os;
}


istream &operator>>(istream &is, Person &p) {

    is >> p.surname >> p.name >> p.patronymic >> p.emp_id;
    return is;
}
