#include <iostream>
#include <vector>
#include "Sphere.h"
#include "Color.h"

class CornellBox {
    private:
        std::vector<float> eye = {0, 0, -4};
        std::vector<float> lookAt = {0, 0, 6};
        int fov = 36;

        std::vector<Sphere> scene;


    void populateScene() {
        scene.push_back(Sphere({-1001, 0, 0}, 1000, red));
        scene.push_back(Sphere({1001, 0, 0}, 1000, {0, 0, 1}));
    }
};
/*


Sphere(Center=[-1001,
0,
0], r=1000, Color=
        Red) a
        Sphere(Center=[ 1001,
0,
0], r=1000, Color=
        Blue) b
        Sphere(Center=[
0,
0,1001], r=1000, Color=
        Gray) c
        Sphere(Center=[
0, -1001,
0], r=1000, Color=
        Gray) d
        Sphere(Center=[
0, 1001,
0], r=1000, Color=
        White) e
        Sphere(Center=[ -0.6, -0.7,-0.6], r= 0.3, Color=
        Yellow) f
        Sphere(Center=[ 0.3, -0.4, 0.3], r= 0.6, Color=LightCyan) g
}
(Vec3 o, Vec3 d) CreateEyeRay(Vec3 Eye, Vec3 LookAt,
float FOV, Vec2 Pixel) { … }
f
        d
HitPoint FindClosestHitPoint(Scene s, Vec3 o, Vec3 d) { … }
Color ComputeColor(Scene s, Vec3 o, Vec3 d) { … }*/
