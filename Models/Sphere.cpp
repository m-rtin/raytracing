//
// Created by Martin Voigt on 06.01.21.
//

#include <cmath>
#include "Sphere.h"

Sphere::Sphere(const Vector& O, double R, const Vector& albedo, bool isMirror, bool isTransparent): O(O), R(R), albedo(albedo), isMirror(isMirror), isTransparent(isTransparent) {};

/**
 * Check if a given ray intersects with a given sphere.
 *
 * @param r incoming ray
 * @param P intersection point
 * @param N normal vector of sphere at intersection point
 * @param t length of direction vector to intersect with the sphere
 * @return true if the input ray intersects with the sphere, false otherwise
 */
bool Sphere::intersect(const Ray& r, Vector& P, Vector& N, double &t) {
    // if the ray intersects with the sphere, the following equation is satisfied
    // t^2 + 2t*<u, C-O> + ||O-C||^2 - R^2 = 0
    // in the following we represent this equation using the general form of quadratic equations
    // a*t^2 + b*t + c = 0
    double a = 1;
    double b = 2*dot(r.u, r.C - this->O);
    double c = (r.C - this->O).sqrNorm() - this->R*this->R;

    double discriminant = b*b - 4*a*c;

    if (discriminant < 0) return false;

    double sqDelta = sqrt(discriminant);
    double t2 = (-b + sqDelta) / (2*a);

    if (t2 < 0) return false;

    double t1 = (-b - sqDelta) / (2*a);
    if (t1 > 0)
        t = t1;
    else
        t = t2;

    P = r.C + t*r.u;
    N = (P - O).getNormalized();

    return true;
}
