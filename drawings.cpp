#include "drawings.h" 

#include <string>
#include <sstream>
#include <math.h>
#include <stdio.h>

std::string debugString;
std::stringstream ss(std::stringstream::in | std::stringstream::out);

void drawScene(void) {

    static GLfloat material[] = {0.0, 0.0, 0.0, 1.0};
    glEnable(GL_TEXTURE_2D);

    // Draw faces
    for (int i = 0; i < mesh->nfaces; i++) {
        Face& face = mesh->faces[i];
        switch (face.nverts) {
            case 3:
                glBindTexture(GL_TEXTURE_2D, textures[0]);
                material[0] = 0.0;
                material[1] = 0.5;
                material[2] = 0.5;
                material[3] = 1.0;
                break;
            case 4:
                glBindTexture(GL_TEXTURE_2D, textures[1]);
                material[0] = 0.33;
                material[1] = 0.0;
                material[2] = 0.33;
                material[3] = 1.0;
                break;
            case 5:
                glBindTexture(GL_TEXTURE_2D, textures[2]);
                material[0] = 0.77;
                material[1] = 0.77;
                material[2] = 0.0;
                material[3] = 1.0;
                break;
        }

        glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        
        glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);

        glBegin(GL_POLYGON);
        glNormal3fv(face.normal);
        for (int j = 0; j < face.nverts; j++) {
            switch (face.nverts) {
                case 3:
                    switch (j) {
                        case 0: glTexCoord2f(0.0f, 0.0f);
                            break;
                        case 1: glTexCoord2f(0.0f, 1.0f);
                            break;
                        case 2: glTexCoord2f(0.5f, 1.0f);
                            break;
                    }
                    break;
                case 4:
                    switch (j) {
                        case 0: glTexCoord2f(0.0f, 0.0f);
                            break;
                        case 1: glTexCoord2f(0.0f, 1.0f);
                            break;
                        case 2: glTexCoord2f(1.0f, 0.0f);
                            break;
                        case 3: glTexCoord2f(1.0f, 1.0f);
                            break;
                    }
                    break;
                case 5:
                    switch (j) {
                        case 0: glTexCoord2f(0.0f, 0.0f);
                            break;
                        case 1: glTexCoord2f(0.5, 1.0f);
                            break;
                        case 2: glTexCoord2f(0.0f, 0.5f);
                            break;
                        case 3: glTexCoord2f(0.33f, 0.0f);
                            break;
                        case 4: glTexCoord2f(0.66f, 0.0f);
                            break;
                        case 5: glTexCoord2f(1.0f, 0.5f);
                            break;
                    }
                    break;
            }

            Vertex *vert = face.verts[j];
            glVertex3f(vert->x, vert->y, vert->z);
        }

        glEnd();
    }
    glDisable(GL_TEXTURE_2D);
//    glBindTexture(GL_TEXTURE_2D, 0);

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
        ss << "triangle: " << goto_triangle << " square: " << goto_square << " pent: " << goto_pentagon << std::endl;
        ss << "light: " << lightOn << ", at " << difflight_color[0] << " " << difflight_color[1] << " " << difflight_color[2] << std::endl;
        ss << "tex: " << textures[0] << " " << textures[1] << " " << textures[2] << std::endl;

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

void processAnimation(void) {
    if (goto_square || goto_triangle || goto_pentagon) {
        if ((translation[0] + translation[1]) > 0) {
            if (translation[0] > 0) {
                translation[0] -= 0.05f;
            } else if (translation[0] < 0) {
                translation[0] += 0.05f;
            }

            if (translation[1] > 0) {
                translation[1] -= 0.05f;
            } else if (translation[2] < 0) {
                translation[1] += 0.05f;
            }

            if (translation[0] < 0.1 && translation[0] > -0.1 &&
                    translation[1] < 0.1 && translation[1] > -0.1) {
                translation[0] = 0;
                translation[1] = 0;
            }
        }

    }

    if (goto_triangle) {
        if (modulus(rotation[0], 45.0f) > 0) {
            rotation[0] -= 1.0f;
        } else if (modulus(rotation[0], 45.0f) < 0) {
            rotation[0] += 1.0f;
        }

        if (modulus(rotation[2], 45.0f) > 0) {
            rotation[2] -= 1.0f;
        } else if (modulus(rotation[2], 45.0f) < 0) {
            rotation[2] += 1.0f;
        }

        if (modulus(rotation[0], 45.0f) < 1 && modulus(rotation[0], 45.0f) > -1 &&
                modulus(rotation[2], 45.0f) < 1 && modulus(rotation[2], 45.0f) > -1) {
            rotation[0] = static_cast<int> (rotation[0]);
            rotation[2] = static_cast<int> (rotation[2]);
            goto_triangle = 0;
        }
    }

    if (goto_square) {
        if (modulus(rotation[0], 90.0f) > 0) {
            rotation[0] -= 1.0f;
        } else if (modulus(rotation[0], 90.0f) < 0) {
            rotation[0] += 1.0f;
        }

        ////        This actually never happens
        //        if (rotation[1] > 0) {
        //            rotation[1] -= 1.0f;
        //        } else if (rotation[1] < 0) {
        //            rotation[1] += 1.0f;            
        //        }

        if (modulus(rotation[2], 90.0f) > 0) {
            rotation[2] -= 1.0f;
        } else if (modulus(rotation[2], 90.0f) < 0) {
            rotation[2] += 1.0f;
        }

        if (modulus(rotation[0], 90.0f) < 1 && modulus(rotation[0], 90.0f) > -1 &&
                ////            Never happens as well
                //            modulus(rotation[1], 90.0f) < 1 && (rotation[1] % 90) > -1 &&
                modulus(rotation[2], 90.0f) < 1 && modulus(rotation[2], 90.0f) > -1) {
            rotation[0] = static_cast<int> (rotation[0]);
            rotation[2] = static_cast<int> (rotation[2]);
            goto_square = 0;
        }

    }




}