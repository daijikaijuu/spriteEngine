// 2015 Domrachev Alexandr <Alexandr.Domrachev@gmail.com>
#pragma once
#include <windows.h>
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <math.h>
#include <vector>
#include <GL/glew.h>
#include <GL/wglew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#ifdef _MSC_VER
#pragma comment(lib, "opengl32.lib")
#pragma comment(lib, "glu32.lib")
#pragma comment(lib, "glew32.lib")
#endif // _MSC_VER

#define PI 3.1415926535897932384626433832795

using std::cout;
using std::string;
using std::endl;
