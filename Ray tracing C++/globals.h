#include <cmath>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <string>
#include <queue>
#include <pthread.h>
#include <time.h>
#include <math.h>
#include <GL/glut.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/norm.hpp>
#include <glm/gtx/fast_square_root.hpp>

#define RAY_T_MAX 1.0e30f
#define RAY_T_MIN 0.0001f

#ifndef NULL
#define NULL 0
#endif

#ifndef PI
#define PI 3.1415926f
#endif

#ifndef DEG
#define DEG PI / 180.0f
#endif

#ifndef RAND_MAX_INV
#define RAND_MAX_INV 1.0f/RAND_MAX
#endif