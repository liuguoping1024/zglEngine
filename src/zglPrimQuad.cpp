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
#include <assert.h> //!< assert

#include "zglCore.h"
#include "zglPrimQuad.h"

#include "zglEngine.h"

zglPrimQuad::zglPrimQuad()
{
	m_tex_group = NULL;
}

zglPrimQuad::~zglPrimQuad()
{
}

void zglPrimQuad::init(zglDrawType type, zglVertex* vertex, int vert_num,
		zglTexture * texture)
{
	if ( (vert_num & 0x03) != 0)
	{
		return;
	}

	zglPrim3D::init(type, vertex, vert_num, texture);
}

void zglPrimQuad::init(zglDrawType type, zglVertex* vertex, int vert_num,
		unsigned short * indices, int index_num, zglTexture * texture)
{
	//!< Not support yet.
	assert(0);
}

void zglPrimQuad::setTextures(zglTexture ** texs)
{
	m_tex_group = texs;
}

void zglPrimQuad::render()
{
	if(m_vertex_num == 0){
		return;
	}

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
		zglMatrix mat = zglEngine::getEngine()->world();
		mat.T() = m_world.T();
		zglCore::setModelView(zglEngine::getEngine()->modelview() * mat);
	}
	else
	{
		zglCore::setModelView(zglEngine::getEngine()->modelview() * m_world);
	}

	if (m_material)
	{
		m_material->applyMaterial();
	}

	zglCore::setBlendType((zglBlendType) m_blend_type);

	if (m_hwbuffer_type != HWBUFFER_AVAILABLE)
	{
		zglCore::setVertexPointer3((float*) &m_vertex->x, sizeof(zglVertex));
		zglCore::setColorPointer4(&m_vertex->r, sizeof(zglVertex));
		zglCore::setTexCoordPointer2((float*) &m_vertex->u, sizeof(zglVertex));

		//!< Background
		/*if (m_texture)
		{
			zglCore::setTexture(m_texture->getTexID(), true);
		}
		else
		{
			zglCore::setTexture(NULL, false);
		}
		zglCore::drawArrays(DRAW_TRI_STRIP, 0, 4);*/

		int group = m_vertex_num >> 2;
		if (m_tex_group == NULL)
		{
			zglCore::setTexture(NULL, false);

			for (int i = 0; i < group; i++)
			{
				zglCore::drawArrays(DRAW_TRI_STRIP, (i << 2), 4);
			}
		}
		else
		{
			int index = 0;
			zglTexture * tex = NULL;
			for (int i = 0; i < group; i++)
			{
				index = i;
				tex = m_tex_group[index];

				if (tex)
				{
					zglCore::setTexture(tex->getTexID(), true);
				}
				else
				{
					zglCore::setTexture(NULL, false);
				}
				zglCore::drawArrays(DRAW_TRI_STRIP, (i << 2), 4);
			}
		}
	}
	else
	{
		zglCore::setBuffer(INDEX_BUFFER, 0);
		zglCore::setBuffer(VERTEX_BUFFER, m_vbo_vertex);
		zglCore::setVertexPointer3(NULL, sizeof(zglVertex));
		zglCore::setColorPointer4(((unsigned char*) NULL + 0x0c),
				sizeof(zglVertex));
		zglCore::setTexCoordPointer2((float*) ((char*) NULL + 0x10),
				sizeof(zglVertex));

		//!< Background
		/*if (m_texture)
		{
			zglCore::setTexture(m_texture->getTexID(), true);
		}
		else
		{
			zglCore::setTexture(NULL, false);
		}
		zglCore::drawArrays(DRAW_TRI_STRIP, 0, 4);*/

		int group = m_vertex_num >> 2;
		if (m_tex_group == NULL)
		{
			zglCore::setTexture(NULL, false);

			for (int i = 0; i < group; i++)
			{
				zglCore::drawArrays(DRAW_TRI_STRIP, (i << 2), 4);
			}
		}
		else
		{
			int index = 0;
			zglTexture * tex = NULL;
			for (int i = 0; i < group; i++)
			{
				index = i;//
				tex = m_tex_group[index];

				if (tex)
				{
					zglCore::setTexture(tex->getTexID(), true);
				}
				else
				{
					zglCore::setTexture(NULL, false);
				}
				zglCore::drawArrays(DRAW_TRI_STRIP, (i << 2), 4);
			}
		}

		zglCore::setBuffer(VERTEX_BUFFER, 0);
	}

	//!< Restore environment.
	if (m_material)
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
