//
// Created by Martin Voigt on 06.01.21.
//

#ifndef HELLOWORLD_VECTOR_H
#define HELLOWORLD_VECTOR_H

class Vector {
public:
    explicit Vector (double x=0, double y=0, double z=0);
    double operator[](int i) const;
    double &operator[](int i);
    double sqrNorm();
    Vector getNormalized();
private:
    double coords[3];
};

Vector operator+(const Vector &a, const Vector &b);
Vector operator-(const Vector &a, const Vector &b);
Vector operator-(const Vector &a);
Vector operator*(double a, const Vector &b);
Vector operator*(const Vector &a, double b);
Vector operator/(const Vector &a, double b);

double dot(const Vector &a, const Vector &b);

#endif //HELLOWORLD_VECTOR_H
