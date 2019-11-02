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

#include "zglCore.h"

#include "zglQuadMesh.h"

zglQuadMesh::zglQuadMesh()
{
	m_rows = 0;
	m_cols = 0;
}

void zglQuadMesh::init(int rows, int cols, zglTexture * texture)
{
	m_rows = rows;
	m_cols = cols;
	int vertex_count = m_rows * m_cols;
	int index_count = 0;

	switch (m_mesh_type)
	{

	case MESH_TEXTURE:
		index_count = m_cols * 2 * (m_rows - 1);
		break;

	case MESH_WIREFRAME:
		index_count = m_cols * 2 * m_rows;
		break;

	case MESH_POINTS:
		index_count = m_cols * m_rows;
		break;

	default:
		index_count = m_cols * 2;
		break;
	}

	zglMesh::init(vertex_count, index_count, texture);

	updateMeshType();
}

void zglQuadMesh::updateMesh(zglVertexEx * vlt, zglVertexEx * vrb)
{
	if (vlt == NULL || vrb == NULL)
		return;

	if (m_rows < 2 || m_cols < 2)
		return;

	int i, j;

	double dx = (vrb->x - vlt->x) / (m_cols - 1);
	double dy = (vrb->y - vlt->y) / (m_rows - 1);
	//!< double dz = (vrb->z - vlt->z);
	double du = (vrb->u - vlt->u) / (m_cols - 1);
	double dv = (vrb->v - vlt->v) / (m_rows - 1);

	zglVertexEx * p = (zglVertexEx *)m_vertex;

	for (i = 0; i < m_rows; i++)
	{
		if (i == 0) //First row (top)
		{
			// [0,0];
			p->x = vlt->x;//Left
			p->y = vlt->y;//!< FIX
			p->z = vlt->z;
			p->u = vlt->u;//Left
			p->v = vlt->v;//!< FIX

			p++;

			//
			for (j = 1; j < m_cols - 1; j++)
			{
				p->x = vlt->x + dx * j;
				p->y = vlt->y;//!< FIX
				p->z = vlt->z;
				p->u = vlt->u + du * j;
				p->v = vlt->v;//!< FIX

				p++;
			}

			//
			p->x = vrb->x; //Right
			p->y = vlt->y;
			p->z = vlt->z;
			p->u = vrb->u; //Right
			p->v = vlt->v;

			p++;
		}
		else if (i == m_rows - 1) //Last row (bottom)
		{
			// [0,0];
			p->x = vlt->x;
			p->y = vrb->y;//!< FIX
			p->z = vlt->z;
			p->u = vlt->u;
			p->v = vrb->v;//!< FIX

			p++;

			//
			for (j = 1; j < m_cols - 1; j++)
			{
				p->x = (p - m_cols)->x; //vlt->x + dx * j;
				p->y = vrb->y;//!< FIX
				p->z = vlt->z;
				p->u = (p - m_cols)->u;//vlt->u + du * j;
				p->v = vrb->v;//!< FIX

				p++;
			}

			//
			p->x = vrb->x;
			p->y = vrb->y;//!< FIX
			p->z = vlt->z;
			p->u = vrb->u;
			p->v = vrb->v;//!< FIX

			p++;
		}
		else
		{
			double y = vlt->y + dy * i;
			double v = vlt->v + dv * i;

			p->x = vlt->x;
			p->y = y;
			p->z = vlt->z;
			p->u = vlt->u;
			p->v = v;

			p++;

			//
			for (j = 1; j < m_cols - 1; j++)
			{
				p->x = (p - m_cols)->x;//vlt->x + dx * j;
				p->y = y;
				p->z = vlt->z;
				p->u = (p - m_cols)->u;//vlt->u + du * j;
				p->v = v;

				p++;
			}

			//
			p->x = vrb->x;
			p->y = y;
			p->z = vlt->z;
			p->u = vrb->u;
			p->v = v;

			p++;
		}
	}
}

