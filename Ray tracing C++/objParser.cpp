#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <string>

// #include "vector.cpp"
// #include "ray.cpp"
// #include "shape.cpp"
// #include "camera.cpp"
// #include "color.cpp"
// #include "matarials.h"
// #include "light.cpp"


using namespace std;

string strip(const string &str)
{
	auto start_it = str.begin();
    auto end_it = str.rbegin();

    while (std::isspace(*start_it))
        ++start_it;
    while (std::isspace(*end_it))
        ++end_it;

    return std::string(start_it, end_it.base()); 
}

vector<string> split(const string &str, char splitChar=' ')
{
	vector<string> ans;
	int n = str.size();
	int i=0, j=0;

	while (j<n)
	{
		while (str[j] != splitChar && j<n) j++;
		ans.push_back(str.substr(i, j-i));
		while (str[j] == splitChar && j<n) j++;
		i = j;
	}

	return ans;
}

void fileReader(string filename, ShapeSet *shapes, float randColor=0.0) 
{

	ifstream file;

	file.open(filename);
	vector<Vector> vertices;
	vector<vector<int>> faces;
	string data;

	string FACE = "f";
	string VERTIX = "v";

	while (getline(file, data))
	{
		string check1 = strip(data);
		vector<string> check2 = split(check1);
		int n = check2.size();

		if (n>0)
		{
			if (check2[0] == FACE){

				vector<int> temp;
				for (int k = 1; k<n; k++)
				{	
					vector<string> check3 = split(check2[k], '/');
					temp.push_back(stoi(check3[0]));
				}
				faces.push_back(temp);
			}
			else if (check2[0] == VERTIX){
				float p1 = stof(check2[1]);
				float p2 = -stof(check2[3]);
				float p3 = stof(check2[2]);

				Vector v = Vector(p1, p2, p3);
				vertices.push_back(v);
			}
		}
	}
	// for (auto x: vertices)
	// 	cout<<x<<endl;

	// for (auto x: faces){
	// 	for (auto y: x)
	// 		cout<<y<<" ";
	// 	cout<<endl;
	// }

	// converting to shape
	cout<<"Number of vertices: "<<vertices.size()<<endl;
	cout<<"Number of faces   : "<<faces.size()<<endl;

	for (auto &face : faces)
	{
		int n = face.size();
		if (n>=3){

			vector<Point> polyPoints;

			for (int i=0; i<n; i++){
				polyPoints.push_back(Point(vertices[face[i]-1]));
			}
			Polygon* poly = new Polygon(polyPoints);

			if (randColor > 0.0) {
				Matarial mat = Matarial();
				Color color = Color(randColor * float(rand()) / RAND_MAX, 
									randColor * float(rand()) / RAND_MAX, 
									randColor * float(rand()) / RAND_MAX);
				mat.setAmbient(color);
				mat.setAmbient(color);
				poly->setMatarial(mat);
			}
			shapes->addShape(poly);
		} else {
			cout<<"error face with less than 3 vertices!\n";
		}
	}

	file.close();
}


// int main() {

// 	// ShapeSet
//     fileReader("object.obj", &shapes);

// }