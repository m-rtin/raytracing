//
// Created by Martin Voigt on 10.02.21.
//

#include "TriangleMesh.h"
#include <list>

TriangleMesh::TriangleMesh(const Vector& albedo, bool mirror = false, bool transparent = false) {
    this->albedo = albedo;
    isMirror = mirror;
    isTransparent = transparent;
    BVH = new Node;
};

BoundingBox TriangleMesh::buildBB(int beginning, int end) {
    BoundingBox bb;
    bb.mini = Vector(1E9, 1E9, 1E9);
    bb.maxi = Vector(-1E9, -1E9, -1E9);
    for (int i = beginning; i < end; i++) {
        for (int j = 0; j < 3; j++) {
            bb.mini[j] = std::min(bb.mini[j], vertices[indices[i].vtxi][j]);
            bb.maxi[j] = std::max(bb.maxi[j], vertices[indices[i].vtxi][j]);
            bb.mini[j] = std::min(bb.mini[j], vertices[indices[i].vtxj][j]);
            bb.maxi[j] = std::max(bb.maxi[j], vertices[indices[i].vtxj][j]);
            bb.mini[j] = std::min(bb.mini[j], vertices[indices[i].vtxk][j]);
            bb.maxi[j] = std::max(bb.maxi[j], vertices[indices[i].vtxk][j]);
        }
    }
    return bb;
}

void TriangleMesh::buildBVH(Node* n, int beginning, int end) {
    n->b = buildBB(n->beginning, n->end);
    n->beginning = beginning;
    n->end = end;
    Vector diag = n->b.maxi - n->b.mini;
    int dim;
    if (diag[0] >= diag[1] && diag[0] >= diag[2]) {
        dim = 0;
    } else {
        if (diag[1] >= diag[0] && diag[1] >= diag[2]) {
            dim = 1;
        } else {
            dim = 2;
        }
    }
    double middle = (n->b.mini[dim] + n->b.maxi[dim])*0.5;
    int indicePivot = n->beginning;
    for (int i = n->beginning; i < n->end; i++) {
        double middleTriangle = (vertices[indices[i].vtxi][dim] + vertices[indices[i].vtxj][dim] + vertices[indices[i].vtxk][dim])/3.0;
        if (middleTriangle < middle) {
            std::swap(indices[i], indices[indicePivot]);
            indicePivot++;
        }
    }
    n->fg = NULL;
    n->fd = NULL;
    if (indicePivot == beginning || indicePivot == end || (end - beginning < 5)) {
        return;
    }
    n->fg = new Node;
    n->fd = new Node;

    buildBVH(n->fg, n->beginning, indicePivot);
    buildBVH(n->fd, indicePivot, n->end);
}

bool TriangleMesh::intersect(const Ray& r, Vector& P, Vector& normal, double &t) {
    if (!BVH->b.intersect(r)) return false;
    t = 1E9;
    bool hasInter = false;
    std::list<Node*> l;
    l.push_back(BVH);
    while (!l.empty()) {
        Node* c = l.front();
        l.pop_front();
        if (c->fg) {
            if (c->fg->b.intersect(r)) {
                l.push_front(c->fg);
            }
            if (c->fd->b.intersect(r)) {
                l.push_front(c->fd);
            }
        } else {
            for (int i = c->beginning; i <c->end; i++) {
                const Vector &A = vertices[indices[i].vtxi];
                const Vector &B = vertices[indices[i].vtxj];
                const Vector &C = vertices[indices[i].vtxk];

                Vector e1 = B - A;
                Vector e2 = C - A;
                Vector N = cross(e1, e2);
                Vector AO = r.C - A;
                Vector AOu = cross(AO, r.u);
                double invUN = 1./dot(r.u, N);
                double beta = - dot(e2, AOu)*invUN;
                double gamma = dot(e1, AOu)*invUN;
                double alpha = 1 - beta - gamma;
                double localt = - dot(AO, N)*invUN;
                if (beta >= 0 && gamma >= 0 && beta <= 1 && gamma <= 1 && alpha >= 0 && localt > 0) {
                    hasInter = true;
                    if (localt < t) {
                        t = localt;
                        normal = N.getNormalized();
                        P = r.C + t * r.u;
                    }
                }
            }
        }
    }

    return hasInter;
}

