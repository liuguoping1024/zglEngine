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

#include <stdlib.h>
#include <unistd.h>

#include "zglDevice.h"
#include "zglAndroidNativeDevice.h"

#ifdef _ANDROID_NATIVE_

#include <android/log.h>

#include "zglCore.h"
#include "zglEngine.h"

using namespace android;

static zglEngine * glEngine = (zglEngine *)NULL;
static zglDevice * glDevice = (zglDevice *)NULL;

//!< static interface for the zglDevice::getDevice
zglDevice * zglDevice::getDevice()
{
	if(glDevice == NULL)
	{
		glDevice = new zglAndroidNativeDevice();
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

//!< Implement of the zglAndroidNativeDevice

//!< GL/EGL attributes.
static EGLint configAttribs[] =
{
	EGL_DEPTH_SIZE, 0,
	EGL_NONE
};

bool zglAndroidNativeDevice::init(int width, int height, const char * title)
{
	__android_log_print(ANDROID_LOG_INFO, "zglDemo", "zglAndroidNativeDevice::init()");

	EGLNativeWindowType window = android_createDisplaySurface();

	dpy = eglGetDisplay(EGL_DEFAULT_DISPLAY);
	eglInitialize(dpy, &majorVersion, &minorVersion);

	status_t err = EGLUtils::selectConfigForNativeWindow(
			dpy, configAttribs, window, &config);
	if (err)
	{
		fprintf(stderr, "couldn't find an EGLConfig matching the screen format\n");
		return 0;
	}

	surface = eglCreateWindowSurface(dpy, config, window, NULL);
	context = eglCreateContext(dpy, config, NULL, NULL);
	eglMakeCurrent(dpy, surface, surface, context);
	eglQuerySurface(dpy, surface, EGL_WIDTH, &m_width);
	eglQuerySurface(dpy, surface, EGL_HEIGHT, &m_height);

	//post initial
	glEngine = zglEngine::getEngine();

	glEngine->setSize(m_width, m_height);
	glEngine->setViewport(0, 0, m_width, m_height);

	zglCore::reset();
	return true;
}

bool zglAndroidNativeDevice::fini()
{
	eglMakeCurrent (dpy, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
	eglDestroySurface (dpy, surface);
	eglDestroyContext (dpy, context);
	eglTerminate (dpy);

	zglEngine::destroyEngine();
	return true;
}

bool zglAndroidNativeDevice::run()
{
	int loop = 0;

	while(loop < 65536)
	{

		if(glEngine->update())
		{
			//glEngine->postUpdate();
			glEngine->renderScreen();
			flipScreen();
		}

		//usleep(20000);
	}

	fini();
	return true;
}

long zglAndroidNativeDevice::getTickTime()
{
	return 0l;
}

void zglAndroidNativeDevice::flipScreen()
{
	eglSwapBuffers(dpy, surface);
}

#endif

