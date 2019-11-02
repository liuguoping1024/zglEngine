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


#ifndef __zgl_util_h__
#define __zgl_util_h__

#include "zglImage.h"
#include "zglTexture.h"
#include "zglVertex.h"
#include "zglMatrix.h"

#include <time.h>

// ----------------------------------------------------------------------------

/**
 * @brief Convert the degree to radian.
 *
 * map 0~180 degree to 0 ~ 3.14159
 * @param deg degree, 0~~360, or other degrees
 */
float zglDegToRad(float deg);

/**
 * @brief Convert the radian to degree
 *
 * map 0 ~ 3.14159 to 0~180 degree
 * @param radian value.
 */
float zglRadToDeg(float rad);

float _zsin(float rad);

float _zcos(float rad);

// ----------------------------------------------------------------------------

bool zglIsPowerOf2(int n);

int zglNextPowerOf2(int n);

int zglPrevPowerOf2(int n);

// ----------------------------------------------------------------------------

void zglInitRandom(unsigned int seed);

int zglRandom(int min, int max);

float zglRandom(float min, float max, float step);

// ----------------------------------------------------------------------------
/**
 * @brief Bind the image with a texture.
 */
bool zglBindTexture(zglTexture * tex, zglImage * image);


/**
 * @brief Bind the image with a buffer.
 *
 * @param width The width unit of the buffer in pixel.
 * @param height The height unit of the buffer in pixel.
 * @param pixel_format The Format of the buffer.
 * @param pixel The Buffer.
 *
 * @note The width and height should be qualified dimension, such as 64, 128, etc.
 * If the texture coordinate need to be updated after binding, The interface
 * #setNormalDimension() should be called.
 *
 * @deprecated
 */
bool zglBindTexture(zglTexture * tex, int width, int height, zglPixelFormat pixel_format,
		const char * pixel);

#if defined(linux) && (defined(i386) || defined(__x86_64))
//#if defined(i386) //!< PC(MINGW & UBUNTU)
bool zglLoadPngTexture(const char * png, zglTexture * tex);
bool zglLoadBmpTexture(const char * bmp, zglTexture * tex);
#endif

long zglGetCurrentTime(void);


// ----------------------------------------------- ><8 ------------------------

/**
 * @brief The Velocity tracker to calculate the finger fling speed.
 */
class zglVelocityTrack
{
	static const int max_length = 8;
	int m_pastX[max_length];
	int m_pastY[max_length];
	long m_pastTime[max_length];

	int m_frame;

public:
	zglVelocityTrack()
	{
		clear();
	}

	/**
	 * @brief Add the movement recorder to track.
	 *
	 * @param x Current moving position.
	 * @param y Current moving position.
	 * @param time Current moving time.
	 *
	 */
	void addMoveMent(int x, int y, long time);

	/**
	 * @brief Clear the movement recorder.
	 */
	void clear()
	{
		for (int i = 0; i < max_length; i++)
		{
			m_pastTime[i] = 0;
		}
		m_frame = 0;
	}

	/**
	 * @brief Get the x velocity.
	 */
	int getVelocityX(int unit);

	/**
	 * @brief Get the y velocity.
	 */
	int getVelocityY(int unit);

	/**
	 * @brief Dump the data.
	 */
	void dump();
};

/**
 * @brief A typical pointer container for data.
 */
class zglPinterArray
{
	const static int INIT_MAX = 16;
public:
	zglPinterArray();
	virtual ~zglPinterArray();

	/**
	 * @brief Add a new pointer to the container.
	 * @param item New pointer item.
	 * @param pos Insert position. A negative value will be skipped.
	 */
	void addInArray(void * item, int pos);

	/**
	 * @brief Remove a pointer from the container.
	 * @param pos Remove position.
	 */
	void * removeFromArray(int pos);

	/**
	 * @brief Get the child pointer in the container.
	 * @param pos The position of the Item.
	 */
	void * getChild(int pos);

	/**
	 * @brief Remove all the pointer from the container.
	 * @note The memory of the pointer will not be freed.
	 */
	void clear();

	/**
	 * @brief Get the item count in the container.
	 */
	inline int getCount()
	{
		return m_count;
	}
private:
	void ** m_array;
	int m_count;
	int m_size;
};



#endif

