#include "globals.h"

#include "vector.cpp"
#include "ray.cpp"
#include "shape.cpp"
#include "camera.cpp"
#include "color.cpp"
#include "materials.h"
#include "light.cpp"
#include "objParser.cpp"
#include "threadPool.h"

using namespace std;

void *scene(void *args)
{

    struct ThreadData *data;
    data = (struct ThreadData *) args;

    int b      = data->b     ;
    int a      = data->a     ;
    int w      = data->w     ;
    int h      = data->h     ;
    int width  = data->width ;
    int height = data->height;

    int multiSamplingX = 0;
    int multiSamplingY = 0;

    float multiSamplingXwidth = 1.0 / (multiSamplingX + 1);
    float multiSamplingYwidth = 1.0 / (multiSamplingY + 1);

    for (int y = h - 1; y >= a; y--) {
        for (int x = b; x < w; x++) {

            float r = 0.0f, g = 0.0f, b = 0.0f;
            int depth = 0;

            for (float subX = float(x) - 0.5 + multiSamplingXwidth * 0.5;
                    subX < x + 0.5;
                    subX += multiSamplingXwidth)
            {
                for (float subY = float(y) - 0.5 + multiSamplingYwidth * 0.5;
                        subY < y + 0.5;
                        subY += multiSamplingYwidth)
                {
                    Ray ray = data->camera->getRays(subX, subY, width, height);
                    Intersection intersection(ray);

                    data->shapes->intersectTree(0, intersection);

                    intersection.getSurfaceLight(ray.direction, data->shapes, depth, false);
                    // intersection.light.color.applyGammaCorrection(1.0, 2.2);
                    intersection.light.color.clamp(0, 1);

                    r += intersection.light.color.r;
                    g += intersection.light.color.g;
                    b += intersection.light.color.b;
                }
            }

            r = r * (255 * multiSamplingXwidth * multiSamplingYwidth);
            g = g * (255 * multiSamplingXwidth * multiSamplingYwidth);
            b = b * (255 * multiSamplingXwidth * multiSamplingYwidth);

            (*data->v)[x][y] = to_string(int(r)) + " " + to_string(int(g)) + " " + to_string(int(b)) + "\n";
        }
    }
    return NULL;
}

