//
// Created by Martin Voigt on 06.01.21.
//

#include "Scene.h"
#include <random>
static std::default_random_engine engine(10); // random seed = 10
static std::uniform_real_distribution<double> uniform(0, 1);

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
bool Scene::intersect(const Ray& r, Vector& P, Vector& N, Vector &albedo, bool &mirror, bool &transparency, double &t, int& objectid) {
    t = 1E10;
    bool hasInter = false;

    for (int i = 0; i<objects.size(); i++) {
        Vector localP, localN;
        double localt;

        // localt < t assures that the object is at the front of the scene
        if (objects[i]->intersect(r, localP, localN, localt) && localt < t) {
            t = localt;
            hasInter = true;
            albedo = objects[i]->albedo;
            P = localP;
            N = localN;
            mirror = objects[i]->isMirror;
            transparency = objects[i]->isTransparent;
            objectid = i;
        }
    }

    return hasInter;
}

Vector random_cos(const Vector& N) {
    double u1 = uniform(engine);
    double u2 = uniform(engine);
    double x = cos(2*M_PI*u1)*sqrt(-2 * log(u2));
    double y = sin(2*M_PI*u1)*sqrt(-2 * log(u2));
    double z = sqrt(u2);
    Vector T1;
    if (N[0] < N[1] && N[0] < N[2]) {
        T1 = Vector(0, N[2], -N[1]);
    } else {
        if (N[1] < N[2] && N[1] < N[0]) {
            T1 = Vector(N[2], 0, -N[0]);
        } else {

            T1 = Vector(N[1], -N[0], 0);
        }
    }
    T1 = T1.getNormalized();
    Vector T2 = cross(N, T1);
    return z*N + x*T1 + y*T2;
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
Vector Scene::getColor(const Ray& r, int rebound, bool lastDiffuse) {
    if (rebound > 5) {
        return Vector(0., 0., 0.);
    }

    Vector P, N, albedo;
    double t;
    bool mirror, transparent;
    int objectid;
    bool inter = this->intersect(r, P, N, albedo, mirror, transparent, t, objectid);
    Vector color(0, 0, 0);

    if (inter) {

        if (objectid == 0) {
            if (rebound == 0 || !lastDiffuse) {
                double R2 = pow(dynamic_cast<Sphere *>(objects[0])->R, 2);
                return Vector(I, I, I) / (4 * M_PI * M_PI * R2);
            }
            else
                return Vector(0., 0., 0.);
        }

        if (mirror) {
            // use the formula for reflection of vectors
            Vector reflectedDir = r.u - 2*dot(r.u, N)*N;
            Ray reflectedRay(P + 0.00001*N, reflectedDir);
            return getColor(reflectedRay, rebound + 1, false);
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
                    return getColor(reflectedRay, rebound + 1, false);
                }
                // normal component
                Vector Tn = -sqrt(rad) * N2;

                // the refracted vector is made up of the tangential and normal component
                Vector refractedDir = Tt + Tn;
                Ray refractedRay(P - 0.00001 * N2, refractedDir);
                return getColor(refractedRay, rebound + 1, false);
            }

            // direct lighting
            Vector PL = L - P;
            PL = PL.getNormalized();
            Vector w = random_cos(-PL);
            Vector xprime = w*dynamic_cast<Sphere*>(objects[0])->R + dynamic_cast<Sphere*>(objects[0])->O;
            Vector Pxprime = xprime - P;
            double d = sqrt(Pxprime.sqrNorm());
            Pxprime = Pxprime/d;

            Vector shadowP, shadowN, shadowAlbedo;
            double shadowt;
            bool shadowMirror, shadowTransp;
            int objectid;
            Ray shadowRay(P + 0.00001 * N, Pxprime);
            bool shadowInter = this->intersect(shadowRay, shadowP, shadowN, shadowAlbedo, shadowMirror, shadowTransp, shadowt, objectid);
            if (shadowInter && shadowt < d-0.0001) {
                color = Vector(0., 0., 0.);
            } else {
                double R2 = pow(dynamic_cast<Sphere*>(objects[0])->R, 2);
                double proba = std::max(0., dot(-PL, w)) / (M_PI*R2);
                double J = std::max(0., dot(w, -Pxprime)) / (d*d);
                color = I / (4 * M_PI * M_PI * R2) * albedo / M_PI * std::max(0., dot(N, Pxprime)) * J/proba;
            }

            // indirect lighting
            Vector wiDir = random_cos(N);
            Ray wiRay(P + 0.00001*N, wiDir);
            color += albedo*getColor(wiRay, rebound + 1, true);
        }
    }
    return color;
}