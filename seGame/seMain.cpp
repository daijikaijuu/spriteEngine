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
#include "Render/seGameLevel.hpp"
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
    scene->HandleInput(window, key, scancode, action, mods);
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
    scene->Update(secondsElapsed);
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
