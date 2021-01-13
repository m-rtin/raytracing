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
    Sphere S1(Vector(-10, 10, 0), 10, Vector(1, 0., 0.));
    Sphere S2(Vector(10, -10, 0), 10, Vector(1., 0., 1.));
    scene.objects.push_back(S1);
    scene.objects.push_back(S2);

    // camera angle in rad
    double fov = 60*M_PI/180;

    // light intensity
    double I = 5E9;

    Vector rho(1, 0, 0);
    Vector L(-10, 20, 40);

    std::vector<unsigned char> image(W*H * 3, 0);
    for (int i = 0; i < H; i++) {
        for (int j = 0; j < W; j++) {

            // create ray from pixel coordinates
            Vector u(j - W/2, i - H/2, -W/(2.*tan(fov/2)));
            u = u.getNormalized();
            Ray r(C, u);

            // check if ray intersects with an object in the scene
            Vector P, N, albedo;
            double t;
            bool inter = scene.intersect(r, P, N, albedo, t);
            Vector color(0, 0, 0);

            if (inter) {
                // Lambertian model for shadows
                Vector PL = L - P;
                double d = sqrt(PL.sqrNorm());
                Vector shadowP, shadowN, shadowAlbedo;
                double shadowt;
                Ray shadowRay(P, PL/d);
                bool shadowInter = scene.intersect(shadowRay, shadowP, shadowN, shadowAlbedo, shadowt);
                if (shadowInter && shadowt < d) {
                    color = Vector(0., 0., 0.);
                } else {
                    color = I/(4*M_PI*d*d)*albedo/M_PI*std::max(0., dot(N, PL/d));
                }
            }

            image[((H - i - 1)*W + j)* 3 + 0] = std::min(255.0, pow(color[0], 0.45));
            image[((H - i - 1)*W + j)* 3 + 1] = std::min(255.0, pow(color[1], 0.45));
            image[((H - i - 1)*W + j)* 3 + 2] = std::min(255.0, pow(color[2], 0.45));
        }
    }
    stbi_write_png("image4_with_shadows.png", W, H, 3, &image[0], 0);

    return 0;
}
