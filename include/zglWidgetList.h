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


#ifndef __zgl_widget_list_h__
#define __zgl_widget_list_h__

#include "zglEventType.h"

/**
 * @brief Widget is the container of all the widget,(Particle widget is excluded).
 *
 * The widget list will check the hit widget, pass event to widget,
 * and update the widget when needed.
 */
class zglWidgetList
{
private:
	zglWidgetList();

	~zglWidgetList();

	/**
	 * @brief Enable the lock capture flag,
	 *
	 * The action will make the current selected widget as the default target
	 * until the #unlockCapture is invoked, even a new widget is hit.
	 *
	 * @see #unlockCapture
	 */
	void lockCapture();

	/**
	 * @brief Disable the lock capture flag
	 * @see #lockCapture
	 */
	void unlockCapture();

	/**
	 * @brief reset the widget list to original state.
	 */
	void reset();


	/**
	 * @brief Update all the widgets at a new time line.
	 *
	 * @return
	 * 		if the widget changed during the procedure, return true,
	 * 		else, return false.
	 *
	 */
	bool update(long time_val);

	/**
	 * @brief Dispatch the touch event to target widget.
	 * @return
	 * 		if the widget process the event, return true. if the widget
	 * 		skip the event, just return false.
	 */
	bool dispatchTouchEvent(int event_type, int x, int y, long time);

	/**
	 * @brief Dispatch the touch event to target widget.
	 * @return
	 * 		if the widget process the event, return true. if the widget
	 * 		skip the event, just return false.
	 */
	bool dispatchKeyEvent(int event_type, int key_value);

	/**
	 * @brief Hit test to the widget,
	 *
	 * @return
	 * 		if widgets hit, return the top-most widget, else, return NULL.
	 */
	zglWidget * hitTest(float x, float y);

private:
	static const int m_max_hit = 256;
	zglWidget m_obj_list_head[zglWidget::TYPE_NUM];
	zglWidget m_obj_list_tail[zglWidget::TYPE_NUM];

	//!< Mouse capture
	bool m_capture_lock;

	zglWidget * m_hit_widget; //for touch event;
	//!< TODO BUG If too many widget hit at the same time ...
	zglWidget * m_hit_widgets[m_max_hit]; //for touch event;

	zglTouchEvent m_event;

	friend class zglWidget;
	friend class zglEngine;

#ifdef _DEBUG_
	friend class zglEngineDebugger;
#endif
};

#endif

