#include <iostream>
#include <vector>

using namespace std;

struct myData
{
	int a;
	void (*f)(int);
};

void cb(int x) {
	std::cout<<x<<endl;
}

int main() {
	struct myData trueData;
	struct myData *data = &trueData;

	data->a = 10;
	data->f = cb;

	(*data->f)(data->a);
    return 0;
}