void TriangleMesh::readOBJ(const char* obj) {

    char matfile[255];
    char grp[255];

    FILE* f;
    f = fopen(obj, "r");
    int curGroup = -1;
    while (!feof(f)) {
        char line[255];
        if (!fgets(line, 255, f)) break;

        std::string linetrim(line);
        linetrim.erase(linetrim.find_last_not_of(" \r\t") + 1);
        strcpy(line, linetrim.c_str());

        if (line[0] == 'u' && line[1] == 's') {
            sscanf(line, "usemtl %[^\n]\n", grp);
            curGroup++;
        }

        if (line[0] == 'v' && line[1] == ' ') {
            Vector vec;

            Vector col;
            if (sscanf(line, "v %lf %lf %lf %lf %lf %lf\n", &vec[0], &vec[1], &vec[2], &col[0], &col[1], &col[2]) == 6) {
                col[0] = std::min(1., std::max(0., col[0]));
                col[1] = std::min(1., std::max(0., col[1]));
                col[2] = std::min(1., std::max(0., col[2]));

                vertices.push_back(vec);
                vertexcolors.push_back(col);

            } else {
                sscanf(line, "v %lf %lf %lf\n", &vec[0], &vec[1], &vec[2]);
                vertices.push_back(vec);
            }
        }
        if (line[0] == 'v' && line[1] == 'n') {
            Vector vec;
            sscanf(line, "vn %lf %lf %lf\n", &vec[0], &vec[1], &vec[2]);
            normals.push_back(vec);
        }
        if (line[0] == 'v' && line[1] == 't') {
            Vector vec;
            sscanf(line, "vt %lf %lf\n", &vec[0], &vec[1]);
            uvs.push_back(vec);
        }
        if (line[0] == 'f') {
            TriangleIndices t;
            int i0, i1, i2, i3;
            int j0, j1, j2, j3;
            int k0, k1, k2, k3;
            int nn;
            t.group = curGroup;

            char* consumedline = line + 1;
            int offset;

            nn = sscanf(consumedline, "%u/%u/%u %u/%u/%u %u/%u/%u%n", &i0, &j0, &k0, &i1, &j1, &k1, &i2, &j2, &k2, &offset);
            if (nn == 9) {
                if (i0 < 0) t.vtxi = vertices.size() + i0; else t.vtxi = i0 - 1;
                if (i1 < 0) t.vtxj = vertices.size() + i1; else t.vtxj = i1 - 1;
                if (i2 < 0) t.vtxk = vertices.size() + i2; else t.vtxk = i2 - 1;
                if (j0 < 0) t.uvi = uvs.size() + j0; else   t.uvi = j0 - 1;
                if (j1 < 0) t.uvj = uvs.size() + j1; else   t.uvj = j1 - 1;
                if (j2 < 0) t.uvk = uvs.size() + j2; else   t.uvk = j2 - 1;
                if (k0 < 0) t.ni = normals.size() + k0; else    t.ni = k0 - 1;
                if (k1 < 0) t.nj = normals.size() + k1; else    t.nj = k1 - 1;
                if (k2 < 0) t.nk = normals.size() + k2; else    t.nk = k2 - 1;
                indices.push_back(t);
            } else {
                nn = sscanf(consumedline, "%u/%u %u/%u %u/%u%n", &i0, &j0, &i1, &j1, &i2, &j2, &offset);
                if (nn == 6) {
                    if (i0 < 0) t.vtxi = vertices.size() + i0; else t.vtxi = i0 - 1;
                    if (i1 < 0) t.vtxj = vertices.size() + i1; else t.vtxj = i1 - 1;
                    if (i2 < 0) t.vtxk = vertices.size() + i2; else t.vtxk = i2 - 1;
                    if (j0 < 0) t.uvi = uvs.size() + j0; else   t.uvi = j0 - 1;
                    if (j1 < 0) t.uvj = uvs.size() + j1; else   t.uvj = j1 - 1;
                    if (j2 < 0) t.uvk = uvs.size() + j2; else   t.uvk = j2 - 1;
                    indices.push_back(t);
                } else {
                    nn = sscanf(consumedline, "%u %u %u%n", &i0, &i1, &i2, &offset);
                    if (nn == 3) {
                        if (i0 < 0) t.vtxi = vertices.size() + i0; else t.vtxi = i0 - 1;
                        if (i1 < 0) t.vtxj = vertices.size() + i1; else t.vtxj = i1 - 1;
                        if (i2 < 0) t.vtxk = vertices.size() + i2; else t.vtxk = i2 - 1;
                        indices.push_back(t);
                    } else {
                        nn = sscanf(consumedline, "%u//%u %u//%u %u//%u%n", &i0, &k0, &i1, &k1, &i2, &k2, &offset);
                        if (i0 < 0) t.vtxi = vertices.size() + i0; else t.vtxi = i0 - 1;
                        if (i1 < 0) t.vtxj = vertices.size() + i1; else t.vtxj = i1 - 1;
                        if (i2 < 0) t.vtxk = vertices.size() + i2; else t.vtxk = i2 - 1;
                        if (k0 < 0) t.ni = normals.size() + k0; else    t.ni = k0 - 1;
                        if (k1 < 0) t.nj = normals.size() + k1; else    t.nj = k1 - 1;
                        if (k2 < 0) t.nk = normals.size() + k2; else    t.nk = k2 - 1;
                        indices.push_back(t);
                    }
                }
            }

            consumedline = consumedline + offset;

            while (true) {
                if (consumedline[0] == '\n') break;
                if (consumedline[0] == '\0') break;
                nn = sscanf(consumedline, "%u/%u/%u%n", &i3, &j3, &k3, &offset);
                TriangleIndices t2;
                t2.group = curGroup;
                if (nn == 3) {
                    if (i0 < 0) t2.vtxi = vertices.size() + i0; else    t2.vtxi = i0 - 1;
                    if (i2 < 0) t2.vtxj = vertices.size() + i2; else    t2.vtxj = i2 - 1;
                    if (i3 < 0) t2.vtxk = vertices.size() + i3; else    t2.vtxk = i3 - 1;
                    if (j0 < 0) t2.uvi = uvs.size() + j0; else  t2.uvi = j0 - 1;
                    if (j2 < 0) t2.uvj = uvs.size() + j2; else  t2.uvj = j2 - 1;
                    if (j3 < 0) t2.uvk = uvs.size() + j3; else  t2.uvk = j3 - 1;
                    if (k0 < 0) t2.ni = normals.size() + k0; else   t2.ni = k0 - 1;
                    if (k2 < 0) t2.nj = normals.size() + k2; else   t2.nj = k2 - 1;
                    if (k3 < 0) t2.nk = normals.size() + k3; else   t2.nk = k3 - 1;
                    indices.push_back(t2);
                    consumedline = consumedline + offset;
                    i2 = i3;
                    j2 = j3;
                    k2 = k3;
                } else {
                    nn = sscanf(consumedline, "%u/%u%n", &i3, &j3, &offset);
                    if (nn == 2) {
                        if (i0 < 0) t2.vtxi = vertices.size() + i0; else    t2.vtxi = i0 - 1;
                        if (i2 < 0) t2.vtxj = vertices.size() + i2; else    t2.vtxj = i2 - 1;
                        if (i3 < 0) t2.vtxk = vertices.size() + i3; else    t2.vtxk = i3 - 1;
                        if (j0 < 0) t2.uvi = uvs.size() + j0; else  t2.uvi = j0 - 1;
                        if (j2 < 0) t2.uvj = uvs.size() + j2; else  t2.uvj = j2 - 1;
                        if (j3 < 0) t2.uvk = uvs.size() + j3; else  t2.uvk = j3 - 1;
                        consumedline = consumedline + offset;
                        i2 = i3;
                        j2 = j3;
                        indices.push_back(t2);
                    } else {
                        nn = sscanf(consumedline, "%u//%u%n", &i3, &k3, &offset);
                        if (nn == 2) {
                            if (i0 < 0) t2.vtxi = vertices.size() + i0; else    t2.vtxi = i0 - 1;
                            if (i2 < 0) t2.vtxj = vertices.size() + i2; else    t2.vtxj = i2 - 1;
                            if (i3 < 0) t2.vtxk = vertices.size() + i3; else    t2.vtxk = i3 - 1;
                            if (k0 < 0) t2.ni = normals.size() + k0; else   t2.ni = k0 - 1;
                            if (k2 < 0) t2.nj = normals.size() + k2; else   t2.nj = k2 - 1;
                            if (k3 < 0) t2.nk = normals.size() + k3; else   t2.nk = k3 - 1;
                            consumedline = consumedline + offset;
                            i2 = i3;
                            k2 = k3;
                            indices.push_back(t2);
                        } else {
                            nn = sscanf(consumedline, "%u%n", &i3, &offset);
                            if (nn == 1) {
                                if (i0 < 0) t2.vtxi = vertices.size() + i0; else    t2.vtxi = i0 - 1;
                                if (i2 < 0) t2.vtxj = vertices.size() + i2; else    t2.vtxj = i2 - 1;
                                if (i3 < 0) t2.vtxk = vertices.size() + i3; else    t2.vtxk = i3 - 1;
                                consumedline = consumedline + offset;
                                i2 = i3;
                                indices.push_back(t2);
                            } else {
                                consumedline = consumedline + 1;
                            }
                        }
                    }
                }
            }

        }

    }
    fclose(f);

}
