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

#ifndef __zgl_android_native_device_h__
#define __zgl_android_native_device_h__

/**
 * The implement on the Android Linux (AOSP) platform.
 * The Native device will update the frame buffer directly; the screen will be
 * erased by the applications, and the executable file need to be put into the
 * target device, and be executed by 'adb shell'
 *
 * @deprecated
 */
#ifdef _ANDROID_NATIVE_

#include "zglDevice.h"

#include <stdlib.h>

#include <EGL/egl.h>
#include <GLES/gl.h>
#include <GLES/glext.h>

//!< Android Framebuffer
#include <ui/FramebufferNativeWindow.h>
#include <ui/EGLUtils.h>

class zglAndroidNativeDevice : public zglDevice
{
private:
	EGLint majorVersion;
	EGLint minorVersion;
	EGLContext context;
	EGLConfig config;
	EGLSurface surface;
	EGLDisplay dpy;

public:
	zglAndroidNativeDevice()
	{
	}

	virtual ~zglAndroidNativeDevice()
	{
	}

	/**
	 * set up the enviroment on the Android platform.
	 * @param width
	 * @param height
	 */
	bool init(int width, int height, const char * title);

	/**
	 * destroy the enviroment
	 */
	bool fini();

	/**
	 * main loop of the application.
	 */
	bool run();

	long getTickTime();

	void flipScreen();
};

#endif //_ANDROID_NATIVE_
#endif


