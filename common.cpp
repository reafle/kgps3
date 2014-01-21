#include "common.h"

Mesh *mesh = NULL;

// GLUT variables 
int GLUTwindow = 0;
int GLUTwindow_height = 500;
int GLUTwindow_width = 500;
int GLUTmouse[2] = { 0, 0 };
bool GLUTmouse_wheel = false;
int GLUTbutton[3] = { 0, 0, 0 };
//int GLUTarrows[4] = { 0, 0, 0, 0 };
int GLUTmodifiers = 0;


// Display variables
int scaling = 0;
int translating = 0;
int rotating = 0;
float scale = 1.0;
float center[3] = { 0.0, 0.0, 0.0 };
float rotation[3] = { 0.0, 0.0, 0.0 };
float translation[3] = { 0.0, 0.0, -4.0 };

bool debugInfo = false;
