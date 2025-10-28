#include <iostream>
#include <string>
#include "../include/Actions.h"

using namespace std;

int main() {
    system("chcp 1251");
    system("cls");

    Queue<int> queue;

    while (true) {
        printMenu();

        char opt = chooseTaskNtoM('0', '4');
        cout << opt << endl;

        switch (opt) {
            case '1':
                handleAddElement(queue);
            break;
            case '2':
                handleShowQueue(queue);
            break;
            case '3':
                handleSortQueue(queue);
            break;
            case '4':
                handleFindElement(queue);
            break;
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
