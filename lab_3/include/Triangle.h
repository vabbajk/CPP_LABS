#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Shape.h"

class Triangle : public Shape {
public:
  bool intersect(Shape *other) override;

  bool intersectWithCircle(Circle *other) override;
  bool intersectWithSquare(Square *other) override;
  bool intersectWithTriangle(Triangle *other) override;
};

#endif
