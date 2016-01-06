// 2015 Domrachev Alexandr <Alexandr.Domrachev@gmail.com>
#include "homework02.h"
#include "Scene.h"
#include "helpers.h"
#include <GL/glut.h>

const int WIDTH = 800;
const int HEIGHT = 600;

Scene *scene = NULL;

void renderScene()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

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
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowPosition(100, 100);
    glutInitWindowSize(WIDTH, HEIGHT);
    glutCreateWindow("Homework OpegnGL window");

    glewExperimental = GL_TRUE;
    GLenum glew_status = glewInit();
    if (glew_status != GLEW_OK)
    {
#ifdef _DEBUG
        std::cout << "Error: " << glewGetErrorString(glew_status) << std::endl;
        std::cin.ignore();
#endif // _DEBUG
        return 1;
    }

    if (!GLEW_VERSION_4_0)
    {
#ifdef _DEBUG
        std::cout << "Error: your graphic card does not support OpenGL 4.0\n" << std::endl;
        std::cin.ignore();
#endif // _DEBUG
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
