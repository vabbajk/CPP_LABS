#include "../include/Header.h"
#include <iostream>

using namespace std;

int main() {
  system("chcp 1251");
  system("cls");

  bool running = true;

  while (running) {

    cout << "������������ ������ N3, ������� 2" << endl;

    Shape *fig1 = chooseFigure("����� ������ ������");
    if (!fig1) {
      exit(1);
    }

    Shape *fig2 = chooseFigure("����� ������ ������");
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
