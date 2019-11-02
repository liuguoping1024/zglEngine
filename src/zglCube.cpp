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
#include <string.h> //!< memcpy

#include "zglCube.h"
//#include "zglEngine.h"
#include "zglCore.h"

static const unsigned short g_index[24] = {
		0, 1, 2, 3,
		4, 5, 6, 7,

		8, 9, 10, 11,
		12, 13, 14, 15,

		16, 17, 18, 19,
		20, 21, 22, 23
};

zglCube::zglCube()
{
	m_tex_num = 0;
	m_flag |= FLAG_CULL_FACE;
}

void zglCube::init(float width, float height, float thick,
		zglTexture * texture, unsigned int tex_num)
{
	zglMesh::init(m_vertex_max, m_vertex_max, texture);

	m_tex_num = texture == NULL ? 0 : tex_num;

	//!< Index array
	memcpy(m_indices, g_index, sizeof(short) * m_vertex_max);

	float w = width * 0.5f;
	float h = height * 0.5f;
	float t = thick * 0.5f;

	float u = 1.0f;
	float v = 1.0f;

	zglVertexEx * mesh_vertexs = (zglVertexEx *)m_vertex;
	mesh_vertexs[0].init(-w, h, -t, 0.0f, 0.0f);
	mesh_vertexs[0].setNormal(0.0f, 0.0f, -1.0f);
	mesh_vertexs[1].init(-w, -h, -t, 0.0f, v);
	mesh_vertexs[1].setNormal(0.0f, 0.0f, -1.0f);
	mesh_vertexs[2].init(w, -h, -t, u, v);
	mesh_vertexs[2].setNormal(0.0f, 0.0f, -1.0f);
	mesh_vertexs[3].init(w, h, -t, u, 0.0f);
	mesh_vertexs[3].setNormal(0.0f, 0.0f, -1.0f);

	mesh_vertexs[4].init(w, h, t, 0.0f, 0.0f);
	mesh_vertexs[4].setNormal(0.0f, 0.0f, 1.0f);
	mesh_vertexs[5].init(w, -h, t, 0.0f, v);
	mesh_vertexs[5].setNormal(0.0f, 0.0f, 1.0f);
	mesh_vertexs[6].init(-w, -h, t, u, v);
	mesh_vertexs[6].setNormal(0.0f, 0.0f, 1.0f);
	mesh_vertexs[7].init(-w, h, t, u, 0.0f);
	mesh_vertexs[7].setNormal(0.0f, 0.0f, 1.0f);

	mesh_vertexs[8].init(w, h, -t, 0.0f, 0.0f);
	mesh_vertexs[8].setNormal(1.0f, 0.0f, 0.0f);
	mesh_vertexs[9].init(w, -h, -t, 0.0f, v);
	mesh_vertexs[9].setNormal(1.0f, 0.0f, 0.0f);
	mesh_vertexs[10].init(w, -h, t, u, v);
	mesh_vertexs[10].setNormal(1.0f, 0.0f, 0.0f);
	mesh_vertexs[11].init(w, h, t, u, 0.0f);
	mesh_vertexs[11].setNormal(1.0f, 0.0f, 0.0f);

	mesh_vertexs[12].init(-w, h, t, 0.0f, 0.0f);
	mesh_vertexs[12].setNormal(-1.0f, 0.0f, 0.0f);
	mesh_vertexs[13].init(-w, -h, t, 0.0f, v);
	mesh_vertexs[13].setNormal(-1.0f, 0.0f, 0.0f);
	mesh_vertexs[14].init(-w, -h, -t, u, v);
	mesh_vertexs[14].setNormal(-1.0f, 0.0f, 0.0f);
	mesh_vertexs[15].init(-w, h, -t, u, 0.0f);
	mesh_vertexs[15].setNormal(-1.0f, 0.0f, 0.0f);

	mesh_vertexs[16].init(-w, h, t, 0.0f, 0.0f);
	mesh_vertexs[16].setNormal(0.0f, 1.0f, 0.0f);
	mesh_vertexs[17].init(-w, h, -t, 0.0f, v);
	mesh_vertexs[17].setNormal(0.0f, 1.0f, 0.0f);
	mesh_vertexs[18].init(w, h, -t, u, v);
	mesh_vertexs[18].setNormal(0.0f, 1.0f, 0.0f);
	mesh_vertexs[19].init(w, h, t, u, 0.0f);
	mesh_vertexs[19].setNormal(0.0f, 1.0f, 0.0f);

	mesh_vertexs[20].init(w, -h, t, 0.0f, 0.0f);
	mesh_vertexs[20].setNormal(0.0f, -1.0f, 0.0f);
	mesh_vertexs[21].init(w, -h, -t, 0.0f, v);
	mesh_vertexs[21].setNormal(0.0f, -1.0f, 0.0f);
	mesh_vertexs[22].init(-w, -h, -t, u, v);
	mesh_vertexs[22].setNormal(0.0f, -1.0f, 0.0f);
	mesh_vertexs[23].init(-w, -h, t, u, 0.0f);
	mesh_vertexs[23].setNormal(0.0f, -1.0f, 0.0f);
}

