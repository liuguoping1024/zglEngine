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

#include "zglEngine.h"
#include "zglAnimation.h"
#include "zglWidget.h"
#include "zglWidgetList.h"

zglWidget::zglWidget()
{
	m_next = NULL;
	m_prev = NULL;

	m_parent = NULL;
	m_next_sibling = NULL;
	m_prev_sibling = NULL;

	m_listener = NULL;
	m_animation = NULL;

	m_prim_ref = NULL;

	m_flag = 0;
	m_id = 0; //!< Widget ID;

	m_dirty_flag = 0;

	m_map_flag = FLAG_MAPPING_NONE;

	//!< Position
	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;
}

zglWidget::~zglWidget()
{
	dequeue();
}

/**
 * The function will put the widget into the Widget list of the engine.
 */
void zglWidget::init()
{
	m_flag &= ~FLAG_INVISIBLE;
	queue();

	m_dirty_flag |= FLAG_DIM_UPDATE;

#ifdef ARB_VBO_ENABLE
	updateHardwareSetting();
#endif
}

void zglWidget::lockCapture()
{
	zglWidgetList * list = &(zglEngine::getEngine()->m_widget_list);
	list->lockCapture();
}

void zglWidget::unlockCapture()
{
	zglWidgetList * list = &(zglEngine::getEngine()->m_widget_list);
	list->unlockCapture();
}

unsigned short zglWidget::tryHitTest(float x, float y)
{
	zglWidgetList * list = &(zglEngine::getEngine()->m_widget_list);

	zglWidget * w = list->hitTest(x, y);

	if(w == NULL)
	{
		return 0;
	}

	return w->getID();
}

void zglWidget::getMappingVertex(zglVector* pos, int size, zglMatrix & m)
{
	zglEngine * glEngine = zglEngine::getEngine();
	glEngine->calcWorldTo2D(pos, size, m);
}

void zglWidget::setVisible(bool visible)
{
	if (visible)
	{
		queue();
		m_flag &= ~FLAG_INVISIBLE;
	}
	else
	{
		dequeue();
		m_flag |= FLAG_INVISIBLE;
	}

	if (m_prim_ref != NULL)
	{
		m_prim_ref->setVisible(visible);
	}
}

void zglWidget::setMappingStyle(unsigned char flag)
{
	m_map_flag = flag;
#ifdef ARB_VBO_ENABLE
	updateHardwareSetting();
#endif
}

void zglWidget::setEnable(bool enable)
{
	if (enable)
	{
		//queue();
		m_flag &= ~FLAG_DISABLE;
	}
	else
	{
		//dequeue();
		m_flag |= FLAG_DISABLE;
	}
}

void zglWidget::setListener(zglWidgetListener * listener)
{
	m_listener = listener;
}

bool zglWidget::hitTest(float x, float y)
{
	return false;
}

void zglWidget::setFlag(zglWidgetFlag obj_flag)
{
	m_flag |= obj_flag;
}

void zglWidget::resetFlag(zglWidgetFlag obj_flag)
{
	m_flag &= ~obj_flag;
}

void zglWidget::queue()
{
	if (isQueued())
	{
		return;
	}

	zglWidgetList * list = &(zglEngine::getEngine()->m_widget_list);

	m_prev = list->m_obj_list_tail[m_type].m_prev;
	m_next = &list->m_obj_list_tail[m_type];

	m_prev->m_next = this;
	m_next->m_prev = this;
}

void zglWidget::dequeue()
{
	if (isQueued())
	{
		m_prev->m_next = m_next;
		m_next->m_prev = m_prev;
	}
	m_prev = m_next = NULL;
}

bool zglWidget::isQueued()
{
	if (m_prev && m_next)
	{
		if (m_prev->m_next == this && m_next->m_prev == this)
		{
			return true;
		}
	}

	return false;
}

