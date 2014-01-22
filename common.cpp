#include "common.h"

Mesh *mesh = NULL;

// GLUT variables 
int GLUTwindow = 0;
int GLUTwindow_height = 500;
int GLUTwindow_width = 500;
int GLUTmouse[2] = {0, 0};
bool GLUTmouse_wheel = false;
int GLUTbutton[3] = {0, 0, 0};
//int GLUTarrows[4] = { 0, 0, 0, 0 };
int GLUTmodifiers = 0;

GLuint textures[3] = {0, 0, 0};
 
// Display variables
int scaling = 0;
int translating = 0;
int rotating = 0;
float scale = 1.0;
float center[3] = {0.0, 0.0, 0.0};
float rotation[3] = {0.0, 0.0, 0.0};
float translation[3] = {0.0, 0.0, -4.0};

bool lightOn = true;
GLfloat difflight_position[4] = {3.0, 4.0, 5.0, 0.0};
GLfloat difflight_color[4] = {1.0, 1.0, 1.0, 1.0};

int goto_triangle = 0, goto_square = 0, goto_pentagon = 0;

bool debugInfo = false;

double modulus(double a, double b) {
    int result = static_cast<int> (a / b);
    return a - static_cast<double> (result) * b;
}