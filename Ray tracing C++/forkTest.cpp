#include <iostream>
#include <thread>
#include <vector>
#include <string>
#include <pthread.h>

using namespace std;

struct Inputs
{
    int a, b, x, y, w;
    vector<vector<int>> *v;
};

void *fille(void *threadArgs)
{

    struct Inputs *input;

    input = (struct Inputs *) threadArgs;
    int a = input->a;
    int b = input->b;
    int x = input->x;
    int y = input->y;
    int w = input->w;

    for (int i=a; i<x; i++){
        for (int j=b; j<y; j++){
            (*input->v)[i][j] = w;
        }
    }
}

int main(){
    int w, l;
    w = 10;
    l = 10;
    vector<vector<int>> v(w, vector<int>(l, 0));
    
    // thread th1(fille, 0, 0, 5, 10, ref(a), ref(v));
    // thread th2(fille, 5, 0, 10, 10, ref(b), ref(v));




    struct Inputs input1;
    
    input1.a = 0;
    input1.b = 0;
    input1.x = 5;
    input1.y = 10;
    input1.w = 2;
    input1.v = &v;

    struct Inputs input2;
    
    input2.a = 5;
    input2.b = 0;
    input2.x = 10;
    input2.y = 10;
    input2.w = 3;
    input2.v = &v;
    pthread_t th[2];
    struct Inputs thData[2] = {input1, input2};
    for (int i=0; i<2; i++){
        pthread_create(&th[i], NULL, fille, (void *)&thData[i]);
    }
    for (int i=0; i<2; i++){
        pthread_join(th[i], NULL);
    }

    // fille((void *)&input);

    for (auto x: v){
        for (auto y: x) cout<<y<<" ";
        cout<<endl;
    }
    return 0;
}
