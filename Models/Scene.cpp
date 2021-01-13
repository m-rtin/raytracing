//
// Created by Martin Voigt on 06.01.21.
//

#include "Scene.h"

Scene::Scene() {};

/**
 * Check if a given ray intersects an object in a given scene.
 *
 * If the ray intersects multiple objects in the scene, we take the object which has the
 * shortest distance to the camera (smallest t value).
 *
 * @param r incoming ray
 * @param P intersection point
 * @param N normal vector of the sphere at the intersection point
 * @param albedo color of the sphere the ray intersects
 * @return true if the input ray intersects with at least one of the spheres in the scene
 */
bool Scene::intersect(const Ray& r, Vector& P, Vector& N, Vector &albedo, double &t) {
    t = 1E10;
    bool hasInter = false;

    for (int i = 0; i<objects.size(); i++) {
        Vector localP, localN;
        double localt;

        // localt < t assures that the object is at the front of the scene
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
