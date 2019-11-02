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

#if defined(linux) && (defined(i386) || defined(__x86_64))
#include <unistd.h>
#include <sys/time.h>
#include <time.h>
#include <string.h>

#include <stdio.h>

#include "zglDevice.h"
#include "zglCore.h"
#include "zglEngine.h"

#include "zglEventType.h"

static zglEngine * glEngine = (zglEngine *)NULL;
static zglDevice * glDevice = (zglDevice *)NULL;

//!< static interface for the zglDevice::getDevice
zglDevice * zglDevice::getDevice()
{
	if(glDevice == NULL)
	{
		glDevice = new zglX11Device();
	}
	return glDevice;
}

void zglDevice::destroyDevice()
{
	if(glDevice != NULL)
	{
		delete glDevice;
		glDevice = (zglDevice *)0;
	}
}

GLint att[] =
{	GLX_RGBA, GLX_DEPTH_SIZE, 24, GLX_DOUBLEBUFFER, None};

bool zglX11Device::init(int width, int height, const char * title)
{
	Atom wmDelete;

	dpy = XOpenDisplay(NULL);

	if(dpy == NULL)
	{
		return false;
	}

	root = DefaultRootWindow(dpy);

	vi = glXChooseVisual(dpy, 0, att);

	if(vi == NULL)
	{
		return false;
	}

	cmap = XCreateColormap(dpy, root, vi->visual, AllocNone);

	swa.colormap = cmap;
	swa.event_mask = ExposureMask|KeyPressMask|KeyReleaseMask|ButtonPressMask|ButtonReleaseMask|ButtonMotionMask | StructureNotifyMask;
	swa.cursor = None;

	win = XCreateWindow(dpy, root, 0, 0, width, height, 0, vi->depth, InputOutput, vi->visual, CWColormap | CWEventMask, &swa);

	wmDelete = XInternAtom(dpy, "WM_DELETE_WINDOW", true);
	XSetWMProtocols(dpy, win, &wmDelete, 1);

	XMapWindow(dpy, win);
	XStoreName(dpy, win, title);

	glc = glXCreateContext(dpy, vi, NULL, GL_TRUE);
	glXMakeCurrent(dpy, win, glc);

	//post initial
	XGetWindowAttributes(dpy, win, &gwa);
	m_width = gwa.width;
	m_height = gwa.height;

	glEngine = zglEngine::getEngine();
#if 0
	glEngine->setSize(m_width, m_height);
	glEngine->setViewport(0, 0, m_width, m_height);

	zglCore::reset();
#endif
	return true;
}

bool zglX11Device::fini()
{
	glXMakeCurrent(dpy, None, NULL);
	glXDestroyContext(dpy, glc);
	XDestroyWindow(dpy, win);
	XCloseDisplay(dpy);

	//Post
	//zglEngine::destroyEngine();

	return true;
}

void zglX11Device::flipScreen()
{
	glXSwapBuffers(dpy, win);
}

bool zglX11Device::run()
{
	long time_val;
	bool bquit = false;
	XGetWindowAttributes(dpy, win, &gwa);

	XSelectInput(dpy, win, ExposureMask | KeyPressMask | KeyReleaseMask
			|ButtonPressMask | ButtonReleaseMask | ButtonMotionMask
			| StructureNotifyMask);

	while (!bquit)
	{
		while (XPending(dpy))
		{
			XNextEvent(dpy, &xev);

			time_val = getTickTime();
			switch (xev.type)
			{
			case KeyPress:
				if (xev.xkey.keycode == 9 || xev.xkey.keycode == 24)
				{
					bquit = true;
					break;
				}
				else if (xev.xkey.keycode == 27) //'restart'
				{

				}
				else if (xev.xkey.keycode == 39) //'stop'
				{

				}

#if 0	//!< Close the key event for zglEngine temperary
				else
				{
					glEngine->onKeyEvent(KEY_DOWN, xev.xkey.keycode);
					break;
				}

				case KeyRelease:
				glEngine->onKeyEvent(KEY_UP, xev.xkey.keycode);
#endif
				break;

			case ButtonPress:
				glEngine->onTouchEvent(TOUCH_DOWN, xev.xbutton.x - m_width / 2,
						-xev.xbutton.y + m_height / 2, time_val);
				break;

			case ButtonRelease:
				glEngine->onTouchEvent(TOUCH_UP, xev.xbutton.x - m_width / 2,
						-xev.xbutton.y + m_height / 2, time_val);
				break;

			case MotionNotify:
				glEngine->onTouchEvent(TOUCH_MOVE, xev.xbutton.x - m_width / 2,
						-xev.xbutton.y + m_height / 2, time_val);
				break;

			case ClientMessage:
				if (strcmp(XGetAtomName(dpy, xev.xclient.message_type),
						"WM_PROTOCOLS") == 0)
				{
					bquit = true;
				}
				break;

			case Expose:
				{
					glEngine->renderScreen();
					flipScreen();
				}
				break;

			default:
				break;

			}
		};

		bool ret = glEngine->update(getTickTime());
		if (ret)
		{
			glEngine->renderScreen();
			flipScreen();

			//printf("onDrawFrame\n");
		}

		usleep(16100);
	}

	fini();
	return true;
}

long zglX11Device::getTickTime()
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return (long)(now.tv_sec*1000 + now.tv_usec/1000);
}

#endif

