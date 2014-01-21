#include "drawings.h" 

#include <string>
#include <sstream>
#include <math.h>
#include <stdio.h>

std::string debugString;
std::stringstream ss(std::stringstream::in | std::stringstream::out);

void drawScene(void) {

    static GLfloat material[] = {0.0, 0.0, 0.0, 1.0};

    // Draw faces
    for (int i = 0; i < mesh->nfaces; i++) {
        Face& face = mesh->faces[i];
        glBegin(GL_POLYGON);
        glNormal3fv(face.normal);
        for (int j = 0; j < face.nverts; j++) {
            switch (face.nverts) {
                case 3:
                    // Set material
                    material[0] = 0.0;
                    material[1] = 0.5;
                    material[2] = 0.5;
                    material[3] = 1.0;
                    break;
                case 4:
                    material[0] = 0.33;
                    material[1] = 0.0;
                    material[2] = 0.33;
                    material[3] = 1.0;
                    break;
                case 5:
                    material[0] = 0.77;
                    material[1] = 0.77;
                    material[2] = 0.0;
                    material[3] = 1.0;
                    break;
            }
            glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);

            Vertex *vert = face.verts[j];
            glVertex3f(vert->x, vert->y, vert->z);
        }
        glEnd();
    }

}

void drawDebugInfo(void) {

    if (debugInfo == true) {

        glPushMatrix();
        glLoadIdentity();

        glMatrixMode(GL_PROJECTION);
        glPushMatrix();
        glLoadIdentity();
        gluOrtho2D(0, GLUTwindow_width, 0, GLUTwindow_height);
        
        glDisable(GL_LIGHTING);

        ss.str("");
        ss << "Debug: " << std::endl;
        ss << "Mouse coord: " << GLUTmouse[0] << " " << GLUTmouse[1] << std::endl;
        ss << "Mouse btn: " << GLUTbutton[0] << " " << GLUTbutton[1] << " " << GLUTbutton[2] << std::endl;
        ss << "Mouse wheel: " << GLUTmouse_wheel << std::endl;

        ss << "trans: " << scaling << " S,  " << translating << " T,  " << rotating << " R" << std::endl;
        ss << "matrices: " << scaling << " S,  " << translating << " T,  " << rotating << " R" << std::endl;
        ss << "scale: " << scale << std::endl;
        ss << "center: " << center[0] << " " << center[1] << " " << center[2] << std::endl;
        ss << "rotation: " << rotation[0] << " " << rotation[1] << " " << rotation[2] << std::endl;
        ss << "translation: " << translation[0] << " " << translation[1] << " " << translation[2] << std::endl;

        debugString = ss.str();
        glColor3f(1.0f, 0.0f, 0.0f);
        glRasterPos2f(20.0f, GLUTwindow_height - 20.0f);

        glutBitmapString(GLUT_BITMAP_HELVETICA_12, (const unsigned char*) debugString.c_str());
        
        glEnable(GL_LIGHTING);        
        glPopMatrix();
        glMatrixMode(GL_MODELVIEW);
        glPopMatrix();
    }

}