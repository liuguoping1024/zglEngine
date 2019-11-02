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
#include "zglLight.h"

zglLight::zglLight()
{
	m_enabled = false;
	m_updated = true;

	for(unsigned short i=0; i<m_max_light; i++)
	{
		m_light[i].setIndex(i);
		m_light[i].m_enabled = false;
		m_light[i].m_light = this;
	}

	m_sceneAmbientColor[0] = 0.2f;
	m_sceneAmbientColor[1] = 0.2f;
	m_sceneAmbientColor[2] = 0.2f;
	m_sceneAmbientColor[3] = 0.2f;

	m_twoSideLighting = false;
}

zglLight::~zglLight()
{
}

void zglLight::enableLight(bool enable)
{
	m_enabled = enable;
	m_updated = true;
}

zglLightInfo * zglLight::getLight(unsigned int index)
{
	if(index >= m_max_light)
	{
		return NULL;
	}

	return &m_light[index];
}

void zglLight::setSceneAmbientColor(float r, float g, float b, float a)
{
	m_sceneAmbientColor[0] = r;
	m_sceneAmbientColor[1] = g;
	m_sceneAmbientColor[2] = b;
	m_sceneAmbientColor[3] = a;

	m_updated = true;
}

void zglLight::enableTwoSideMode(bool enable)
{
	if(enable != m_twoSideLighting)
	{
		m_twoSideLighting = enable;
		m_updated = true;
	}
}

void zglLight::appLighting()
{
	//TODO SceneAmbientColor & TwoSideLighting
	if(m_enabled)
	{
		zglCore::setLight(true);
	}
	else
	{
		zglCore::setLight(false);
	}

	for(unsigned int i=0; i<m_max_light; i++)
	{
		m_light[i].applyLight();
	}

	m_updated = false;
}

//!< --------------------------------------------------------------------------

zglLightInfo::zglLightInfo()
{
	m_ambientColor[0] = 0.0f;
	m_ambientColor[1] = 0.0f;
	m_ambientColor[2] = 0.0f;
	m_ambientColor[3] = 1.0f;

	m_position[0] = 0.0f;
	m_position[1] = 0.0f;
	m_position[2] = -1.0f;
	m_position[3] = 1.0f;

	m_enabled = false;
	m_light = NULL;

	m_lightIndex = 0xffff;
}

void zglLightInfo::applyLight()
{
	if (m_lightIndex == 0xffff)
		return;

	if (m_enabled)
	{
		zglCore::setLightParam(m_lightIndex, m_ambientColor, m_diffuseColor,
				m_specularColor, m_position);
	}
	else
	{
		zglCore::unSetLightParam( m_lightIndex);
	}
}

void zglLightInfo::setAmbientColor(float r, float g, float b, float a)
{
	m_ambientColor[0] = r;
	m_ambientColor[1] = g;
	m_ambientColor[2] = b;
	m_ambientColor[3] = a;

	m_light->update();
}

void zglLightInfo::setDiffuseColor(float r, float g, float b, float a)
{
	m_diffuseColor[0] = r;
	m_diffuseColor[1] = g;
	m_diffuseColor[2] = b;
	m_diffuseColor[3] = a;

	m_light->update();
}

void zglLightInfo::setSpecularColor(float r, float g, float b, float a)
{
	m_specularColor[0] = r;
	m_specularColor[1] = g;
	m_specularColor[2] = b;
	m_specularColor[3] = a;

	m_light->update();
}

void zglLightInfo::setPosition(float x, float y, float z)
{
	m_position[0] = x;
	m_position[1] = y;
	m_position[2] = z;
	m_position[3] = 1.0f;//!< Zero means the point at infinity

	m_light->update();
}

void zglLightInfo::enable(bool enable)
{
	m_enabled = enable;
	m_light->update();
}

void zglLightInfo::setIndex(unsigned short index)
{
	if (index == 0)
	{
		//!< refer to spec 1.1.12 P.39, This is the default value of Light0
		m_diffuseColor[0] = 1.0f;
		m_diffuseColor[1] = 1.0f;
		m_diffuseColor[2] = 1.0f;
		m_diffuseColor[3] = 1.0f;

		m_specularColor[0] = 1.0f;
		m_specularColor[1] = 1.0f;
		m_specularColor[2] = 1.0f;
		m_specularColor[3] = 1.0f;
	}
	else
	{
		//!< refer to spec 1.1.12 P.39, This is the default value of Lighti(i>0)
		m_diffuseColor[0] = 0.0f;
		m_diffuseColor[1] = 0.0f;
		m_diffuseColor[2] = 0.0f;
		m_diffuseColor[3] = 1.0f;

		m_specularColor[0] = 0.0f;
		m_specularColor[1] = 0.0f;
		m_specularColor[2] = 0.0f;
		m_specularColor[3] = 1.0f;
	}

	m_lightIndex = index;
}

//!< --------------------------------------------------------------------------

zglFog::zglFog()
{
	m_mode = FOG_LINEAR;

	m_start = 0.0f;
	m_end = 1.0f;

	m_density = 1.0f;

	m_color[0] = 0.0f;
	m_color[1] = 0.0f;
	m_color[2] = 0.0f;
	m_color[3] = 0.0f;

	m_enabled = false;
	m_updated = true;
}

void zglFog::applyFog()
{
	m_updated = false;

	if (m_enabled)
	{
		zglCore::setFogParam(m_mode, m_color, m_density, m_start, m_end);
	}
	else
	{
		zglCore::unSetFogParam();
	}
}

void zglFog::setMode(int mode)
{
	switch(mode)
	{
	default:
	case FOG_LINEAR:
		this->m_mode = FOG_LINEAR;
		break;

	case FOG_EXP:
	case FOG_EXP2:
		this->m_mode = mode;
		break;
	}

	m_updated = true;
}

void zglFog::setFog(float density, float start, float end)
{
	this->m_start = start;
	this->m_end = end;
	this->m_density = density;

	m_updated = true;
}

void zglFog::setColor(float r, float g, float b, float a)
{
	m_color[0] = r;
	m_color[1] = g;
	m_color[2] = b;
	m_color[3] = a;

	m_updated = true;
}

void zglFog::enable(bool enable)
{
	m_enabled = enable;
	m_updated = true;
}








