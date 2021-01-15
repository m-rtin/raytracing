//
// Created by Martin Voigt on 06.01.21.
//

#ifndef HELLOWORLD_SPHERE_H
#define HELLOWORLD_SPHERE_H

#include "Vector.h"
#include "Ray.h"

class Sphere {
public:
    Sphere(const Vector& O, double R, const Vector& albedo, bool isMirror=false, bool isTransparent=false);
    bool intersect(const Ray& r, Vector& P, Vector& N, double &t);

    // center of the sphere
    Vector O;
    // radius of the sphere
    double R;
    // color of the sphere
    Vector albedo;
    // a mirror sphere is a sphere which reflects all incoming rays
    bool isMirror;
    // a transparent sphere uses Snell's law for incoming rays
    bool isTransparent;
};


#endif //HELLOWORLD_SPHERE_H
