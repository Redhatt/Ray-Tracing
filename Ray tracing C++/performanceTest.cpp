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

#include <windows.h>

using namespace std;

Ray ray(Point(0, 0, 0), Vector(1, 1, 1).normalized(), 1e30);

int main()
{
    // clock_t start, end;

    // float avg = 0.0f;
    // float total = 0;

    // float w = 100;

    // vector<int> v = {1,2,3,4,5,6,7,8,9};
    // for (int k=0; k<w; k++)
    // {
    //     start = clock();
    //     // float xx = ray.origin.x;
    //     for (unsigned int i=0; i<100000; i++)
    //     {

    //     }
    //     end = clock();
    //     total += (end - start);
    // }

    // total /= w;
    // cout<<total<<endl;

    return 0;
}