#include <iostream>
#include "../include/Self_made_string.h"
#include "../include/Header.h"

using namespace std;

int main(){

    system("chcp 1251");
    system("cls");
    String my_string;
    String temp_string;

    char choice;
    int running = 1;

    while (running) {

        print_menu();

        choice = choce_task_n_to_m('0', '9');

        switch (choice) {
            case '1':
                system("cls");
                cout << "Введите строку : ";
                cin >> my_string;
                system("cls");
                break;
            case '2':
                system("cls");
                cout << my_string << endl;
                system("pause");
                break;
            case '3':
                my_string.clear();
                break;
            case '4':
                system("cls");
                if (!my_string) {cout << "Строка пуста!" << endl;}
                else {cout << "В строке есть символы! Вот они : " << my_string << endl;}
                system("pause");
                break;
            case '5':
                system("cls");
                if (!my_string) {
                    cout << "Строка пустая." << endl;
                    system("pause");
                    break;
                }else {
                    cout << substr_menu(my_string) << endl;
                    system("pause");
                }
                break;
            case '6':
                system("cls");
                if (!my_string) {
                    cout << "Строка пустая." << endl;
                    system("pause");
                    break;
                }
                cout << "Введите индекс: ";
                int i;
                i = new_input_metod(0, my_string.get_length() - 1);
                cout << "Вот символ " << i << " - " << my_string[i] << endl;
                system("pause");
                break;
            case '7':
                temp_string = my_string;
                break;
            case '8':
                system("cls");
                cout << temp_string << endl;
                system("pause");
                break;
            case '9':
                temp_string.clear();
                break;
            case '0':
                running = 0;
                break;
            default:
                break;
        }

        system("cls");
    }
}