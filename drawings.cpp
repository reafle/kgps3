#include "drawings.h" 

#include <string>
#include <sstream>
#include <math.h>
#include <stdio.h>

std::string debugString;
std::stringstream ss(std::stringstream::in | std::stringstream::out);


void drawScene(void) {
    
    
  // Draw faces
  for (int i = 0; i < mesh->nfaces; i++) {
    Face& face = mesh->faces[i];
    glBegin(GL_POLYGON);
    glNormal3fv(face.normal);
    for (int j = 0; j < face.nverts; j++) {
        switch(face.nverts) {
            case 3 : glColor3f(0.77f, 0.0f, 0.33f);
                break;
            case 4 : glColor3f(0.0f, 0.33f, 0.77f);
                break;
            case 5 : glColor3f(0.33f, 0.77f, 0.77f);
                break;
        }
      Vertex *vert = face.verts[j];
      glVertex3f(vert->x, vert->y, vert->z);
    }
    glEnd();
  }
//
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
//
//    glLoadIdentity();
//    glTranslatef(-1, -1, 0);
//
//    glPushMatrix();
//    glScalef((GLfloat) 2 / MATRIX_SIZE, (GLfloat) 2 / MATRIX_SIZE, 0);
//    glColor3f(0.7f, 0.7f, 0.7f);
//
//    for (int i = 0; i < MATRIX_SIZE; i++) {
//        for (int j = 0; j < MATRIX_SIZE; j++) {
//            glPushMatrix();
//            glTranslatef((GLfloat) i, (GLfloat) j, 0);
//            drawSquareBorder();
//            glPopMatrix();
//        }
//    }
//
//    glPopMatrix();
//
//
//    /* making sure we have both points set */
//    if (linex1 * liney1 * linex2 * liney2 > 0) {
//        drawBresenhamLine();
//        drawLine();
//    }
//
//    drawLinePoints();
//    drawDebugInfo();
//
//    glFlush();

}

void drawDebugInfo(void) {

    if (debugInfo == true) {

        glPushMatrix();
        glLoadIdentity();

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, GLUTwindow_width, 0, GLUTwindow_height);

        ss.str("");
        ss << "Debug: " << std::endl;
        ss << "Mouse coord: " << GLUTmouse[0] << " " << GLUTmouse[1] << std::endl;
        ss << "Mouse btn: " << GLUTbutton[0] << " " << GLUTbutton[1] << " "  << GLUTbutton[2] << std::endl;
        ss << "Mouse wheel: " << GLUTmouse_wheel << std::endl;
        
        ss << "trans: " << scaling << " S,  " << translating << " T,  "  << rotating << " R" << std::endl;
        ss << "matrices: " << scaling << " S,  " << translating << " T,  "  << rotating << " R" << std::endl;
        ss << "scale: " << scale << std::endl;
        ss << "center: " << center[0] << " " << center[1] << " "  << center[2] << std::endl;
        ss << "rotation: " << rotation[0] << " " << rotation[1] << " "  << rotation[2] << std::endl;
        ss << "translation: " << translation[0] << " " << translation[1] << " "  << translation[2] << std::endl;
         
//        int scaling = 0;
//        int translating = 0;
//        int rotating = 0;
//        float scale = 1.0;
//        float center[3] = { 0.0, 0.0, 0.0 };
//        float rotation[3] = { 0.0, 0.0, 0.0 };
//        float translation[3] = { 0.0, 0.0, -4.0 };
        

        debugString = ss.str();
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(20.0f, GLUTwindow_height - 20.0f);

        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*) debugString.c_str());
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }

}