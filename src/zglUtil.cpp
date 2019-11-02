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

#include <stdlib.h>		//!< NULL
#include <sys/time.h> 	//!< Time
#include <time.h>		//!< Time
#include <string.h>
#include <stdio.h>

#include "zglUtil.h"

#include "zglEngine.h"
#include "zglCore.h"

#include "zglImage.h"
#include "zglImageLoader.h"

#include "zglBasicType.h"

float zglDegToRad(float deg)
{
	return (float) (deg * ZGL_PI / 180.0);
}

float zglRadToDeg(float rad)
{
	return (float) (rad * 180.0 / ZGL_PI);
}

static const float g_sin_table[] =
{
		0.000000f, 0.008727f, 0.017452f, 0.026177f, 0.034899f, 0.043619f,
		0.052336f, 0.061049f, 0.069756f, 0.078459f, 0.087156f, 0.095846f,
		0.104528f, 0.113203f, 0.121869f, 0.130526f, 0.139173f, 0.147809f,
		0.156434f, 0.165048f, 0.173648f, 0.182236f, 0.190809f, 0.199368f,
		0.207912f, 0.216440f, 0.224951f, 0.233445f, 0.241922f, 0.250380f,
		0.258819f, 0.267238f, 0.275637f, 0.284015f, 0.292372f, 0.300706f,
		0.309017f, 0.317305f, 0.325568f, 0.333807f, 0.342020f, 0.350207f,
		0.358368f, 0.366501f, 0.374607f, 0.382683f, 0.390731f, 0.398749f,
		0.406737f, 0.414693f, 0.422618f, 0.430511f, 0.438371f, 0.446198f,
		0.453991f, 0.461749f, 0.469472f, 0.477159f, 0.484810f, 0.492424f,
		0.500000f, 0.507538f, 0.515038f, 0.522499f, 0.529919f, 0.537300f,
		0.544639f, 0.551937f, 0.559193f, 0.566406f, 0.573576f, 0.580703f,
		0.587785f, 0.594823f, 0.601815f, 0.608761f, 0.615661f, 0.622515f,
		0.629320f, 0.636078f, 0.642788f, 0.649448f, 0.656059f, 0.662620f,
		0.669131f, 0.675590f, 0.681998f, 0.688355f, 0.694658f, 0.700909f,
		0.707107f, 0.713250f, 0.719340f, 0.725374f, 0.731354f, 0.737277f,
		0.743145f, 0.748956f, 0.754710f, 0.760406f, 0.766044f, 0.771625f,
		0.777146f, 0.782608f, 0.788011f, 0.793353f, 0.798636f, 0.803857f,
		0.809017f, 0.814116f, 0.819152f, 0.824126f, 0.829038f, 0.833886f,
		0.838671f, 0.843391f, 0.848048f, 0.852640f, 0.857167f, 0.861629f,
		0.866025f, 0.870356f, 0.874620f, 0.878817f, 0.882948f, 0.887011f,
		0.891007f, 0.894934f, 0.898794f, 0.902585f, 0.906308f, 0.909961f,
		0.913545f, 0.917060f, 0.920505f, 0.923880f, 0.927184f, 0.930418f,
		0.933580f, 0.936672f, 0.939693f, 0.942642f, 0.945519f, 0.948324f,
		0.951057f, 0.953717f, 0.956305f, 0.958820f, 0.961262f, 0.963630f,
		0.965926f, 0.968148f, 0.970296f, 0.972370f, 0.974370f, 0.976296f,
		0.978148f, 0.979925f, 0.981627f, 0.983255f, 0.984808f, 0.986286f,
		0.987688f, 0.989016f, 0.990268f, 0.991445f, 0.992546f, 0.993572f,
		0.994522f, 0.995396f, 0.996195f, 0.996917f, 0.997564f, 0.998135f,
		0.998630f, 0.999048f, 0.999391f, 0.999657f, 0.999848f, 0.999962f,
		1.000000f
};

float _zsin(float x)
{
#if 0
    int angle = (int)(x * 360.0f / ZGL_PI);

    angle %= 720;
    if(angle < 0) angle += 720;

    if(angle <= 180)
        return g_sin_table[angle];
    else if(angle <= 360)
        return g_sin_table[360 - angle];
    else if(angle <= 540)
        return - g_sin_table[angle - 360];
    else
        return - g_sin_table[720 - angle];
#else

    int angle = (int)(x * 114.591559f);
    bool negetive = false;
    float value = 0.0f;

    if(angle < 0)
    {
        negetive = true;
        angle = -angle;
    }

    if(angle > 720)
    {
        angle %= 720;
    }

    if(angle < 180)
        value = g_sin_table[angle];
    else if(angle < 360)
        value = g_sin_table[360 - angle];
    else if(angle < 540)
        value =  - g_sin_table[angle - 360];
    else
        value =  - g_sin_table[720 - angle];

    return negetive ? - value : value;
#endif
}

