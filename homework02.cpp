// 2015 Domrachev Alexandr <Alexandr.Domrachev@gmail.com>
#include "homework02.h"
#include "Scene.h"
#include "helpers.h"
#include "Logger.h"
#include <GL/glut.h>

const int WIDTH = 800;
const int HEIGHT = 600;

Scene *scene = NULL;

void renderScene()
{
    scene->Draw();

    glutSwapBuffers();
}

void onTimer(int value)
{
    glutTimerFunc(33, onTimer, 0);

    int elapsedTime = glutGet(GLUT_ELAPSED_TIME);
    scene->Animate(elapsedTime);

    glutPostRedisplay();
}

void reshape(GLsizei width, GLsizei height)
{
    glutReshapeWindow(WIDTH, HEIGHT);
}

void processNormalKeys(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    debug("Glut initialized.");
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Homework OpenGL window");
    debug("Created window.");

    glewExperimental = GL_TRUE;
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK)
    {
        debugError(glewGetErrorString(glew_status));
        return 1;
    }
    debug("Glew initialized.");

    if (!GLEW_VERSION_3_3)
    {
        debugError("Your graphic card does not support OpenGL 4.0\n");
        return 1;
    }

    if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
        debug("Vertex & fragment shaders supported.");
    else
    {
        debugError("Vertex/fragment shaders not supported. Abort.");
        return 1;
    }

    scene = new Scene(WIDTH, HEIGHT);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(renderScene);
    glutTimerFunc(33, onTimer, 0);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(processNormalKeys);

    glutMainLoop();

    delete scene;

    return 0;
}
