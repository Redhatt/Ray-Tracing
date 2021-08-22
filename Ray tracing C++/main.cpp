#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <string>
#include <pthread.h>
#include <queue>

#include "vector.cpp"
#include "ray.cpp"
#include "shape.cpp"
#include "camera.cpp"
#include "color.cpp"
#include "matarials.h"
#include "light.cpp"
#include "objParser.cpp"

using namespace std;

struct ThreadData
{
    int id, b, a, w, h, width, height;
    Camera *camera;
    ShapeSet *shapes;
    vector<vector<string>> *v;
};

struct Task
{
    int id;
    ThreadData *data;
    void *(*f)(void *args);
};

inline void *executeTask(Task *task)
{
    void *r = (*task->f)(task->data);
    cout<<"Task id: "<< task->id<<" has been executed.\n";
    return r;
}

queue<Task*> taskQueue;

pthread_mutex_t mutexQueue;
pthread_cond_t condQueue;

void submitTask(Task *task)
{
    pthread_mutex_lock(&mutexQueue);
    taskQueue.push(task);
    pthread_mutex_unlock(&mutexQueue);
    pthread_cond_signal(&condQueue);
    cout<<"Task id: "<<task->id<<" submitted. Current queue size: "<<taskQueue.size()<<endl;
}

void *startThread(void *args)
{
    int *id = (int *) args;
    while (!taskQueue.empty()) {
        Task *task;

        pthread_mutex_lock(&mutexQueue);

        // if (taskQueue.empty()) break;

        // while (taskQueue.empty()) {
        //     pthread_cond_wait(&condQueue, &mutexQueue);
        // }

        task = taskQueue.front();
        taskQueue.pop();

        pthread_mutex_unlock(&mutexQueue);

        cout<<"Task id: "<<task->id<<" aquired on thread id: "<<*id<<". Starting execution.\n";
        executeTask(task);
    }
    return NULL;
}


// void scene(int b, int a, int w, int h, int width, int height, Camera &camera, ShapeSet &shapes, vector<vector<string>> &v)
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

    srand(time(0));
    for (int y = h - 1; y >= a; y--) {
        for (int x = b; x < w; x++) {

            int r, g, b;
            int depth = 0;

            Ray ray = data->camera->getRay(x, y, width, height);
            Intersection intersection(ray);
            data->shapes->intersect(intersection);

            intersection.getSurfaceLight(ray.direction, data->shapes, depth);
            intersection.light.color.applyGammaCorrection(1.0, 2.2);
            intersection.light.color.clamp(0, 1);

            r = 255 * intersection.light.color.r;
            g = 255 * intersection.light.color.g;
            b = 255 * intersection.light.color.b;

            (*data->v)[x][y] = to_string(r) + " " + to_string(g) + " " + to_string(b) + "\n";
        }
    }
    return NULL;
}

