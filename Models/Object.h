//
// Created by Martin Voigt on 10.02.21.
//

#ifndef HELLOWORLD_OBJECT_H
#define HELLOWORLD_OBJECT_H

#include "Vector.h"
#include "Ray.h"

class Object {
public:
    Object() {};
    virtual bool intersect(const Ray& r, Vector& P, Vector& normal, double &t) = 0;

    // color of the sphere
    Vector albedo;
    // a mirror sphere is a sphere which reflects all incoming rays
    bool isMirror;
    // a transparent sphere uses Snell's law for incoming rays
    bool isTransparent;
};


#endif //HELLOWORLD_OBJECT_H
