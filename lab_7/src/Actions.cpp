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
        std::cout << "������ ���������." << std::endl;
    }
    catch (const FileOpenException& e) {
        std::cerr << "�� ������� ������� ����: " << e.what() << std::endl;
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "�������� ������: " << e.what() << std::endl;
    }
    catch (const std::system_error& e) {
        std::cerr << "����������� ������: " << e.what() << std::endl;
    }
}

void handleShowAll(const string &storageFilename) {
    cout << "������ �������:" << endl;
    bool ok = print_all(storageFilename, cout);
    if (!ok) {
        cout << "���� ���� �� �������� ������� ��� �����������." << endl;
    }
}

void handleRemoveFile(const string &storageFilename) {
    if (remove_file(storageFilename)) {
        cout << "���� �����." << endl;
    } else {
        cout << "�� ������� ������� ���� (��������, ��� ���)." << endl;
    }
}

