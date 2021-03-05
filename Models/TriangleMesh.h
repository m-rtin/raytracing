//
// Created by Martin Voigt on 10.02.21.
//

#ifndef HELLOWORLD_TRIANGLEMESH_H
#define HELLOWORLD_TRIANGLEMESH_H

#include "Object.h"
#include "Vector.h"
#include "BoundingBox.h"
#include "TriangleIndices.h"
#include "Node.h"

class TriangleMesh : public Object {
public:
    ~TriangleMesh() {}
    TriangleMesh(const Vector& albedo, bool mirror, bool transparent);
    BoundingBox buildBB(int beginning, int end);
    void buildBVH(Node* n, int beginning, int end);
    bool intersect(const Ray& r, Vector& P, Vector& normal, double &t);
    void readOBJ(const char* obj);

    std::vector<TriangleIndices> indices;
    std::vector<Vector> vertices;
    std::vector<Vector> normals;
    std::vector<Vector> uvs;
    std::vector<Vector> vertexcolors;
    BoundingBox bb;
    Node* BVH;
};


#endif //HELLOWORLD_TRIANGLEMESH_H
