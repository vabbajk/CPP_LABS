#include "../include/Triangle.h"

bool Triangle::intersect(Shape* other) {
    return other->intersectWithTriangle(this);
}

bool Triangle::intersectWithCircle(Circle* other) {
    std::cout << "�������� ����������� �����������-����\n";
    return false;
}

bool Triangle::intersectWithSquare(Square* other) {
    std::cout << "�������� ����������� �����������-�������\n";
    return false;
}

bool Triangle::intersectWithTriangle(Triangle* other) {
    std::cout << "�������� ����������� �����������-�����������\n";
    return false;
}
