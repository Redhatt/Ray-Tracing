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


class Test
{
public:
    int check = 1;

    Test(){};
    ~Test(){};

    void setCheck(int val);
    int getCheck();
};

void Test::setCheck(int val)
{   
    this->check = val;
}

int Test::getCheck()
{
    return this->check;
}

int main()
{
    
    Sphere *ball = new Sphere(Point(0, 0, 0), 2);
    Material *mat = new Material();

    ball->setMaterial(*mat);
    cout<<ball->getMaterial().getReflection()<<endl;
    ball->getMaterial().setReflection(false);
    cout<<ball->getMaterial().getReflection()<<endl;

    return 0;
}