int main()
{
    // srand(time(0));

    int height = 720;
    int width = 1280;

    // Point cameraOrigin = Point(-1, 5, 1);
    // Point cameraOrigin = Point(4, 3.5, 17);
    // Point cameraOrigin = Point(8, 8, 8);
    Point cameraOrigin = Point(0, 10, 0);

    Vector cameraTraget = Vector(0, 0, 0);
    // Vector cameraTraget = Vector(4, 3.5, 0);

    // Vector cameraGuide = Vector(-1, -1, 1);
    Vector cameraGuide = Vector(0, 0, 1);
    // Vector cameraGuide = Vector(0, 1, 0);

    float cameraFOV = PI / 8;
    float cameraRatio = float(width) / float(height);

    class PerspectiveCamera camera = PerspectiveCamera(cameraOrigin, cameraTraget, cameraGuide, cameraFOV, cameraRatio);

    vector<vector<string>> v(width, vector<string>(height));
    ShapeSet shapes;


    // Sky box setup
    float d = 1e4;
    float radius = 1e10;
    Material skyMat = Material(0.8, 0.0, 0.0, PI / 50,
                               0.0, 0.01, 0,
                               Color(169, 207, 245),
                               Color(169, 207, 245),
                               Color(1.0, 1.0, 1.0));
    skyMat.setReflection(false);
    skyMat.setLight(false);
    skyMat.setRefraction(false);
    skyMat.setAmbienceShadow(false);

    Plane skyX(Point(d, 0, 0), Vector(-1, 0, 0));
    Plane skyX_(Point(-d, 0, 0), Vector(1, 0, 0));
    Plane skyY(Point(0, d, 0), Vector(0, -1, 0));
    Plane skyY_(Point(0, -d, 0), Vector(0, 1, 0));
    Plane skyZ(Point(0, 0, d), Vector(0, 0, 1));
    Plane skyZ_(Point(0, 0, -d), Vector(0, 0, -1));

    skyX.setMaterial(skyMat);
    skyX_.setMaterial(skyMat);
    skyY.setMaterial(skyMat);
    skyY_.setMaterial(skyMat);
    skyZ.setMaterial(skyMat);
    skyZ_.setMaterial(skyMat);

    skyX.setRadius(radius);
    skyX_.setRadius(radius);
    skyY.setRadius(radius);
    skyY_.setRadius(radius);
    skyZ.setRadius(radius);
    skyZ_.setRadius(radius);

    shapes.addShape(&skyX);
    shapes.addShape(&skyX_);
    shapes.addShape(&skyY);
    shapes.addShape(&skyY_);
    shapes.addShape(&skyZ);
    shapes.addShape(&skyZ_);
    // sky box end

    // Plane backWall(Point(0, -2, 0), Vector(0, 1, 0));
    // Material mat1 = Material(0.2, 0.0, 0.0, PI/50, 0.0, 2,
    //                          Color(1.0, 1.0, 1.0),
    //                          Color(1.0, 1.0, 1.0),
    //                          Color(1.0, 1.0, 1.0));
    // // Material mat1 = Material();
    // backWall.setMaterial(mat1);

    Plane floor(Point(0, 0, -1.1), Vector(0, 0, 1));
    Material mat2 = Material(0.025, 0, 0, 2 * PI / 150, 0.0, 0.0, 2,
                             Color(0.1, 0.3, 0.2),
                             Color(0.1, 0.3, 0.2),
                             Color(1.0, 1.0, 1.0));
    floor.setMaterial(mat2);

    Sphere ball1(Point(-2.1, 0, 0), 1);
    Material mat3 = Material(0.02, 0.0, 0.0, PI / 150, 0.0, 0.0, 2,
                             Color(0.7, 0.4, 0.1),
                             Color(0.7, 0.4, 0.1),
                             Color(1.0, 1.0, 1.0));

    ball1.setMaterial(mat3);

    Sphere ball2(Point(0, 0, 0.5), 1);
    Material mat4 = Material(0.02, 0.0, 0.0, PI / 150, 0.0, 0.0, 2,
                             Color(0.1, 0.6, 0.4),
                             Color(0.1, 0.6, 0.4),
                             Color(1.0, 1.0, 1.0));
    ball2.setMaterial(mat4);

    Sphere ball3(Point(2.1, 0, 1), 1);
    Material mat5 = Material(0.02, 0.0, 0.0, PI / 150, 0.0, 0.0, 2,
                             Color(0.4, 0.2, 0.7),
                             Color(0.4, 0.2, 0.7),
                             Color(1.0, 1.0, 1.0));

    ball3.setMaterial(mat5);

    Sphere light(Point(500, -500, 500), 1);
    Material mat6 = Material(0.0, 0.0, 10, PI / 100, 0.0, 0, 0,
                             Color(1.0, 1.0, 1.0),
                             Color(1.0, 1.0, 1.0),
                             Color(1.0, 1.0, 1.0));
    light.setMaterial(mat6);


    floor.getMaterial().setReflection(true);
    floor.getMaterial().setLight(false);
    floor.getMaterial().setAmbienceShadow(false);
    ball1.getMaterial().setReflection(false);
    ball1.getMaterial().setRefraction(false);
    ball2.getMaterial().setReflection(false);
    ball3.getMaterial().setReflection(false);
    ball2.getMaterial().setRefraction(false);
    ball3.getMaterial().setRefraction(false);

    cout << ball3.getMaterial().getReflection();

    for (int j=-20; j>=-25; j-=2){
        for (int i=-50; i<50; i+=2) {

            if (((i+j+1000)%3)){}
            else continue;

            float randColor = 0.8;
            float base = 0.3;
            Sphere *ball = new Sphere(Point(i, j + float(rand())* RAND_MAX_INV, -0.5 + 2*float(rand())* RAND_MAX_INV), 1);
            Material mat = Material();
            Color color = Color(base + randColor * float(rand())* RAND_MAX_INV,
                                base + randColor * float(rand())* RAND_MAX_INV,
                                base + randColor * float(rand())* RAND_MAX_INV);

            mat.setAmbient(color);
            mat.setDiffuse(color);
            mat.setGrain(0.0);
            mat.setRefraction(false);
            mat.setReflection(false);
            mat.setAbsorb(0.02);
            ball->setMaterial(mat);

            shapes.addShape(ball);
        }
    }



    shapes.addLight(&light);
    // shapes.addShape(&light);

    // shapes.addShape(&backWall);
    shapes.addShape(&floor);
    shapes.addShape(&ball1);
    shapes.addShape(&ball2);
    shapes.addShape(&ball3);


    clock_t start, fileRead, buildingTree, render, fileWrite, threadStart, threadEnd, submitStart, submitEnd;

    start = clock(); //---------------------

    // fileReader("ultimateTest.obj", &shapes, 0.0f);

    fileRead = clock(); //---------------------

    shapes.buildTree(0, 0, shapes.shapes.size() - 1);

    buildingTree = clock(); //---------------------

    int i, noThreads = 8;
    struct ThreadData dataArr[noThreads];
    pthread_t threadArr[noThreads];

    pthread_mutex_init(&mutexQueue, NULL);
    // pthread_cond_init(&condQueue, NULL);

    submitStart = clock();
    int GridX = 60;
    int GridY = 60;

    int X = ceil(float(width) / GridX);
    int Y = ceil(float(height) / GridY);
    int noTask = X * Y;

    cout << "Total number of tasks: " << noTask << endl;
    for (int x = 0, i = 0; x < width; x += GridX, i++) {
        for (int y = 0, j = 0; y < height; y += GridY, j++) {
            Task *task = new Task;
            ThreadData *data = new ThreadData;

            task->id     = i * Y + j;
            data->id     = i * Y + j;

            data->b      = x;
            data->a      = y;
            data->w      = min((x + GridX), width);
            data->h      = min((y + GridY), height);
            data->width  = width;
            data->height = height;
            data->camera = &camera;
            data->shapes = &shapes;
            data->v      = &v;

            task->data = data;
            task->f = scene;

            submitTask(task);
        }
    }

    submitEnd = clock();

    for (i = 0; i < noThreads; i++)
    {
        int *id = new int();
        *id = i;
        if (pthread_create(&threadArr[i], NULL, &startThread, id)) {
            cout << "Error in Thread " << i;
            exit(-1);
        }
        else cout << "Running Thread " << i;
        cout << endl;
    }

    threadStart = clock(); //---------------------

    for (i = 0; i < noThreads; i++) {
        if (pthread_join(threadArr[i], NULL)) cout << "Error in ending Thread " << i;
        else cout << "Ending Thread successfully " << i;
        cout << endl;
    }

    threadEnd = clock(); //---------------------
    pthread_mutex_destroy(&mutexQueue);
    // pthread_cond_destroy(&condQueue);

    cout << "Completed! Writing file." << endl;

    ofstream file;
    file.open("test.ppm");
    file << "P3\n" << width << " " << height << "\n255\n";
    for (int y = height - 1; y >= 0; y--) {
        for (int x = 0; x < width; x++) {
            file << v[x][y] << "\n";
        }
    }
    file.close();
    cout << "file writing completed!\n";

    fileWrite = clock(); //---------------------

    cout << "File reading : " << float(fileRead - start) / float(CLOCKS_PER_SEC / 1000) << " ms." << endl;
    cout << "Building tree: " << float(buildingTree - fileRead) / float(CLOCKS_PER_SEC / 1000) << " ms." << endl;
    cout << "Thread start : " << float(threadStart - buildingTree) / float(CLOCKS_PER_SEC / 1000) << " ms." << endl;
    cout << "task submit  : " << float(submitEnd - submitStart) / float(CLOCKS_PER_SEC / 1000) << " ms." << endl;
    cout << "Render time->: " << float(threadEnd - threadStart) / float(CLOCKS_PER_SEC / 1000) << " ms." << endl;
    cout << "File writing : " << float(fileWrite - threadEnd) / float(CLOCKS_PER_SEC / 1000) << " ms." << endl;
    cout << "Total        : " << float(fileWrite - start) / float(CLOCKS_PER_SEC / 1000) << " ms." << endl;
    cout << "calls        : " << float(shapes.calls) * 1.0f << endl;

    return 0;
}

// depth: 0
// total Pixels: 500x1000 = 500,000
// total polygons: 500
// total rays:      1x500,000 = 500,000
// brute force: 500,000x500   = 250,000,000
// tree approach: 500,000x34  =   1,700,000
// brute reallity:            = 267,022,805
// tree reallity:             =   4,869,989
// max tree intersction calls: 2*(2*log_2(n) - 1) 


// 6130
// 6069
// 5901
// 5852
// 6187