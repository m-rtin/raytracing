#define _CRT_SECURE_NO_WARNINGS 1
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <random>
static std::default_random_engine engine(10); // random seed = 10
static std::uniform_real_distribution<double> uniform(0, 1);

#include <algorithm>
#include "Models/Vector.h"
#include "Models/Ray.h"
#include "Models/Sphere.h"
#include "Models/Scene.h"
#include "Models/TriangleIndices.h"
#include "Models/TriangleMesh.h"


int main() {
    int W = 128;
    int H = 128;

    // camera position
    Vector C(0, 0, 55);

    // Create a scene
    Scene scene;
    // light intensity
    scene.I = 5E9;
    // radiance
    scene.L = Vector(-10, 20, 40);

    Sphere lightBall(scene.L, 5, Vector(1, 1., 1.));
    Sphere S1(Vector(0, 0, 0), 10, Vector(1, 0., 0.), false, true);
    Sphere S2(Vector(-10, 0, -20), 3, Vector(1., 0., 1.), true, false);
    Sphere S3(Vector(10, 0, 20), 5, Vector(1., 0., 1.));

    Sphere floor(Vector(0, -1000, 0), 990, Vector(1., 1., 1.));
    Sphere leftWall(Vector(-1000, 0, 0), 940, Vector(1., 0., 0.));
    Sphere rightWall(Vector(1000, 0, 0), 940, Vector(0., 1., 0.));
    Sphere backgroundWall(Vector(0, 0, -1000), 940, Vector(0., 0.5, 0.5));
    Sphere frontWall(Vector(0, 0, 1000), 940, Vector(1., 1., 0.));
    Sphere ceiling(Vector(0, 1000, 0), 940, Vector(1., 1., 1.));

    TriangleMesh m(Vector(1., 1., 1.));
    m.readOBJ("13463_Australian_Cattle_Dog_v3.obj");
    for (int i = 0; i <m.vertices.size(); i++) {
        std::swap(m.vertices[i][1], m.vertices[i][2]);
        m.vertices[i][2] += 22;
        m.vertices[i][1] -= 10;
    }
    for (int i = 0; i <m.normals.size(); i++) {
        std::swap(m.normals[i][1], m.normals[i][2]);
    }

    m.buildBB();

    scene.objects.push_back(&lightBall);
    // scene.objects.push_back(&S1);
    // scene.objects.push_back(&S2);
    // scene.objects.push_back(&S3);
    scene.objects.push_back(&floor);
    scene.objects.push_back(&leftWall);
    scene.objects.push_back(&rightWall);
    scene.objects.push_back(&backgroundWall);
    scene.objects.push_back(&frontWall);
    scene.objects.push_back(&ceiling);

    // camera angle in rad
    double fov = 60*M_PI/180;

    int numberOfRays = 1;

    std::vector<unsigned char> image(W*H * 3, 0);

#pragma omp parallel for schedule(dynamic, 1)
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {

            Vector color(0, 0, 0);
            for (int k = 0; k < numberOfRays; k++) {
                double u1 = uniform(engine);
                double u2 = uniform(engine);
                double x1 = 0.25*cos(2*M_PI*u1)*sqrt(-2 * log(u2));
                double x2 = 0.25*sin(2*M_PI*u1)*sqrt(-2 * log(u2));
                u1 = uniform(engine);
                u2 = uniform(engine);
                double x3 = 0.01*cos(2*M_PI*u1)*sqrt(-2 * log(u2));
                double x4 = 0.01*sin(2*M_PI*u1)*sqrt(-2 * log(u2));



                // create ray from pixel coordinates
                Vector u(j - W/2 + x2 + 0.5, i - H/2 + x1 + 0.5, -W/(2.*tan(fov/2)));
                u = u.getNormalized();
                Vector target = C + 55 * u;
                Vector Cprim = C + Vector(x3, x4, 0);
                Vector uprime = (target - Cprim).getNormalized();

                Ray r(Cprim, uprime);
                color += scene.getColor(r, 0, false);
            }
            color = color/numberOfRays;

            // gamma correction
            double gamma = 2.2;

            image[((H - i - 1)*W + j)* 3 + 0] = std::min(255.0, pow(color[0], 1/gamma));
            image[((H - i - 1)*W + j)* 3 + 1] = std::min(255.0, pow(color[1], 1/gamma));
            image[((H - i - 1)*W + j)* 3 + 2] = std::min(255.0, pow(color[2], 1/gamma));
        }
    }
    stbi_write_png("image9_dog.png", W, H, 3, &image[0], 0);

    return 0;
}