void zglCube::render()
{
	preRender();

	if(m_hwbuffer_type != HWBUFFER_AVAILABLE)
	{
		renderNoVBO();
	}
	else
	{
		renderVBO();
	}

	postRender();
}

void zglCube::renderNoVBO()
{
	zglVertexEx * mesh_vertexs = (zglVertexEx *)m_vertex;
	zglCore::setVertexPointer3((float*) &mesh_vertexs[0].x, sizeof(zglVertexEx));
	zglCore::setColorPointer4(&mesh_vertexs[0].r, sizeof(zglVertexEx));

	if (! (m_flag & FLAG_NO_NORMAL))
	{
		zglCore::setNormalPointer(&mesh_vertexs[0].nx, sizeof(zglVertexEx));
	}

	if (m_mesh_type == MESH_TEXTURE)
	{
		if (m_material)
		{
			m_material->applyMaterial();
		}

		m_draw_type = DRAW_QUADS;
		switch (m_tex_num)
		{
		default:
		case 0:
			zglCore::setTexture(0, false);
			zglCore::drawElements(m_draw_type, m_vertex_max, m_indices);
			break;

		case 1:
			zglCore::setTexture(m_texture->getTexID(), true);
			zglCore::setTexCoordPointer2((float*) &mesh_vertexs[0].u,
					sizeof(zglVertexEx));

			zglCore::drawElements(m_draw_type, m_vertex_max, m_indices);
			break;

		case 2:
			zglCore::setTexCoordPointer2((float*) &mesh_vertexs[0].u,
					sizeof(zglVertex));
			for (int i = 0; i < (m_vertex_max >> 2); i++)
			{
				zglCore::setTexture(m_texture[i % 2].getTexID(), true);
				zglCore::drawElements(m_draw_type, 4, &m_indices[i << 2]);
			}
			break;

		case 3:
			zglCore::setTexCoordPointer2((float*) &mesh_vertexs[0].u,
					sizeof(zglVertex));
			for (int i = 0; i < (m_vertex_max >> 2); i++)
			{
				zglCore::setTexture(m_texture[i % 3].getTexID(), true);
				zglCore::drawElements(m_draw_type, 4, &m_indices[i << 2]);
			}
			break;

		case 6:
			zglCore::setTexCoordPointer2((float*) &mesh_vertexs[0].u,
					sizeof(zglVertexEx));
			for (int i = 0; i < (m_vertex_max >> 2); i++)
			{
				zglCore::setTexture(m_texture[i].getTexID(), true);
				zglCore::drawElements(m_draw_type, 4, &m_indices[i << 2]);
			}
			break;
		}

		//!< Restore environment.
		if (m_material)
		{
			zglCore::setDefaultMaterial();
		}
	}
	else if (m_mesh_type == MESH_WIREFRAME)
	{
		zglCore::setTexture(0, false);
		for (int i = 0; i < (m_vertex_max >> 2); i++)
		{
			zglCore::drawElements(DRAW_LINE_LOOP, 4, &m_indices[i << 2]);
		}
	}
	else if(m_mesh_type == MESH_POINTS)
	{
		zglCore::setTexture(0, false);
		zglCore::drawElements(DRAW_POINTS, 8, m_indices);
	}
}