void zglQuadMesh::updateMesh(zglVertexEx * vlt, zglVertexEx * vlb, zglVertexEx * vrt,
		zglVertexEx * vrb)
{
	if (vlt == NULL || vlb == NULL || vrt == NULL || vrb == NULL)
		return;
	if (m_rows < 2 || m_cols < 2)
		return;

	int i, j;

	zglVertexEx vl, vr;
	zglVertexEx * mesh_vertexs = (zglVertexEx *)m_vertex;

	for (i = 0; i < m_rows; i++)
	{
		//!< calculate the left/right edge
		if (i == 0)
		{
			vl.x = vlt->x;
			vl.y = vlt->y;
			vl.z = vlt->z;
			vl.u = vlt->u;
			vl.v = vlt->v;

			vr.x = vrt->x;
			vr.y = vrt->y;
			vr.z = vrt->z;
			vr.u = vrt->u;
			vr.v = vrt->v;
		}
		else if (i == m_rows - 1)
		{
			vl.x = vlb->x;
			vl.y = vlb->y;
			vl.z = vlb->z;
			vl.u = vlb->u;
			vl.v = vlb->v;

			vr.x = vrb->x;
			vr.y = vrb->y;
			vr.z = vrb->z;
			vr.u = vrb->u;
			vr.v = vrb->v;
		}
		else
		{
			double f = i / (double) (m_rows - 1);
			vl.x = (float) (vlt->x - (vlt->x - vlb->x) * f);
			vl.y = (float) (vlt->y - (vlt->y - vlb->y) * f);
			vl.z = (float) (vlt->z - (vlt->z - vlb->z) * f);
			vl.u = (float) (vlt->u - (vlt->u - vlb->u) * f);
			vl.v = (float) (vlt->v - (vlt->v - vlb->v) * f);

			vr.x = (float) (vrt->x - (vrt->x - vrb->x) * f);
			vr.y = (float) (vrt->y - (vrt->y - vrb->y) * f);
			vr.z = (float) (vrt->z - (vrt->z - vrb->z) * f);
			vr.u = (float) (vrt->u - (vrt->u - vrb->u) * f);
			vr.v = (float) (vrt->v - (vrt->v - vrb->v) * f);
		}

		zglVertexEx * p = mesh_vertexs + m_cols * i;

		p->x = vl.x;
		p->y = vl.y;
		p->z = vl.z;
		p->u = vl.u;
		p->v = vl.v;

		(p + m_cols - 1)->x = vr.x;
		(p + m_cols - 1)->y = vr.y;
		(p + m_cols - 1)->z = vr.z;
		(p + m_cols - 1)->u = vr.u;
		(p + m_cols - 1)->v = vr.v;

		p++;

		double r = 1 / (double) (m_cols - 1);
		//!< Calculate the lines
		for (j = 1; j < m_cols - 1; j++)
		{
			p->x = vl.x + (vr.x - vl.x) * j * r;
			p->y = vl.y + (vr.y - vl.y) * j * r;
			p->z = vl.z + (vr.z - vl.z) * j * r;
			p->u = vl.u + (vr.u - vl.u) * j * r;
			p->v = vl.v + (vr.v - vl.v) * j * r;
			p++;
		}
	}
}

void zglQuadMesh::setMeshType(unsigned char type)
{
	if (type == m_mesh_type)
		return;

	m_mesh_type = type;

	if(m_indices == NULL)
		return;

	int index_count = 0;
	switch (m_mesh_type)
	{
	case MESH_TEXTURE:
		{
			index_count = m_cols * 2 * (m_rows - 1);
			resizeIndices(index_count);
		}
		break;

	case MESH_WIREFRAME:
		{
			index_count = m_cols * 2 * m_rows;
			resizeIndices(index_count);
		}
		break;

	case MESH_POINTS:
		{
			index_count = m_cols * m_rows;
			resizeIndices(index_count);
		}
		break;

	default:
		index_count = m_cols * 2;
		break;
	}

	updateMeshType();
}