void zglWidget::updateHardwareBuffer()
{
#ifdef ARB_VBO_ENABLE
	if( m_prim_ref != NULL && m_prim_ref->isHwBufferEnable())
	{
		if(m_prim_ref->isHwBufferAvailable())
		{
			m_prim_ref->updateHwBuffers();
		}
		else if(m_prim_ref->isHwBufferEmpty())
		{
			bool ret = m_prim_ref->createHwBuffers();
			m_prim_ref->postHwBuffer(ret);
		}
	}
#endif
}

void zglWidget::updateHardwareSetting()
{
	if (m_prim_ref != NULL)
	{
		switch (m_map_flag)
		{
		default:
		case FLAG_MAPPING_NONE:
			m_prim_ref->resetFlag(FLAG_VBO_ENABLE);
			break;

		case FLAG_MAPPING_STATIC:
			m_prim_ref->setFlag(FLAG_VBO_ENABLE);
			break;

		case FLAG_MAPPING_DYNAMIC:
			m_prim_ref->setFlag(FLAG_VBO_ENABLE | FLAG_VBO_DYNAMIC);
			break;
		}
	}
}

void zglWidget::forceUpdateDimension()
{
	updateDimension();
#ifdef ARB_VBO_ENABLE
	updateHardwareBuffer();
#endif
	m_dirty_flag = FLAG_BOUND_UPDATE;
}

bool zglWidget::onUpdate(long time)
{
	if ((m_dirty_flag & FLAG_MASK_UPDARE) != 0)
	{
		if ((m_dirty_flag & FLAG_DIM_UPDATE) != 0)
		{
			updateDimension();
#ifdef ARB_VBO_ENABLE
			updateHardwareBuffer();
#endif
			m_dirty_flag = FLAG_BOUND_UPDATE;
		}
		else
		{
#ifdef ARB_VBO_ENABLE
			updateHardwareBuffer();
#endif
			m_dirty_flag = 0;
		}
	}

	if (m_animation != NULL)
	{
		bool ret = false;
		zglAnimation * anim = m_animation;

		if (anim->isRunning())
		{
			ret = anim->update(time);
		}
		else
		{
			m_animation->bindWidget(NULL);
			m_animation = NULL;
		}

		return ret;
	}

	return false;
}

bool zglWidget::onTouchEvent(zglTouchEvent *event)
{
	bool ret = false;

	if (m_id != 0)
	{
		event->m_widget = m_id;
	}

	if (m_listener != NULL)
	{
		ret = m_listener->onTouchEvent(event);
	}

	if (!ret && m_parent != NULL)
	{
		ret = m_parent->onTouchEvent(event);
	}

	return ret;
}

void zglWidget::updateDimension()
{
	//!< Override needed.
}

void zglWidget::getPosition(float *x, float *y, float *z)
{
	if (x)
		*x = m_x;
	if (y)
		*y = m_y;
	if (z)
		*z = m_z;
}

void zglWidget::setAlpha(unsigned char alpha)
{
	if (m_prim_ref != NULL)
	{
		int vertex = m_prim_ref->m_vertex_num;
		zglVertex * v = m_prim_ref->m_vertex;

		for (int i = 0; i < vertex; i++)
		{
			v[i].a = alpha;
		}

		m_dirty_flag |= FLAG_ALPHA_UPDATE;
	}
}

void zglWidget::setColor(unsigned int c)
{
	if (m_prim_ref != NULL)
	{
		unsigned char r = (c & 0xff000000) >> 24;
		unsigned char g = (c & 0x00ff0000) >> 16;
		unsigned char b = (c & 0x0000ff00) >> 8;
		unsigned char a = (c & 0x000000ff);

		int vertex = m_prim_ref->m_vertex_num;
		zglVertex * v = m_prim_ref->m_vertex;

		for (int i = 0; i < vertex; i++)
		{
			v[i].r = r;
			v[i].g = g;
			v[i].b = b;
			v[i].a = a;
		}

		m_dirty_flag |= FLAG_COLOR_UPDATE;
	}
}

