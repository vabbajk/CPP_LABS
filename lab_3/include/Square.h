#ifndef SQUARE_H
#define SQUARE_H

#include "Shape.h"

class Square : public Shape {
public:
    bool intersect(Shape *other) override;

    bool intersectWithCircle(Circle *other) override;
    bool intersectWithSquare(Square *other) override;
    bool intersectWithTriangle(Triangle *other) override;
};

#endif
