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

#ifndef __zgl_plugin_h__
#define __zgl_plugin_h__

#include "zglEventType.h"

/**
 * @brief A listener to the engine.
 *
 * The listener is suitable for handle the graphics element.
 * The event to GUI elements will be handled by the Engine by default.
 *
 * As to an event, if the engine listener handled it,
 * the processing to the event will be finished, otherwise, the event will be passed
 * to next module.
 */
class zglPlugin
{
public:

	/**
	 * @brief interface of plug-in initial, called by #zglEngine.
	 */
	virtual void onInit(void) = 0;

	/**
	 * @brief interface of plug-in finish, called by #zglEngine.
	 */
	virtual void onFini(void)= 0;

	/**
	 * @brief interface of plug-in resize, called by #zglEngine.
	 */
	virtual void onResize(int w, int h)= 0;

	/**
	 * @brief interface of plug-in update, called by #zglEngine.
	 *
	 * @return True if the update event is handled. false if not handled.
	 */
	virtual bool onUpdate(long time_val);

	/**
	 * @brief interface of plug-in touch handler, called by #zglEngine.
	 *
	 * @param type Touch event type, @see zglTouchEventType.
	 * @param x Touch position.
	 * @param y Touch position.
	 * @param time Touch time-line.
	 *
	 * @return True if the touch event is handled, false if not handled.
	 */
	virtual bool handleTouchEvent(int type, int x, int y, long time);

#if 0
	virtual bool handleKeyEvent(int type, int key_value, long time) = 0;

	virtual bool HandleSensorEvent(int type, float v1, float v2, float v3, long time) = 0;
#endif

	virtual ~zglPlugin()
	{
	}
};

#endif
