/**
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

#ifndef __zgl_material_h__
#define __zgl_material_h__

/**
 * @brief Material of the graphics system.
 */
class zglMaterial
{
public:
	zglMaterial();

	/**
	 * @brief Ambient color.
	 * @param r r color, [0.0, 1.0]
	 * @param g g color, [0.0, 1.0]
	 * @param b b color, [0.0, 1.0]
	 * @param a a color, [0.0, 1.0]
	 */
	void setAmbientColor(float r, float g, float b, float a);

	/**
	 * @brief Diffuse color.
	 * @param r r color, [0.0, 1.0]
	 * @param g g color, [0.0, 1.0]
	 * @param b b color, [0.0, 1.0]
	 * @param a a color, [0.0, 1.0]
	 */
	void setDiffuseColor(float r, float g, float b, float a);

	/**
	 * @brief Specular color.
	 * @param r r color, [0.0, 1.0]
	 * @param g g color, [0.0, 1.0]
	 * @param b b color, [0.0, 1.0]
	 * @param a a color, [0.0, 1.0]
	 */
	void setSpecularColor(float r, float g, float b, float a);

	/**
	 * @brief Emission color
	 * @param r r color, [0.0, 1.0]
	 * @param g g color, [0.0, 1.0]
	 * @param b b color, [0.0, 1.0]
	 * @param a a color, [0.0, 1.0]
	 */
	void setEmissionColor(float r, float g, float b, float a);

	/**
	 * @brief Shininess
	 * @param shininess [0.0, 128.0]
	 */
	void setShininess(float shininess);

public:
	/**
	 * @brief Apply material to whole system.
	 *
	 * Material is a Global setting, which will be applied to current primitives.
	 */
	void applyMaterial();

private:
	void setMaterialType(int type);

private:

	float m_ambientColor[4]; //!< default(0.2, 0.2, 0.2, 1.0)
	float m_diffuseColor[4]; //!< default(0.8, 0.8, 0.8, 1.0)
	float m_specularColor[4]; //!< default(0.0, 0.0, 0.0, 1.0)
	float m_emissionColor[4]; //!< default(0.0, 0.0, 0.0, 1.0)

	float m_shiness;//!< [0, 128]

	friend class zglEngine;
};

#endif
