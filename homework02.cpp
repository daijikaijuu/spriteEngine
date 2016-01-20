// 2015 Domrachev Alexandr <Alexandr.Domrachev@gmail.com>
#include "homework02.h"
#include "Scene.h"
#include "helpers.h"
#include "Logger.h"

const int WIDTH = 800;
const int HEIGHT = 600;

Scene *scene = NULL;

HGLRC hRC = NULL;
HGLRC hTempRC = NULL;
HDC hDC = NULL;
HWND hWnd = NULL;
HINSTANCE hInstance;

static const wchar_t* WNDCLASSNAME = L"Homework02OGLWindow";
static const wchar_t* WINDOWTITLE = L"Homework02 OpenGL Window";

bool keys[256];
bool active = TRUE;

LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
#ifdef _MSC_VER
void APIENTRY openglDebugCallbackFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, void *userParam);
#else
void APIENTRY openglDebugCallbackFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam);
#endif //_MSC_VER

bool InitOpenGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0, 0, 0, 0);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    return true;
}

void KillGLWindow()
{
    if (hRC)
    {
        if (!wglMakeCurrent(NULL, NULL))
            debugError("KillGLWindow: Release of DC and RC failed.");
        if (!wglDeleteContext(hRC))
            debugError("KillGLWindow: RC release failed.");
        hTempRC = NULL;
        hRC = NULL;
    }
    if (hDC && !ReleaseDC(hWnd, hDC))
    {
        debugError("KillGLWindow: DC release failed.");
        hDC = NULL;
    }
    if (hWnd && !DestroyWindow(hWnd))
    {
        debugError("KillGLWindow: HWND release failed.");
        hWnd = NULL;
    }
    if (!UnregisterClass(WNDCLASSNAME, hInstance))
    {
        debugError("KillGLWindow: Could not unregister class.");
        hInstance = NULL;
    }
}

bool CreateGLWindow(int width, int height, int bits)
{
    GLuint PixelFormat;
    WNDCLASS wc;
    DWORD dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;
    DWORD dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    RECT WindowRect = { 0, 0, width, height };

    hInstance = GetModuleHandle(NULL);
    wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    wc.lpfnWndProc = (WNDPROC)WndProc;
    wc.cbClsExtra = 0;
    wc.cbWndExtra = 0;
    wc.hInstance = hInstance;
    wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.hbrBackground = NULL;
    wc.lpszMenuName = NULL;
    wc.lpszClassName = WNDCLASSNAME;
    if (!RegisterClass(&wc))
    {
        debugError("CreateGLWindow: Failed to register window class.");
        return false;
    }

    AdjustWindowRectEx(&WindowRect, dwStyle, false, dwExStyle);

    hWnd = CreateWindowEx(
        dwExStyle,
        WNDCLASSNAME,
        WINDOWTITLE,
        WS_CLIPSIBLINGS | WS_CLIPCHILDREN | dwStyle,
        0, 0,
        WindowRect.right - WindowRect.left, WindowRect.bottom - WindowRect.top,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (!hWnd)
    {
        KillGLWindow();
        debugError("CreateGLWindow: Failed to create window.");
        return false;
    }

    static PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),  // Size Of This Pixel Format Descriptor
        1,                              // Version Number
        PFD_DRAW_TO_WINDOW |            // Format Must Support Window
        PFD_SUPPORT_OPENGL |            // Format Must Support OpenGL
        PFD_DOUBLEBUFFER,               // Must Support Double Buffering
        PFD_TYPE_RGBA,                  // Request An RGBA Format
        (BYTE)bits,                     // Select Our Color Depth
        0, 0, 0, 0, 0, 0,               // Color Bits Ignored
        0,                              // No Alpha Buffer
        0,                              // Shift Bit Ignored
        0,                              // No Accumulation Buffer
        0, 0, 0, 0,                     // Accumulation Bits Ignored
        16,                             // 16Bit Z-Buffer (Depth Buffer)
        0,                              // No Stencil Buffer
        0,                              // No Auxiliary Buffer
        PFD_MAIN_PLANE,                 // Main Drawing Layer
        0,                              // Reserved
        0, 0, 0                         // Layer Masks Ignored
    };

    if (!(hDC = GetDC(hWnd)))
    {
        KillGLWindow();
        debugError("CreateGLWindow: Could not create OpenGL DC.");
        return false;
    }

    if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))
    {
        KillGLWindow();
        debugError("CreateGLWindow: Can't find suitable PixelFormat.");
        return false;
    }

    if (!SetPixelFormat(hDC, PixelFormat, &pfd))
    {
        KillGLWindow();
        debugError("CreateGLWindow: Can't set PixelFormat.");
        return false;
    }

    if (!(hTempRC = wglCreateContext(hDC)))
    {
        KillGLWindow();
        debugError("CreateGLWindow: Can't create OpenGL RC.");
        return false;
    }

    if (!wglMakeCurrent(hDC, hTempRC))
    {
        KillGLWindow();
        debugError("CreateGLWindow: Can't activate OpenGL RC.");
        return false;
    }


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
        debugError("Your graphic card does not support OpenGL 3.3\n");
        return 1;
    }
    if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
        debug("Vertex & fragment shaders supported.");
    else
    {
        debugError("Vertex/fragment shaders not supported. Abort.");
        return 1;
    }

    hRC = hTempRC;
    int attribs[] = {
        WGL_CONTEXT_MAJOR_VERSION_ARB, 3,
        WGL_CONTEXT_MINOR_VERSION_ARB, 1,
        WGL_CONTEXT_FLAGS_ARB, 0
#ifdef _DEBUG
            | WGL_CONTEXT_DEBUG_BIT_ARB
#endif // _DEBUG
        , 0
    };
    if (wglewIsSupported("WGL_ARB_create_context"))
    {
        hRC = wglCreateContextAttribsARB(hDC, 0, attribs);
        if (hRC)
        {
            wglMakeCurrent(NULL, NULL);
            wglDeleteContext(hTempRC);
            wglMakeCurrent(hDC, hRC);
        }
        else
            hRC = hTempRC;
    }

    debugInfo("OpenGL initialized: OpenGL version: ", glGetString(GL_VERSION), " GLSL version: ", glGetString(GL_SHADING_LANGUAGE_VERSION));
