/*
 * Copyright (C) 2010 ZiiLABS Pte Ltd
 * All Rights Reserved.
 *
 * This software and its associated documentation may contain some
 * proprietary, confidential and trade secret information of
 * ZiiLABS Pte Ltd and except as provided by written agreement with
 * ZiiLABS Pte Ltd
 *
 * a)   no part may be disclosed, distributed, reproduced, transmitted,
 *      transcribed, stored in a retrieval system, adapted or translated
 *      in any form or by any means electronic, mechanical, magnetic,
 *      optical, chemical, manual or otherwise,
 *
 * and
 *
 * b)   the recipient is not entitled to discover through reverse
 *      engineering or reverse compiling or other such techniques or
 *      processes the trade secrets contained therein or in the
 *      documentation.
 */


#include "zglX11Device.h"

#ifdef WIN32

#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "zglDevice.h"
#include "zglCore.h"
#include "zglEngine.h"

#include "zglWin32Device.h"
#include "zglEventType.h"

static zglEngine * glEngine = (zglEngine *)NULL;
static zglDevice * glDevice = (zglDevice *)NULL;

static bool g_isQuitWindow = false;

static int g_width = 512;
static int g_height = 512;

static HWND  m_hWnd = NULL;		/* window */
static bool g_pressed = false;

//!< static interface for the zglDevice::getDevice
zglDevice * zglDevice::getDevice()
{
	if(glDevice == NULL)
	{
		glDevice = new zglWin32Device();
	}
	return glDevice;
}

void zglDevice::destroyDevice()
{
	if(glDevice != NULL)
	{
		delete glDevice;
		glDevice = (zglDevice *)NULL;
	}
}


static LONG WINAPI
WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{ 
    static PAINTSTRUCT ps;
    
    int x;
    int y;

    switch(uMsg) {
	case WM_ENTERSIZEMOVE:
	case WM_EXITSIZEMOVE:
		return 0;

	case WM_SYSCOMMAND:
		switch (wParam)
		{
		case SC_SCREENSAVE:
		case SC_MONITORPOWER:
		case SC_KEYMENU:
			return 0;
		}
		break;
		
    case WM_CHAR:
	switch (wParam) {
	case 0x51:            /*Q*/
	case 0x71:            /*q*/
	case 0x1b:			/* ESC key */
	    PostQuitMessage(0);
	    break;
	}
	return 0;
	
    case WM_LBUTTONDOWN:
    case WM_RBUTTONDOWN:
        g_pressed = true;
        SetCapture(m_hWnd);
        x = (int)(lParam & 0xFFFF);
        y = (int)((lParam >> 16 ) & 0xffff);
        glEngine->onTouchEvent(TOUCH_DOWN, x - g_width/2, -y + g_height/2, ::GetTickCount());
        break;
        
    case WM_MOUSEMOVE:
        if(g_pressed)
        {
            x = (int)(lParam & 0xFFFF);
            y = (int)((lParam >> 16 ) & 0xffff);
            glEngine->onTouchEvent(TOUCH_MOVE, x - g_width/2, -y + g_height/2, ::GetTickCount());
        }
        break;    

    case WM_LBUTTONUP:
    case WM_RBUTTONUP:
        ReleaseCapture();
        x = (int)(lParam & 0xFFFF);
        y = (int)((lParam >> 16 ) & 0xffff);
        glEngine->onTouchEvent(TOUCH_UP, x - g_width/2, -y + g_height/2, ::GetTickCount());        
        g_pressed = false;
        return 0;
        
	case WM_CLOSE:
		PostQuitMessage(0);
		return 0; 
    }

    return DefWindowProc(hWnd, uMsg, wParam, lParam); 
} 

