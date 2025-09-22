#include <iostream>
#include "../include/Self_made_string.h"
#include "../include/Header.h"

using namespace std;

int main() {
    system("chcp 1251");
    system("cls");

    String my_string;
    String temp_string;

    char choice;
    int running = 1;

    while (running) {
        print_menu_2();
        choice = choce_task_n_to_m('0', '9');

        switch (choice) {
            case '1':
                input_string(my_string);
                break;
            case '2':
                print_string(my_string);
                break;
            case '3':
                clear_string(my_string);
                break;
            case '4':
                check_empty(my_string);
                break;
            case '5':
                print_substring(my_string);
                break;
            case '6':
                print_char_by_index(my_string);
                break;
            case '7':
                copy_string(my_string, temp_string);
                break;
            case '8':
                print_temp_string(temp_string);
                break;
            case '9':
                clear_temp_string(temp_string);
                break;
            case '0':
                running = 0;
                break;
            default: break;
        }
        system("cls");
    }

    return 0;
}