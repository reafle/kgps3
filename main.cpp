#include "loader.h"
#include "common.h"
#include "callbacks.h"

#include "GL/freeglut.h"

void myGlutInit(int *argc, char **argv) {
    // Open window 
    glutInit(argc, argv);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(GLUTwindow_width, GLUTwindow_height);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH); // | GLUT_STENCIL
    GLUTwindow = glutCreateWindow(TITLE_STRING);

    // Initialize GLUT callback functions 
	glutKeyboardFunc( KeyboardDownCallback );
//	glutKeyboardUpFunc( KeyboardUpCallback );			
	glutMouseFunc(MouseCallback);
	glutReshapeFunc( WindowResizeCallback );
	glutDisplayFunc( SceneDisplayCallback );
//    glutSpecialFunc(GLUTSpecial);
    glutMotionFunc(MotionCallback);
    glutIdleFunc(0);

    // Initialize lights 
    static GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    glShadeModel(GL_SMOOTH);

    
    static GLfloat light0_diffuse[] = {1.0, 1.0, 1.0, 1.0};
    static GLfloat light1_diffuse[] = {0.5, 0.5, 0.5, 1.0};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0_diffuse);
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1_diffuse);

    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);

    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);

    // Initialize graphics modes 
    glEnable(GL_DEPTH_TEST);
}

void myGlutMain(void) {
    // Compute bounding box
    float bbox[2][3] = {
        { 1.0E30F, 1.0E30F, 1.0E30F},
        { -1.0E30F, -1.0E30F, -1.0E30F}
    };
    
    for (int i = 0; i < mesh->nverts; i++) {
        Vertex& vert = mesh->verts[i];
        if (vert.x < bbox[0][0]) bbox[0][0] = vert.x;
        else if (vert.x > bbox[1][0]) bbox[1][0] = vert.x;
        if (vert.y < bbox[0][1]) bbox[0][1] = vert.y;
        else if (vert.y > bbox[1][1]) bbox[1][1] = vert.y;
        if (vert.z < bbox[0][2]) bbox[0][2] = vert.z;
        else if (vert.z > bbox[1][2]) bbox[1][2] = vert.z;
    }

    // Setup initial viewing scale
    float dx = bbox[1][0] - bbox[0][0];
    float dy = bbox[1][1] - bbox[0][1];
    float dz = bbox[1][2] - bbox[0][2];
    scale = 2.0 / sqrt(dx * dx + dy * dy + dz * dz);

    // Setup initial viewing center
    center[0] = 0.5 * (bbox[1][0] + bbox[0][0]);
    center[1] = 0.5 * (bbox[1][1] + bbox[0][1]);
    center[2] = 0.5 * (bbox[1][2] + bbox[0][2]);

    glutMainLoop();
}


int main(int argc, char **argv) {
    myGlutInit(&argc, argv);

    if (argc != 2) {
        fprintf(stderr, "Usage: ./main [filename]\n");
        return 1;
    }
    
    mesh = ReadOffFile(argv[1]);
    if (!mesh) {
        exit(1);
    } else {
        fprintf(stdout, "Mesh parsed successfully\n");
    }

    myGlutMain();
    
    return 0;
}