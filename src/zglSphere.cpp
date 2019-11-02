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
#include <math.h>

#include "zglEngine.h"
#include "zglCore.h"

#include "zglSphere.h"

#include "zglVector.h"

zglSphere::zglSphere()
{
	m_radius = 5.0f;
	m_layer = 6;//0, 30, 60, 90, 120, 150, 180
	m_slice = 36;
	m_color = 0xffffffff;

	m_flag |= FLAG_CULL_FACE;

	m_blend_type = BLEND_HALF;
}

void zglSphere::init(float radius, unsigned short layer, unsigned short slice, zglTexture * texture)
{
	if (radius > 0.0f)
		m_radius = radius;
	if (layer > 1)
		m_layer = layer;
	if (slice > 1)
		m_slice = slice;

	int vertex_count = (m_layer + 1) * (m_slice + 1);
	int index_count = 0;

	switch (m_mesh_type)
	{
	case MESH_TEXTURE:
		index_count = m_layer * (m_slice + 1) * 2;
		break;

	case MESH_WIREFRAME:
		index_count = (m_layer + 1) * (m_slice + 1) * 2;
		break;

	case MESH_POINTS:
		index_count = 0;
		break;

	default:
		index_count = m_layer * 2;
		break;
	}

	zglMesh::init(vertex_count, index_count, texture);

	updateMeshVertex();

	updateMeshType();
}

void zglSphere::setColor(unsigned int color)
{
	m_color = color;
	if (isQueued())
	{
		unsigned char r = (unsigned char) ((color >> 24) & 0xff);
		unsigned char g = (unsigned char) ((color >> 16) & 0xff);
		unsigned char b = (unsigned char) ((color >> 8) & 0xff);
		unsigned char a = (unsigned char) ((color) & 0xff);

		zglVertexEx * v = (zglVertexEx *)m_vertex;
		int count = (m_layer + 1) * (m_slice + 1);
		for(int i=0; i<count; i++)
		{
			v->r = r;
			v->g = g;
			v->b = b;
			v->a = a;
			v ++;
		}
	}
}

void zglSphere::updateMeshVertex()
{
	if(m_vertex == NULL)
	{
		//Error when allocating memory.
		return;
	}

	//!< 1. Prepare sin(), cos() value-table
	float * slice_cos_a = new float[m_slice + 1];
	float * slice_sin_a = new float[m_slice + 1];

	float * slice_u = new float[m_slice + 1];

	float angle;
	unsigned short i, j;
	float div = (float) m_slice;
	for (i = 1; i < m_slice; i++)
	{
		slice_u[i] = i / div;
		angle = ZGL_2PI * slice_u[i];
		slice_cos_a[i] = cos(angle);
		slice_sin_a[i] = sin(angle);
	}

	slice_u[0] = 0.0f;
	slice_u[m_slice] = 1.0f;

	slice_cos_a[0] = 1.0f;
	slice_cos_a[m_slice] = 1.0f;

	slice_sin_a[0] = 0.0f;
	slice_sin_a[m_slice] = 0.0f;

	zglVertexEx * mesh_vertexs = (zglVertexEx *)m_vertex;
	//!< 2. Update the top line.
	for (i = 0; i < m_slice + 1; i++)
	{
		mesh_vertexs[i].init(0.0f, m_radius, 0.0f, slice_u[i], 0.0f, m_color);
		mesh_vertexs[i].setNormal(0.0f, 1.0f, 0.0f);
	}

	//!< 3. Update the bottom line.
	j = (m_slice + 1) * m_layer;
	for (i = 0; i < m_slice + 1; i++)
	{
		mesh_vertexs[j + i].init(0.0f, -m_radius, 0.0f, slice_u[i], 1.0f, m_color);
		mesh_vertexs[j + i].setNormal(0.0f, -1.0f, 0.0f);
	}

	//!< 4. Update the other lines.
	float h, r, v;
	div = (float) m_layer;
	int p = 0;

	zglVector normal;
	for (j = 1; j < m_layer; j++)
	{
		v = j / div;
		angle = ZGL_PI * v;
		h = m_radius * cos(angle);
		r = m_radius * sin(angle);

		p = (m_slice + 1) * j;

		for (i = 0; i < m_slice + 1; i++)
		{
			mesh_vertexs[i + p].init(r * slice_cos_a[i], h, r * slice_sin_a[i],
					slice_u[i], v, m_color);

			normal(mesh_vertexs[i + p].x,
					mesh_vertexs[i + p].y,
					mesh_vertexs[i + p].z);

			normal.normalize();
			mesh_vertexs[i + p].setNormal(normal.x,
					normal.y,
					normal.z);
		}
	}

	//!< 5. Free the tables
	delete []slice_u;
	delete []slice_cos_a;
	delete []slice_sin_a;
}

void zglSphere::setMeshType(unsigned char type)
{
	if (type == m_mesh_type)
		return;

	m_mesh_type = type;

	if (!isQueued())
	{
		//!< If the primitive is not registered, just skip it.
		return;
	}

	int index_count = 0;
	switch (m_mesh_type)
	{
	default:
	case MESH_TEXTURE:
		{
			index_count = m_layer * (m_slice + 1) * 2;
			resizeIndices(index_count);
		}
		break;

	case MESH_WIREFRAME:
		{
			index_count = (m_layer + 1) * (m_slice + 1) * 2;
			resizeIndices(index_count);
		}
		break;

	case MESH_POINTS:
		{
			//index_count = m_cols * m_rows;
			resizeIndices(index_count);
		}
		break;
	}

	updateMeshType();
}

