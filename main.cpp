#define _CRT_SECURE_NO_WARNINGS 1
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <algorithm>
#include "Vector.h"



class Ray {
public:
    Ray(const Vector& C, const Vector& u): C(C), u(u) {

    }

    Vector C, u;
};

class Sphere {
public:
    Sphere(const Vector& O, double R, const Vector& albedo): O(O), R(R), albedo(albedo) {

    }
    // P intersection point, N normal vector
    bool intersect(const Ray& r, Vector& P, Vector& N, double &t) {
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
    Vector O;
    double R;
    Vector albedo;
};

class Scene {
    public:
    Scene() {};
    bool intersect(const Ray& r, Vector& P, Vector& N, Vector &albedo) {
        double t = 1E10;
        bool hasInter = false;
        for (int i = 0; i<objects.size(); i++) {
            Vector localP, localN;
            double localt;
            if (objects[i].intersect(r, localP, localN, localt) && localt < t) {
               t = localt;
               hasInter = true;
               albedo = objects[i].albedo;
               P = localP;
               N = localN; 
            }
        }

        return hasInter;
    }
    std::vector<Sphere> objects;
};

int main() {
    int W = 512;
    int H = 512;

    Vector C(0, 0, 55);
    Scene scene;
    Sphere S1(Vector(0, 0, 0), 10, Vector(1, 0., 0.));
    Sphere S2(Vector(8, 0, 0), 10, Vector(0., 0., 1.));
    scene.objects.push_back(S1);
    scene.objects.push_back(S2);
    // angle in rad
    double fov = 60*M_PI/180;
    double I = 1E7;
    Vector rho(1, 0, 0);
    Vector L(-10, 20, 40);

    std::vector<unsigned char> image(W*H * 3, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {

            // create direction vector
            Vector u(j - W/2, i - H/2, -W/(2.*tan(fov/2)));
            u = u.getNormalized();

            Ray r(C, u);

            Vector P, N, albedo;
            bool inter = scene.intersect(r, P, N, albedo);
            Vector color(0, 0, 0);

            if (inter) {
                Vector PL = L - P;
                double d = sqrt(PL.sqrNorm());
                color = I/(4*M_PI*d*d)*albedo/M_PI*std::max(0., dot(N, PL/d));
            }

            image[((H - i - 1)*W + j)* 3 + 0] = std::min(255.0, color[0]);
            image[((H - i - 1)*W + j)* 3 + 1] = std::min(255.0, color[1]);
            image[((H - i - 1)*W + j)* 3 + 2] = std::min(255.0, color[2]);
        }
    }
    stbi_write_png("image.png", W, H, 3, &image[0], 0);

    return 0;
}
