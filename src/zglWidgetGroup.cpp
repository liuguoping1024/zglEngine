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

#include <stdlib.h>	//!< NULL
#include <string.h>	//!< memcpy
#include "zglWidgetGroup.h"

zglWidgetGroup::zglWidgetGroup()
{
	m_type = TYPE_WIDGETGROUP;

	m_children.m_next_sibling = &m_children;
	m_children.m_prev_sibling = &m_children;
	m_count = 0;

	m_current = NULL;

	m_width = 256;
	m_height = 256;

	m_gesture = GESTURE_NONE;
	m_motion_max = 8;

	m_motion_hold_dur = 500;
}

// -------- 8>< ---------------------------------------------------------------

void zglWidgetGroup::init(zglTexture * texture, unsigned int width,
		unsigned int height)
{
	m_background.init(texture, width, height);
	m_background.setOffset(0xff);
	m_background.setParent(this);

	m_width = width;
	m_height = height;
}

void zglWidgetGroup::setDimension(unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;

	m_background.setDimension(width, height);
}

void zglWidgetGroup::setTexture(zglTexture * texture)
{
	m_background.setTexture(texture);
}

void zglWidgetGroup::setTexCoord(float left, float top, float right, float bottom)
{
	m_background.setTexCoord(left, top, right, bottom);
}

void zglWidgetGroup::setColor(unsigned int color)
{
	m_background.setColor(color);
}

void zglWidgetGroup::setOffset(int offset)
{
	m_background.setOffset(offset);
}

void zglWidgetGroup::setVisible(bool visible)
{
	m_background.setVisible(false);
}

void zglWidgetGroup::locate(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;

	m_background.locate(x, y, z);
}

// -------- 8>< ---------------------------------------------------------------

void zglWidgetGroup::insert(zglWidget* node)
{
	node->m_parent = this;

	node->m_next_sibling = m_children.m_next_sibling;
	node->m_prev_sibling = &m_children;

	node->m_next_sibling->m_prev_sibling = node;
	node->m_prev_sibling->m_next_sibling = node;

	++m_count;
}

void zglWidgetGroup::append(zglWidget* node)
{
	node->m_parent = this;

	node->m_next_sibling = &m_children;
	node->m_prev_sibling = m_children.m_prev_sibling;

	node->m_next_sibling->m_prev_sibling = node;
	node->m_prev_sibling->m_next_sibling = node;

	++m_count;
}

void zglWidgetGroup::add(int pos, zglWidget* w)
{
	if (pos < 0 || pos >= m_count)
	{
		append(w);
		return;
	}

	zglWidget * node = findWidgetByPosition(pos);

	w->m_parent = this;

	w->m_next_sibling = node;
	w->m_prev_sibling = w->m_prev_sibling;

	w->m_next_sibling->m_prev_sibling = w;
	w->m_prev_sibling->m_next_sibling = w;

	++m_count;
}

zglWidget* zglWidgetGroup::drop(int pos)
{
	if (pos >= m_count)
	{
		return NULL;
	}
	else if(pos < 0)
	{
		pos = m_count - 1;
	}

	zglWidget * node = findWidgetByPosition(pos);

	node->m_parent = NULL;

	node->m_prev_sibling->m_next_sibling = node->m_next_sibling;
	node->m_next_sibling->m_prev_sibling = node->m_prev_sibling;

	node->m_next_sibling = NULL;
	node->m_prev_sibling = NULL;

	--m_count;

	return node;
}

void zglWidgetGroup::dropAll()
{
	zglWidget * node = m_children.m_next_sibling;

	while(node != &m_children)
	{
		node->m_parent = NULL;
		node = node->m_next_sibling;
	}

	m_children.m_next_sibling = &m_children;
	m_children.m_prev_sibling = &m_children;
	m_count = 0;
}

