#include "../include/FileManager.h"
#include <fstream>
#include <iostream>

using namespace std;

void append_record(const string& filename, const Person& p) {
    ofstream ofs(filename, ios::app);
    if (!ofs) {
        throw FileOpenException(filename);
    }
    ofs << p;
    ofs.close();
}


bool print_all(const string& filename, ostream &os) {
    ifstream ifs(filename);
    if (!ifs) {
        return false;
    }
    Person p;
    size_t idx = 1;
    while (ifs >> p) {
        os << idx << ") "
           << p.surname << ' ' << p.name << ' ' << p.patronymic
           << ", ID: " << p.emp_id << '\n';
        idx++;
    }
    ifs.close();
    return true;
}


bool remove_file(const string& filename) {
    return remove(filename.c_str()) == 0;
}
