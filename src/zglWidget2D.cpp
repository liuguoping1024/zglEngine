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

#include "zglWidget2D.h"
#include "zglWidgetList.h"

#include "zglEngine.h"
#include <stdio.h>

zglWidget2D::zglWidget2D()
{
	m_type = TYPE_WIDGET2D;

	m_x = 0.0f;
	m_y = 0.0f;
	m_width = 64;
	m_height = 64;

	m_prim_ref = &m_prim;

	m_local_timestamp = 0xffff;
}

void zglWidget2D::setDimension(unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;

	applyDimensionUpdate();
}

void zglWidget2D::updateHitRect()
{
	zglMatrix m = m_prim.world();

	float width2 = m_width * 0.5f;
	float height2 = m_height * 0.5f;

	m_boundary[0].x = - width2;
	m_boundary[0].y = - height2;
	m_boundary[0].z = 0;

	m_boundary[1].x = + width2;
	m_boundary[1].y = + height2;
	m_boundary[1].z = 0;

	//m_boundary[0] = m * m_boundary[0];
	//m_boundary[1] = m * m_boundary[1];

	m_boundary[0] += m_prim.world().T();
	m_boundary[1] += m_prim.world().T();
}

bool zglWidget2D::hitTest(float x, float y)
{
	if( ((m_dirty_flag & FLAG_BOUND_UPDATE) != 0) ||
			(m_local_timestamp != m_prim.world().getTimestamp()))
	{
		updateHitRect();
		m_local_timestamp = m_prim.world().getTimestamp();
		m_dirty_flag &= ~FLAG_BOUND_UPDATE;
	}

	bool bx = false;
	bool by = false;

	if ((x > m_boundary[0].x && x < m_boundary[1].x)
			|| (x < m_boundary[0].x && x > m_boundary[1].x))
	{
		bx = true;
	}

	if ((y > m_boundary[0].y && y < m_boundary[1].y)
			|| (y < m_boundary[0].y && y > m_boundary[1].y))
	{
		by = true;
	}

	if (bx && by)
	{
		return true;
	}

	return false;
}

#ifdef _DEBUG_
void zglWidget2D::getBound(zglVertex * v, int *count)
{
	if(count)
	{
		*count = 4;
	}

	if(v)
	{
		v[0].setPos(m_boundary[0].x, m_boundary[1].y, 0);
		v[1].setPos(m_boundary[0].x, m_boundary[0].y, 0);
		v[2].setPos(m_boundary[1].x, m_boundary[0].y, 0);
		v[3].setPos(m_boundary[1].x, m_boundary[1].y, 0);
	}
}
#endif



