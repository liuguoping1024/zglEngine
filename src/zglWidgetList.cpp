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

#include <stdlib.h> 		//!< 'NULL'

#include "zglWidget.h"
#include "zglWidget2D.h"	//!< 2D widget
#include "zglWidget3D.h"	//!< 3D widget
#include "zglWidgetList.h"

zglWidgetList::zglWidgetList()
{
	for (int i = 0; i < zglWidget::TYPE_NUM; ++i)
	{
		m_obj_list_head[i].m_next = &m_obj_list_tail[i];
		m_obj_list_head[i].m_prev = NULL;

		m_obj_list_tail[i].m_prev = &m_obj_list_head[i];
		m_obj_list_tail[i].m_next = NULL;
	}

	m_hit_widget = NULL;

	m_capture_lock = false;
}

zglWidgetList::~zglWidgetList()
{
#if 0 //!defined(i386) //!< Android

	zglWidget* remove;
	zglWidget* widget = m_obj_list_head[zglWidget::TYPE_WIDGET2D].m_next;
	zglWidget* end = &m_obj_list_tail[zglWidget::TYPE_WIDGET2D];

	while(widget != end)
	{
		remove = widget;
		widget = widget->m_next;
		remove->m_prev = NULL;
		remove->m_next = NULL;
	}

	widget = m_obj_list_head[zglWidget::TYPE_WIDGET3D].m_next;
	end = &m_obj_list_tail[zglWidget::TYPE_WIDGET3D];

	while(widget != end)
	{
		remove = widget;
		widget = widget->m_next;
		remove->m_prev = NULL;
		remove->m_next = NULL;
	}
#endif
}

bool zglWidgetList::update(long time_val)
{
	bool ret = false;
	zglWidget* widget = m_obj_list_head[zglWidget::TYPE_WIDGET2D].m_next;
	zglWidget* end = &m_obj_list_tail[zglWidget::TYPE_WIDGET2D];

	for (; widget != end; widget = widget->m_next)
	{
		if (widget->onUpdate(time_val) == true)
		{
			ret = true;
		}
	}

	widget = m_obj_list_head[zglWidget::TYPE_WIDGET3D].m_next;
	end = &m_obj_list_tail[zglWidget::TYPE_WIDGET3D];

	for (; widget != end; widget = widget->m_next)
	{
		if (widget->onUpdate(time_val) == true)
		{
			ret = true;
		}
	}

	return ret;
}

bool zglWidgetList::dispatchTouchEvent(int event_type, int x, int y,
		long time)
{
	zglWidget* w = NULL;
	bool bRet = false;
	switch (event_type)
	{
	case TOUCH_DOWN:
		{
			m_event.m_event_type = TOUCH_DOWN;
			m_event.m_time = time;
			m_event.m_widget = 0;
			m_event.m_x = x;
			m_event.m_y = y;

			m_hit_widget = NULL;
			w = hitTest(x, y);
			if (w != NULL /*&& w->m_listener != 0*/)
			{
				m_hit_widget = w;
				bRet = m_hit_widget->onTouchEvent(&m_event);
			}
			else
			{
				m_hit_widget = NULL;
			}
		}
		break;

	case TOUCH_MOVE:
		if (m_hit_widget != NULL)
		{
			if (m_capture_lock || m_hit_widget->hitTest(x, y))
			{
				m_event.m_event_type = TOUCH_MOVE;
				m_event.m_time = time;
				m_event.m_widget = 0;
				m_event.m_x = x;
				m_event.m_y = y;

				bRet = m_hit_widget->onTouchEvent(&m_event);
				return bRet;
			}
			else
			{
				m_event.m_event_type = TOUCH_CANCEL;
				m_event.m_time = time;
				m_event.m_widget = 0;
				m_event.m_x = x;
				m_event.m_y = y;

				m_hit_widget->onTouchEvent(&m_event);
				m_hit_widget = NULL;
				//!< {
#if 0 // DO NOT REMOVE IT!!!!
				w = hitTest(x, y);
				if(w != NULL)
				{
					m_hit_widget = w;
					m_hit_widget->onTouchEvent(TOUCH_DOWN, x, y);
					bRet = m_hit_widget->onTouchEvent(event_type, x, y, time);
				}
#endif
				//!< }
			}
		}

		//!< If current hit widget lost focus, we will not try to find another hit widget.
		//!< Because find another one will make the application more complex.
#if 0
		w = hitTest(x, y);
		if (w != NULL )
		{
			m_hit_widget = w;
			m_hit_widget->onTouchEvent(TOUCH_DOWN, x, y);
			bRet = m_hit_widget->onTouchEvent(event_type, x, y);
		}
		else
		{
			m_hit_widget = NULL;
		}
#endif

		break;

	case TOUCH_UP:
		if (m_hit_widget != NULL)
		{
			m_event.m_event_type = TOUCH_UP;
			m_event.m_time = time;
			m_event.m_widget = 0;
			m_event.m_x = x;
			m_event.m_y = y;

			bRet = m_hit_widget->onTouchEvent(&m_event);
			m_hit_widget = NULL;
		}
		m_capture_lock = false;
		break;

	default:
		if (m_hit_widget != NULL)
		{
			m_event.m_event_type = event_type;
			m_event.m_time = time;
			m_event.m_widget = 0;
			m_event.m_x = x;
			m_event.m_y = y;

			bRet = m_hit_widget->onTouchEvent(&m_event);
			m_hit_widget = NULL;
		}
		break;

	}

	return bRet;
}