void zglSphere::updateMeshType()
{
	switch (m_mesh_type)
	{
	case MESH_TEXTURE:
		{
			int p = 0;
			int index = 0;
			int stride = m_slice + 1;

			for (unsigned short i = 0; i < m_layer; i++)
			{
				for (unsigned short j = 0; j < stride; j++)
				{
					m_indices[p++] = index;
					m_indices[p++] = index + stride;

					index++;
				}
			}
		}
		break;

	case MESH_WIREFRAME:
		{
			int p = 0;
			int index = 0;

			unsigned short i, j;

			for (i = 0; i < m_layer + 1; i++)
			{
				for (j = 0; j < m_slice + 1; j++)
				{
					m_indices[p++] = index ++;
				}
			}

			for (i = 0; i < m_slice + 1; i++)
			{
				index = i;
				for (j = 0; j < m_layer + 1; j++)
				{
					m_indices[p++] = index;
					index += (m_slice + 1);
				}
			}
		}
		break;

	case MESH_POINTS:
		break;
	}
}

void zglSphere::render()
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

void zglSphere::renderNoVBO()
{
	zglVertexEx * mesh_vertexs = (zglVertexEx *)m_vertex;
	zglCore::setVertexPointer3((float*) &mesh_vertexs[0].x,
			sizeof(zglVertexEx));
	zglCore::setColorPointer4(&mesh_vertexs[0].r, sizeof(zglVertexEx));

	if (!(m_flag & FLAG_NO_NORMAL))
	{
		zglCore::setNormalPointer(&mesh_vertexs[0].nx, sizeof(zglVertexEx));
	}

	switch (m_mesh_type)
	{
	case MESH_TEXTURE:
		{
			if (m_material)
			{
				m_material->applyMaterial();
			}

			if (m_texture)
			{
				zglCore::setTexture(m_texture->getTexID(), true);
				zglCore::setTexCoordPointer2((float*) &mesh_vertexs[0].u,
						sizeof(zglVertexEx));
			}
			else
			{
				zglCore::setTexture(0, false);
			}

			unsigned short p = 0;
			unsigned short stride = (m_slice + 1) << 1;
			for (unsigned short i = 0; i < m_layer; i++)
			{
				zglCore::drawElements(DRAW_TRI_STRIP, stride, &m_indices[p]);

				p += stride;
			}

			//!< Restore environment.
			if (m_material)
			{
				zglCore::setDefaultMaterial();
			}
		}
		break;

	case MESH_WIREFRAME:
		{
			unsigned short i;
			zglCore::setTexture(0, false);

			for (i = 0; i < m_layer + 1; i++)
			{
				zglCore::drawElements(DRAW_LINE_STRIP, (m_slice + 1),
						&m_indices[(m_slice + 1) * i]);
			}

			int p = (m_layer + 1) * (m_slice + 1);

			for (i = 0; i < (m_slice + 1); i++)
			{
				zglCore::drawElements(DRAW_LINE_STRIP, (m_layer + 1),
						&m_indices[p + (m_layer + 1) * i]);
			}
		}
		break;

	case MESH_POINTS:
		{
			zglCore::setTexture(0, false);
			zglCore::drawArrays(DRAW_POINTS, 0, (m_layer + 1) * (m_slice + 1));
		}
		break;
	}
}

void zglSphere::renderVBO()
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
	switch (m_mesh_type)
	{
	case MESH_TEXTURE:
		{
			if (m_material)
			{
				m_material->applyMaterial();
			}

			if (m_texture)
			{
				zglCore::setTexture(m_texture->getTexID(), true);
				zglCore::setTexCoordPointer2((float*) ((char*) NULL + 0x10),
						sizeof(zglVertexEx));
			}
			else
			{
				zglCore::setTexture(0, false);
			}

			unsigned short p = 0;
			unsigned short stride = (m_slice + 1) << 1;
			for (unsigned short i = 0; i < m_layer; i++)
			{
				zglCore::drawElements(DRAW_TRI_STRIP, stride, pIndex + p);
				p += stride;
			}

			//!< Restore environment.
			if (m_material)
			{
				zglCore::setDefaultMaterial();
			}
		}
		break;

	case MESH_WIREFRAME:
		{
			unsigned short i;
			zglCore::setTexture(0, false);

			for (i = 0; i < m_layer + 1; i++)
			{
				zglCore::drawElements(DRAW_LINE_STRIP, (m_slice + 1), pIndex + (m_slice + 1) * i);
			}

			int p = (m_layer + 1) * (m_slice + 1);
			pIndex += p;

			for (i = 0; i < (m_slice + 1); i++)
			{
				zglCore::drawElements(DRAW_LINE_STRIP, (m_layer + 1), pIndex + (m_layer + 1) * i);
			}
		}
		break;

	case MESH_POINTS:
		{
			zglCore::setTexture(0, false);
			zglCore::drawArrays(DRAW_POINTS, 0, (m_layer + 1) * (m_slice + 1));
		}
		break;
	}

	zglCore::setBuffer(VERTEX_BUFFER, 0);
	zglCore::setBuffer(INDEX_BUFFER, 0);
}