float _zcos(float x)
{
#if 0
    int angle = (int)(x * 360.0f / ZGL_PI);

    angle = 180 - angle;
    angle %= 720;
    if(angle < 0) angle += 720;

    if(angle <= 180)
        return g_sin_table[angle];
    else if(angle <= 360)
        return g_sin_table[360 - angle];
    else if(angle <= 540)
        return - g_sin_table[angle - 360];
    else
        return - g_sin_table[720 - angle];
#else

    int angle = 180 - (int)(x * 114.591559f);
    bool negetive = false;
    float value = 0.0f;

    if(angle < 0)
    {
        negetive = true;
        angle = -angle;
    }

    if(angle > 720)
    {
        angle %= 720;
    }

    if(angle < 180)
        value = g_sin_table[angle];
    else if(angle < 360)
        value = g_sin_table[360 - angle];
    else if(angle < 540)
        value =  - g_sin_table[angle - 360];
    else
        value =  - g_sin_table[720 - angle];

    return negetive ? - value : value;

#endif
}

//!< --------------------------------------------------------------------------
bool zglIsPowerOf2(int n)
{
	return (n & -n) == n;
}

int zglNextPowerOf2(int n)
{
	n -= 1;
	n |= n >> 16;
	n |= n >> 8;
	n |= n >> 4;
	n |= n >> 2;
	n |= n >> 1;
	return n + 1;
}

int zglPrevPowerOf2(int n)
{
	if (zglIsPowerOf2(n))
	{
		return zglNextPowerOf2(n);
	}
	else
	{
		return zglNextPowerOf2(n) - 1;
	}
}

//!< --------------------------------------------------------------------------
void zglInitRandom(unsigned int seed)
{
	srand(seed);
}

int zglRandom(int min, int max)
{
	return min + (rand() % (max - min + 1));
}

float zglRandom(float min, float max, float step)
{
	int stride = (int) ((max - min) / step) + 1;
	if(stride == 0)
	{
		return min;
	}
	else
	{
		return min + (rand() % stride) * step;
	}
}

//!< --------------------------------------------------------------------------
//!< Texture helper
//!<

bool zglBindTexture(zglTexture * tex, zglImage * image)
{
	if (image == NULL || tex == NULL)
	{
		return false;
	}

	int texID = 0;

	texID = zglCore::genTexture(image->getWidth(), image->getHeight(),
			image->getFormat(), (const char*)image->getImageData());

	tex->setTexID(texID);
	tex->setDimension(image->getWidth(), image->getHeight());
	tex->setUV(image->getU(), image->getV());

	return true;
}


bool zglBindTexture(zglTexture * tex, int width, int height, zglPixelFormat pixel_format,
		const char * pixel)
{
	if (tex == NULL || pixel == NULL)
		return false;

	int texID = 0;

	texID = zglCore::genTexture(width, height, pixel_format, pixel);

	tex->setTexID(texID);
	tex->setDimension(width, height);
	return true;
}

#if defined(linux) && (defined(i386) || defined(__x86_64))
//#if defined(i386) //!< PC(MINGW & UBUNTU)
bool zglLoadPngTexture(const char * png, zglTexture * tex)
{
	zglImageLoader imageLoader;
	if(imageLoader.loadPNG(png) == false)
	{
		printf("load png failed: %s\n", png);
		return false;
	}

	if(imageLoader.isQualifiedTexture())
	{
		zglBindTexture(tex, imageLoader.getWidth(),
				imageLoader.getHeight(),
				(zglPixelFormat)imageLoader.getFormat(),
				imageLoader.getImageData());
	}
	else
	{
		zglImage image;
		image.loadPixel(imageLoader.getWidth(),imageLoader.getHeight(),
				 (zglPixelFormat)imageLoader.getFormat(),
				 imageLoader.getStride(),
				 imageLoader.getImageData());
		zglBindTexture(tex, image.getWidth(),
				image.getHeight(),
				image.getFormat(),
				image.getImageData());

		float u, v;
		image.getTexCoord(&u, &v);
		tex->setNormalDimension(u, v);

		image.clear();
	}
	imageLoader.clear();
	return true;
}

bool zglLoadBmpTexture(const char * bmp, zglTexture * tex)
{
	zglImageLoader imageLoader;
	if(imageLoader.loadBMP(bmp) == false)
	{
		return false;
	}

	if(imageLoader.isQualifiedTexture()) // 2^n
	{
		zglBindTexture(tex, imageLoader.getWidth(),
				imageLoader.getHeight(),
				(zglPixelFormat)imageLoader.getFormat(),
				imageLoader.getImageData());
	}
	else
	{
		zglImage image;
		image.loadPixel(imageLoader.getWidth(),imageLoader.getHeight(),
				 imageLoader.getFormat(),
				 imageLoader.getStride(),
				 imageLoader.getImageData());
		zglBindTexture(tex, image.getWidth(),
				image.getHeight(),
				image.getFormat(),
				image.getImageData());

		image.clear();
	}
	imageLoader.clear();
	return true;
}
#endif

