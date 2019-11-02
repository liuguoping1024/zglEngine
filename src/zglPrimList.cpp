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
#include <string.h>
#include "zglPrimList.h"
//#include <pthread.h>

#include "zglPrim2D.h"
#include "zglPrim3D.h"

/*
 static pthread_mutex_t g_primitive_mutex;
 */
zglPrimList::zglPrimList()
{
	for (int i = 0; i < zglPrim::TYPE_NUM; ++i)
	{
		m_prim_list_head[i].m_next = &m_prim_list_tail[i];
		m_prim_list_head[i].m_prev = NULL;

		m_prim_list_tail[i].m_prev = &m_prim_list_head[i];
		m_prim_list_tail[i].m_next = NULL;
	}

	memset(m_sort_list, 0, sizeof(zglPrim*)*m_sort_list_max);
	//pthread_mutex_init(&g_primitive_mutex, NULL);
}

zglPrimList::~zglPrimList()
{
	//pthread_mutex_destroy(&g_primitive_mutex);
}

void zglPrimList::queueSortList(zglPrim* prim)
{
	unsigned char index = (unsigned char) (prim->m_sort_value & 0xff);
	prim->m_next_sort = m_sort_list[index];
	m_sort_list[index] = prim;
}

//!< @deprecated
void zglPrimList::reset()
{
	for (int i = 0; i < zglPrim::TYPE_NUM; ++i)
	{
		zglPrim* node;
		zglPrim* prim = m_prim_list_head[i].m_next;
		zglPrim* end = &m_prim_list_tail[i];

		while (prim != end)
		{
			node = prim;
			prim = prim->m_next;

			node->m_next = NULL;
			node->m_prev = NULL;
		}

		m_prim_list_head[i].m_next = &m_prim_list_tail[i];
		m_prim_list_head[i].m_prev = NULL;

		m_prim_list_tail[i].m_prev = &m_prim_list_head[i];
		m_prim_list_tail[i].m_next = NULL;
	}
}

void zglPrimList::lock()
{
	//pthread_mutex_lock(&g_primitive_mutex);
}

void zglPrimList::unlock()
{
	//pthread_mutex_unlock(&g_primitive_mutex);
}

//!< --------------------------------------------------------------------------

void zglPrimList::renderPrim()
{
	zglPrim* prim = (zglPrim*) m_prim_list_head[zglPrim::TYPE_PRIM3D].m_next;
	zglPrim* end = &m_prim_list_tail[zglPrim::TYPE_PRIM3D];

	for (; prim != end; prim = (zglPrim3D*) prim->m_next)
	{
		if (prim->m_blend_type != BLEND_OFF)
		{
			//!< calculate the z depth, and put into the pending list
			prim->calcSortDepth();//Prim3D
			queueSortList(prim);
			continue;
		}

		prim->render();
	}
}

void zglPrimList::renderPrimAlpha()
{
	zglPrim* prim;
	//!< render alpha (pending list/sort list)
	for (int i = 0xff; i >= 0; i--)
	{
		for (prim = m_sort_list[i]; prim != NULL; prim = prim->m_next_sort)
		{
			prim->render();
		}

		m_sort_list[i] = NULL;
	}
}

void zglPrimList::renderPrim2D()
{
	zglPrim* prim = (zglPrim*) m_prim_list_head[zglPrim::TYPE_PRIM2D].m_next;
	zglPrim* end = &m_prim_list_tail[zglPrim::TYPE_PRIM2D];

	for (; prim != end; prim = (zglPrim2D*) prim->m_next)
	{
		prim->calcSortDepth();//Prim2D
		queueSortList(prim);
	}

	for (int i = 0xff; i >= 0; i--)
	{
		for (prim = m_sort_list[i]; prim != NULL; prim = prim->m_next_sort)
		{
			prim->render();
		}

		m_sort_list[i] = NULL;
	}
}

void zglPrimList::renderSprite()
{
	zglPrim* prim = (zglPrim*) m_prim_list_head[zglPrim::TYPE_SPRITE].m_next;
	zglPrim* end = &m_prim_list_tail[zglPrim::TYPE_SPRITE];

	for (; prim != end; prim = (zglPrim3D*) prim->m_next)
	{
		prim->render();
	}
}

