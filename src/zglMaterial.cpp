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

#include "zglCore.h"
#include "zglMaterial.h"
#include "zglBasicType.h"

zglMaterial::zglMaterial()
{
	m_ambientColor[0] = 0.2f;
	m_ambientColor[1] = 0.2f;
	m_ambientColor[2] = 0.2f;
	m_ambientColor[3] = 1.0f;

	m_diffuseColor[0] = 0.8f;
	m_diffuseColor[1] = 0.8f;
	m_diffuseColor[2] = 0.8f;
	m_diffuseColor[3] = 1.0f;

	m_specularColor[0] = 0.0f;
	m_specularColor[1] = 0.0f;
	m_specularColor[2] = 0.0f;
	m_specularColor[3] = 1.0f;

	m_emissionColor[0] = 0.0f;
	m_emissionColor[1] = 0.0f;
	m_emissionColor[2] = 0.0f;
	m_emissionColor[3] = 1.0f;

	m_shiness = 0.0f;
}

void zglMaterial::applyMaterial()
{
	zglCore::setMaterial(m_ambientColor,
			m_diffuseColor,
			m_specularColor,
			m_emissionColor,
			m_shiness);
}

void zglMaterial::setAmbientColor(float r, float g, float b, float a)
{
	m_ambientColor[0] = r;
	m_ambientColor[1] = g;
	m_ambientColor[2] = b;
	m_ambientColor[3] = a;
}

void zglMaterial::setDiffuseColor(float r, float g, float b, float a)
{
	m_diffuseColor[0] = r;
	m_diffuseColor[1] = g;
	m_diffuseColor[2] = b;
	m_diffuseColor[3] = a;
}

void zglMaterial::setSpecularColor(float r, float g, float b, float a)
{
	m_specularColor[0] = r;
	m_specularColor[1] = g;
	m_specularColor[2] = b;
	m_specularColor[3] = a;
}

void zglMaterial::setEmissionColor(float r, float g, float b, float a)
{
	m_emissionColor[0] = r;
	m_emissionColor[1] = g;
	m_emissionColor[2] = b;
	m_emissionColor[3] = a;
}

void zglMaterial::setShininess(float shininess)
{
	if(shininess > 0.0f && shininess<= 128.0f)
	{
		m_shiness = shininess;
	}
}

void zglMaterial::setMaterialType(int type)
{
	switch(type)
	{
	default:

	case MATERIAL_NONE:
	case MATERIAL_DEFAULT:
		break;
	case 	MATERIAL_BRASS:
		break;
	case 	MATERIAL_BRONZE:
		break;
	case 	MATERIAL_POLISHED_BRONZE:
		break;
	case 	MATERIAL_CHROME:
		break;
	case 	MATERIAL_COPPER:
		break;
	case 	MATERIAL_POLISHED_COPPER:
		break;
	case 	MATERIAL_GOLD:
		break;
	case 	MATERIAL_POLISHED_GOLD:
	case 	MATERIAL_PEWTER:
	case 	MATERIAL_SILVER:
	case 	MATERIAL_POLISHED_SILVER:
	case 	MATERIAL_EMERALD:
	case 	MATERIAL_JADE:
	case 	MATERIAL_OBSIDIAN:
	case 	MATERIAL_PEARL:
	case 	MATERIAL_RUBY:
	case 	MATERIAL_TURQUOISE:
	case 	MATERIAL_BLACK_PLASTIC:
	case 	MATERIAL_BLACK_RUBBER:
		break;
	}
}