#ifdef _DEBUG
    if (glDebugMessageCallback)
    {
        debug("Register OpenGL debug callback.");
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glEnable(GL_DEBUG_OUTPUT);
        glDebugMessageCallback(openglDebugCallbackFunc, nullptr);
        glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, 0, true);
    }
    else
        debugError("glDebugMessageCallback not available.");
#endif // _DEBUG


    ShowWindow(hWnd, SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);

    //Resize;

    if (!InitOpenGL())
    {
        KillGLWindow();
        debugError("CreateGLWindow: Initialization failed.");
        return false;
    }

    return true;
}

#ifdef _MSC_VER
void APIENTRY openglDebugCallbackFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, void *userParam)
#else
void APIENTRY openglDebugCallbackFunc(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
#endif //_MSC_VER
{
    std::stringstream msg;
    msg << "--------------opengl-callback-start------------" << endl;
    msg << "message: " << message << endl;
    msg << "type: ";
    switch (type)
    {
    case GL_DEBUG_TYPE_ERROR:
        msg << "ERROR";
        break;
    case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
        msg << "DEPRECATED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
        msg << "UNDEFINED_BEHAVIOR";
        break;
    case GL_DEBUG_TYPE_PORTABILITY:
        msg << "PORTABILITY";
        break;
    case GL_DEBUG_TYPE_PERFORMANCE:
        msg << "PERFORMANCE";
        break;
    case GL_DEBUG_TYPE_OTHER:
        msg << "OTHER";
        break;
    }
    msg << endl;

    msg << "id: " << id << endl;
    msg << "severity: ";
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_LOW:
        msg << "LOW";
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        msg << "MEDIUM";
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        msg << "HIGH";
        break;
    }
    msg << endl;
    msg << "---------------------opengl-callback-end--------------";
    debug(msg.str());
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_ACTIVATE:
        if (!HIWORD(wParam))
            active = TRUE;
        else
            active = FALSE;
        return 0;
    case WM_CLOSE:
        PostQuitMessage(0);
        return 0;
    case WM_KEYDOWN:
        keys[wParam] = true;
        return 0;
    case WM_KEYUP:
        keys[wParam] = false;
        return 0;
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

#ifdef _DEBUG
int main(int argc, char **argv)
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif // _DEBUG
{
    bool bUseFrameBuffer = true;

#ifdef _DEBUG
    debugInfo("Command-line arguments:");
    for (size_t i = 0; i < argc; i++)
    {
        debugInfo("argv[", i, "] = ", argv[i]);
        if (strcmp(argv[i], "-nofb") == 0)
            bUseFrameBuffer = false;
    }
#endif // _DEBUG

    MSG msg;
    bool done = false;

    if (!CreateGLWindow(WIDTH, HEIGHT, 16))
        return 0;
    debug("Created main window.");

    HANDLE hTimer = CreateWaitableTimer(NULL, true, NULL);
    LARGE_INTEGER liDueTime;
    liDueTime.QuadPart = -166666LL;
    if (hTimer == NULL)
    {
        debugError("CreateWaitableTimer failed ", GetLastError());
    }
    if (!SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0))
    {
        debugError("SetWaitableTimer failed ", GetLastError());
    }

    scene = new Scene(WIDTH, HEIGHT, bUseFrameBuffer);

    while (!done)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
                done = true;
            else
            {
                TranslateMessage(&msg);
                DispatchMessage(&msg);
            }
        }
        else
        {
            if (active)
            {
                if (keys[VK_ESCAPE])
                    done = true;
                else
                {
                    if (scene)
                    {
                        SetWaitableTimer(hTimer, &liDueTime, 0, NULL, NULL, 0);

                        scene->Draw();
                        SwapBuffers(hDC);
                        scene->Animate(0);

                        if (WaitForSingleObject(hTimer, INFINITE) != WAIT_OBJECT_0)
                            debugError("WaitForSingleObject failed ", GetLastError());
                    }
                }
            }
        }
    }


    if (scene)
    {
        delete scene;
        scene = NULL;
    }

    KillGLWindow();

    LogManager::Destroy();

    return 0;
}
