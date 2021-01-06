//
// Created by Martin Voigt on 06.01.21.
//

#ifndef HELLOWORLD_SPHERE_H
#define HELLOWORLD_SPHERE_H

#include "Vector.h"
#include "Ray.h"

class Sphere {
public:
    Sphere(const Vector& O, double R, const Vector& albedo);
    bool intersect(const Ray& r, Vector& P, Vector& N, double &t);
    Vector O;
    double R;
    Vector albedo;
};


#endif //HELLOWORLD_SPHERE_H
