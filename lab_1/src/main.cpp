#include <iostream>

#include "Array.h"
#include "Header.h"

using namespace std;

int main()
{
    setlocale(LC_ALL, "Russian");

    char choice;
    int running = 1;

    MyArray first_array;
    MyArray second_array;
    MyArray result_array;

    while (running)
    {
        print_menu();

        choice = choce_task_n_to_m('0', '8');
        system("cls");
        switch (choice)
        {
            case '1':
                input_array(first_array);
                break;
            case '2':
                input_array(second_array);
                break;
            case '3':
                first_array.clear();
                break;
            case '4':
                second_array.clear();
                break;
            case '5':
                system("cls");
                result_array = first_array & second_array;
                result_array.print();
                result_array.clear();
                system("pause");
                break;
            case '6':
                system("cls");
                result_array = first_array + second_array;
                result_array.print();
                result_array.clear();
                system("pause");
                break;
            case '7':
                system("cls");
                first_array.print();
                second_array.print();
                system("pause");
                break;
            case '8':
                system("cls");
                second_array.print();
                system("pause");
                break;
            case '0':
                running = 0;
                break;
            default:
                cout << "Как ты попал сюда?" << endl;
                running = 0;
                system("pause");
        }
        system("cls");
    }
}