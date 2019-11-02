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

#ifndef __zgl_device_h__
#define __zgl_device_h__

/**
 * @brief An abstract layer for the different platforms,
 *
 * such as win32/linux/Android.
 */
class zglDevice
{
public:
	zglDevice()
	{
		m_width = 0;
		m_height = 0;
	}

	virtual ~zglDevice()
	{
	}

	/**
	 * Obtain global device.
	 *
	 * If multiple instance needed, try not to use the function.
	 */
	static zglDevice * getDevice();

	/**
	 * destroy global device.
	 *
	 * If multiple instance needed, try not to use the function.
	 */
	static void destroyDevice();

	/**
	 * 1. Set up the client window;
	 * 2. Set up the GL environment.
	 * 3. Show window when needed.
	 */
	virtual bool init(int width, int height, const char * title) = 0;

	/**
	 * 1. Destroy the GL
	 * 2. Destroy the Window
	 */
	virtual bool fini() = 0;

	/**
	 * Main loop
	 */
	virtual bool run() = 0;

#if 0
	/**
	 * Tick time.
	 */
	virtual long getTickTime() = 0;
#endif

	/**
	 * swap buffer
	 */
	virtual void flipScreen() = 0;

protected:
	int m_width;//!< width of frame buffer (or window)
	int m_height;//!< height of frame buffer (or window)
};

#endif

