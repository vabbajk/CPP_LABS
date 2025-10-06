#include <iostream>
#include <string>
#include "../include/Queue.h"
#include "../include/Header.h"

using namespace std;

int main() {
    system("chcp 1251 > nul");

    Queue<int> intQueue;
    Queue<string> strQueue;
    Queue<double> dblQueue;

    bool running = true;
    while (running) {
        system("cls");
        printMainMenu();
        char choice = chooseTaskNtoM('0', '3');
        cout << choice << endl;

        switch (choice) {
            case '0':
                running = false;
                break;
            case '1':
                handleIntQueue(intQueue);
                break;
            case '2':
                handleStringQueue(strQueue);
                break;
            case '3':
                handleDoubleQueue(dblQueue);
                break;
            default:
                cout << "Ты как тут оказался?" << endl;
        }
    }

    cout << "Завершение работы программы.\n";
    return 0;
}
