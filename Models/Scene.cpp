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
bool Scene::intersect(const Ray& r, Vector& P, Vector& N, Vector &albedo, bool &mirror, double &t) {
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
            mirror = objects[i].isMirror;
        }
    }

    return hasInter;
}


Vector Scene::getColor(const Ray& r, int rebound) {
    if (rebound > 5) {
        return Vector(0., 0., 0.);
    }

    Vector P, N, albedo;
    double t;
    bool mirror;
    bool inter = intersect(r, P, N, albedo, mirror, t);
    Vector color(0, 0, 0);


    if (inter) {
        if (mirror) {
            Vector reflectedDir = r.u - 2*dot(r.u, N)*N;
            Ray reflectedRay(P + 0.001*N, reflectedDir);
            return getColor(reflectedRay, rebound + 1);     
        } else {
            // Lambertian model for shadows
            Vector PL = L - P;
            double d = sqrt(PL.sqrNorm());
            Vector shadowP, shadowN, shadowAlbedo;
            double shadowt;
            Ray shadowRay(P + 0.001*N, PL/d);
            bool shadowInter = intersect(shadowRay, shadowP, shadowN, shadowAlbedo, mirror, shadowt);
            if (shadowInter && shadowt < d) {
                color = Vector(0., 0., 0.);
            } else {
                color = I/(4*M_PI*d*d)*albedo/M_PI*std::max(0., dot(N, PL/d));
            }
            return color;
        }
    }
}