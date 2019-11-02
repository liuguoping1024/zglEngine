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

#include "zglButton.h"
#include "zglEventType.h"
#include "zglBasicType.h"

zglButton::zglButton()
{
	m_vertex[0].setUV(0.0f, 0.0f);
	m_vertex[1].setUV(0.0f, 1.0f);
	m_vertex[2].setUV(1.0f, 0.0f);
	m_vertex[3].setUV(1.0f, 1.0f);

	m_tx = 0.0f;
	m_ty = 0.0f;

	m_vertex[0].a = 0xff;
	m_vertex[1].a = 0xff;
	m_vertex[2].a = 0xff;
	m_vertex[3].a = 0xff;

	m_map_flag = FLAG_MAPPING_DYNAMIC;
}

void zglButton::init(zglTexture * tex, unsigned int width, unsigned int height)
{
	zglWidget2D::init();

	m_prim.init(DRAW_TRI_STRIP, m_vertex, 4, (zglTexture *) tex);
	m_prim.setBlendType(BLEND_HALF);

	m_width = width;
	m_height = height;
}

void zglButton::setTexCoord(float left, float top, float right, float bottom,
		float tx, float ty)
{
	m_vertex[0].setUV(left, top);
	m_vertex[1].setUV(left, bottom);
	m_vertex[2].setUV(right, top);
	m_vertex[3].setUV(right, bottom);

	m_tx = tx;
	m_ty = ty;

	applyVertexUpdate();
}

void zglButton::updateDimension()
{
	float width2 = m_width * 0.5f;
	float height2 = m_height * 0.5f;

	m_vertex[0].setPos(-width2, height2, 0.0f);
	m_vertex[1].setPos(-width2, -height2, 0.0f);
	m_vertex[2].setPos(width2, height2, 0.0f);
	m_vertex[3].setPos(width2, -height2, 0.0f);
}

bool zglButton::onTouchEvent(zglTouchEvent *event)
{
	bool ret = false;
	switch (event->m_event_type)
	{
	case TOUCH_DOWN: //down
		{

			m_vertex[0].u += m_tx;
			m_vertex[0].v += m_ty;
			m_vertex[1].u += m_tx;
			m_vertex[1].v += m_ty;
			m_vertex[2].u += m_tx;
			m_vertex[2].v += m_ty;
			m_vertex[3].u += m_tx;
			m_vertex[3].v += m_ty;

			applyVertexUpdate();
		}
		break;

	case TOUCH_UP:
	case TOUCH_CANCEL:
		{
			m_vertex[0].u -= m_tx;
			m_vertex[0].v -= m_ty;
			m_vertex[1].u -= m_tx;
			m_vertex[1].v -= m_ty;
			m_vertex[2].u -= m_tx;
			m_vertex[2].v -= m_ty;
			m_vertex[3].u -= m_tx;
			m_vertex[3].v -= m_ty;

			applyVertexUpdate();
		}
		break;

	case TOUCH_MOVE:
		break;
	}

	return zglWidget::onTouchEvent(event) || ret;
}