void zglWidget::resetWorld()
{
	if (m_prim_ref != NULL)
	{
		m_x = 0.0f;
		m_y = 0.0f;
		m_z = 0.0f;

		m_prim_ref->world() = 1.0f;
	}
}

void zglWidget::locate(float x, float y, float z)
{
	if (m_prim_ref != NULL)
	{
		m_x = x;
		m_y = y;
		m_z = z;

		m_prim_ref->world() = 1.0f;

		m_prim_ref->world().translate(m_x, m_y, m_z);
	}
}

void zglWidget::locatex(float ax, float ay, float az)
{
	if (m_prim_ref != NULL)
	{
		m_prim_ref->world() = 1.0f;

		m_prim_ref->world().translate(m_x, m_y, m_z);

		if(ax != 0.0f)
		{
			m_prim_ref->world().rotateX(ax);
		}

		if(ay != 0.0f)
		{
			m_prim_ref->world().rotateY(ay);
		}

		if(az != 0.0f)
		{
			m_prim_ref->world().rotateZ(az);
		}
	}
}

void zglWidget::rotateX(float angle)
{
	if (m_prim_ref)
	{
		m_prim_ref->world().rotateX(angle);
	}
}

void zglWidget::rotateY(float angle)
{
	if (m_prim_ref)
	{
		m_prim_ref->world().rotateY(angle);
	}
}

void zglWidget::rotateZ(float angle)
{
	if (m_prim_ref)
	{
		m_prim_ref->world().rotateZ(angle);
	}
}

void zglWidget::moveTo(float x, float y, float z)
{
	if (m_prim_ref != NULL)
	{
		m_prim_ref->world().translate(x - m_x, y - m_y, z - m_z);
		m_x = x;
		m_y = y;
		m_z = z;
	}
}

void zglWidget::moveTo(float x, float y)
{
	if (m_prim_ref != NULL)
	{
		m_prim_ref->world().translate(x - m_x, y - m_y, 0);
		m_x = x;
		m_y = y;
	}
}

void zglWidget::move(float dx, float dy, float dz)
{
	if (m_prim_ref != NULL)
	{
		m_x += dx;
		m_y += dy;
		m_z += dz;
		m_prim_ref->world().translate(dx, dy, dz);
	}
}

void zglWidget::moveXTo(float x)
{
	if (m_prim_ref != NULL)
	{
		m_prim_ref->world().translate(x - m_x, 0.0f, 0.0f);
		m_x = x;
	}
}

void zglWidget::moveYTo(float y)
{
	if (m_prim_ref != NULL)
	{
		m_prim_ref->world().translate(0.0f, y - m_y, 0.0f);
		m_y = y;
	}
}

void zglWidget::moveZTo(float z)
{
	if (m_prim_ref != NULL)
	{
		m_prim_ref->world().translate(0.0f, 0.0f, z - m_z);
		m_z = z;
	}
}

void zglWidget::setOffset(int offset)
{
	if (m_prim_ref != NULL)
	{
		return m_prim_ref->setSortOffset(offset);
	}
}

unsigned char zglWidget::getZOffset()
{
	if (m_prim_ref != NULL)
	{
		return m_prim_ref->getSortValue();
	}
	return 0x80;//Half z-depth
}

void zglWidget::setAnimation(zglAnimation * anim)
{
	if (m_animation != NULL)
	{
		m_animation->stop();
		m_animation->bindWidget(NULL);
	}

	m_animation = anim;

	if (anim != NULL)
	{
		anim->reset();
		anim->bindWidget(this);
	}
}

void zglWidget::setTexture(zglTexture * tex)
{
	if (m_prim_ref != NULL)
	{
		return m_prim_ref->setTexture(tex);
	}
}

#ifdef _DEBUG_
void zglWidget::getBound(zglVertex * v, int *count)
{
	if (count)
	{
		*count = 0;
	}
}
#endif


