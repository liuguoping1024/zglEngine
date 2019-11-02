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


#ifndef __zgl_texture_h__
#define __zgl_texture_h__

#include "zglImage.h"

/**
 * @brief A simple texture class.
 *
 * @note keep it simple to make it easy to use on Android JNI plz.
 */
class zglTexture
{

public:
	zglTexture()
	{
		m_width = -1;
		m_height = -1;
		m_tex_id = 0;

		m_normal_width = 1.0f;
		m_normal_height = 1.0f;
	}

	virtual ~zglTexture();

	/**
	 * @brief get the idea width of the texture.
	 *
	 * As for a texture, the accepted width and height of the original image
	 * should be 2^n(64, 128,256, ...). So for example, as to a image of 200x200,
	 * the image should be extend to 256x256 to make itself accepted by GL,
	 * even the image was stretched or filled, the idea width and height was
	 * thought as 200.
	 *
	 * In current implementation, the image was filled with blanket pixels.
	 * so the texture coordinate should not be 1.0f, a new float value,a new
	 * texture coordinate should be supplied to make the development make full
	 * use of the texture. Here the float values are called #m_normal_width
	 * and #m_normal_height
	 */
	inline int getWidth() const
	{
		return (int) m_width;
	}

	/**
	 * @brief get the idea height of the texture.
	 *
	 * @see #getWidth
	 */
	inline int getHeight() const
	{
		return (int) m_height;
	}

	/**
	 * @brief get the ideal texture coordinate of the texture.
	 * @deprecated
	 */
	inline float getNormalWidth() const
	{
		return (float) m_normal_width;
	}

	/**
	 * @brief get the ideal texture coordinate of the texture.
	 */
	inline float getU() const
	{
		return (float) m_normal_width;
	}


	/**
	 * @brief get the ideal texture coordinate of the texture.
	 * @deprecated
	 */
	inline float getNormalHeight() const
	{
		return (float) m_normal_height;
	}

	/**
	 * @brief get the ideal texture coordinate of the texture.
	 */
	inline float getV() const
	{
		return (float) m_normal_height;
	}

	/**
	 * @brief get the Texture ID of the texture.
	 */
	inline unsigned int getTexID() const
	{
		return m_tex_id;
	}

	/**
	 * @brief set ideal size of the texture.
	 */
	inline void setDimension(int width, int height)
	{
		m_width = (unsigned short) width;
		m_height = (unsigned short) height;
	}

	/**
	 * @brief set idea texture coordinate of the texture.
	 * @deprecated
	 */
	inline void setNormalDimension(float nwidth, float nheight)
	{
		m_normal_width = nwidth;
		m_normal_height = nheight;
	}

	/**
	 * @brief set idea texture coordinate of the texture.
	 */
	inline void setUV(float u, float v)
	{
		m_normal_width = u;
		m_normal_height = v;
	}

	/**
	 * @brief set the Texture ID of the texture.
	 */
	inline void setTexID(unsigned int texID)
	{
		m_tex_id = texID;
	}

	/**
	 * @brief free the texture.
	 *
	 * please refer to <i> glDeleteTexture</i>
	 */
	void clearTexture();

protected:
	unsigned short m_width; //!< idea width of the texture
	unsigned short m_height;//!< idea height of the texture
	unsigned int m_tex_id;//!< Texture id, return by <i>glGenTextures</i>

	float m_normal_width; //!< [0.0f -> 1.0f]
	float m_normal_height; //!< [0.0f -> 1.0f]

};


#endif

