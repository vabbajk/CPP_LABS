#include "../include/Header.h"

using namespace std;

char choceTaskNtoM(char n, char m) {
    char c = 'l';
    while (c < n || c > m) {
        c = _getch();
    }
    return c;
}

void printMenu() {
    cout << "�������� :\n"; 
    cout << "1 - ����\n";
    cout << "2 - �������\n";
    cout << "3 - �����������\n";
    cout << "0 - �����\n";
    cout << "��� �����: ";
}

Shape *chooseFigure(const string &displays_text) {
    cout << "\n--- " << displays_text << " ---\n";
    printMenu();
    char choice = choceTaskNtoM('0', '3');
    cout << choice << endl;
    switch (choice) {
        case '1':
            return new Circle();
        case '2':
            return new Square();
        case '3':
            return new Triangle();
        case '0':
            return nullptr;
        default:
            return nullptr;
    }
}
