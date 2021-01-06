//
// Created by Martin Voigt on 06.01.21.
//

#include <cmath>
#include "Vector.h"


Vector::Vector(double x, double y, double z) {
        coords[0] = x;
        coords[1] = y;
        coords[2] = z;
};
double Vector::operator[](int i) const {
    return coords[i];
}

double &Vector::operator[](int i) {
    return coords[i];
}

double Vector::sqrNorm() {
    return coords[0]*coords[0] + coords[1]*coords[1] + coords[2]*coords[2];
}

Vector Vector::getNormalized() {
    double n = sqrt(sqrNorm());
    return Vector(coords[0]/n, coords[1]/n, coords[2]/n);
}

Vector operator+(const Vector &a, const Vector &b) {
    return Vector(a[0] + b[0], a[1] + b[1], a[2] + b[2]);
}

Vector operator-(const Vector &a, const Vector &b) {
    return Vector(a[0] - b[0], a[1] - b[1], a[2] - b[2]);
}

Vector operator*(double a, const Vector &b) {
    return Vector(a*b[0], a*b[1], a*b[2]);
}

Vector operator*(const Vector &a, double b) {
    return Vector(b*a[0], b*a[1], b*a[2]);
}

Vector operator/(const Vector &a, double b) {
    return Vector(a[0]/b, a[1]/b, a[2]/b);
}
double dot(const Vector &a, const Vector &b) {
    return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}