bool zglWin32Device::init(int width, int height, const char * title)
{
    int         pf;
    HDC         hDC;
    HWND        hWnd;
    WNDCLASS    wc;
    PIXELFORMATDESCRIPTOR pfd;
    static HINSTANCE hInstance = 0;

    g_width = width;
    g_height = height;
    
    if (!hInstance) {
    	hInstance = GetModuleHandle(NULL);
    	wc.style         = CS_OWNDC;
    	wc.lpfnWndProc   = (WNDPROC)WindowProc;
    	wc.cbClsExtra    = 0;
    	wc.cbWndExtra    = 0;
    	wc.hInstance     = hInstance;
    	wc.hIcon         = LoadIcon(NULL, IDI_WINLOGO);
    	wc.hCursor       = LoadCursor(NULL, IDC_ARROW);
    	wc.hbrBackground = NULL;
    	wc.lpszMenuName  = NULL;
    	wc.lpszClassName = "zglEngine";
    
    	if (!RegisterClass(&wc)) {
    	    MessageBox(NULL, "RegisterClass() failed:  "
    		       "Cannot register window class.", "Error", MB_OK);
    	    return false;
    	}
    }
    
	DWORD dwExStyle	= WS_EX_APPWINDOW | WS_EX_WINDOWEDGE;;
	DWORD dwStyle	= WS_OVERLAPPEDWINDOW & ~WS_THICKFRAME & ~WS_MAXIMIZEBOX;;
    
	RECT rect;

	rect.left	= 0;
	rect.right	= width;
	rect.top	= 0;
	rect.bottom	= height;

	AdjustWindowRectEx(&rect, dwStyle, false, dwExStyle);
        
	hWnd = CreateWindowEx(
		dwExStyle,
		"zglEngine",
		title,
		dwStyle | WS_CLIPSIBLINGS |	WS_CLIPCHILDREN,
		CW_USEDEFAULT, CW_USEDEFAULT,
		rect.right  - rect.left,
		rect.bottom - rect.top,	
		NULL,
		NULL,
		hInstance,
		NULL);
		
    if (hWnd == NULL) {
    	MessageBox(NULL, "CreateWindow() failed:  Cannot create a window.",
		   "Error", MB_OK);
    	return false;
    }

    hDC = GetDC(hWnd);

    memset(&pfd, 0, sizeof(pfd));
    pfd.nSize        = sizeof(pfd);
    pfd.nVersion     = 1;
    pfd.dwFlags      = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
    pfd.iPixelType   = PFD_TYPE_RGBA;
    pfd.cColorBits   = 32;

    pf = ChoosePixelFormat(hDC, &pfd);
    if (pf == 0) {
    	MessageBox(NULL, "ChoosePixelFormat() failed:  "
    		   "Cannot find a suitable pixel format.", "Error", MB_OK); 
    	return false;
    } 
 
    if (SetPixelFormat(hDC, pf, &pfd) == FALSE) {
    	MessageBox(NULL, "SetPixelFormat() failed:  "
    		   "Cannot set format specified.", "Error", MB_OK);
    	return false;
    } 

    DescribePixelFormat(hDC, pf, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

    ReleaseDC(hWnd, hDC);
    
    m_hWnd = hWnd;
    m_hDC = GetDC(hWnd);
    m_hRC = wglCreateContext(m_hDC);
    wglMakeCurrent(m_hDC, m_hRC);
    
	glEngine = zglEngine::getEngine();
	
#if 0
	glEngine->setSize(m_width, m_height);
	glEngine->setViewport(0, 0, m_width, m_height);

	zglCore::reset();
#endif

    ShowWindow(hWnd, SW_SHOW);    
    UpdateWindow(hWnd);
    
    SetForegroundWindow(hWnd);
    SetFocus(hWnd);
    
	return true;
}

bool zglWin32Device::fini()
{
    wglMakeCurrent(NULL, NULL);
    ReleaseDC(m_hWnd, m_hDC);
    wglDeleteContext(m_hRC);
    DestroyWindow(m_hWnd);

	return true;
}

void zglWin32Device::flipScreen()
{
    ::SwapBuffers(m_hDC);
}

bool zglWin32Device::run()
{
    MSG   msg;
    msg.wParam = 0;
    g_isQuitWindow = false;
    
    while(!g_isQuitWindow)
    {
        if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
        {
            if(msg.message == WM_QUIT)
            {
                g_isQuitWindow = true;
                continue;
            }
            else
            {
                TranslateMessage(&msg);
    	        DispatchMessage(&msg);
            }
        }
        
        if(glEngine->update(::GetTickCount()))
        {
            glEngine->renderScreen();
            flipScreen();    
        }

        ::Sleep(16);
    }
    
    fini();

	return true;
}

long zglWin32Device::getTickTime()
{
	return (long)::GetTickCount();
}

#endif

