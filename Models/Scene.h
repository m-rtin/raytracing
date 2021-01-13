//
// Created by Martin Voigt on 06.01.21.
//

#ifndef HELLOWORLD_SCENE_H
#define HELLOWORLD_SCENE_H

#include <vector>
#include "Vector.h"
#include "Ray.h"
#include "Sphere.h"

class Scene {
public:
    Scene();
    bool intersect(const Ray& r, Vector& P, Vector& N, Vector &albedo, double &t);
    // list of objects in the scene
    std::vector<Sphere> objects;
};


#endif //HELLOWORLD_SCENE_H