void zglCube::renderVBO()
{
	zglCore::setBuffer(VERTEX_BUFFER, m_vbo_vertex);
	zglCore::setVertexPointer3(NULL, sizeof(zglVertexEx));
	zglCore::setColorPointer4(((unsigned char*) NULL + 0x0c),
			sizeof(zglVertexEx));

	if (!(m_flag & FLAG_NO_NORMAL))
	{
		zglCore::setNormalPointer((float*) ((char*) NULL + 0x18),
				sizeof(zglVertexEx));
	}

	zglCore::setBuffer(INDEX_BUFFER, m_vbo_index);
	unsigned short *pIndex = NULL;
	if (m_mesh_type == MESH_TEXTURE)
	{
		if (m_material)
		{
			m_material->applyMaterial();
		}

		m_draw_type = DRAW_QUADS;
		switch (m_tex_num)
		{
		default:
		case 0:
			zglCore::setTexture(0, false);
			zglCore::drawElements(m_draw_type, m_vertex_max, NULL);
			break;

		case 1:
			zglCore::setTexture(m_texture->getTexID(), true);
			zglCore::setTexCoordPointer2((float*) ((char*) NULL + 0x10),
					sizeof(zglVertexEx));

			zglCore::drawElements(m_draw_type, m_vertex_max, NULL);
			break;

		case 2:
			zglCore::setTexCoordPointer2((float*) ((char*) NULL + 0x10),
					sizeof(zglVertexEx));
			for (int i = 0; i < (m_vertex_max >> 2); i++)
			{
				zglCore::setTexture(m_texture[i % 2].getTexID(), true);
				zglCore::drawElements(m_draw_type, 4, pIndex + (i << 2));
			}
			break;

		case 3:
			zglCore::setTexCoordPointer2((float*) ((char*) NULL + 0x10),
					sizeof(zglVertexEx));
			for (int i = 0; i < (m_vertex_max >> 2); i++)
			{
				zglCore::setTexture(m_texture[i % 3].getTexID(), true);
				zglCore::drawElements(m_draw_type, 4, pIndex + (i << 2));
			}
			break;

		case 6:
			zglCore::setTexCoordPointer2((float*) ((char*) NULL + 0x10),
					sizeof(zglVertexEx));
			for (int i = 0; i < (m_vertex_max >> 2); i++)
			{
				zglCore::setTexture(m_texture[i].getTexID(), true);
				zglCore::drawElements(m_draw_type, 4, pIndex + (i << 2));
			}
			break;
		}

		//!< Restore environment.
		if (m_material)
		{
			zglCore::setDefaultMaterial();
		}
	}
	else if (m_mesh_type == MESH_WIREFRAME)
	{
		zglCore::setTexture(0, false);
		for (int i = 0; i < (m_vertex_max >> 2); i++)
		{
			zglCore::drawElements(DRAW_LINE_LOOP, 4, pIndex + (i << 2));
		}
	}
	else if(m_mesh_type == MESH_POINTS)
	{
		zglCore::setTexture(0, false);
		zglCore::drawElements(DRAW_POINTS, 8, NULL);
	}

	zglCore::setBuffer(VERTEX_BUFFER, 0);
	zglCore::setBuffer(INDEX_BUFFER, 0);
}

