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

#include <stdlib.h> //!< NULL
#include "zglCore.h"
#include "zglPrim3D.h"

#include "zglEngine.h"

zglPrim3D::zglPrim3D()
{
	m_type = TYPE_PRIM3D;
}

void zglPrim3D::init(zglDrawType type, zglVertex* vertex, int vert_num,
		zglTexture * texture)
{
	m_draw_type = type;
	m_vertex = vertex;
	m_vertex_num = vert_num;
	m_texture = texture;

	m_indices = NULL;
	m_indices_num = 0;

	//!< reset the local coordinate system matrix
	m_world = 1.0f;

	//Add to the node list
	queue();
}

void zglPrim3D::init(zglDrawType type, zglVertex* vertex, int vert_num,
		unsigned short * indices, int index_num,
		zglTexture * texture)
{
	m_draw_type = type;

	m_vertex = vertex;
	m_vertex_num = vert_num;

	m_texture = texture;

	m_indices = indices;
	m_indices_num = index_num;

	//!< reset the local coordinate system matrix
	m_world = 1.0f;

	//Add to the node list
	queue();
}

void zglPrim3D::render()
{
	// TODO : initial the env parameters.
	if (m_vertex_num < 1)
		return;

	const unsigned int CHECK_FLAG = FLAG_NO_FOG | FLAG_CULL_FACE;
	if (m_flag & CHECK_FLAG)
	{
		if (m_flag & FLAG_CULL_FACE)
		{
			zglCore::setCullFace(true);
		}
	}

	if (m_flag & FLAG_BILLBOARD)
	{
		zglMatrix mat = zglEngine::getEngine()->m_view;
		mat.T() = m_world.T();
		zglCore::setModelView(zglEngine::getEngine()->m_world_to_view * mat);
	}
	else
	{
		zglCore::setModelView(zglEngine::getEngine()->m_world_to_view * m_world);
	}

	if(m_material)
	{
		m_material->applyMaterial();
	}

	zglCore::setBlendType((zglBlendType) m_blend_type);
	if (m_hwbuffer_type != HWBUFFER_AVAILABLE)
	{
		zglCore::setVertexPointer3((float*) &m_vertex->x, sizeof(zglVertex));
		zglCore::setColorPointer4(&m_vertex->r, sizeof(zglVertex));

		if (m_texture)
		{
			zglCore::setTexture(m_texture->getTexID(), true);
			zglCore::setTexCoordPointer2((float*) &m_vertex->u,	sizeof(zglVertex));
		}
		else
		{
			zglCore::setTexture(0, false);
		}

		if (m_indices != NULL && m_indices_num > 0)
		{
			zglCore::drawElements(m_draw_type, m_indices_num, m_indices);
		}
		else
		{
			zglCore::drawArrays(m_draw_type, 0, m_vertex_num);
		}
	}
	else
	{
		zglCore::setBuffer(VERTEX_BUFFER, m_vbo_vertex);
		zglCore::setVertexPointer3(NULL, sizeof(zglVertex));
		zglCore::setColorPointer4( ((unsigned char*)NULL + 0x0c), sizeof(zglVertex));

		if (m_texture)
		{
			zglCore::setTexture(m_texture->getTexID(), true);
			zglCore::setTexCoordPointer2((float*)((char*)NULL + 0x10), sizeof(zglVertex));
		}
		else
		{
			zglCore::setTexture(0, false);
		}

		if(	m_indices_num > 0)
		{
			zglCore::setBuffer(INDEX_BUFFER, m_vbo_index);
			zglCore::drawElements(m_draw_type, m_indices_num, NULL);//
			zglCore::setBuffer(INDEX_BUFFER, 0);
		}
		else
		{
			zglCore::drawArrays(m_draw_type, 0, m_vertex_num);
		}

		zglCore::setBuffer(VERTEX_BUFFER, 0);
	}

	//!< Restore environment.
	if(m_material)
	{
		zglCore::setDefaultMaterial();
	}

	if (m_flag & CHECK_FLAG)
	{
		if (m_flag & FLAG_CULL_FACE)
		{
			zglCore::setCullFace(false);
		}
	}
}

void zglPrim3D::calcSortDepth()
{
	float rel_z;
	int sort_value;
	zglEngine * glEngine = zglEngine::getEngine();

	rel_z = m_world.T().RelZ(glEngine->m_view);

	if (rel_z < glEngine->m_near_dist)
	{
		sort_value = 0;
	}
	else if (rel_z > glEngine->m_far_dist)
	{
		sort_value = 0xff;
	}
	else
	{
		sort_value = (unsigned short) ((rel_z - glEngine->m_near_dist) * 255.0f
				/ (glEngine->m_far_dist - glEngine->m_near_dist));
	}

	sort_value += m_sort_offset;

	if (sort_value > 0xff)
	{
		sort_value = 0xff;
	}
	if (sort_value < 0)
	{
		sort_value = 0;
	}

	m_sort_value = sort_value;
}

