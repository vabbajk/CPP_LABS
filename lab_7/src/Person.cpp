#include "../include/Person.h"


using namespace std;


void Person::inputFromConsole() {
    cout << "¬ведите фамилию: ";
    cin >> surname;
    cout << "¬ведите им€: ";
    cin >> name;
    cout << "¬ведите отчество: ";
    cin >> patronymic;
    cout << "¬ведите номер сотрудника (целое): ";
    int id = secureInputMethod(0, INT_MAX);
    while (id == INT_MIN) {
        cout << "\n¬ведите номер сотрудника (целое): ";
        id = secureInputMethod(0, INT_MAX);
    }
    emp_id = id;
}

