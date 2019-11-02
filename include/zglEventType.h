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

#ifndef __zgl_event_h__
#define __zgl_event_h__

#if 0
enum zglEventType
{
	EVENT_UPDATE = 0x0001,
	EVENT_TOUCH  = 0x0002,
	EVENT_KEY    = 0x0004,

	EVENT_MISC1  = 0x0100,
	EVENT_MISC2  = 0x0200,
};
#endif

/**
 * @brief The type of touch event.
 */
enum zglTouchEventType
{
	TOUCH_DOWN = 0,
	TOUCH_MOVE,
	TOUCH_UP,
	TOUCH_CANCEL,
};


/**
 * @brief The touch event information of the engine.
 */
class zglTouchEvent
{
public:

	/**
	 * @brief the type of touch event, down, move, etc.
	 */
	int m_event_type;

	/**
	 * @brief the widget id.
	 *
	 * If the widget id is zero, means the event come from the engine.
	 */
	int m_widget;

	/**
	 * @brief the x coordinate of the touch point.
	 */
	int m_x;

	/**
	 * @brief the y coordinate of the touch point.
	 */
	int m_y;

	/**
	 * @brief the time line of the touch event.
	 *
	 * Animation will make use of the time.
	 */
	long m_time;
};

#if 0
enum zglKeyEvent
{
	KEY_DOWN = 0,
	KEY_UP,
	KEY_LONG,
};

enum zglSensorEvent
{
	SENSOR_XXX = 0,
};
#endif

#endif
