//
//  seMain.cpp
//  spriteEngine
//
//  Created by Domrachev Alexandr on 27.01.16.
//  Copyright © 2016 Domrachev Alexandr. All rights reserved.
//

#include "Render/seScene.hpp"
#include "Debug/seDebug.hpp"
#include <GL/glew.h>
#include <GLFW/glfw3.h>

GLFWwindow *gWindow = nullptr;
spriteEngine::seScene *scene = nullptr;

using namespace spriteEngine;

void OnError(int errorCode, const char* msg) {
    std::string ss = "OnError callback: " + std::string(msg);
    throw seException(ss);
}

static void Render() {
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT);

    if (scene) {
        scene->Render();
    }

    glfwSwapBuffers(gWindow);
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
    if (!gWindow)
        throw seException("glfwCreateWindow failed. Can your hardware handle OpenGL 3.3?");

    // GLFW settings
    glfwMakeContextCurrent(gWindow);

    // initalize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
        throw seException("glewInit failed");

    // print out some info about the graphics drivers
    LogDebug << "OpenGL version: " << glGetString(GL_VERSION) << eol;
    LogDebug << "GLSL version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << eol;
    LogDebug << "Vendor: " << glGetString(GL_VENDOR) << eol;
    LogDebug << "Renderer: " << glGetString(GL_RENDERER) << eol;

    // make sure OpenGL version 3.3 API is available
    if (!GLEW_VERSION_3_3)
        throw seException("OpenGL 3.3 API is not available");

    scene = new spriteEngine::seScene(800, 600);

    while (!glfwWindowShouldClose(gWindow)) {
        glfwPollEvents();

        Render();
    }

    // clean up and exit
    delete scene;
    scene = nullptr;

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
