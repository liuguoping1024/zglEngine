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
#include <math.h>

#include "zglBasicType.h"

#include "zglWidgetParticle.h"

#include "zglUtil.h"

zglWidgetParticle::zglWidgetParticle()
{
	m_type = TYPE_PARTICLE;

	m_particleInfo = NULL;
	m_particleNodeInfo = NULL;

	m_particleCount = 0;
	m_aliveCount = 0;

	m_lastTick = 0l;

	m_width = m_height = 32.0f;

	m_particalLifeMin = 500; //!< 500 ms
	m_particalLifeMax = 1500;//!< 3500 ms

	m_direction = 6.283185f * 0.25;
	m_spread = 6.283185f * 0.25;

	m_velocityMin = 284.0f;
	m_velocityMax = 184.0f;

	m_gravityMin = 130.571411f;
	m_gravityMax = 142.857178f;

	m_radialAccelMin = -142.857117f;
	m_radialAccelMax = -142.857117f;

	m_tangentialAccelMin = -0.793652f;
	m_tangentialAccelMax = -0.793652f;

	m_sizeEnd = 0.81250f;
	m_sizeStart = 1.529018f;

	m_spinStart = 10.317459f;
	m_spinEnd = 10.317459;

	m_colorRStart = 1.000000f;
	m_colorGStart = 0.738095f;
	m_colorBStart = 0.000000f;
	m_colorAStart = 0.904762f;

	m_colorREnd = 0.571429f;
	m_colorGEnd = 0.000000f;
	m_colorBEnd = 0.246032f;
	m_colorAEnd = 0.055556f;

	//!< prim link
	m_prim_ref = &m_particlePrim;
}

zglWidgetParticle::~zglWidgetParticle()
{
	if (m_particleInfo != NULL)
	{
		free( m_particleInfo);
	}
	if (m_particleNodeInfo != NULL)
	{
		free( m_particleNodeInfo);
	}
}

void zglWidgetParticle::init(int totalCount, int initCount, zglTexture * tex,
		float width, float height)
{
	//!< Do not put the particle into widget list.
	/** zglWidget::init(); **/

	if (totalCount != m_particleCount)
	{
		if (m_particleInfo != NULL)
			free( m_particleInfo);
		if (m_particleNodeInfo != NULL)
			free( m_particleNodeInfo);
	}

	m_particleCount = totalCount;
	m_aliveCount = 0;

	m_particleInfo = (zglParticleInfo*) malloc(totalCount
			* sizeof(zglParticleInfo));
	memset(m_particleInfo, 0, totalCount * sizeof(zglParticleInfo));

	m_particleNodeInfo = (zglParticleNode *) malloc(totalCount * sizeof(zglParticleNode));
	memset(m_particleNodeInfo, 0, totalCount * sizeof(zglParticleNode));

	m_particlePrim.init(m_particleNodeInfo, totalCount, (zglTexture *) tex);
	m_particlePrim.setBlendType(BLEND_HALF);
	m_particlePrim.setAliveCount(m_aliveCount);

	m_width = width;
	m_height = height;
}

void zglWidgetParticle::setTexture(zglTexture * tex)
{
	m_particlePrim.setTexture(tex);
}

void zglWidgetParticle::setTexCoord(float left, float top, float right,
		float bottom)
{
	m_particlePrim.setTexCoord(left, top, right, bottom);
}

void zglWidgetParticle::setVisible(bool visible)
{
	zglWidget::setVisible(visible);
	m_particlePrim.setVisible(visible);
}

