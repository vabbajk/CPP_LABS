#include "../include/Person.h"
#include <iostream>
#include <limits>
#include "../include/Header.h"

using namespace std;


void Person::inputFromConsole() {
    cout << "������� �������: ";
    cin >> surname;
    cout << "������� ���: ";
    cin >> name;
    cout << "������� ��������: ";
    cin >> patronymic;
    cout << "������� ����� ���������� (�����): ";
    int id = secureInputMethod(0, INT_MAX);
    while (id == INT_MIN) {
        cout << "\n������� ����� ���������� (�����): ";
        id = secureInputMethod(0, INT_MAX);
    }
    emp_id = id;
}

