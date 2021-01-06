//
// Created by Martin Voigt on 06.01.21.
//

#ifndef HELLOWORLD_RAY_H
#define HELLOWORLD_RAY_H

#include "Vector.h"

class Ray {
public:
    Ray(const Vector& C, const Vector& u);
    Vector C, u;
};


#endif //HELLOWORLD_RAY_H
