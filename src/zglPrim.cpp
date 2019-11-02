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
#include <stdio.h>	//!< NULL
#include "zglEngine.h"
#include "zglCore.h"
#include "zglPrim.h"
#include "zglPrimList.h"

zglPrim::zglPrim()
{
	m_prev = NULL;
	m_next = NULL;

	m_texture = NULL;
	m_material = NULL;
	m_blend_type = BLEND_OFF;

	m_sort_value = 0;
	m_sort_offset = 0;

	m_world = 1.0f;

	m_vertex = NULL;
	m_indices = NULL;
	m_vertex_num = 0;
	m_indices_num = 0;

	m_flag = 0x00;

	m_hwbuffer_type = HWBUFFER_NONE;

	m_vbo_vertex = 0;
	m_vbo_index = 0;
}

zglPrim::~zglPrim()
{
#ifdef ARB_VBO_ENABLE
	if (HWBUFFER_AVAILABLE == m_hwbuffer_type)
	{
		destroyHwBuffers();
	}
#endif

	dequeue(); // TODO ??
}

//!< ----------------------------------------------------------------------
void zglPrim::setFlag(unsigned int obj_flag)
{
	m_flag |= obj_flag;

	if (obj_flag & FLAG_INVISIBLE)
	{
		dequeue();
	}
}

void zglPrim::resetFlag(unsigned int obj_flag)
{
	m_flag &= ~obj_flag;

	if (obj_flag & FLAG_INVISIBLE)
	{
		queue();
	}
}

//!< ----------------------------------------------------------------------
bool zglPrim::update(long time)
{
	return false;
}

void zglPrim::render()
{
}

//!< ----------------------------------------------------------------------
void zglPrim::queue()
{
	if (isQueued())
	{
		return;
	}

	zglPrimList * list = &(zglEngine::getEngine()->m_node_list);

	m_prev = list->m_prim_list_tail[m_type].m_prev;
	m_next = &list->m_prim_list_tail[m_type];

	m_prev->m_next = this;
	m_next->m_prev = this;
}

void zglPrim::dequeue()
{
	if (isQueued())
	{
		m_prev->m_next = m_next;
		m_next->m_prev = m_prev;
	}

	m_prev = m_next = NULL;
}

bool zglPrim::isQueued()
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

void zglPrim::preRender()
{
	zglCore::setModelView(zglEngine::getEngine()->modelview() * m_world);

	if (m_flag & FLAG_CULL_FACE)
	{
		zglCore::setCullFace(true);
	}

	zglCore::setBlendType((zglBlendType) m_blend_type);
}

void zglPrim::postRender()
{
	//!< Restore global parameters.
	if (m_flag & FLAG_CULL_FACE)
	{
		zglCore::setCullFace(false);
	}

	if (!(m_flag & FLAG_NO_NORMAL))
	{
		//zglCore::setNormalPointer(NULL, 0);
		zglCore::setDefaultNormal();
	}
}

//!< ----------------------------------------------------------------------
void zglPrim::applyHwBuffers(bool enable)
{
#ifdef ARB_VBO_ENABLE
	if(enable)
	{
		switch(m_hwbuffer_type)
		{
			case HWBUFFER_NONE:
			{
				if (createHwBuffers())
				{
					m_hwbuffer_type = HWBUFFER_AVAILABLE;
				}
				else
				{
					m_hwbuffer_type = HWBUFFER_FAIL;
				}
			}
			break;

			case HWBUFFER_AVAILABLE:
			{
				updateHwBuffers();
			}
			break;

			default:
			case HWBUFFER_FAIL:
			break;
		}
	}
	else
	{
		if (HWBUFFER_AVAILABLE == m_hwbuffer_type)
		{
			destroyHwBuffers();
		}
	}
#endif
}

/**
 * That is a protected function, the 'zglWidget' will check the 'available'
 * flag before invoke it, so do not check the flag again.
 */
void zglPrim::updateHwBuffers()
{
#ifdef ARB_VBO_ENABLE
	if ( (m_flag & FLAG_VBO_ENABLE) != 0)
	{
		if ( (m_flag & FLAG_VBO_DYNAMIC) != 0)
		{
			bool ret = false;
			if (m_vbo_vertex != 0)
			{
				ret = zglCore::updateBuffer(VERTEX_BUFFER, m_vbo_vertex,
						(const void*) &m_vertex[0].x,
						sizeof(zglVertex) * m_vertex_num);

				//!< size changed ? OK, try again!
				if (!ret)
				{
					zglCore::deleteBuffers(m_vbo_vertex);
					m_vbo_vertex = 0;
					m_vbo_vertex = zglCore::genBuffers(VERTEX_BUFFER,
							(const void*) &m_vertex[0].x,
							sizeof(zglVertex) * m_vertex_num, true);
				}
			}

			if (m_vbo_index != 0)
			{
				ret = zglCore::updateBuffer(INDEX_BUFFER, m_vbo_index,
						(const void*) m_indices,
						sizeof(unsigned short) * m_indices_num);

				//!< size changed ? OK, try again!
				if (!ret)
				{
					zglCore::deleteBuffers(m_vbo_index);
					m_vbo_index = 0;

					m_vbo_index = zglCore::genBuffers(INDEX_BUFFER,
							(const void*) m_indices,
							sizeof(unsigned short) * m_indices_num, true);
				}
			}
#if 0
			printf("updateHwBuffers: vertex = %d, index = %d\n", m_vbo_vertex, m_vbo_index);
#endif
		}
	}
#endif
}

bool zglPrim::createHwBuffers()
{
#ifdef ARB_VBO_ENABLE
	//if (!isQueued()) return false;

	if ( (m_flag & FLAG_VBO_ENABLE) != 0)
	{
		bool dynamic = (m_flag & FLAG_VBO_DYNAMIC) != 0;

		m_vbo_vertex = zglCore::genBuffers(VERTEX_BUFFER,
				(const void*) &m_vertex[0].x,
				sizeof(zglVertex) * m_vertex_num, dynamic);

		if (m_vbo_vertex == 0)
		{
			return false;
		}

		if (m_indices != NULL)
		{
			m_vbo_index = zglCore::genBuffers(INDEX_BUFFER, m_indices,
					sizeof(unsigned short) * m_indices_num, dynamic);
			if (m_vbo_index == 0)
			{
				zglCore::deleteBuffers(m_vbo_vertex);
				m_vbo_vertex = 0;
				return false;
			}
		}

#if defined(i386)
		printf("zglPrim(%d) VBO(0x%x)vertex = %d, index = %d\n", this->m_type,
				(unsigned int)this, m_vbo_vertex, m_vbo_index);
#endif
		return true;
	}
#endif

	return false;
}

void zglPrim::destroyHwBuffers()
{
#ifdef ARB_VBO_ENABLE
	if(m_vbo_vertex != 0)
	{
		zglCore::deleteBuffers(m_vbo_vertex);
		m_vbo_vertex = 0;
	}

	if(m_vbo_index != 0 )
	{
		zglCore::deleteBuffers(m_vbo_index);
		m_vbo_index = 0;
	}
#endif
}

//!< ----------------------------------------------------------------------
void zglPrim::calcSortDepth()
{
}


