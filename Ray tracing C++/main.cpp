#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <string>

#include "vector.cpp"
#include "ray.cpp"
#include "shape.cpp"
#include "camera.cpp"
#include "color.cpp"
#include "matarials.h"
#include "light.cpp"
#include "objParser.cpp"


using namespace std;

void scene(int b, int a, int w, int h, int width, int height, Camera &camera, ShapeSet &shapes, vector<vector<string>> &v)
{

    // int done = 0;
    int rayp = 1;
    srand(time(0));
    for (int y = h - 1; y >= a; y--)
    {
        for (int x = b; x < w; x++)
        {   
            int r, g, b;
            Ray ray = camera.getRay(x, y, width, height);
            Intersection intersection(ray);
            shapes.intersect(intersection);
            int depth = 1;
            intersection.getSurfaceLight(ray.direction, &shapes, depth);
            intersection.light.color.applyGammaCorrection(1.0, 2.2);
            intersection.light.color.clamp(0, 1);
            r = 255 * intersection.light.color.r;
            g = 255 * intersection.light.color.g;
            b = 255 * intersection.light.color.b;
            v[x][y] = to_string(r) + " " + to_string(g) + " " + to_string(b) + "\n";
        }

        // if (y%3 == 0) cout<<float(done) * 100/(w*h)<<" ...."<<endl;
    }
    cout << "DONE" << endl;
}

int main()
{

    int height = 1080;
    int width = 1920;

    Point cameraOrigin = Point(5.2, 4.5, 9);
    Vector cameraTraget = Vector(5.2, 4.5, 0.0);
    Vector cameraGuide = Vector(0, 1, 0);
    float cameraFOV = PI / 4;
    float cameraRatio = float(width) / float(height);

    class PerspectiveCamera camera = PerspectiveCamera(cameraOrigin, cameraTraget, cameraGuide, cameraFOV, cameraRatio);

    ShapeSet shapes;
    fileReader("LOGO.obj", &shapes);

    // Plane backWall(Point(0, -1, 0), Vector(0, 1, 0));
    // Matarial mat1 = Matarial(0.8, 0.0, 0.0, PI/30, 0.0f,
    //                          Color(0.5, 0.5, 0.5),
    //                          Color(0.5, 0.5, 0.5),
    //                          Color(1.0, 1.0, 1.0));
    // // Matarial mat1 = Matarial();
    // backWall.setMatarial(mat1);
    // // shapes.addShape(&backWall);
    // // shapes.addLight(&backWall);

    // Plane floor(Point(0, 0, -1), Vector(0, 0, 1));
    // Matarial mat2 = Matarial(0.8, 0.0, 0.0, PI / 50, 0.0f,
    //                          Color(0.4, 0.3, 0.2),
    //                          Color(0.4, 0.3, 0.2),
    //                          Color(1.0, 1.0, 1.0));
    // floor.setMatarial(mat2);
    // // shapes.addShape(&floor);

    // Sphere ball1(Point(4, 0, 0), 1);
    // Matarial mat3 = Matarial(0.02, 0.0, 0.0, PI / 100, 0.0f,
    //                          Color(0.2, 0.1, 0.05),
    //                          Color(0.2, 0.1, 0.05),
    //                          Color(1.0, 1.0, 1.0));
    // ball1.setMatarial(mat3);
    // // shapes.addShape(&ball1);

    // Sphere ball2(Point(0, 0, 0), 1);
    // Matarial mat4 = Matarial(0.02, 0.0, 0.0, PI / 100, 0.0f,
    //                          Color(0.1, 0.05, 0.2),
    //                          Color(0.1, 0.05, 0.2),
    //                          Color(1.0, 1.0, 1.0));
    // ball2.setMatarial(mat4);
    // // shapes.addShape(&ball2);

    // Sphere ball3(Point(0, 0, 4), 1);
    // Matarial mat5 = Matarial(0.02, 0.0, 0.0, PI / 100, 0.0f,
    //                          Color(0.05, 0.2, 0.1),
    //                          Color(0.05, 0.2, 0.1),
    //                          Color(1.0, 1.0, 1.0));
    // ball3.setMatarial(mat5);
    // // shapes.addShape(&ball3);

    Sphere light(Point(-500, -500, 500), 1);
    Matarial mat6 = Matarial(0.0, 0.0, 0.5, PI / 100, 0.0f, 0,
                             Color(1.0, 1.0, 1.0),
                             Color(1.0, 1.0, 1.0),
                             Color(1.0, 1.0, 1.0));
    light.setMatarial(mat6);
    shapes.addShape(&light);
    shapes.addLight(&light);

    // Triangle tri1(Point(4, 0, 0), Point(0, 0, 0), Point(0, 0, 4));
    // Matarial mat7 = Matarial(0.9, 0.0, 0.0, PI / 100, 0.0f,
    //                          Color(0.5, 0.4, 0.6),
    //                          Color(0.5, 0.4, 0.6),
    //                          Color(1.0, 1.0, 1.0));
    // tri1.setMatarial(mat7);
    // // shapes.addShape(&tri1); 


    // vector<Point> polyPoints = {Point(-4, 0, 0),
    //                             Point(-5, 0, 0), 
    //                             Point(-5, 0, 3),
    //                             Point(-4, 0, 3)};

    // Polygon poly(polyPoints);
    // Matarial mat8 = Matarial(0.9, 0.0, 0.0, PI / 100, 0.0f,
    //                          Color(0.8, 0.5, 0.2),
    //                          Color(0.8, 0.5, 0.2),
    //                          Color(1.0, 1.0, 1.0));
    // poly.setMatarial(mat8);
    // // shapes.addShape(&poly); 



    vector<vector<string>> v(width, vector<string>(height));

    // scene(0, 0, width, height, width, height, camera, shapes, v);
    
    int no = 8;
    int times = 1;
        thread th1(scene, 0 * width / no, 0, 1 * width / no, height, width, height, ref(camera), ref(shapes), ref(v));
        thread th2(scene, 1 * width / no, 0, 2 * width / no, height, width, height, ref(camera), ref(shapes), ref(v));
        thread th3(scene, 2 * width / no, 0, 3 * width / no, height, width, height, ref(camera), ref(shapes), ref(v));
        thread th4(scene, 3 * width / no, 0, 4 * width / no, height, width, height, ref(camera), ref(shapes), ref(v));
        thread th5(scene, 4 * width / no, 0, 5 * width / no, height, width, height, ref(camera), ref(shapes), ref(v));
        thread th6(scene, 5 * width / no, 0, 6 * width / no, height, width, height, ref(camera), ref(shapes), ref(v));
        thread th7(scene, 6 * width / no, 0, 7 * width / no, height, width, height, ref(camera), ref(shapes), ref(v));
        thread th8(scene, 7 * width / no, 0, 8 * width / no, height, width, height, ref(camera), ref(shapes), ref(v));

        th1.join();
        th2.join();
        th3.join();
        th4.join();
        th5.join();
        th6.join();
        th7.join();
        th8.join();

    cout << "Complete" << endl;

    ofstream file;
    file.open("test.ppm");

    file << "P3\n" << width << " " << height << "\n255\n";

    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            file << v[x][y] << "\n";
        }
    }
    file.close();

    return 0;
}