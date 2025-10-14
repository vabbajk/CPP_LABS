#include <iostream>
#include "../include/Header.h"
using namespace std;

int main() {
    system("chcp 1251");
    system("cls");

    cout << "¬ведите размер массива (ESC Ч 0): ";

    int initSize = secureInputMethod(0, INT_MAX);
    if (initSize == INT_MIN) {exit(0);}

    SafeArray arr(static_cast<size_t>(initSize));

    while (true) {
        printMenu();

        char choice = chooseTaskNtoM('0', '4');
        cout << choice << "\n";

        switch (choice) {
            case '1': {
                addByIndex(arr);
                break;
            }
            case '2': {
                printArray(arr);
                break;
            }
            case '3': {
                readByIndex(arr);
                break;
            }
            case '4': {
                recreateArray(arr);
                break;
            }
            case '0':
                cout << "¬ыход..." << endl;
                return 0;
        }
        system("pause");
        system("cls");
    }
}
