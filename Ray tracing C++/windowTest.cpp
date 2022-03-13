// run this command in cmd
// g++ windowTest.cpp -L"C:\msys64\mingw64\x86_64-w64-mingw32\freeglut\lib\x64" -lfreeglut -lopengl32 -Wl,--subsystem,windows
// remove -wl, --sub.... for logs


#include <stdlib.h>
#include <GL/glut.h>
#include <iostream>

using namespace std;

float *PixelBuffer;
void setPixel(int, int);

void keyboard(unsigned char key, int x, int y);
void display();

int size = 500 * 1000 * 3;

void keyboard(unsigned char key, int x, int y)
{
  switch (key)
  {
    case '\x1B':
      exit(EXIT_SUCCESS);
      break;
  }
}

int main(int argc, char *argv[])
{

    PixelBuffer = new float[size];

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);

    glutInitWindowSize(400, 400);

    glutInitWindowPosition(100, 100);
    glColor3f(0, 1.0, 0);

    int firstWindow = glutCreateWindow("First Color");



    glClearColor(0, 0, 0, 0); //clears the buffer of OpenGL

  for(int i = 0; i < 20; i++)
  {
    setPixel(i, 10);
  }



    glutDisplayFunc(display);

    glutMainLoop();


    return 0;
}

void display()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glLoadIdentity();

    glDrawPixels(400, 400, GL_RGB, GL_FLOAT, PixelBuffer);
    glFlush();
}

void setPixel(int x, int y)
{
  int pixelLocation;
  int width = 400;
  pixelLocation = (y * width * 3) + (x * 3);
  PixelBuffer[pixelLocation + 0] = 1;
  PixelBuffer[pixelLocation + 1] = 1;
  PixelBuffer[pixelLocation + 2] = 1;
};