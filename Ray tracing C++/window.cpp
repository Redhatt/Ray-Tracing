// run this command in cmd
// g++ window.cpp -L"C:\msys64\mingw64\x86_64-w64-mingw32\freeglut\lib\x64" -lfreeglut -lopengl32 -Wl,--subsystem,windows
// g++ animate.cpp -L"C:\msys64\mingw64\x86_64-w64-mingw32\freeglut\lib\x64" -lfreeglut -lopengl32 -Wl,--subsystem,windows
// g++ -O3 animate.cpp -L"C:\msys64\mingw64\x86_64-w64-mingw32\freeglut\lib\x64" -lfreeglut -lopengl32 -Wl,--subsystem,windows
// g++ windowTest.cpp -L"C:\msys64\mingw64\x86_64-w64-mingw32\freeglut\lib\x64" -lfreeglut -lopengl32 -Wl,--subsystem,windows

#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>

#include "color.cpp"

using namespace std;

int height= 500;
int width = 1000;

float *PixelBuffer;
void setPixel();

void keyboard(unsigned char key, int x, int y);
void display();

void keyboard(unsigned char key, int x, int y)
{
	// GLUT_KEY_LEFT, GLUT_KEY_RIGHT, GLUT_KEY_UP, GLUT_KEY_DOWN
	switch (key)
	{
	case '\x1B':
		exit(EXIT_SUCCESS);
		break;
	}
}

int main(int argc, char *argv[])
{

	int size = width * height* 3;
	PixelBuffer = new float[size];

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutKeyboardFunc(&keyboard);

	glutInitWindowPosition(100, 100);
	glColor3f(0, 1.0, 0);

	int firstWindow = glutCreateWindow("First Color");

	glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL

	glutDisplayFunc(display);
	glutIdleFunc(display);

	glutMainLoop();


	return 1;
}

void display()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	glLoadIdentity();
	setPixel();
	glDrawPixels(1000, 500, GL_RGB, GL_FLOAT, PixelBuffer);
	
	glutSwapBuffers();
}

void setPixel()
{
	int pixelLocation;
	for (int i = 0; i < height; i++)
		{
			for (int j = 0; j < width; j++){
				Color color = randomColor(0.5f);
				pixelLocation = (i * width* 3) + (j * 3);
				PixelBuffer[pixelLocation + 0] = color.r;
				PixelBuffer[pixelLocation + 1] = color.g;
				PixelBuffer[pixelLocation + 2] = color.b;
			}
		}
};