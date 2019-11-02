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

#include "zglWidget3D.h"
#include "zglEngine.h"

zglWidget3D::zglWidget3D()
{
	m_type = TYPE_WIDGET3D;

	m_x = 0.0f;
	m_y = 0.0f;
	m_z = 0.0f;

	m_prim_ref = &m_prim;

	m_local_timestamp = 0xffff;
	m_global_timestamp = 0xffff;
}

zglVector zglWidget3D::getScreenPosition()
{
	zglVector v(m_x, m_y, m_z);
	getMappingVertex(&v, 1, m_prim_ref->world());
	return v;
}

void zglWidget3D::updateHitRect()
{
}

bool zglWidget3D::hitTest(float x, float y)
{
	zglEngine * glEngine = zglEngine::getEngine();

	if (m_prim_ref == NULL
		) return false;

	if (((m_dirty_flag & FLAG_BOUND_UPDATE) != 0)
			|| (m_local_timestamp != m_prim_ref->world().getTimestamp())
			|| (m_global_timestamp != glEngine->world().getTimestamp()))
	{
		updateHitRect();
		m_local_timestamp = m_prim_ref->world().getTimestamp();
		m_global_timestamp = glEngine->world().getTimestamp();
		m_dirty_flag &= ~FLAG_BOUND_UPDATE;
	}

	int x0, y0, x1, y1;
	int xi = (int) x;
	int yi = (int) y;

	for (int i = 0; i < 4; i++)
	{
		int j = i + 1;
		if (j >= 4)
			j = 0;

		x0 = (int) m_boundary[i].x;
		y0 = (int) m_boundary[i].y;

		x1 = (int) m_boundary[j].x;
		y1 = (int) m_boundary[j].y;

		if ((yi - y0) * (x1 - x0) - (xi - x0) * (y1 - y0) < 0)
		{
			return false;
		}
	}

	return true;
}

#ifdef _DEBUG_
void zglWidget3D::getBound(zglVertex * v, int *count)
{
	if(count)
	{
		*count = 4;
	}

	if(v)
	{
		for(int i=0; i<4; i++)
		{
			v[i].setPos(m_boundary[i].x, m_boundary[i].y, 0);
		}
	}
}
#endif


