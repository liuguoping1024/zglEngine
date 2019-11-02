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


#ifndef __zgl_light_h__
#define __zgl_light_h__

#include "zglVector.h"

class zglLight;

/**
 * @brief information of a single light source.
 */
class zglLightInfo
{
public:
	zglLightInfo();

	/**
	 * @brief Ambient color.
	 * @param r r color, [0.0, 1.0].
	 * @param g g color, [0.0, 1.0].
	 * @param b b color, [0.0, 1.0].
	 * @param a a color, [0.0, 1.0].
	 */
	void setAmbientColor(float r, float g, float b, float a);

	/**
	 * @brief Diffuse color.
	 * @param r r color, [0.0, 1.0].
	 * @param g g color, [0.0, 1.0].
	 * @param b b color, [0.0, 1.0].
	 * @param a a color, [0.0, 1.0].
	 */
	void setDiffuseColor(float r, float g, float b, float a);

	/**
	 * @brief Specular color.
	 * @param r r color, [0.0, 1.0].
	 * @param g g color, [0.0, 1.0].
	 * @param b b color, [0.0, 1.0].
	 * @param a a color, [0.0, 1.0].
	 */
	void setSpecularColor(float r, float g, float b, float a);

	/**
	 * @brief Set the position of the light source.
	 * @param x x-coordinate.
	 * @param y y-coordinate.
	 * @param z z-coordinate.
	 */
	void setPosition(float x, float y, float z);

	/**
	 * @brief Enable the light source.
	 */
	void enable(bool enable);

private:
	void applyLight();

	void setIndex(unsigned short index);
private:

	float m_ambientColor[4];
	float m_diffuseColor[4];
	float m_specularColor[4];
	float m_position[4];

	unsigned short m_lightIndex;
	bool m_enabled;

	zglLight * m_light; //!< parent

	friend class zglLight;
};

/**
 * @brief Light system of the graphics system.
 *
 * As to the normal OpenGL GPU, 8 lights will be supported. #zglLight manages
 * the whole 8 lights.
 *
 * TODO The remain feature of the light.
 * If more light supported by the GPU, just update this file.
 */
class zglLight
{

public:
	zglLight();
	virtual ~zglLight();

	/**
	 * @brief Enable the light function.
	 *
	 * If the light function is not enabled, there will be no light
	 * even all the light source enabled.
	 */
	void enableLight(bool enable);

	/**
	 * @brief get the appointed light.
	 *
	 * @param index the light index, the range is [0, 7] due to the hardware.
	 * @return the light information.
	 */
	zglLightInfo * getLight(unsigned int index);

	/**
	 * @brief The scene ambient light color.
	 */
	void setSceneAmbientColor(float r, float g, float b, float a);

	/**
	 * @brief Two side mode.
	 * @see <i>GL_LIGHT_MODE_TWO_SIDE</i>
	 */
	void enableTwoSideMode(bool enable);

	/**
	 * @brief notify interface for notifying when the light source changed.
	 */
	inline void update() { m_updated = true; }

private:
	void appLighting();

private:
	static const unsigned int m_max_light = 8;

	zglLightInfo m_light[m_max_light];
	float m_sceneAmbientColor[4];
	bool m_twoSideLighting; //!< Two Side Mode

	bool m_enabled; //!< All light enable flag
	bool m_updated; //!< Parameter update needed flag.

	friend class zglEngine;
};

/**
 * @brief Fog system of the graphics system.
 */
class zglFog
{
public:
	zglFog();

	/**
	 * @brief apply fog to the whole system.
	 *
	 * Fog is a global setting.
	 */
	void applyFog();

	/**
	 * @brief set the mode of the fog.
	 * @param mode zglFogMode
	 * @see zglFogMode
	 */
	void setMode(int mode);

	/**
	 * @brief set fog parameters.
	 *
	 * @param density fog density
	 * @param start fog start position.
	 * @param end fog end position.
	 */
	void setFog(float density, float start, float end);

	/**
	 * @brief set color of the fog
	 * @param r fog r color, [0.0, 1.0].
	 * @param g fog g color, [0.0, 1.0].
	 * @param b fog b color, [0.0, 1.0].
	 * @param a fog a color, [0.0, 1.0].
	 */
	void setColor(float r, float g, float b, float a);

	/**
	 * @brief enable or disable the fog.
	 */
	void enable(bool enable);

private:
	int m_mode;
	float m_start;
	float m_end;
	float m_density;

	float m_color[4];

	bool m_enabled;
	bool m_updated;

	friend class zglEngine;
};


#endif
