//
//  seMain.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "Scene/gameScene.hpp"
#include "Debug/Debug.hpp"
#include "Resources/Resources.hpp"
#include "Render/seSceneObject.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow *gWindow = nullptr;
gameScene *scene = nullptr;
unsigned int spr = 1;

using namespace spriteEngine;

void OnError(int errorCode, const char* msg) {
    std::string ss = "OnError callback: " + std::string(msg);
    throw seException(ss);
}

void OnKeyPressed(GLFWwindow *window, int key, int scancode, int action, int mods) {
    seGenericSceneObject *iceman = scene->GetItem("sceneObject:iceman");
    iceman->GetProgram()->Bind();

    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS) {
        spr++;
    }
    if (key == GLFW_KEY_RIGHT) {
        spr++;
        iceman->SetMirrored(false);
        iceman->Move(2.0f, 0.0f);
    }
    if (key == GLFW_KEY_LEFT) {
        spr++;
        iceman->SetMirrored(true);
        iceman->Move(-2.0f, 0.0f);
    }

    iceman->GetProgram()->SetUniform("spriteCurrent", spr);
    iceman->GetProgram()->Unbind();
    if (spr > 5) spr = 1;
}

void Render() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    if (scene) {
        scene->Render();
    }

    glfwSwapBuffers(gWindow);
}

void Update(GLfloat secondsElapsed) {
    seGenericSceneObject *sun = scene->GetItem("sceneObject:sun");
    sun->Rotate(secondsElapsed);

    seGenericSceneObject *bird = scene->GetItem("sceneObject:bird");
    GLfloat x = bird->X();
    bird->GetProgram()->Bind();
    if (x > (800 - bird->Width() / 2) && !bird->IsMirrored())
        bird->SetMirrored(true);
    if (x < bird->Width() / 2 && bird->IsMirrored())
        bird->SetMirrored(false);
    GLfloat dx = bird->IsMirrored() ? -1 : 1;
    bird->Move(dx, 0);
    bird->GetProgram()->Unbind();

    static GLfloat counter;
    counter += secondsElapsed;
    if (counter > 0.2f) {
        bird->GetProgram()->Bind();
        bird->Animate();
        bird->GetProgram()->Unbind();
        counter = 0;
    }
}

void AppMain() {
    // initialize GLFW
    glfwSetErrorCallback(OnError);
    if (!glfwInit())
        throw seException("glfwInit failed");

    // open a window with GLFW
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    gWindow = glfwCreateWindow(800, 600, "spriteEngine main window", nullptr, nullptr);
    if (!gWindow) {
        glfwTerminate();
        throw seException("glfwCreateWindow failed. Can your hardware handle OpenGL 3.3?");
    }

    // GLFW settings
    glfwMakeContextCurrent(gWindow);
    glfwSetKeyCallback(gWindow, OnKeyPressed);

    // initalize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        glfwTerminate();
        throw seException("glewInit failed");
    }

    // print out some info about the graphics drivers
    LogDebug << "OpenGL version: " << glGetString(GL_VERSION) << eol;
    LogDebug << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << eol;
    LogDebug << "Vendor: " << glGetString(GL_VENDOR) << eol;
    LogDebug << "Renderer: " << glGetString(GL_RENDERER) << eol;

    // make sure OpenGL version 3.3 API is available
    if (!GLEW_VERSION_3_3)
        throw seException("OpenGL 3.3 API is not available");

    glShadeModel(GL_SMOOTH);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    scene = new gameScene(800, 600);

    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(gWindow)) {
        glfwPollEvents();

        double thisTime = glfwGetTime();
        Update((GLfloat)(thisTime - lastTime));
        lastTime = thisTime;

        Render();
    }

    // clean up and exit
    delete scene;
    scene = nullptr;

    glfwDestroyWindow(gWindow);
    glfwTerminate();
}

int main(int argc, char *argv[]) {
    seLogger::Initialize();
    try {
        AppMain();
    } catch (const seException &e) {
        LogError << e.what() << eol;
    }

    seLogger::Destroy();

    return 0;
}
