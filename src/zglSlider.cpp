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
#include "zglSlider.h"
#include "zglEventType.h"

zglSlider::zglSlider()
{
	m_mode = SLIDER_BAR;

	m_vertical = false;

	m_valMax = 100.0f;
	m_valMin = 0.0f;

	m_value = 10.0f;

	m_map_flag = FLAG_MAPPING_DYNAMIC;
}

void zglSlider::init(zglTexture * tex, unsigned int width, unsigned int height)
{
	zglWidget2D::init();

	m_prim.init(DRAW_TRI_STRIP, m_vertex, 4, (zglTexture *) tex);
	m_prim.setBlendType(BLEND_HALF);

	//!< Update the dimension, and request updating the vertex.
	m_width = width;
	m_height = height;
}

void zglSlider::setMode(float min, float max, int mode)
{
	m_mode = mode;

	m_valMax = max;
	m_valMin = min;

	applyDimensionUpdate(); //Mode will make the same effect as dimension.
}

void zglSlider::setDirection(bool bVertical)
{
	if (m_vertical != bVertical)
	{
		applyDimensionUpdate(); //Direction will make the same effect as dimension.
	}

	m_vertical = bVertical;
}

void zglSlider::setValue(float value)
{
	if (value > m_valMax)
		m_value = m_valMax;
	else if (value < m_valMin)
		m_value = m_valMin;
	else
		m_value = value;

	applyDimensionUpdate(); //Value will make the same effect as dimension.
}

void zglSlider::setTexCoord(float left, float top, float right, float bottom)
{
	m_vertex[0].setUV(left, top);
	m_vertex[1].setUV(left, bottom);
	m_vertex[2].setUV(right, top);
	m_vertex[3].setUV(right, bottom);

	applyVertexUpdate();
}

bool zglSlider::onTouchEvent(zglTouchEvent *event)
{
	m_pressed = true;

	bool ret = false;
	switch (event->m_event_type)
	{
	case TOUCH_DOWN: //down
		m_pressed = true;
		lockCapture();
		ret = true;
		break;

	case TOUCH_CANCEL:
		m_pressed = false;
		ret = true;
		break;

	case TOUCH_UP:
		m_pressed = false;
		unlockCapture();
		ret = true;

		if (m_vertical)
		{

		}
		else
		{
			float dx = event->m_x - m_x;
			if (dx > m_width)
				dx = m_width;
			else if (dx < 0)
				dx = 0;

			updateHorizontalBar(dx);
		}
		break;

	case TOUCH_MOVE:
		if (m_pressed)
		{
			if (m_vertical)
			{

			}
			else
			{
				float dx = event->m_x - m_x;
				if (dx > m_width)
					dx = m_width;
				else if (dx < 0)
					dx = 0;

				updateHorizontalBar(dx);
			}
			ret = true;
		}

		break;
	}

	return zglWidget::onTouchEvent(event) || ret;
}

void zglSlider::updateHitRect()
{
	m_boundary[0].x = 0.0f;
	m_boundary[0].y = 0.0f;
	m_boundary[0].z = 0.0f;

	m_boundary[1].x = (float)m_width;
	m_boundary[1].y = -(float)m_height;
	m_boundary[1].z = 0.0f;

	m_boundary[0] += m_prim.world().T();
	m_boundary[1] += m_prim.world().T();
}

void zglSlider::updateDimension()
{
	float width2 = m_width * 0.5f;
	float height2 = m_height * 0.5f;

	float xx, yy;

	if (m_vertical)
	{
		yy = height2 + m_height * (m_value - m_valMin) / (m_valMax - m_valMin);

		switch (m_mode)
		{
		default:
		case SLIDER_BAR:
			m_vertex[0].setPos(-width2, height2, 0.0f);
			m_vertex[1].setPos(-width2, yy, 0.0f);
			m_vertex[2].setPos(width2, height2, 0.0f);
			m_vertex[3].setPos(width2, yy, 0.0f);
			break;

		case SLIDER_BARRELATIVE:
			m_vertex[0].setPos(-width2, 0.0f, 0.0f);
			m_vertex[1].setPos(-width2, yy, 0.0f);
			m_vertex[2].setPos(width2, 0.0f, 0.0f);
			m_vertex[3].setPos(width2, yy, 0.0f);
			break;
		}
	}
	else
	{
		xx = m_width * (m_value - m_valMin) / (m_valMax - m_valMin);

		switch (m_mode)
		{
		default:
		case SLIDER_BAR:
			m_vertex[0].setPos(0.0f, 0.0f, 0.0f);
			m_vertex[1].setPos(0.0f, -(float)m_height, 0.0f);
			m_vertex[2].setPos(xx, 0.0f, 0.0f);
			m_vertex[3].setPos(xx, -(float)m_height, 0.0f);
			break;

		case SLIDER_BARRELATIVE:
			m_vertex[0].setPos(width2, 0.0f, 0.0f);
			m_vertex[1].setPos(width2, -(float)m_height, 0.0f);
			m_vertex[2].setPos(xx, 0.0f, 0.0f);
			m_vertex[3].setPos(xx, -(float)m_height, 0.0f);
			break;
		}
	}
}

void zglSlider::updateVerticalBar(float dy)
{

}

void zglSlider::updateHorizontalBar(float dx)
{
	m_value = m_valMin + (m_valMax - m_valMin) * dx / (float)m_width;

	switch (m_mode)
	{
	default:
	case SLIDER_BAR:
		m_vertex[0].setPos(0.0f, 0.0f, 0.0f);
		m_vertex[1].setPos(0.0f, -(float)m_height, 0.0f);
		m_vertex[2].setPos(dx, 0.0f, 0.0f);
		m_vertex[3].setPos(dx, -(float)m_height, 0.0f);
		break;

	case SLIDER_BARRELATIVE:
		m_vertex[0].setPos(m_width * 0.5f, 0.0f, 0.0f);
		m_vertex[1].setPos(m_width * 0.5f, -(float)m_height, 0.0f);
		m_vertex[2].setPos(dx, 0.0f, 0.0f);
		m_vertex[3].setPos(dx, -(float)m_height, 0.0f);
		break;
	}

	applyVertexUpdate();
}