int main()
{

    int height = 500;
    int width = 1000;

    Point cameraOrigin = Point(0, 3, 5);
    Vector cameraTraget = Vector(0, 0, 0);
    Vector cameraGuide = Vector(0, 0, 1);
    float cameraFOV = PI / 6;
    float cameraRatio = float(width) / float(height);

    class PerspectiveCamera camera = PerspectiveCamera(cameraOrigin, cameraTraget, cameraGuide, cameraFOV, cameraRatio);

    ShapeSet shapes;
    fileReader("sphere.obj", &shapes);

    Plane backWall(Point(0, -1, 0), Vector(0, 1, 0));
    // Matarial mat1 = Matarial(0.8, 0.0, 0.0, PI/30, 0.0f, 2,
    //                          Color(0.5, 0.5, 0.5),
    //                          Color(0.5, 0.5, 0.5),
    //                          Color(1.0, 1.0, 1.0));
    Matarial mat1 = Matarial();
    backWall.setMatarial(mat1);
    // shapes.addShape(&backWall);

    Plane floor(Point(0, 0, -1), Vector(0, 0, 1));
    Matarial mat2 = Matarial(0.85, 0, 0, PI / 40, 0.0f, 2,
                             Color(0.8313, 0.466, 0.0745),
                             Color(0.8313, 0.466, 0.0745),
                             Color(1.0, 1.0, 1.0));
    floor.setMatarial(mat2);
    // shapes.addShape(&floor);

    Sphere ball1(Point(0.2, 0.2, 1.5), 0.3);    
    Matarial mat3 = Matarial(0.02, 0.0, 0.0, PI / 100, 0.0f, 2,
                             Color(0.2, 0.1, 0.05),
                             Color(0.2, 0.1, 0.05),
                             Color(1.0, 1.0, 1.0));
    ball1.setMatarial(mat3);
    // shapes.addShape(&ball1);

    Sphere ball2(Point(-1, 0, 0), 1);
    Matarial mat4 = Matarial(0.02, 0.0, 0.0, PI / 100, 0.0f, 2,
                             Color(0.1, 0.05, 0.2),
                             Color(0.1, 0.05, 0.2),
                             Color(1.0, 1.0, 1.0));
    ball2.setMatarial(mat4);
    // shapes.addShape(&ball2);

    Sphere ball3(Point(2.5, 0, 0), 1);
    Matarial mat5 = Matarial(0.02, 0.0, 0.0, PI / 100, 0.0f, 2,
                             Color(0.05, 0.2, 0.1),
                             Color(0.05, 0.2, 0.1),
                             Color(1.0, 1.0, 1.0));
    ball3.setMatarial(mat5);
    // shapes.addShape(&ball3);

    Sphere light(Point(10, 5, 5 ), 1);
    Matarial mat6 = Matarial(0.0, 0.0, 0.5, PI / 100, 0.0f, 0,
                             Color(1.0, 1.0, 1.0),
                             Color(1.0, 1.0, 1.0),
                             Color(1.0, 1.0, 1.0));
    light.setMatarial(mat6);
    shapes.addShape(&light);
    shapes.addLight(&light);

    vector<vector<string>> v(width, vector<string>(height)); 

    int i, noThreads = 8;
    struct ThreadData dataArr[noThreads];
    pthread_t threadArr[noThreads];

    pthread_mutex_init(&mutexQueue, NULL);
    pthread_cond_init(&condQueue, NULL);

    int nGridX = 50;
    int GridY = 50;

    int noTask = height * width / (nGridX * GridY);

    cout<<"Total number of tasks: "<<noTask<<endl;
    for (int x=0; x<width; x+=nGridX) {
        for (int y=0; y<height; y+=GridY) {
            Task *task = new Task;
            ThreadData *data = new ThreadData;
            data->id     = (x*width/nGridX) + (y/GridY);
            data->b      = x;
            data->a      = y;
            data->w      = min((x+nGridX), width);
            data->h      = min((y+GridY), height);
            data->width  = width;
            data->height = height;
            data->camera = &camera;
            data->shapes = &shapes;
            data->v      = &v;

            task->id = (x*width/nGridX) + (y/GridY);
            task->data = data;
            task->f = scene;

            submitTask(task);
        }
    }

    for (i=0; i<noThreads; i++)
    {
        int *id = new int();
        *id = i;
        if (pthread_create(&threadArr[i], NULL, &startThread, id)) {
            cout<<"Error in Thread "<<i;
            exit(-1);
        }
        else cout<<"Running Thread "<<i;

        cout<<endl;
    }

    for (i=0; i<noThreads; i++) {
        if (pthread_join(threadArr[i], NULL)) cout<<"Error in ending Thread "<<i;
        else cout<<"Ending Thread successfully "<<i;

        cout<<endl;
    }

    pthread_mutex_destroy(&mutexQueue);
    pthread_cond_destroy(&condQueue);

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
    cout<<"file writing completed!\n";

    return 0;
}