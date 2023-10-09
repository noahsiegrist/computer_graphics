#include <math.h>
#include "ScreenPainter.h"
#include "CornellBox.h"
#include <iostream>

bool isClose(Vec3 a, Vec3 b) {
    if(a.null) {
        return b.null;
    }
    std::cout << "The difference is " << (a - b).length() << std::endl;
    return (a - b).length() < 0.0001;
}

bool testIntersects() {
    bool passed = true;
    Sphere sphere(Vec3(0, 0, 0), 1.0f, BLACK);  // A sphere centered at origin with radius 1

    // Test case 1: Ray intersects sphere at two points
    Vec3 origin1(0, 0, -2);
    Vec3 to1(0, 0, 2);
    Vec3 expectedIntersection1(0, 0, -1);
    if (isClose(origin1 + to1 *sphere.intersect(origin1, to1), expectedIntersection1)) {
        std::cout << "Test case 1 passed!" << std::endl;
    } else {
        std::cout << "Test case 1 failed!" << std::endl;
        passed = false;
    }

    // Test case 2: Ray touches the sphere at one point
    Vec3 origin2(0, 0, -4);
    Vec3 to2(1, 0, 0);
    Vec3 expectedIntersection2(0.88235294117647067, 0, -0.47058823529411731);
    if (isClose(origin2 + to2 *sphere.intersect(origin2, to2), expectedIntersection2)) {
        std::cout << "Test case 2 passed!" << std::endl;
    } else {
        std::cout << "Test case 2 failed!" << std::endl;
        passed = false;
    }

    // Test case 3: Ray misses the sphere
    Vec3 origin3(0, 2, 0);
    Vec3 to3(0, 3, 0);
    Vec3 expectedIntersection3 = {};  // No intersection
    if (isClose(origin3 + to3 * sphere.intersect(origin3, to3), expectedIntersection3)) {
        std::cout << "Test case 3 passed!" << std::endl;
    } else {
        std::cout << "Test case 3 failed!" << std::endl;
        passed = false;
    }

    // Test case 4: ray passes through the sphere
    Vec3 origin4(0, 0, -4);
    Vec3 to4(0, 0, 1);
    Vec3 expectedIntersection4(0, 0, -1);
    if (isClose(origin4 + to4 * sphere.intersect(origin4, to4), expectedIntersection4)) {
        std::cout << "Test case 4 passed!" << std::endl;
    } else {
        std::cout << "Test case 4 failed!" << std::endl;
        passed = false;
    }


}

int main() {


    ScreenPainter painter(SCREEN_WIDTH, SCREEN_HEIGHT);

    /**
     * This is the code that draws the cornel box.
     */


    CornellBox cornellBox({0, 0, -4}, {0, 0, 6});
    //CornellBox cornellBox(110, {-0.9, -0.5, 0.9}, {0, 0, 0});

    cornellBox.display(painter);


    /*
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        for(int y = 0; y < SCREEN_HEIGHT; y++) {

            float factor = sin(x/30.f) + sin( y/50.f)* sin( y/50.f);
            float r = cos(factor);
            float g = sin(factor);
            float b = cos(factor);
            painter.setPixel(x, y, r, g, b);
        }
    }
     */


    /**
     * This is the code that draws a gradient.
     *
    for (int x = 0; x < SCREEN_WIDTH; x++) {
        float factor = (float)x / SCREEN_WIDTH;
        float r = factor;
        float g = 1.f - factor;
        for (int y = 0; y < SCREEN_HEIGHT; y++) {
            painter.setPixel(x, y, r, g, 0);
        }
    }*/

    painter.display();

    painter.wait();
    painter.clear();
    return 0;
}
