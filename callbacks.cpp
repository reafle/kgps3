#include "GL/freeglut.h"
#include "GL/glu.h"
#include <stdio.h>

#include "callbacks.h"
#include "drawings.h"
#include "common.h"

void MouseCallback(int button, int state, int x, int y) {
    
    // Invert y coordinate
    y = GLUTwindow_height - y;
    // Process mouse button event

    if ((button == 3) || (button == 4)) { // It's a wheel event
        // Each wheel event reports like a button click, GLUT_DOWN then GLUT_UP
        if (state == GLUT_UP) return; // Disregard redundant GLUT_UP events
        
        if (button == 3) {
            scale *= exp(2.0 * 0.033);
        } else {
            scale /= exp(2.0 * 0.033);
        }
    } else { // normal button event

        rotating = (button == GLUT_LEFT_BUTTON);
        scaling = (button == GLUT_MIDDLE_BUTTON);
        translating = (button == GLUT_RIGHT_BUTTON);
        
        // Remember button state 
        int b = (button == GLUT_LEFT_BUTTON) ? 0 : ((button == GLUT_MIDDLE_BUTTON) ? 1 : 2);
        GLUTbutton[b] = (state == GLUT_DOWN) ? 1 : 0;
    }

    // Remember modifiers 
    GLUTmodifiers = glutGetModifiers();

    // Remember mouse position 
    GLUTmouse[0] = x;
    GLUTmouse[1] = y;

    glutPostRedisplay();
}

void MotionCallback(int x, int y) {
    // Invert y coordinate
    y = GLUTwindow_height - y;

    // Process mouse motion event
    if (rotating) {
        // Rotate model
        rotation[0] += -0.5 * (y - GLUTmouse[1]);
        rotation[2] += 0.5 * (x - GLUTmouse[0]);
    } else if (scaling) {
        // Scale window
        scale *= exp(2.0 * (float) (x - GLUTmouse[0]) / (float) GLUTwindow_width);
    } else if (translating) {
        // Translate window
        translation[0] += 2.0 * (float) (x - GLUTmouse[0]) / (float) GLUTwindow_width;
        translation[1] += 2.0 * (float) (y - GLUTmouse[1]) / (float) GLUTwindow_height;
    }

    // Remember mouse position 
    GLUTmouse[0] = x;
    GLUTmouse[1] = y;
}

void KeyboardDownCallback(unsigned char key, int x, int y) {
    switch (key) {
        case 'i':
        case 'I': 
            debugInfo = !debugInfo;
            break;
        case 27: // ESCAPE
            myGlutStop();
            break;
            
// animation controls
        case '1' :
            goto_triangle = 1;
            break;
        case '2' :
            goto_square = 1;
            break;
        case '3' :
            break;
            
// light controls            
        case 'a' :
        case 'A' :
            if (difflight_color[0] > 0) {
                difflight_color[0] -= 0.05;
            } else {
                difflight_color[0] = 0;                
            }
            if (difflight_color[0] > 0) {
                difflight_color[1] -= 0.05;
            } else {
                difflight_color[1] = 0;                
            }
            if (difflight_color[2] > 0) {
                difflight_color[2] -= 0.05;
            } else {
                difflight_color[2] = 0;                
            }
            break;
            
        case 's' :
        case 'S' :
            if (difflight_color[0] < 1) {
                difflight_color[0] += 0.05;
            } else {
                difflight_color[0] = 1;
            }            
            if (difflight_color[1] < 1) {
                difflight_color[1] += 0.05;
            } else {
                difflight_color[1] = 1;
            }            
            if (difflight_color[2] < 1) {
                difflight_color[2] += 0.05;
            } else {
                difflight_color[2] = 1;
            }            
            break;
        
        case 'd' :
        case 'D' :
            if (lightOn) {
                glDisable(GL_LIGHT0);
            } else {
                glEnable(GL_LIGHT0);
            }
            lightOn = !lightOn;
            break;
            
            
    }

    // Remember mouse position 
    GLUTmouse[0] = x;
    GLUTmouse[1] = GLUTwindow_height - y;

    // Remember modifiers 
    GLUTmodifiers = glutGetModifiers();

    glutPostRedisplay();
}

void WindowResizeCallback(int w, int h) {
    glViewport(0, 0, w, h);

    GLUTwindow_width = w;
    GLUTwindow_height = h;

}

void SceneDisplayCallback(void) {

    // Setup viewing transformation
    glLoadIdentity();
    glScalef(scale, scale, scale);
    glTranslatef(translation[0], translation[1], 0.0);

    // Set projection transformation
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (GLfloat) GLUTwindow_width / (GLfloat) GLUTwindow_height, 0.1, 100.0);

    // Set camera transformation
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(translation[0], translation[1], translation[2]);
    glScalef(scale, scale, scale);
    glRotatef(rotation[0], 1.0, 0.0, 0.0);
    glRotatef(rotation[1], 0.0, 1.0, 0.0);
    glRotatef(rotation[2], 0.0, 0.0, 1.0);
    glTranslatef(-center[0], -center[1], -center[2]);

    // Clear window 
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Set lights
    glLightfv(GL_LIGHT0, GL_POSITION, difflight_position);
    glLightfv(GL_LIGHT0, GL_DIFFUSE, difflight_color);

    static GLfloat light1_position[] = {-3.0, -4.0, -5.0, 0.0};
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);


    // Draw stuff 
    drawScene();
    drawDebugInfo();

    processAnimation();
    // Swap buffers 
    glutSwapBuffers();

}

void myGlutStop(void) {
    glutDestroyWindow(GLUTwindow);

    exit(0);
}
