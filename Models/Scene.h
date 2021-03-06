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
    bool intersect(const Ray& r, Vector& P, Vector& N, Vector &albedo, bool &mirror, bool &transparency, double &t, int& objectid);//, Object* &s);
    Vector getColor(const Ray& r, int rebound, bool lastDiffuse);

    // list of objects in the scene
    std::vector<Object*> objects;
    // radiance
    Vector L;
    // light intensity
    double I;
};


#endif //HELLOWORLD_SCENE_H
