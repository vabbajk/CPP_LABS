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
    cout << "Выберите :\n"; 
    cout << "1 - Круг\n";
    cout << "2 - Квадрат\n";
    cout << "3 - Треугольник\n";
    cout << "0 - Выход\n";
    cout << "Ваш выбор: ";
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
