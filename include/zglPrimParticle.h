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

#ifndef __zgl_sprite_h__
#define __zgl_sprite_h__


/**
 * @brief Graphics information of the Particle primitive node.
 */
struct zglParticleNode
{
	float x;//!< x position
	float y;//!< y position
	float z;//!< z position
	unsigned char r; //!< r color
	unsigned char g; //!< g color
	unsigned char b; //!< b color
	unsigned char a; //!< a color
	float width; //!< original size
	float height; //!< original size
	float angle; //!< angle z-axes
	float scale; //!< size scale factor

	/**
	 * @brief Set the position of the particle node.
	 */
	void setPosition(float x, float y, float z);

	/**
	 * @brief Set the color of the particle node.
	 */
	void setColor(unsigned int color);

	/**
	 * @brief Set the dimension of the particle.
	 */
	void setDimension(float width, float height, float angle);
};

/**
 * @brief Primitive particle.
 * Typical usage of one primitive to show multi-objects.
 *
 * Similar objects, called particle, are displayed by one primitive.
 */
class zglPrimParticle: public zglPrim
{
public:
	zglPrimParticle();

	virtual ~zglPrimParticle();

	/**
	 * @brief create a new particle.
	 *
	 * @param particles particle graphics related informations.
	 * @param num particles' count.
	 * @param texture texture of the particles.
	 */
	void init(zglParticleNode* particles, unsigned int num, zglTexture * texture);

	/**
	 * @brief set the alive count of the particles.
	 */
	void setAliveCount(int count);

	/**
	 * @brief Set the texture coordinate to zglPrimParticle.
	 *
	 * The texture coordinate will be applied to all the particles.
	 * @param left the left coordinate.
	 * @param top the top coordinate.
	 * @param right the right coordinate.
	 * @param bottom the bottom coordinate.
	 */
	void setTexCoord(float left, float top, float right, float bottom);

protected:
	/**
	 * @brief Render the 3D objects to the screen.
	 */
	virtual void render();

#if 0
	/**
	 * @brief Calculate the z depth, and put into the sort list.
	 */
	virtual void calcSortDepth();
#endif

private:
	float u1, v1, u2, v2;

	zglParticleNode * m_particles;
	unsigned int m_aliveCount;
};

#endif

