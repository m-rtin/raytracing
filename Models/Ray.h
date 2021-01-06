//
// Created by Martin Voigt on 06.01.21.
//

#ifndef HELLOWORLD_RAY_H
#define HELLOWORLD_RAY_H

#include "Vector.h"

class Ray {

public:
    Ray(const Vector& C, const Vector& u);
    // camera position
    Vector C;
    // direction vector
    Vector u;
};


#endif //HELLOWORLD_RAY_H
