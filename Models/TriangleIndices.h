//
// Created by Martin Voigt on 10.02.21.
//

#ifndef HELLOWORLD_TRIANGLEINDICES_H
#define HELLOWORLD_TRIANGLEINDICES_H

#include <string>
#include <iostream>
#include <stdio.h>
#include <algorithm>
#include <vector>
#include "Vector.h"
#include "BoundingBox.h"

class TriangleIndices {
public:
    TriangleIndices(int vtxi = -1, int vtxj = -1, int vtxk = -1, int ni = -1, int nj = -1, int nk = -1, int uvi = -1, int uvj = -1, int uvk = -1, int group = -1, bool added = false) : vtxi(vtxi), vtxj(vtxj), vtxk(vtxk), uvi(uvi), uvj(uvj), uvk(uvk), ni(ni), nj(nj), nk(nk), group(group) {
    };
    int vtxi, vtxj, vtxk; // indices within the vertex coordinates array
    int uvi, uvj, uvk;  // indices within the uv coordinates array
    int ni, nj, nk;  // indices within the normals array
    int group;       // face group
};
#endif //HELLOWORLD_TRIANGLEINDICES_H
