#include "globals.h"
#include "vector.cpp"


using namespace std;
int main()
{

    Vector a(1.0f);
    Vector b(1.0f);
    Vector c = a + b;
    cout<<c;
    return 0;
}