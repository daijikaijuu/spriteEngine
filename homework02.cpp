// 2015 Domrachev Alexandr <Alexandr.Domrachev@gmail.com>
#include "homework02.h"
#include "Scene.h"

HDC			hDC = NULL;
HGLRC		hRC = NULL;
HWND		hWnd = NULL;
HINSTANCE	hInstance;
Scene       *scene = NULL;

bool keys[256];
bool active = true;

LRESULT	CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

GLvoid ResizeGLScene(GLsizei width, GLsizei height)
{
    if (height == 0)
        height = 1;

    glViewport(0, 0, width, height);
    if (scene)
        scene->ResizeScene(width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluOrtho2D(0.0f, width, 0.0f, height);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int InitGL()
{
    glShadeModel(GL_SMOOTH);
    glClearColor(0.0f, 0.0f, 0.0f, 0.5f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    return true;
}

GLvoid KillGLWindow()
{
    if (hRC)
    {
        wglMakeCurrent(NULL, NULL);
        wglDeleteContext(hRC);
        hRC = NULL;
    }

    if (hDC && !ReleaseDC(hWnd, hDC))
    {
        hDC = NULL;
    }

    if (hWnd && !DestroyWindow(hWnd))
    {
        hWnd = NULL;
    }

    if (!UnregisterClass(L"Homework02OpenGLWindow", hInstance))
        hInstance = NULL;
}

BOOL CreateGLWindow(LPWSTR title, int width, int height, int bits)
{
    GLuint		PixelFormat;
    WNDCLASS    wc;
    DWORD       dwExStyle;
    DWORD       dwStyle;
    RECT        WindowRect = { 0, 0, width, height };

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
    wc.lpszClassName = L"Homework02OpenGLWindow";

    if (!RegisterClass(&wc))
        return FALSE;

    dwExStyle = WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;
    dwStyle = WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;

    AdjustWindowRectEx(&WindowRect, dwStyle, FALSE, dwExStyle);

    hWnd = CreateWindowEx(
        dwExStyle,
        L"Homework02OpenGLWindow",
        title,
        dwStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        0, 0,
        WindowRect.right - WindowRect.left,
        WindowRect.bottom - WindowRect.top,
        NULL,
        NULL,
        hInstance,
        NULL);
    if (!hWnd)
    {
        KillGLWindow();
        return FALSE;
    }

    static	PIXELFORMATDESCRIPTOR pfd =				// pfd Tells Windows How We Want Things To Be
    {
        sizeof(PIXELFORMATDESCRIPTOR),				// Size Of This Pixel Format Descriptor
        1,											// Version Number
        PFD_DRAW_TO_WINDOW |						// Format Must Support Window
        PFD_SUPPORT_OPENGL |						// Format Must Support OpenGL
        PFD_DOUBLEBUFFER,							// Must Support Double Buffering
        PFD_TYPE_RGBA,								// Request An RGBA Format
        BYTE(bits),										// Select Our Color Depth
        0, 0, 0, 0, 0, 0,							// Color Bits Ignored
        0,											// No Alpha Buffer
        0,											// Shift Bit Ignored
        0,											// No Accumulation Buffer
        0, 0, 0, 0,									// Accumulation Bits Ignored
        16,											// 16Bit Z-Buffer (Depth Buffer)
        0,											// No Stencil Buffer
        0,											// No Auxiliary Buffer
        PFD_MAIN_PLANE,								// Main Drawing Layer
        0,											// Reserved
        0, 0, 0										// Layer Masks Ignored
    };

    if (!(hDC = GetDC(hWnd)))
    {
        KillGLWindow();
        return FALSE;
    }

    if (!(PixelFormat = ChoosePixelFormat(hDC, &pfd)))
    {
        KillGLWindow();
        return FALSE;
    }

    if (!SetPixelFormat(hDC, PixelFormat, &pfd))
    {
        KillGLWindow();
        return FALSE;
    }

    if (!(hRC = wglCreateContext(hDC)))
    {
        KillGLWindow();
        return FALSE;
    }

    if (!wglMakeCurrent(hDC, hRC))
    {
        KillGLWindow();
        return FALSE;
    }

    ShowWindow(hWnd, SW_SHOW);
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);

    ResizeGLScene(width, height);

    if (!InitGL())
    {
        KillGLWindow();
        return FALSE;
    }

    return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_ACTIVATE:
        active = HIWORD(wParam) ? false : true;
        return 0;
    case WM_SIZE:
        ResizeGLScene(LOWORD(lParam), HIWORD(lParam));
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

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    MSG     msg;
    bool    done = false;

    CreateGLWindow(L"Homework02 openGL rework", 800, 600, 32);

    scene = new Scene(800, 600);

    while (!done)
    {
        if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if (msg.message == WM_QUIT)
            {
                done = true;
            }
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
                {
                    done = true;
                }
                else
                {
                    scene->Draw();
                    SwapBuffers(hDC);
                }
            }
        }
    }

    KillGLWindow();
    delete scene;

    return (int)msg.wParam;
}
