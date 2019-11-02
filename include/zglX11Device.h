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


#ifndef __zgl_x11_device_h__
#define __zgl_x11_device_h__

#if defined(linux) && (defined(i386) || defined(__x86_64))

#include "zglDevice.h"

#include<stdlib.h>
//#include<X11/X.h>
#include<X11/Xlib.h>
#include<GL/gl.h>
#include<GL/glx.h>
//#include<GL/glu.h>

/**
 * X11 + XGL porting.
 *
 */
class zglX11Device : public zglDevice
{
private:
	Display *dpy;
	Window root;
	XVisualInfo*vi;
	Colormap cmap;
	XSetWindowAttributes swa;
	Window win;
	GLXContext glc;
	XWindowAttributes gwa;
	XEvent xev;

public:
	zglX11Device()
	{
		dpy = NULL;
		vi = NULL;
	}

	virtual ~zglX11Device()
	{
	}

	bool init(int width, int height,
			const char * title = "Sample Application on zglEngine");

	bool fini();

	bool run();

	long getTickTime();

	void flipScreen();
};

#endif

#endif

