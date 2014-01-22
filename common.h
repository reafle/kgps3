#ifndef __COMMON__
#define __COMMON__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <assert.h>

#include "constants.h"
#include "GL/freeglut.h"

double modulus(double a, double b);

typedef struct Vertex {
  float x, y, z;
} Vertex;

typedef struct Face {
  Face(void) : nverts(0), verts(0) {};
  int nverts;
  Vertex **verts;
  float normal[3];
} Face;

typedef struct Mesh {
  Mesh(void) : nverts(0), verts(0), nfaces(0), faces(0) {};
  int nverts;
  Vertex *verts;
  int nfaces;
  Face *faces;
} Mesh;

extern int goto_triangle, goto_square, goto_pentagon;

extern int GLUTwindow;
extern int GLUTwindow_height;
extern int GLUTwindow_width;
extern bool GLUTmouse_wheel;
extern int GLUTmouse[2];
extern int GLUTbutton[3];
extern int GLUTmodifiers;

extern bool lightOn;
extern GLfloat difflight_position[4];
extern GLfloat difflight_color[4];

extern int scaling;
extern int translating;
extern int rotating;
extern float scale;
extern float center[3];
extern float rotation[3];
extern float translation[3];

extern bool debugInfo;

extern Mesh *mesh;
extern GLuint textures[3];

#endif