#include <iostream>
#include <conio.h>
#include "../include/Actions.h"
using namespace std;

void printMenu() {
    cout << "������������ ������ �8 - STL ����������\n";
    cout << "===== ���� ������� =====\n";
    cout << "1. �������� �������\n";
    cout << "2. �������� �������\n";
    cout << "3. ������������� �������\n";
    cout << "4. ����� �������\n";
    cout << "0. �����\n";
    cout << "=========================\n";
    cout << "�������� ��������: ";
}

char chooseTaskNtoM(char n, char m) {
    char c = 'l';
    while (c < n || c > m) {
        c = _getch();
    }
    return c;
}

void handleAddElement(Queue<int>& q) {
    int val;
    cout << "������� �����: ";
    cin >> val;
    q.push(val);
    cout << "������� ��������.\n";
}

void handleShowQueue(Queue<int>& q) {
    if (q.empty()) {
        cout << "������� �����.\n";
        return;
    }
    cout << "�������� �������: ";
    for (auto it = q.begin(); it != q.end(); ++it)
        cout << *it << " ";
    cout << "\n";
}

void handleSortQueue(Queue<int>& q) {
    if (q.empty()) {
        cout << "������� �����.\n";
        return;
    }
    cout << "�������� ��� ����������:\n"
            "1. �� �����������\n"
            "2. �� ��������\n> ";
    char type;
    cin >> type;

    if (type == '1')
        QueueAlgorithms<int>::sort(q, [](int a, int b){ return a < b; });
    else
        QueueAlgorithms<int>::sort(q, [](int a, int b){ return a > b; });

    cout << "������� �������������.\n";
}

void handleFindElement(Queue<int>& q) {
    if (q.empty()) {
        cout << "������� �����.\n";
        return;
    }
    int val;
    cout << "������� �������� ��� ������: ";
    cin >> val;

    auto node = QueueAlgorithms<int>::find(q, val);
    if (node)
        cout << "������� " << val << " ������.\n";
    else
        cout << "������� " << val << " �� ������.\n";
}
