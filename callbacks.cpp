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
        if (rotating || scaling || translating) {
            glutIdleFunc(SceneDisplayCallback);
        } else {
            glutIdleFunc(0);
        }

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
        case 'I': // ESCAPE
            debugInfo = !debugInfo;
            break;
        case 27: // ESCAPE
            myGlutStop();
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
    //
    //    double scale, center;
    //    double windowXmin, windowXmax, windowYmin, windowYmax;
    //    
    //    // Define the portion of the window used for OpenGL rendering.
    //    glViewport(0, 0, w, h); // View port uses whole window
    //
    //    
    //    // Set up the projection view matrix: orthographic projection
    //    // Determine the min and max values for x and y that should appear in the window.
    //    // The complication is that the aspect ratio of the window may not match the
    //    //		aspect ratio of the scene we want to view.
    //    w = (w == 0) ? 1 : w;
    //    h = (h == 0) ? 1 : h;
    //    if ((Xmax - Xmin) / w < (Ymax - Ymin) / h) {
    //        scale = ((Ymax - Ymin) / h) / ((Xmax - Xmin) / w);
    //        center = (Xmax + Xmin) / 2;
    //        windowXmin = center - (center - Xmin) * scale;
    //        windowXmax = center + (Xmax - center) * scale;
    //        windowYmin = Ymin;
    //        windowYmax = Ymax;
    //    } else {
    //        scale = ((Xmax - Xmin) / w) / ((Ymax - Ymin) / h);
    //        center = (Ymax + Ymin) / 2;
    //        windowYmin = center - (center - Ymin) * scale;
    //        windowYmax = center + (Ymax - center) * scale;
    //        windowXmin = Xmin;
    //        windowXmax = Xmax;
    //    }
    //
    //    // Now that we know the max & min values for x & y that should be visible in the window,
    //    //		we set up the orthographic projection.
    //    glMatrixMode(GL_PROJECTION);
    //    glLoadIdentity();
    //    glOrtho(windowXmin, windowXmax, windowYmin, windowYmax, -1, 1);
    //

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
    static GLfloat light0_position[] = {3.0, 4.0, 5.0, 0.0};
    glLightfv(GL_LIGHT0, GL_POSITION, light0_position);
    static GLfloat light1_position[] = {-3.0, -2.0, -3.0, 0.0};
    glLightfv(GL_LIGHT1, GL_POSITION, light1_position);

    // Set material
    static GLfloat material[] = {1.0, 0.5, 0.5, 1.0};
    glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, material);


    drawScene();
    drawDebugInfo();

    // Swap buffers 
    glutSwapBuffers();

}

void myGlutStop(void) {
    glutDestroyWindow(GLUTwindow);

    exit(0);
}
