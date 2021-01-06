//
// Created by Martin Voigt on 06.01.21.
//

#include <cmath>
#include "Sphere.h"

Sphere::Sphere(const Vector& O, double R, const Vector& albedo): O(O), R(R), albedo(albedo) {};

bool Sphere::intersect(const Ray& r, Vector& P, Vector& N, double &t) {
    // solves a*t^2 + b*t + c = 0
    double a = 1;
    double b = 2*dot(r.u, r.C - O);
    double c = (r.C-O).sqrNorm() - R*R;
    double delta = b*b - 4*a*c;

    if (delta < 0) return false;

    double sqDelta = sqrt(delta);
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