void zglQuadMesh::updateMeshType()
{
	switch (m_mesh_type)
	{
	case MESH_TEXTURE:
		{
			int p, index;
			p = 0;
			//resizeIndices(m_cols * 2 * (m_rows - 1));
			for (int i = 0; i < m_rows - 1; i++)
			{
				index = m_cols * i;
				for (int j = 0; j < m_cols; j++)
				{
					m_indices[p++] = index;
					m_indices[p++] = index + m_cols;

					index++;
				}
			}
		}
		break;

	case MESH_WIREFRAME:
		{
			//index_count = m_cols * 2 * m_rows;
			int i, j, p, index;
			p = 0;
			index = 0;
			for (i = 0; i < m_rows; i++)
			{
				for (j = 0; j < m_cols; j++)
				{
					m_indices[p++] = index ++;
				}
			}

			for (i = 0; i < m_cols; i++)
			{
				index = i;
				for (j = 0; j < m_rows; j++)
				{
					m_indices[p++] = index;
					index += m_cols;
				}
			}
		}
		break;

	case MESH_POINTS:
		{
			for (int i = 0; i < m_rows * m_cols; i++)
			{
				m_indices[i] = i;
			}
		}
		break;

	default:
		break;

	}
}

void zglQuadMesh::renderNoVBO()
{
	zglVertexEx * mesh_vertexs = (zglVertexEx *)m_vertex;
	zglCore::setVertexPointer3((float*) &mesh_vertexs[0].x, sizeof(zglVertexEx));
	zglCore::setColorPointer4(&mesh_vertexs[0].r, sizeof(zglVertexEx));

	switch (m_mesh_type)
	{
	case MESH_TEXTURE:
		{
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

			for (int i = 0; i < m_rows - 1; i++)
			{
				zglCore::drawElements(DRAW_TRI_STRIP, m_cols * 2,
						&m_indices[m_cols * 2 * i]);
			}
		}
		break;

	case MESH_WIREFRAME:
		{
			int i;
			zglCore::setTexture(0, false);

			for (i = 0; i < m_rows; i++)
			{
				zglCore::drawElements(DRAW_LINE_STRIP, m_cols,
						&m_indices[m_cols * i]);
			}

			int p = m_rows * m_cols;

			for (i = 0; i < m_cols; i++)
			{
				zglCore::drawElements(DRAW_LINE_STRIP, m_rows,
						&m_indices[p + m_rows * i]);
			}
		}
		break;

	case MESH_POINTS:
		{
			zglCore::setTexture(0, false);
			zglCore::drawElements(DRAW_POINTS, m_cols * m_rows, m_indices);
		}
		break;
	}
}

void zglQuadMesh::renderVBO()
{
	zglCore::setBuffer(VERTEX_BUFFER, m_vbo_vertex);
	zglCore::setVertexPointer3(NULL, sizeof(zglVertexEx));
	zglCore::setColorPointer4(((unsigned char*) NULL + 0x0c),
			sizeof(zglVertexEx));

	zglCore::setBuffer(INDEX_BUFFER, m_vbo_index);
	unsigned short *pIndex = NULL;

	switch (m_mesh_type)
	{
	case MESH_TEXTURE:
		{
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

			for (int i = 0; i < m_rows - 1; i++)
			{
				zglCore::drawElements(DRAW_TRI_STRIP, m_cols * 2,
						pIndex + m_cols * 2 * i);
			}
		}
		break;

	case MESH_WIREFRAME:
		{
			int i;
			zglCore::setTexture(0, false);

			for (i = 0; i < m_rows; i++)
			{
				zglCore::drawElements(DRAW_LINE_STRIP, m_cols,
						pIndex + m_cols * i);
			}

			pIndex += m_rows * m_cols;

			for (i = 0; i < m_cols; i++)
			{
				//zglCore::drawElements(DRAW_LINE_STRIP, m_rows,
				//		&m_indices[p + m_rows * i]);
				zglCore::drawElements(DRAW_LINE_STRIP, m_rows,
						pIndex + m_rows * i);
			}
		}
		break;

	case MESH_POINTS:
		{
			zglCore::setTexture(0, false);
			zglCore::drawElements(DRAW_POINTS, m_cols * m_rows, NULL);
		}
		break;
	}

	zglCore::setBuffer(VERTEX_BUFFER, 0);
	zglCore::setBuffer(INDEX_BUFFER, 0);
}

void zglQuadMesh::render()
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