zglWidget * zglWidgetGroup::findWidgetByPosition(int pos)
{
	zglWidget * node = NULL;

#if 0
	//Private function, so the security can be guarantee.
	if(pos < 0 || pos >= m_count)
	{
		return NULL;
	}
#endif

	if(pos == 0) //!< header items access more, so ...
	{
		node = m_children.m_next_sibling;
	}
	else if(pos == m_count - 1) //!< tail items access more, so ...
	{
		node = m_children.m_prev_sibling;
	}
	else
	{
		unsigned short i;

		if(pos < (m_count >> 1)) //!< clock CW search
		{
			i = 0;
			node = m_children.m_next_sibling;

			while(i < pos)
			{
				node = node->m_next_sibling;
				++ i;

				if(i == pos)
				{
					break;
				}
			}
		}
		else //!< clock CCW search
		{
			i = m_count - 1;
			node = m_children.m_prev_sibling;

			while(i > pos)
			{
				node = node->m_prev_sibling;
				-- i;
			}
		}
	}

	return node;
}

zglWidget * zglWidgetGroup::getChildren(unsigned int index)
{
	if (index >= m_count)
	{
		return NULL;
	}

	zglWidget * node = m_children.m_next_sibling;

	unsigned int i = 0;
	while(node != &m_children)
	{
		if(i == index)
		{
			return node;
		}

		node = node->m_next_sibling;
		++ i;
	}

	return NULL;
}

int zglWidgetGroup::findChild(zglWidget* w)
{
	unsigned int i = 0;
	zglWidget * node = m_children.m_next_sibling;
	while(node != &m_children)
	{
		if(node == w)
		{
			return i;
		}

		node = node->m_next_sibling;
		++ i;
	}

	return -1;
}


zglWidgetGroup::zglBlanket::zglBlanket()
{
	m_my_texture = NULL;

	m_my_color = 0xffffffff;

	m_vertex[0].setUV(0.0f, 0.0f);
	m_vertex[1].setUV(0.0f, 1.0f);
	m_vertex[2].setUV(1.0f, 0.0f);
	m_vertex[3].setUV(1.0f, 1.0f);
}

void zglWidgetGroup::zglBlanket::init(zglTexture * texture, unsigned int width,
		unsigned int height)
{
	zglWidget3D::init();
	m_my_texture = texture;

	m_width = width;
	m_height = height;

	m_prim.init(DRAW_TRI_STRIP, m_vertex, 4, m_my_texture);
	m_prim.setBlendType(BLEND_HALF);

	applyVertexUpdate();
	updateVisible();
}

void zglWidgetGroup::zglBlanket::setTexture(zglTexture * texture)
{
	m_my_texture = texture;
	updateVisible();
}

void zglWidgetGroup::zglBlanket::setDimension(unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;

	applyVertexUpdate();
}

void zglWidgetGroup::zglBlanket::setTexCoord(float left, float top, float right, float bottom)
{
	m_vertex[0].setUV(left, top);
	m_vertex[1].setUV(left, bottom);
	m_vertex[2].setUV(right, top);
	m_vertex[3].setUV(right, bottom);

	applyVertexUpdate();
}

void zglWidgetGroup::zglBlanket::setColor(unsigned int color)
{
	m_my_color = color;
	unsigned char r = (color & 0xff000000) >> 24;
	unsigned char g = (color & 0x00ff0000) >> 16;
	unsigned char b = (color & 0x0000ff00) >> 8;
	unsigned char a = (color & 0x000000ff);

	m_vertex[0].r = m_vertex[1].r = m_vertex[2].r = m_vertex[3].r = r;
	m_vertex[0].g = m_vertex[1].g = m_vertex[2].g = m_vertex[3].g = g;
	m_vertex[0].b = m_vertex[1].b = m_vertex[2].b = m_vertex[3].b = b;
	m_vertex[0].a = m_vertex[1].a = m_vertex[2].a = m_vertex[3].a = a;

	applyVertexUpdate();
	updateVisible();
}

void zglWidgetGroup::zglBlanket::updateVisible()
{
	if(m_my_texture != NULL || m_my_color != 0xffffffff)
	{
		m_prim.setVisible(true);
	}else
	{
		m_prim.setVisible(false);
	}
}

void zglWidgetGroup::zglBlanket::updateDimension()
{
	float width2 = m_width * 0.5f;
	float height2 = m_height * 0.5f;

	m_vertex[0].setPos(-width2, height2, 0);
	m_vertex[1].setPos(-width2, -height2, 0);
	m_vertex[2].setPos(width2, height2, 0);
	m_vertex[3].setPos(width2, -height2, 0);
}

