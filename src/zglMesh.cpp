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

#include "zglBasicType.h"

#include "zglMesh.h"

zglMesh::zglMesh()
{
	m_mesh_type = MESH_TEXTURE;
}

zglMesh::~zglMesh()
{
	zglVertexEx * mesh_vertexs = (zglVertexEx *)m_vertex;
	if (mesh_vertexs != NULL)
	{
		delete[] mesh_vertexs;
		mesh_vertexs = NULL;
		m_vertex = NULL;
	}

	if (m_indices != NULL)
	{
		delete[] m_indices;
		m_indices = NULL;
	}
}

void zglMesh::init(int vertex, int indices, zglTexture * texture)
{
	if(m_vertex != NULL)
	{
		if(vertex != m_vertex_num)
		{
			delete[] m_vertex;
			m_vertex = NULL;

			m_vertex_num = vertex;
			m_vertex = new zglVertexEx[m_vertex_num];
		}
	}
	else
	{
		m_vertex_num = vertex;
		m_vertex = new zglVertexEx[m_vertex_num];
	}

	if(m_indices != NULL)
	{
		if(m_indices_num != indices)
		{
			delete[] m_indices;
			m_indices = NULL;

			m_indices_num = indices;
			if(m_indices_num != 0 )
			{
				m_indices = new unsigned short[m_indices_num];
			}
		}
	}
	else
	{
		m_indices_num = indices;
		if(m_indices_num != 0 )
		{
			m_indices = new unsigned short[m_indices_num];
		}
	}

	//Just register the primitive, forget the mesh type!
	//zglPrim3D::init(DRAW_TRIANGLES, m_vertex, m_vertex_num, texture);

	m_draw_type = DRAW_TRIANGLES;

	//m_vertex = vertex;
	//m_vertex_num = vert_num;
	m_texture = texture;

	//m_indices = NULL;
	//m_indices_num = 0;

	//!< reset the local coordinate system matrix
	m_world = 1.0f;

	//Add to the node list
	queue();
}

void zglMesh::setMeshType(unsigned char type)
{
	switch (type)
	{
	case MESH_TEXTURE:
	case MESH_WIREFRAME:
	case MESH_POINTS:
	//case MESH_FLATSHADE:
	//case MESH_SMOOTHSHADE:
		m_mesh_type = type;
		break;
	default:
		m_mesh_type = MESH_TEXTURE;
		break;
	}
}

bool zglMesh::isAvailable()
{
	return isQueued();
}

void zglMesh::resizeIndices(int indices)
{
	if(m_indices != NULL)
	{
		if(m_indices_num != indices)
		{
			delete[] m_indices;
			m_indices = NULL;

			m_indices_num = indices;
			if(m_indices_num != 0)
			{
				m_indices = new unsigned short[m_indices_num];
			}
		}
	}
	else
	{
		m_indices_num = indices;
		if(m_indices_num != 0)
		{
			m_indices = new unsigned short[m_indices_num];
		}
	}
}

void zglMesh::updateHwBuffers()
{
	if ((m_flag & FLAG_VBO_ENABLE) != 0)
	{
		if ((m_flag & FLAG_VBO_DYNAMIC) != 0)
		{
			bool ret = false;
			if (m_vbo_vertex != 0)
			{
				ret = zglCore::updateBuffer(VERTEX_BUFFER, m_vbo_vertex,
						(const void*) &m_vertex[0].x,
						sizeof(zglVertexEx) * m_vertex_num);

				//!< size changed ? OK, try again!
				if (!ret)
				{
					zglCore::deleteBuffers(m_vbo_vertex);
					m_vbo_vertex = 0;
					m_vbo_vertex = zglCore::genBuffers(VERTEX_BUFFER,
							(const void*) &m_vertex[0].x,
							sizeof(zglVertexEx) * m_vertex_num, true);
				}
			}

			if (m_vbo_index != 0)
			{
				if(m_indices_num != 0)
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
				else
				{
					zglCore::deleteBuffers(m_vbo_index);
					m_vbo_index = 0;
				}
			}
			else
			{
				m_vbo_index = zglCore::genBuffers(INDEX_BUFFER, m_indices,
						sizeof(unsigned short) * m_indices_num, true);
				if (m_vbo_index == 0)
				{
					zglCore::deleteBuffers(m_vbo_vertex);
					m_vbo_vertex = 0;
				}
			}

			//printf("updateHwBuffers: vertex = %d, index = %d\n", m_vbo_vertex,
			//		m_vbo_index);
		}
	}
}

bool zglMesh::createHwBuffers()
{
	if ( (m_flag & FLAG_VBO_ENABLE) != 0)
	{
		bool dynamic = (m_flag & FLAG_VBO_DYNAMIC) != 0;

		m_vbo_vertex = zglCore::genBuffers(VERTEX_BUFFER,
				(const void*) &m_vertex[0].x,
				sizeof(zglVertexEx) * m_vertex_num, dynamic);
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

		//printf("zglMesh(%d) VBO(0x%x)vertex = %d, index = %d\n", this->m_type,
		//		(unsigned int)this, m_vbo_vertex, m_vbo_index);
		return true;
	}

	return false;
}



