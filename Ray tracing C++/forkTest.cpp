#include "globals.h"

#include "vector.cpp"
#include "ray.cpp"
#include "shape.cpp"
#include "camera.cpp"
#include "color.cpp"
#include "matarials.h"
#include "light.cpp"
#include "objParser.cpp"
#include "threadPool.h"

using namespace std;



void cc()
{
    for (int i=0; i<1; i++){
        cout<<rand()<<endl;
    }
}

int main() {
    int v = 0;
    
    // for (int i=0; i<10; i++) {
    //     cout<<time(0)<<endl;
    //     for (int k=0; k<100000; k++) v++;
    //     for (int k=0; k<100000; k++) v--;
    //     for (int k=0; k<100000; k++) v++;
    //     for (int k=0; k<100000; k++) v--;
    // }
    srand(time(0));
    
    for (int i=0; i<10; i++) cc();
    
    return 0;
}