//!< --------------------------------------------------------------------------
long zglGetCurrentTime()
{
	struct timeval now;

	gettimeofday(&now, NULL);
	return (long) (now.tv_sec * 1000 + now.tv_usec / 1000);
}

// ----------------------------------------------- ><8 ------------------------

void zglVelocityTrack::addMoveMent(int x, int y, long time)
{
	if(m_frame < max_length )
	{
		m_pastX[m_frame] = x;
		m_pastY[m_frame] = y;
		m_pastTime[m_frame] = time;
		m_frame ++;
	}
	else
	{
		// m_frame == max_length
		//TODO memmove
		for(int i=0; i< m_frame - 1; i ++)
		{
			m_pastX[i] = m_pastX[i + 1];
			m_pastY[i] = m_pastY[i + 1];
			m_pastTime[i] = m_pastTime[i + 1];
		}

		m_pastX[m_frame - 1] = x;
		m_pastY[m_frame - 1] = y;
		m_pastTime[m_frame - 1] = time;
	}
}

int zglVelocityTrack::getVelocityX(int unit)
{
	float accumX = 0.0f;

	if(m_frame > 3) m_frame --;

	int oldx = m_pastX[0];
	int oldtime = m_pastTime[0];

	for(int i=1; i<m_frame; i++)
	{
		int dur = m_pastTime[i] - oldtime;
		if(dur == 0) continue;

		float dist = m_pastX[i] - oldx;
		float vel = dist * unit / dur;
		if(accumX == 0)
		{
			accumX = vel;
		}
		else
		{
			accumX = (accumX + vel) * 0.5f;
		}
	}

	return (int)accumX;
}

int zglVelocityTrack::getVelocityY(int unit)
{
	float accumY = 0.0f;

	if(m_frame > 3) m_frame --;

	int oldx = m_pastY[0];
	int oldtime = m_pastTime[0];

	for(int i=1; i<m_frame; i++)
	{
		int dur = m_pastTime[i] - oldtime;
		if(dur == 0) continue;

		float dist = m_pastY[i] - oldx;
		float vel = dist * unit / dur;
		if(accumY == 0)
		{
			accumY = vel;
		}
		else
		{
			accumY = (accumY + vel) * 0.5f;
		}
	}

	return (int)accumY;
}

void zglVelocityTrack::dump()
{
}

// ----------------------------------------------- ><8 ------------------------

zglPinterArray::zglPinterArray()
{
	m_array = (void**)malloc(sizeof(void *) * INIT_MAX);
	memset(m_array, 0, sizeof(void *) * INIT_MAX);
	m_count = 0;
	m_size = INIT_MAX;
}

zglPinterArray::~zglPinterArray()
{
	if (m_array != NULL)
	{
		free(m_array);
		m_array = NULL;

		m_count = 0;
		m_size = 0;
	}
}

void zglPinterArray::addInArray(void * item, int index)
{
	if(index < 0) return;

	if (m_size == m_count)
	{
		int size = sizeof(void *) * (m_size + INIT_MAX);
		void ** array = (void **) malloc(size);

		memcpy(array, m_array, sizeof(void *) * m_size);

		free(m_array);
		m_array = array;
		m_size += INIT_MAX;
	}

	if (index == m_count)
	{
		m_array[m_count++] = item;
	}
	else if (index < m_count)
	{
#if 1
		memmove( &m_array[index + 1], &m_array[index], sizeof(void*) * (m_count - index));
#else
		for (int i = m_count - 1; i >= index; i--)
		{
			m_array[i + 1] = m_array[i];
		}
#endif
		m_array[index] = item;
		m_count++;
	}
}

void * zglPinterArray::removeFromArray(int index)
{
	if(index < 0) return NULL;

	void * data = m_array[index];
	if (index == m_count - 1)
	{
		m_array[index] = NULL;

		m_count --;
	}
	else if (index < m_count)
	{
#if 1
		memmove( &m_array[index], &m_array[index + 1], sizeof(void*) * (m_count - index - 1));
#else
		for (int i = index; i < m_count; i++)
		{
			m_array[i] = m_array[i + 1];
		}
#endif

		m_array[m_count] = NULL;
		m_count--;
	}
	return data;
}

void zglPinterArray::clear()
{
	m_count = 0;
	m_size = INIT_MAX;
}

void * zglPinterArray::getChild(int index)
{
	if (m_array != NULL && index < m_count)
	{
		return m_array[index];
	}

	return NULL;
}



