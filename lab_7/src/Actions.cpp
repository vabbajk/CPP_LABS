#include "../include/Actions.h"
#include <algorithm>
#include <iostream>
#include <string>
#include "../include/FileManager.h"
#include "../include/Person.h"

using namespace std;

void handleAddRecord(const string &storageFilename) {
    Person p;
    p.inputFromConsole();
    try {
        append_record(storageFilename, p);
        cout << "Запись добавлена." << endl;
    } catch (const exception &e) {
        cerr << "Ошибка: " << e.what() << endl;
    }
}

void handleShowAll(const string &storageFilename) {
    cout << "Список записей:" << endl;
    bool ok = print_all(storageFilename, cout);
    if (!ok) {
        cout << "Файл пока не содержит записей или отсутствует." << endl;
    }
}

void handleRemoveFile(const string &storageFilename) {
    if (remove_file(storageFilename)) {
        cout << "Файл удалён." << endl;
    } else {
        cout << "Не удалось удалить файл (возможно, его нет)." << endl;
    }
}

