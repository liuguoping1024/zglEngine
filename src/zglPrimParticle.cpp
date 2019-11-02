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
#include <memory.h>

#include "zglEngine.h"
#include "zglCore.h"

#include "zglPrim.h"
#include "zglPrimParticle.h"

zglPrimParticle::zglPrimParticle()
{
	m_type = TYPE_SPRITE;
	m_vertex_num = 0;
	m_aliveCount = 0;
}

zglPrimParticle::~zglPrimParticle()
{
	if (m_vertex != NULL)
	{
		free( m_vertex);
	}
}

void zglPrimParticle::init(zglParticleNode* particles, unsigned int particleNum,
		zglTexture * texture)
{
	if (particleNum != m_vertex_num)
	{
		if (m_vertex != NULL)
		{
			free( m_vertex);
		}

		int buffer_size = particleNum * 4 * sizeof(zglVertex);

		m_vertex = (zglVertex*) malloc(buffer_size);
		memset(m_vertex, 0, buffer_size);
	}

	m_particles = particles;
	m_vertex_num = particleNum;
	m_texture = texture;

	m_world = 1.0f;

	if (m_aliveCount == 0 || m_aliveCount > particleNum)
	{
		m_aliveCount = particleNum;
	}

	//Add to the node list
	queue();
}

void zglPrimParticle::setAliveCount(int count)
{
	m_aliveCount = count;
}

void zglPrimParticle::setTexCoord(float left, float top, float right,
		float bottom)
{
	this->u1 = left;
	this->v1 = top;
	this->u2 = right;
	this->v2 = bottom;
}

void zglPrimParticle::render()
{
	zglCore::setModelView(zglEngine::getEngine()->m_world_to_view);

	zglVector vec_w, vec_h;

	zglParticleNode* particle;

	float w, h;
	float angle;
	float cos_a, sin_a;
	unsigned vert_index = 0;

	for (unsigned int i = 0; i < m_aliveCount; i++)
	{
		particle = m_particles + i;
		if (particle->a == 0)
		{
			continue;
		}

		w = particle->width * 0.5f * particle->scale;
		h = particle->height * 0.5f * particle->scale;
		angle = particle->angle;

		if (angle == 0.0f)
		{
			m_vertex[vert_index + 0].x = particle->x - w;
			m_vertex[vert_index + 0].y = particle->y + h;
			m_vertex[vert_index + 0].z = particle->z;

			m_vertex[vert_index + 1].x = particle->x + w;
			m_vertex[vert_index + 1].y = particle->y + h;
			m_vertex[vert_index + 1].z = particle->z;

			m_vertex[vert_index + 2].x = particle->x + w;
			m_vertex[vert_index + 2].y = particle->y - h;
			m_vertex[vert_index + 2].z = particle->z;

			m_vertex[vert_index + 3].x = particle->x - w;
			m_vertex[vert_index + 3].y = particle->y - h;
			m_vertex[vert_index + 3].z = particle->z;
		}
		else
		{
			cos_a = cos(angle);
			sin_a = sin(angle);

			m_vertex[vert_index + 0].x = particle->x - w * cos_a - h * sin_a;
			m_vertex[vert_index + 0].y = particle->y - w * sin_a + h * cos_a;
			m_vertex[vert_index + 0].z = particle->z;

			m_vertex[vert_index + 1].x = particle->x + w * cos_a - h * sin_a;
			m_vertex[vert_index + 1].y = particle->y + w * sin_a + h * cos_a;
			m_vertex[vert_index + 1].z = particle->z;

			m_vertex[vert_index + 2].x = particle->x + w * cos_a + h * sin_a;
			m_vertex[vert_index + 2].y = particle->y + w * sin_a - h * cos_a;
			m_vertex[vert_index + 2].z = particle->z;

			m_vertex[vert_index + 3].x = particle->x - w * cos_a + h * sin_a;
			m_vertex[vert_index + 3].y = particle->y - w * sin_a - h * cos_a;
			m_vertex[vert_index + 3].z = particle->z;
		}

		//!< texture coordinate
		if (m_texture)
		{
			m_vertex[vert_index + 0].setUV(this->u1, this->v1);
			m_vertex[vert_index + 1].setUV(this->u2, this->v1);
			m_vertex[vert_index + 3].setUV(this->u1, this->v2);
			m_vertex[vert_index + 2].setUV(this->u2, this->v2);
		}

		//!< Color, Alpha
		m_vertex[vert_index + 0].r = particle->r;
		m_vertex[vert_index + 0].g = particle->g;
		m_vertex[vert_index + 0].b = particle->b;
		m_vertex[vert_index + 0].a = particle->a;

		m_vertex[vert_index + 1].r = particle->r;
		m_vertex[vert_index + 1].g = particle->g;
		m_vertex[vert_index + 1].b = particle->b;
		m_vertex[vert_index + 1].a = particle->a;

		m_vertex[vert_index + 2].r = particle->r;
		m_vertex[vert_index + 2].g = particle->g;
		m_vertex[vert_index + 2].b = particle->b;
		m_vertex[vert_index + 2].a = particle->a;

		m_vertex[vert_index + 3].r = particle->r;
		m_vertex[vert_index + 3].g = particle->g;
		m_vertex[vert_index + 3].b = particle->b;
		m_vertex[vert_index + 3].a = particle->a;

		vert_index += 4;
	}

	if (m_texture)
	{
		zglCore::setTexture(m_texture->getTexID(),
				(m_flag & FLAG_TEX_POINT) ? false : true);
		zglCore::setTexCoordPointer2((float*) &m_vertex->u, sizeof(zglVertex));
	}
	else
	{
		zglCore::setTexture(0, false);
	}

	zglCore::setBlendType((zglBlendType) m_blend_type);
	zglCore::setVertexPointer3((float*) &m_vertex->x, sizeof(zglVertex));
	zglCore::setColorPointer4(&m_vertex->r, sizeof(zglVertex));

	zglCore::drawArrays(DRAW_QUADS, 0, vert_index);
}

#if 0
//!< The API will not be invoked in current version.
void zglPrimParticle::calcSortDepth()
{
	int i;
	float rel_z;

	float tx = 0.0f;
	float ty = 0.0f;
	float tz = 0.0f;

	for (i = 0; i < m_vertex_num; i++)
	{
		tx += (m_particles + i)->x;
		ty += (m_particles + i)->y;
		tz += (m_particles + i)->z;
	}

	tx = tx / m_vertex_num;
	ty = ty / m_vertex_num;
	tz = tz / m_vertex_num;

	zglVector pos(tx, ty, tz);

	int sort_value;
	zglEngine * glEngine = zglEngine::getEngine();

	rel_z = pos.RelZ(glEngine->m_view);

	if (rel_z < glEngine->m_near_dist)
	{
		sort_value = 0x00;
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
	if (sort_value < 0x00)
	{
		sort_value = 0x00;
	}

	m_sort_value = sort_value;
}
#endif

//!< --------------------------------------------------------------------------
void zglParticleNode::setPosition(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void zglParticleNode::setColor(unsigned int color)
{
	this->r = (unsigned char) ((color >> 24) & 0xff);
	this->g = (unsigned char) ((color >> 16) & 0xff);
	this->b = (unsigned char) ((color >> 8) & 0xff);
	this->a = (unsigned char) ((color) & 0xff);
}

void zglParticleNode::setDimension(float width, float height, float angle)
{
	this->width = width;
	this->height = height;
	this->angle = angle;
}
