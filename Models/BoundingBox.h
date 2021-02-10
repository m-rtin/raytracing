//
// Created by Martin Voigt on 10.02.21.
//

#ifndef HELLOWORLD_BOUNDINGBOX_H
#define HELLOWORLD_BOUNDINGBOX_H

#include <algorithm>
#include "Vector.h"
#include "Ray.h"

class BoundingBox {
public:
    bool intersect(const Ray& r);
    Vector mini, maxi;
};


#endif //HELLOWORLD_BOUNDINGBOX_H
