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
 * @param mirror return value which indicates if the object is a mirror
 * @param transparency indicates if the object is transparent
 * @param t indicates at which point of the ray the intersection happens
 * @return true if the input ray intersects with at least one of the spheres in the scene
 */
bool Scene::intersect(const Ray& r, Vector& P, Vector& N, Vector &albedo, bool &mirror, bool &transparency, double &t) {
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
            transparency = objects[i].isTransparent;
        }
    }

    return hasInter;
}


/**
 * Get color of object in scene which intersects with the incoming ray.
 *
 * If the object is a mirror or transparent, we calculate the new ray direction and call the function recursively.
 *
 * @param r incoming ray
 * @param rebound upper bound for recursion calls
 * @return color of the object the ray intersects with
 */
Vector Scene::getColor(const Ray& r, int rebound) {
    if (rebound > 5) {
        return Vector(0., 0., 0.);
    }

    Vector P, N, albedo;
    double t;
    bool mirror, transparent;
    bool inter = this->intersect(r, P, N, albedo, mirror, transparent, t);
    Vector color(0, 0, 0);

    if (inter) {
        if (mirror) {
            // use the formula for reflection of vectors
            Vector reflectedDir = r.u - 2*dot(r.u, N)*N;
            Ray reflectedRay(P + 0.001*N, reflectedDir);
            return getColor(reflectedRay, rebound + 1);     
        } 
        else {
            if (transparent) {
                // if the body is transparent we use Snell's law
                // n1 and n2 are the phase velocities in the two media
                double n1 = 1, n2 = 1.4;
                Vector N2 = N;

                // normal vector and incoming ray need to have the same orientation
                if (dot(r.u, N) > 0) {
                    std::swap(n1, n2);
                    N2 = -N;
                }
                // tangential component
                Vector Tt = n1 / n2 * (r.u - dot(r.u, N2) * N2);
                double rad = 1 - pow(n1 / n2, 2) * (1 - pow(dot(r.u, N2), 2));
                if (rad < 0) { // the square root is complex which means we have total reflection
                    Vector reflectedDir = r.u - 2 * dot(r.u, N) * N;
                    Ray reflectedRay(P + 0.001 * N, reflectedDir);
                    return getColor(reflectedRay, rebound + 1);
                }
                // normal component
                Vector Tn = -sqrt(rad) * N2;

                // the refracted vector is made up of the tangential and normal component
                Vector refractedDir = Tt + Tn;
                Ray refractedRay(P - 0.00001 * N2, refractedDir);
                return getColor(refractedRay, rebound + 1);
            }

            // Lambertian model for shadows
            Vector PL = L - P;
            double d = sqrt(PL.sqrNorm());
            Vector shadowP, shadowN, shadowAlbedo;
            double shadowt;
            bool shadowTransp;
            Ray shadowRay(P + 0.001 * N, PL / d);
            bool shadowInter = this->intersect(shadowRay, shadowP, shadowN, shadowAlbedo, mirror, shadowTransp, shadowt);
            if (shadowInter && shadowt < d) {
                color = Vector(0., 0., 0.);
            } else {
                color = I / (4 * M_PI * d * d) * albedo / M_PI * std::max(0., dot(N, PL / d));
            }
            return color;
        }
    }
    return color;
}