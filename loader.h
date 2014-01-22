#ifndef __LOADER__
#define __LOADER__

#include "common.h"

Mesh* ReadOffFile(const char *filename);
GLuint LoadTexture(const char * filename);

#endif