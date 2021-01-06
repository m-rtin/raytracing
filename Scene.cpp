//
// Created by Martin Voigt on 06.01.21.
//

#include "Scene.h"

Scene::Scene() {};

bool Scene::intersect(const Ray& r, Vector& P, Vector& N, Vector &albedo) {
    double t = 1E10;
    bool hasInter = false;
    for (int i = 0; i<objects.size(); i++) {
        Vector localP, localN;
        double localt;
        if (objects[i].intersect(r, localP, localN, localt) && localt < t) {
            t = localt;
            hasInter = true;
            albedo = objects[i].albedo;
            P = localP;
            N = localN;
        }
    }

    return hasInter;
}
