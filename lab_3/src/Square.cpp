#include "../include/Square.h"

bool Square::intersect(Shape *other) {
  return other->intersectWithSquare(this);
}

bool Square::intersectWithCircle(Circle *other) {
  std::cout << "�������� ����������� �������-����\n";
  return false;
}

bool Square::intersectWithSquare(Square *other) {
  std::cout << "�������� ����������� �������-�������\n";
  return false;
}

bool Square::intersectWithTriangle(Triangle *other) {
  std::cout << "�������� ����������� �������-�����������\n";
  return false;
}
