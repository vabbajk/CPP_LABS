#include <iostream>
#include <string>
#include "../include/Actions.h"
#include "../include/Header.h"

using namespace std;

int main() {

    system("chcp 1251");
    system("cls");

    const string storage = "employees.txt";

    while (true) {
        printMenu();

        char opt = chooseTaskNtoM('0', '3');

        cout << endl;

        switch (opt) {
            case '1': {
                handleAddRecord(storage);
                break;
            }
            case '2': {
                handleShowAll(storage);
                break;
            }
            case '3': {
                handleRemoveFile(storage);
                break;
            }
            case '0':
                cout << "Выход.\n";
                return 0;
            default:
                cout << "Некорректный выбор\n";
        }
        system("pause");
        system("cls");
    }

    return 0;
}