bool zglWidgetGroup::zglBlanket::onUpdate(long time)
{
	bool ret = false;

	/**
	 * Hook !!
	 * Pass the update event to zglWidgetGroup.
	 */
	if (m_parent != NULL)
	{
		ret = m_parent->onUpdate(time);
	}

	return zglWidget::onUpdate(time) || ret;
}

bool zglWidgetGroup::onTouchEvent(zglTouchEvent *event)
{
	switch (event->m_event_type)
	{
	case TOUCH_DOWN:
		{
			m_gesture = GESTURE_DOWN;

			m_motion_x = event->m_x;
			m_motion_y = event->m_y;

			m_last_x = event->m_x;
			m_last_y = event->m_y;

			zglWidget::lockCapture();

			m_tracker.clear();
			m_tracker.addMoveMent(event->m_x, event->m_y, event->m_time);
		}

		return true;

	case TOUCH_UP:
		{
			bool ret = false;
			switch (m_gesture)
			{
			default:
			case GESTURE_NONE:
				break;

			case GESTURE_DOWN:
				ret = onGestureClick(event);
				break;

			case GESTURE_MOVE_X:
				ret = onGestureFlingX(event);
				break;

			case GESTURE_MOVE_Y:
				ret = onGestureFlingY(event);
				break;
			}

			m_gesture = GESTURE_NONE;
			return ret;
		}

	case TOUCH_MOVE:
		{
			m_tracker.addMoveMent(event->m_x, event->m_y, event->m_time);
			bool ret = false;

			switch (m_gesture)
			{
			case GESTURE_DOWN:
				{
					int dx = event->m_x - m_motion_x;
					dx = dx > 0 ? dx : -dx;

					int dy = event->m_y - m_motion_y;
					dy = dy > 0 ? dy : -dy;

					if ((dx >= m_motion_max) || (dy >= m_motion_max))
					{
						if (dx > dy)
						{
							m_gesture = GESTURE_MOVE_X;
						}
						else
						{
							m_gesture = GESTURE_MOVE_Y;
						}
					}
				}

				break;

			case GESTURE_MOVE_Y:
				{
					ret = onGestureScrollY(event);
				}
				break;

			case GESTURE_MOVE_X:
				{
					ret = onGestureScrollX(event);
				}
				break;

			default:
				break;
			}

			if (!ret)
			{
				ret = onGestureMove(event);
			}
			m_last_x = event->m_x;
			m_last_y = event->m_y;

			return ret;
		}
	}
	return false;
}

bool zglWidgetGroup::onGestureTouchDown(zglTouchEvent *event)
{
	return false;
}

bool zglWidgetGroup::onGestureClick(zglTouchEvent *event)
{
	return false;
}

bool zglWidgetGroup::onGestureFlingX(zglTouchEvent *event)
{
	return false;
}

bool zglWidgetGroup::onGestureFlingY(zglTouchEvent *event)
{
	return false;
}

bool zglWidgetGroup::onGestureScrollX(zglTouchEvent *event)
{
	return false;
}

bool zglWidgetGroup::onGestureScrollY(zglTouchEvent *event)
{
	return false;
}

bool zglWidgetGroup::onGestureMove(zglTouchEvent *event)
{
	return false;
}

bool zglWidgetGroup::onGestureLongPress()
{
	return false;
}

bool zglWidgetGroup::zglBlanket::hitTest(float x, float y)
{
	zglVector v[4];

	float width2 = m_width * 0.5f;
	float height2 = m_height * 0.5f;

	v[0].x = - width2;
	v[0].y = + height2;
	v[0].z = 0;

	v[1].x = - width2;
	v[1].y = - height2;
	v[1].z = 0;

	v[2].x = + width2;
	v[2].y = - height2;
	v[2].z = 0;

	v[3].x = + width2;
	v[3].y = + height2;
	v[3].z = 0;

	getMappingVertex(v, 4, m_prim.world());

	int x0, y0, x1, y1;
	int xi = (int)x;
	int yi = (int)y;

	for(int i=0; i<4; i++)
	{
		int j = i + 1;
		if(j >=4) j = 0;

		x0 = (int)v[i].x;
		y0 = (int)v[i].y;

		x1 = (int)v[j].x;
		y1 = (int)v[j].y;

		if( (yi - y0) * (x1 - x0) - (xi - x0) * (y1 - y0) < 0)
		{
			return false;
		}
	}

	return true;
}





