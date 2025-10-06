#include "../include/Header.h"
#include <iostream>

using namespace std;

int main() {
  system("chcp 1251");
  system("cls");

  bool running = true;

  while (running) {

    cout << "Лабораторная работа N3, Вариант 2" << endl;

    Shape *fig1 = chooseFigure("Выбор первой фигуры");
    if (!fig1) {
      exit(1);
    }

    Shape *fig2 = chooseFigure("Выбор второй фигуры");
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
