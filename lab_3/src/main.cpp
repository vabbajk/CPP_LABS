#include <iostream>
#include "../include/Header.h"

using namespace std;

int main() {
    system("chcp 1251");
    system("cls");

    bool running = true;

    while (running) {

        cout << "������������ ������ N2, ������� 2" << endl;

        Shape* fig1 = choose_figure("����� ������ ������");
        if (!fig1) {
            exit(1);
        }

        Shape* fig2 = choose_figure("����� ������ ������");
        if (!fig2) {
            delete fig1;
            exit(1);
        }

        cout << endl;
        fig2->intersect(fig1);
        cout << endl;

        delete fig1;
        delete fig2;

        system("pause");
        system("cls");
    }

    return 0;
}