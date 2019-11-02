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


#ifndef __zgl_win32_device_h__
#define __zgl_win32_device_h__

#if defined(WIN32) && defined(i386)

#include "zglDevice.h"

#include <windows.h>			/* must include this before GL/gl.h */
#include <GL/gl.h>			/* OpenGL header file */
#include <GL/glu.h>			/* OpenGL utilities header file */

/**
 * Win32 + MINGW + WGL, platform porting.
 */
class zglWin32Device : public zglDevice
{
private:
    HDC m_hDC;				/* device context */
    HGLRC m_hRC;				/* opengl context */
    
    bool m_isQuitWindow;
    
public:
	zglWin32Device()
	{
		m_hDC = NULL;
		m_hRC = NULL;
		m_isQuitWindow = false;
	}

	virtual ~zglWin32Device()
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

