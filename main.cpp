#define _CRT_SECURE_NO_WARNINGS 1
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include <algorithm>
#include "Models/Vector.h"
#include "Models/Ray.h"
#include "Models/Sphere.h"
#include "Models/Scene.h"


int main() {
    int W = 512;
    int H = 512;

    // camera position
    Vector C(0, 0, 55);

    // Create a scene
    Scene scene;
    Sphere S1(Vector(-10, 10, 0), 10, Vector(1, 0., 0.), false, true);
    Sphere S2(Vector(10, 20, 0), 3, Vector(1., 0., 1.), true, false);
    Sphere S3(Vector(10, 20, -40), 5, Vector(1., 0., 1.));

    Sphere floor(Vector(0, -1000, 0), 990, Vector(1., 1., 1.));
    Sphere leftWall(Vector(-1000, 0, 0), 940, Vector(1., 0., 0.));
    Sphere rightWall(Vector(1000, 0, 0), 940, Vector(0., 1., 0.));
    Sphere backgroundWall(Vector(0, 0, -1000), 940, Vector(0., 0.5, 0.5));
    Sphere frontWall(Vector(0, 0, 1000), 940, Vector(1., 1., 0.));
    Sphere ceiling(Vector(0, 1000, 0), 940, Vector(1., 1., 1.));

    scene.objects.push_back(S1);
    scene.objects.push_back(S2);
    scene.objects.push_back(S3);
    scene.objects.push_back(floor);
    scene.objects.push_back(leftWall);
    scene.objects.push_back(rightWall);
    scene.objects.push_back(backgroundWall);
    scene.objects.push_back(frontWall);
    scene.objects.push_back(ceiling);

    // camera angle in rad
    double fov = 60*M_PI/180;

    // light intensity
    scene.I = 5E9;

    Vector rho(1, 0, 0);
    scene.L = Vector(-10, 20, 40);

    std::vector<unsigned char> image(W*H * 3, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {

            // create ray from pixel coordinates
            Vector u(j - W/2, i - H/2, -W/(2.*tan(fov/2)));
            u = u.getNormalized();
            Ray r(C, u);

            Vector color = scene.getColor(r, 0);

            // gamma correction
            double gamma = 2.2;

            image[((H - i - 1)*W + j)* 3 + 0] = std::min(255.0, pow(color[0], 1/gamma));
            image[((H - i - 1)*W + j)* 3 + 1] = std::min(255.0, pow(color[1], 1/gamma));
            image[((H - i - 1)*W + j)* 3 + 2] = std::min(255.0, pow(color[2], 1/gamma));
        }
    }
    stbi_write_png("image6_transp.png", W, H, 3, &image[0], 0);

    return 0;
}