void zglWidgetList::lockCapture()
{
	if (m_hit_widget)
	{
		m_capture_lock = true;
	}
}

void zglWidgetList::unlockCapture()
{
	m_capture_lock = false;
}

bool zglWidgetList::dispatchKeyEvent(int event_type, int key_value)
{
	return false;
}

zglWidget * zglWidgetList::hitTest(float x, float y)
{
	int hit_count = 0;
	unsigned short z = 256; //!< Most far away z-depth

	zglWidget* widget_hit = NULL;

	zglWidget* widget = m_obj_list_head[zglWidget::TYPE_WIDGET2D].m_next;
	zglWidget* end = &m_obj_list_tail[zglWidget::TYPE_WIDGET2D];

	/*
	for (; widget != end; widget = widget->m_next)
	{
#if 0
		if (widget->m_flag & (zglWidget::FLAG_DISABLE
				| zglWidget::FLAG_INVISIBLE))
		{
			continue;
		}
#else
		if (widget->m_flag & zglWidget::FLAG_DISABLE)
		{
			continue;
		}
#endif

		if (widget->hitTest(x, y))
		{
			//TODO Offset;
			return widget;
		}
	}

*/
	hit_count = 0;
	z = 256; //!< Most far away z-depth

	//Find all the hit 3d gui object
	for (; widget != end; widget =  widget->m_next)
	{
#if 0
		if (widget3d->m_flag & (zglWidget::FLAG_DISABLE
				| zglWidget::FLAG_INVISIBLE))
		{
			continue;
		}
#else
		if (widget->m_flag & zglWidget::FLAG_DISABLE)
		{
			continue;
		}
#endif

		if (widget->hitTest(x, y))
		{
			m_hit_widgets[hit_count] = widget;
			++hit_count;
		}
	}

	zglWidget2D* widget2d = NULL;
	//Find the top one.
	if (hit_count > 0)
	{
		for (int i = 0; i < hit_count; i++)
		{
			widget2d = (zglWidget2D*) m_hit_widgets[i];
			unsigned char z_value = widget2d->getZOffset();
			if (z_value < z)
			{
				widget_hit = widget2d;
				z = z_value;
			}
		}
		return widget_hit;
	}


	hit_count = 0;
	z = 256; //!< Most far away z-depth

	widget = m_obj_list_head[zglWidget::TYPE_WIDGET3D].m_next;
	end = &m_obj_list_tail[zglWidget::TYPE_WIDGET3D];

	//Find all the hit 3d gui object
	for (; widget != end; widget =  widget->m_next)
	{
#if 0
		if (widget3d->m_flag & (zglWidget::FLAG_DISABLE
				| zglWidget::FLAG_INVISIBLE))
		{
			continue;
		}
#else
		if (widget->m_flag & zglWidget::FLAG_DISABLE)
		{
			continue;
		}
#endif

		if (widget->hitTest(x, y))
		{
			m_hit_widgets[hit_count] = widget;
			++hit_count;
		}
	}

	zglWidget3D* widget3d = NULL;
	//Find the top one.
	if (hit_count > 0)
	{
		for (int i = 0; i < hit_count; i++)
		{
			widget3d = (zglWidget3D*) m_hit_widgets[i];
			unsigned char z_value = widget3d->getZOffset();
			if (z_value < z)
			{
				widget_hit = widget3d;
				z = z_value;
			}
		}
		return widget_hit;
	}

	return (zglWidget *) NULL;
}

void zglWidgetList::reset()
{
	for (int i = 0; i < zglWidget::TYPE_NUM; ++i)
	{
		zglWidget* node;
		zglWidget* widget = m_obj_list_head[i].m_next;
		zglWidget* end = &m_obj_list_tail[i];

		while (widget != end)
		{
			node = widget;
			widget = widget->m_next;

			node->m_next = NULL;
			node->m_prev = NULL;
		}

		m_obj_list_head[i].m_next = &m_obj_list_tail[i];
		m_obj_list_head[i].m_prev = NULL;

		m_obj_list_tail[i].m_prev = &m_obj_list_head[i];
		m_obj_list_tail[i].m_next = NULL;
	}

	m_hit_widget = NULL;

	m_capture_lock = false;
}