bool zglWidgetParticle::update(long time)
{
	int i;
	if (m_lastTick == 0)//!< First frame
	{
		m_lastTick = time;
	}
	else
	{
		float durTime = (time - m_lastTick) / 1000.0f;

		zglParticleInfo * parInfo = m_particleInfo;
		zglParticleNode * par = m_particleNodeInfo;

		float angle;
		zglVector vecAccel1, vecAccel2;

		for (i = 0; i < m_aliveCount; i++)
		{
			parInfo->m_age += (time - m_lastTick);
			if (parInfo->m_age >= parInfo->m_ageMax)
			{
				m_aliveCount--;
				memcpy(parInfo, &m_particleInfo[m_aliveCount], sizeof(zglParticleInfo));
				memcpy(par, &m_particleNodeInfo[m_aliveCount], sizeof(zglParticleNode));
				i--;

				continue;
			}

			vecAccel1.x = par->x - m_x;
			vecAccel1.y = par->y - m_y;
			vecAccel1.z = par->z - m_z;

			vecAccel1.normalize();
			vecAccel2 = vecAccel1;
			vecAccel1 *= parInfo->m_radialAccel;

			angle = vecAccel2.x;
			vecAccel2.x = -vecAccel2.y;
			vecAccel2.y = angle;

			vecAccel2 *= parInfo->m_tangentialAccel;
			parInfo->m_velocity += (vecAccel1 + vecAccel2) * durTime;
			parInfo->m_velocity.y -= parInfo->m_gravity * durTime;

			//Position
			par->x += parInfo->m_velocity.x * durTime;
			par->y += parInfo->m_velocity.y * durTime;
			par->z += parInfo->m_velocity.z * durTime;

			parInfo->m_colorR += parInfo->m_colorRDelta * durTime;
			parInfo->m_colorG += parInfo->m_colorGDelta * durTime;
			parInfo->m_colorB += parInfo->m_colorBDelta * durTime;
			parInfo->m_colorA += parInfo->m_colorADelta * durTime;

			unsigned int r, g, b, a;
			r = (unsigned int) (parInfo->m_colorR * 0xff);
			g = (unsigned int) (parInfo->m_colorG * 0xff);
			b = (unsigned int) (parInfo->m_colorB * 0xff);
			a = (unsigned int) (parInfo->m_colorA * 0xff);

			if (r > 0xff)
				r = 0xff;
			if (g > 0xff)
				g = 0xff;
			if (b > 0xff)
				b = 0xff;
			if (a > 0xff)
				a = 0xff;

			par->r = r & 0xff;
			par->g = g & 0xff;
			par->b = b & 0xff;
			par->a = a & 0xff;

			//Angle and scale
			par->angle += parInfo->m_spinDelta * durTime;
			par->scale += parInfo->m_sizeDelta * durTime;

			parInfo++;
			par++;
		}

		if (m_aliveCount < m_particleCount)
		{
			int new_pos = m_aliveCount >> 1;
			parInfo = &m_particleInfo[new_pos];
			par = &m_particleNodeInfo[new_pos];

			memcpy(&m_particleInfo[m_aliveCount], parInfo, sizeof(zglParticleInfo));
			memcpy(&m_particleNodeInfo[m_aliveCount], par, sizeof(zglParticleNode));

			parInfo->m_age = 0;
			parInfo->m_ageMax = zglRandom(m_particalLifeMin, m_particalLifeMax);//

			angle = m_direction - ZGL_2PI + zglRandom(0.0f, m_spread, 0.001f)
					- m_spread / 2.0f;

			parInfo->m_velocity.x = cos(angle);
			parInfo->m_velocity.y = sin(angle);
			parInfo->m_velocity.z = 0.0f;

			parInfo->m_velocity *= zglRandom(m_velocityMin, m_velocityMax);

			if (m_gravityMin != m_gravityMax)
			{
				parInfo->m_gravity = zglRandom(m_gravityMin, m_gravityMax, 0.0010f);
			}
			else
			{
				parInfo->m_gravity = m_gravityMin;
			}

			if (m_radialAccelMin != m_radialAccelMax)
			{
				parInfo->m_radialAccel = zglRandom(m_radialAccelMin,
						m_radialAccelMax, 0.0010f);
			}
			else
			{
				parInfo->m_radialAccel = m_radialAccelMin;
			}

			if (m_tangentialAccelMin != m_tangentialAccelMax)
			{
				parInfo->m_tangentialAccel = zglRandom(m_tangentialAccelMin,
						m_tangentialAccelMax, 0.0010f);
			}
			else
			{
				parInfo->m_tangentialAccel = m_tangentialAccelMin;
			}

			par->x = m_x + zglRandom(-2.0f, 2.0f, 0.01f);
			par->y = m_y + zglRandom(-2.0f, 2.0f, 0.01f);
			par->z = m_z;

#if 0
			parInfo->m_colorR = m_colorRStart;
			parInfo->m_colorG = m_colorGStart;
			parInfo->m_colorB = m_colorBStart;
			parInfo->m_colorA = m_colorAStart;
#else
			parInfo->m_colorR = zglRandom(m_colorRStart, m_colorREnd, 0.001f);
			parInfo->m_colorG = zglRandom(m_colorGStart, m_colorGEnd, 0.001f);
			parInfo->m_colorB = zglRandom(m_colorBStart, m_colorBEnd, 0.001f);
			parInfo->m_colorA = zglRandom(m_colorAStart, m_colorAEnd, 0.001f);
#endif
			parInfo->m_colorRDelta = (m_colorREnd - parInfo->m_colorR)
					* 1000.0f / (float) parInfo->m_ageMax;
			parInfo->m_colorGDelta = (m_colorGEnd - parInfo->m_colorG)
					* 1000.0f / (float) parInfo->m_ageMax;
			parInfo->m_colorBDelta = (m_colorBEnd - parInfo->m_colorB)
					* 1000.0f / (float) parInfo->m_ageMax;
			parInfo->m_colorADelta = (m_colorAEnd - parInfo->m_colorA)
					* 1000.0f / (float) parInfo->m_ageMax;

			par->r = (unsigned char) (parInfo->m_colorR * 0xff);
			par->g = (unsigned char) (parInfo->m_colorG * 0xff);
			par->b = (unsigned char) (parInfo->m_colorB * 0xff);
			par->a = (unsigned char) (parInfo->m_colorA * 0xff);

			par->height = m_height;
			par->width = m_width;

			par->angle = zglRandom(m_spinStart, m_spinEnd, 0.01f);
			parInfo->m_spinDelta = (m_spinEnd - par->angle) * 1000.0f
					/ parInfo->m_ageMax;

			par->scale = zglRandom(m_sizeStart, m_sizeEnd, 0.01f);
			parInfo->m_sizeDelta = (m_sizeEnd - par->scale) * 1000.0f
					/ parInfo->m_ageMax;

			m_aliveCount++;
		}

		m_lastTick = time;
		m_particlePrim.setAliveCount(m_aliveCount);
	}

	return false;
}

bool zglWidgetParticle::onUpdate(long time)
{
	//!< Do nothing
	return false;
}



