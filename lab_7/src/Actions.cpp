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
        cout << "������ ���������." << endl;
    }
    catch (const ios_base::failure& e) {
        cerr << "������ �����/������: " << e.what() << endl;
    }
    catch (const invalid_argument& e) {
        cerr << "�������� ��������: " << e.what() << endl;
    }
    catch (const runtime_error& e) {
        cerr << "������ ����������: " << e.what() << endl;
    }
    catch (const exception& e) {
        cerr << "����������� ������: " << e.what() << endl;
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

