// 2015 Domrachev Alexandr <Alexandr.Domrachev@gmail.com>
#include "homework02.h"
#include "Scene.h"
#include "helpers.h"
#include <GL\glut.h>

Scene *scene = NULL;

void renderScene()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    scene->Draw();

    glutSwapBuffers();
}

void reshape(GLsizei width, GLsizei height)
{
    if (height == 0)
        height = 1;

    glViewport(0, 0, width, height);
    scene->ResizeScene(width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void processNormalKeys(unsigned char key, int x, int y)
{
    if (key == 27)
        exit(0);
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(000, 000);
    glutInitWindowSize(800, 600);
    glutCreateWindow("Homework OpegnGL window");

    glewExperimental = GL_TRUE;
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK)
    {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(glew_status));
        std::cin.ignore();
        return 1;
    }

    if (!GLEW_VERSION_4_0)
    {
        fprintf(stderr, "Error: your graphic card does not support OpenGL 4.0\n");
        std::cin.ignore();
        return 1;
    }

    scene = new Scene(800, 600);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);

    glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(processNormalKeys);

    glutMainLoop();

    delete scene;

    return 0;
}
