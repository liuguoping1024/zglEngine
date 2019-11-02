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
#include <stdint.h>
#include <string.h>

#include "zglImage.h"
#include "zglUtil.h"	//!< Power2 calculation
typedef long fixed;
#define ftofx(x) (long)((x) * 256)								// Float to fixed point
#define Mulfx(x,y) (((x) * (y)) >> 8)							// Multiply a fixed by a fixed
#define itofx(x) ((x) << 8)										// Integer to fixed point
#define fxtoi(x) ((x) >> 8)										// Fixed point to integer
zglImage::zglImage()
{
	m_width = 0;
	m_height = 0;
	m_stride = 0;

	m_width_origin = 0;
	m_height_origin = 0;

	m_pData = NULL;

	m_width_normalized = 1.0f;
	m_height_normalized = 1.0f;

	m_format = PIXEL_RGBA8888;
}

zglImage::~zglImage()
{
	if (m_pData != NULL)
	{
		free(m_pData);
		m_pData = NULL;
	}
}

void zglImage::clear()
{
	if (m_pData != NULL)
	{
		free(m_pData);
		m_pData = NULL;
	}

	m_width = 0;
	m_height = 0;
	m_stride = 0;
	m_format = PIXEL_RGBA8888;

	m_width_origin = 0;
	m_height_origin = 0;

	m_width_normalized = 1.0f;
	m_height_normalized = 1.0f;
}

bool zglImage::loadPixel(int width, int height, int format, int stride,
		const char * pixel, bool shrink)
{
	//TODO
	//if(format != PIXEL_RGBA8888)
	//{
	//PIXEL_NONE Must be excluded.
	//return false;
	//}

	if (format == PIXEL_NONE)
	{
		return false;
	}

	if (m_pData != NULL)
	{
		free(m_pData);
		m_pData = NULL;
	}

	if (shrink)
	{
		//!< Check the source pixels, if the source image is too big, scale it into 256*256
		if (width > 256 || height > 256)
		{
			if (format == PIXEL_RGBA8888)
			{
				return loadPixel32(width, height, stride, pixel);
			}
			return false;
		}
	}

	//!< Calculate the power2 width and height
	normalize(width, height);

	//Assign a new depth for the new image.
	m_format = PIXEL_RGBA8888;

	int pixel_size;
	switch (format)
	{
	case PIXEL_RGBA8888:
		pixel_size = 4;
		break;

	case PIXEL_RGB888:
		pixel_size = 3;
		break;

	case PIXEL_R8:
	case PIXEL_A8:
		pixel_size = 1;
		break;

	default: //PIXEL_RGBA5551, PIXEL_RGB565, PIXEL_RA88, PIXEL_RGBA4444
		pixel_size = 2;
		break;
	}

	//!< Allocate the buffer.
	int total_pixel = m_width * m_height * 4;
	m_pData = (unsigned char*) malloc(total_pixel);

	int new_stride = m_width * 4;

	if (m_pData == NULL)
	{
		m_width = 0;
		m_height = 0;
		m_width_origin = 0;
		m_height_origin = 0;

		m_width_normalized = 1.0f;
		m_height_normalized = 1.0f;

		m_format = PIXEL_NONE;
		return false;
	}

	memset(m_pData, 0, total_pixel);

	unsigned char * p_dst = m_pData;
	unsigned char * p_src = (unsigned char *) pixel;

	switch (format)
	{
	case PIXEL_RGBA8888:
		{
			for (int i = 0; i < height; i++)
			{
				memcpy(p_dst, p_src, stride);
				p_dst += new_stride;
				p_src += stride;
			}
		}
		break;

	case PIXEL_RGB888:
		{
			for (int i = 0; i < height; i++)
			{
				unsigned char * p1 = p_dst;
				unsigned char * p2 = p_src;

				for (int j = 0; j < width; j++)
				{
					*(p1 + 0) = *(p2 + 0); //R
					*(p1 + 1) = *(p2 + 1); //G
					*(p1 + 2) = *(p2 + 2); //B

					*(p1 + 3) = 0xff; //A

					p1 += 4;
					p2 += 3;
				}

				p_dst += new_stride;
				p_src += stride;
			}
		}
		break;

	case PIXEL_RGB565:
		{
			for (int i = 0; i < height; i++)
			{
				unsigned char * p1 = p_dst;
				unsigned short * p2 = (unsigned short *) p_src;

				for (int j = 0; j < width; j++)
				{
					*(p1 + 0) = ((*p2) & 0xf800) >> 8; //R
					*(p1 + 1) = ((*p2) & 0x07e0) >> 3; //G
					*(p1 + 2) = ((*p2) & 0x001f) << 3; //B

					*(p1 + 3) = 0xff; //A

					p1 += 4;

					p2++;
				}

				p_dst += new_stride;
				p_src += stride;
			}
		}
		break;

	case PIXEL_RGBA4444:
		{
			for (int i = 0; i < height; i++)
			{
				unsigned char * p1 = p_dst;
				unsigned short * p2 = (unsigned short *) p_src;

				for (int j = 0; j < width; j++)
				{
					*(p1 + 0) = ((*p2) & 0xf000) >> 8; //R
					*(p1 + 1) = ((*p2) & 0x0f00) >> 4; //G
					*(p1 + 2) = ((*p2) & 0x00f0) << 0; //B
					*(p1 + 3) = ((*p2) & 0x000f) << 4; //A

					p1 += 4;

					p2++;
				}

				p_dst += new_stride;
				p_src += stride;
			}
		}
		break;

	default:
		return false;
	}

	m_stride = new_stride;

	return true;
}

bool zglImage::autoLoadPixel(int width, int height, int format, int stride,
		const char * pixel)
{
	if (format == PIXEL_NONE)
	{
		return false;
	}

	if (m_pData != NULL)
	{
		free(m_pData);
		m_pData = NULL;
	}

	//!< Calculate the power2 width and height
	normalize(width, height);

	int pixel_size;
	switch (format)
	{
	case PIXEL_RGBA8888:
		pixel_size = 4;
		break;

	case PIXEL_RGB888:
		pixel_size = 3;
		break;

	case PIXEL_R8:
	case PIXEL_A8:
		pixel_size = 1;
		break;

	default: //PIXEL_RGBA5551, PIXEL_RGB565, PIXEL_RA88, PIXEL_RGBA4444
		pixel_size = 2;
		break;
	}

	m_format = (zglPixelFormat) format; //Keep original depth for the new image.

	//!< Allocate the buffer.
	int total_pixel = m_width * m_height * 4;
	m_pData = (unsigned char*) malloc(total_pixel);

	int new_stride = m_width * pixel_size;

	if (m_pData == NULL)
	{
		m_width = 0;
		m_height = 0;
		m_width_origin = 0;
		m_height_origin = 0;

		m_width_normalized = 1.0f;
		m_height_normalized = 1.0f;

		m_format = PIXEL_NONE;
		return false;
	}

	memset(m_pData, 0, total_pixel);

	unsigned char * p_dst = m_pData;
	unsigned char * p_src = (unsigned char *) pixel;

	switch (format)
	{
	case PIXEL_RGBA8888:
		{
			for (int i = 0; i < height; i++)
			{
				memcpy(p_dst, p_src, stride);
				p_dst += new_stride;
				p_src += stride;
			}
		}
		break;

	case PIXEL_RGB888:
		{
			int byteperline = width * 3;

			for (int i = 0; i < height; i++)
			{
				memcpy(p_dst, p_src, byteperline);
				p_dst += new_stride;
				p_src += stride;
			}
		}
		break;

	case PIXEL_RGB565:
		{
			int byteperline = width * 2;

			for (int i = 0; i < height; i++)
			{
				memcpy(p_dst, p_src, byteperline);
				p_dst += new_stride;
				p_src += stride;
			}
		}
		break;

	case PIXEL_RGBA5551:
	case PIXEL_RGBA4444:
		{
			int byteperline = width * 2;

			for (int i = 0; i < height; i++)
			{
				memcpy(p_dst, p_src, byteperline);
				p_dst += new_stride;
				p_src += stride;
			}
		}
		break;

	default:
		return false;
	}

	m_stride = new_stride;

	return true;
}

void zglImage::normalize(int width, int height)
{
	//!< Calculate the power2 width.
	m_width_origin = width;
	if (zglIsPowerOf2(width))
	{
		if (width < 64)
		{
			m_width = 64;
			m_width_normalized = ((float) width) / m_width;
		}
		else
		{
			m_width = width;
			m_width_normalized = 1.0f;
		}
	}
	else
	{
		m_width = zglNextPowerOf2(width);
		if (m_width < 64)
		{
			m_width = 64;
		}

		m_width_normalized = ((float) width) / m_width;
	}

	//!< Calculate the power2 height.
	m_height_origin = height;
	if (zglIsPowerOf2(height))
	{
		if (height < 64)
		{
			m_height = 64;
			m_height_normalized = ((float) height) / m_height;
		}
		else
		{
			m_height = height;
			m_height_normalized = 1.0f;
		}
	}
	else
	{
		m_height = zglNextPowerOf2(height);
		if (m_height < 64)
		{
			m_height = 64;
		}
		m_height_normalized = ((float) height) / m_height;
	}
}

void zglImage::bindBuffer(zglImage * templates, const char * pixel)
{
	m_width = templates->m_width;
	m_height = templates->m_height;
	m_stride = templates->m_stride;
	m_format = templates->m_format;

	m_width_origin = templates->m_width_origin;
	m_height_origin = templates->m_height_origin;

	m_width_normalized = templates->m_width_normalized;
	m_height_normalized = templates->m_height_normalized;

	if (m_pData != NULL)
	{
		free(m_pData);
	}

	m_pData = (unsigned char*) pixel;
}

//!< Load RGBA8888 TODO
bool zglImage::loadPixel32(int width, int height, int stride,
		const char * pixel)
{
	m_width = 256;
	m_height = 256;

	m_width_origin = 256;
	m_height_origin = 256;

	m_width_normalized = 1.0f;
	m_height_normalized = 1.0f;

	//!< Allocate the buffer.
	int size = 4;
	int step = 4;

	int total_pixel = m_width * m_height * size;
	m_pData = (unsigned char*) malloc(total_pixel);
	m_stride = m_width * size;

	if (m_pData == NULL)
	{
		return false;
	}

	double sx = float(width) / m_width;
	double sy = float(height) / m_height;

	unsigned char * p_src = (unsigned char *) pixel;
	unsigned char * p_dst = m_pData;

	unsigned char * p0 = NULL;
	unsigned char * p1 = NULL;
	for (int y = 0; y < m_height; y++)
	{
		for (int x = 0; x < m_width; x++)
		{
			int tx = x * sx;
			int ty = y * sy;

			p0 = (p_src + stride * ty + step * tx);
			p1 = p_dst;

			if (ty == 0)
			{
				if (tx == 0)
				{
					*p1 = (*p0 + *(p0 + step) + *(p0 + stride)
							+ *(p0 + stride + step)) >> 2;

					p1++;
					p0++;

					*p1 = (*p0 + *(p0 + step) + *(p0 + stride)
							+ *(p0 + stride + step)) >> 2;
					p1++;
					p0++;

					*p1 = (*p0 + *(p0 + step) + *(p0 + stride)
							+ *(p0 + stride + step)) >> 2;
					p1++;
					p0++;

					if (step == 4)
					{
						*p1 = (*p0 + *(p0 + step) + *(p0 + stride)
								+ *(p0 + stride + step)) >> 2;
					}
					else
					{
						*p1 = 255;
					}
				}
				else if (tx == width - 1)
				{
					*p1 = (*p0 + *(p0 + step) + *(p0 - stride)
							+ *(p0 - stride + step)) >> 2;

					p1++;
					p0++;

					*p1 = (*p0 + *(p0 + step) + *(p0 - stride)
							+ *(p0 - stride + step)) >> 2;
					p1++;
					p0++;

					*p1 = (*p0 + *(p0 + step) + *(p0 - stride)
							+ *(p0 - stride + step)) >> 2;
					p1++;
					p0++;

					if (step == 4)
					{
						*p1 = (*p0 + *(p0 + step) + *(p0 - stride)
								+ *(p0 - stride + step)) >> 2;
					}
					else
					{
						*p1 = 255;
					}
				}
				else
				{
					*p1 = (*(p0 - step) + ((*p0) << 1) + *(p0 + step)
							+ *(p0 + stride - step) + ((*(p0 + stride)) << 1)
							+ *(p0 + stride + step)) >> 3;

					p1++;
					p0++;

					*p1 = (*(p0 - step) + ((*p0) << 1) + *(p0 + step)
							+ *(p0 + stride - step) + ((*(p0 + stride)) << 1)
							+ *(p0 + stride + step)) >> 3;
					p1++;
					p0++;

					*p1 = (*(p0 - step) + ((*p0) << 1) + *(p0 + step)
							+ *(p0 + stride - step) + ((*(p0 + stride)) << 1)
							+ *(p0 + stride + step)) >> 3;
					p1++;
					p0++;

					if (step == 4)
					{
						*p1 = (*(p0 - step) + ((*p0) << 1) + *(p0 + step)
								+ *(p0 + stride - step)
								+ ((*(p0 + stride)) << 1)
								+ *(p0 + stride + step)) >> 3;
					}
					else
					{
						*p1 = 255;
					}
				}
			}
			else if (ty == height - 1)
			{
				if (tx == 0)
				{
					*p1 = (*p0 + *(p0 + step) + *(p0 - stride)
							+ *(p0 - stride + step)) >> 2;

					p1++;
					p0++;

					*p1 = (*p0 + *(p0 + step) + *(p0 - stride)
							+ *(p0 - stride + step)) >> 2;
					p1++;
					p0++;

					*p1 = (*p0 + *(p0 + step) + *(p0 - stride)
							+ *(p0 - stride + step)) >> 2;
					p1++;
					p0++;

					if (step == 4)
					{
						*p1 = (*p0 + *(p0 + step) + *(p0 - stride)
								+ *(p0 - stride + step)) >> 2;
					}
					else
					{
						*p1 = 255;
					}
				}
				else if (tx == width - 1)
				{
					*p1 = (*p0 + *(p0 - step) + *(p0 - stride)
							+ *(p0 - stride - step)) >> 2;

					p1++;
					p0++;

					*p1 = (*p0 + *(p0 - step) + *(p0 - stride)
							+ *(p0 - stride - step)) >> 2;
					p1++;
					p0++;

					*p1 = (*p0 + *(p0 - step) + *(p0 - stride)
							+ *(p0 - stride - step)) >> 2;
					p1++;
					p0++;

					if (step == 4)
					{
						*p1 = (*p0 + *(p0 - step) + *(p0 - stride)
								+ *(p0 - stride - step)) >> 2;
					}
					else
					{
						*p1 = 255;
					}
				}
				else
				{
					*p1 = (*(p0 - step) + ((*p0) << 1) + *(p0 + step)
							+ *(p0 - stride - step) + ((*(p0 - stride)) << 1)
							+ *(p0 - stride + step)) >> 3;

					p1++;
					p0++;

					*p1 = (*(p0 - step) + ((*p0) << 1) + *(p0 + step)
							+ *(p0 - stride - step) + ((*(p0 - stride)) << 1)
							+ *(p0 - stride + step)) >> 3;
					p1++;
					p0++;

					*p1 = (*(p0 - step) + ((*p0) << 1) + *(p0 + step)
							+ *(p0 - stride - step) + ((*(p0 - stride)) << 1)
							+ *(p0 - stride + step)) >> 3;
					p1++;
					p0++;

					if (step == 4)
					{
						*p1 = (*(p0 - step) + ((*p0) << 1) + *(p0 + step)
								+ *(p0 - stride - step)
								+ ((*(p0 - stride)) << 1)
								+ *(p0 - stride + step)) >> 3;
					}
					else
					{
						*p1 = 255;
					}
				}
			}
			else
			{
				if (tx == 0)
				{
					*p1 = (((*p0) << 1) + ((*(p0 + step)) << 1) + *(p0 + stride)
							+ *(p0 + stride + step) + *(p0 - stride)
							+ *(p0 - stride + step)) >> 3;

					p1++;
					p0++;

					*p1 = (((*p0) << 1) + ((*(p0 + step)) << 1) + *(p0 + stride)
							+ *(p0 + stride + step) + *(p0 - stride)
							+ *(p0 - stride + step)) >> 3;
					p1++;
					p0++;

					*p1 = (((*p0) << 1) + ((*(p0 + step)) << 1) + *(p0 + stride)
							+ *(p0 + stride + step) + *(p0 - stride)
							+ *(p0 - stride + step)) >> 3;
					p1++;
					p0++;

					if (step == 4)
					{
						*p1 = (((*p0) << 1) + ((*(p0 + step)) << 1)
								+ *(p0 + stride) + *(p0 + stride + step)
								+ *(p0 - stride) + *(p0 - stride + step)) >> 3;
					}
					else
					{
						*p1 = 255;
					}
				}
				else if (tx == width - 1)
				{
					*p1 = (((*p0) << 1) + ((*(p0 - step)) << 1) + *(p0 + stride)
							+ *(p0 + stride - step) + *(p0 - stride)
							+ *(p0 - stride - step)) >> 3;

					p1++;
					p0++;

					*p1 = (((*p0) << 1) + ((*(p0 - step)) << 1) + *(p0 + stride)
							+ *(p0 + stride - step) + *(p0 - stride)
							+ *(p0 - stride - step)) >> 3;
					p1++;
					p0++;

					*p1 = (((*p0) << 1) + ((*(p0 - step)) << 1) + *(p0 + stride)
							+ *(p0 + stride - step) + *(p0 - stride)
							+ *(p0 - stride - step)) >> 3;
					p1++;
					p0++;

					if (step == 4)
					{
						*p1 = (((*p0) << 1) + ((*(p0 - step)) << 1)
								+ *(p0 + stride) + *(p0 + stride - step)
								+ *(p0 - stride) + *(p0 - stride - step)) >> 3;
					}
					else
					{
						*p1 = 255;
					}
				}
				else
				{
					*p1 = (((*(p0 - step)) << 1) + ((*p0) << 2)
							+ ((*(p0 + step)) << 1) + *(p0 - step - stride)
							+ ((*(p0 - stride)) << 1) + *(p0 - stride + step)
							+ *(p0 - step + stride) + ((*(p0 + stride)) << 1)
							+ *(p0 + stride + step)) >> 4;
					p1++;
					p0++;

					*p1 = (((*(p0 - step)) << 1) + ((*p0) << 2)
							+ ((*(p0 + step)) << 1) + *(p0 - step - stride)
							+ ((*(p0 - stride)) << 1) + *(p0 - stride + step)
							+ *(p0 - step + stride) + ((*(p0 + stride)) << 1)
							+ *(p0 + stride + step)) >> 4;
					p1++;
					p0++;

					*p1 = (((*(p0 - step)) << 1) + ((*p0) << 2)
							+ ((*(p0 + step)) << 1) + *(p0 - step - stride)
							+ ((*(p0 - stride)) << 1) + *(p0 - stride + step)
							+ *(p0 - step + stride) + ((*(p0 + stride)) << 1)
							+ *(p0 + stride + step)) >> 4;
					p1++;
					p0++;

					if (step == 4)
					{
						*p1 = (((*(p0 - step)) << 1) + ((*p0) << 2)
								+ ((*(p0 + step)) << 1) + *(p0 - step - stride)
								+ ((*(p0 - stride)) << 1)
								+ *(p0 - stride + step) + *(p0 - step + stride)
								+ ((*(p0 + stride)) << 1)
								+ *(p0 + stride + step)) >> 4;
					}
					else
					{
						*p1 = 255;
					}
				}
			}
			p_dst += 4;
		}
	}

	return true;
}

bool zglImage::loadPixel565(int width, int height, int stride,
		const char * pixel)
{
	return false;
}

bool zglImage::loadPixel4444(int width, int height, int stride,
		const char * pixel)
{
	return false;
}

void zglImage::getTexCoord(float *u, float* v)
{
	if (u != NULL)
	{
		*u = m_width_normalized;
	}

	if (v != NULL)
	{
		*v = m_height_normalized;
	}
}

void zglImage::reset(int color)
{
	if (m_pData == NULL)
	{
		return;
	}
}

static const unsigned int g_factor[] =
{ 0xffff, 0x8000, 0x5555, 0x4000, 0x3333, 0x2aaa, 0x2492, 0x2000, 0x1c71,
	0x1999, 0x1745, 0x1555, 0x13b1, 0x1249, 0x1111, 0x1000, 0x0f0f, 0x0e38,
	0x0d79, 0x0ccc, 0x0c30, 0x0ba2, 0x0b21, 0x0aaa, 0x0a3d, 0x09d8, 0x097b,
	0x0924, 0x08d3, 0x0888, 0x0842, 0x0800, 0x07c1, 0x0787, 0x0750, 0x071c,
	0x06eb, 0x06bc, 0x0690, 0x0666, 0x063e, 0x0618, 0x05f4, 0x05d1, 0x05b0,
	0x0590, 0x0572, 0x0555, 0x0539, 0x051e, 0x0505, 0x04ec, 0x04d4, 0x04bd,
	0x04a7, 0x0492, 0x047d, 0x0469, 0x0456, 0x0444, 0x0432, 0x0421, 0x0410,
	0x0400, 0x03f0, 0x03e0, 0x03d2, 0x03c3, 0x03b5, 0x03a8, 0x039b, 0x038e,
	0x0381, 0x0375, 0x0369, 0x035e, 0x0353, 0x0348, 0x033d, 0x0333, 0x0329,
	0x031f, 0x0315, 0x030c, 0x0303, 0x02fa, 0x02f1, 0x02e8, 0x02e0, 0x02d8,
	0x02d0, 0x02c8, 0x02c0, 0x02b9, 0x02b1, 0x02aa, 0x02a3, 0x029c, 0x0295,
	0x028f, 0x0288, 0x0282, 0x027c, 0x0276, 0x0270, 0x026a, 0x0264, 0x025e,
	0x0259, 0x0253, 0x024e, 0x0249, 0x0243, 0x023e, 0x0239, 0x0234, 0x0230,
	0x022b, 0x0226, 0x0222, 0x021d, 0x0219, 0x0214, 0x0210, 0x020c, 0x0208,
	0x0204, 0x0200, 0x01fc, 0x01f8, 0x01f4, 0x01f0, 0x01ec, 0x01e9, 0x01e5,
	0x01e1, 0x01de, 0x01da, 0x01d7, 0x01d4, 0x01d0, 0x01cd, 0x01ca, 0x01c7,
	0x01c3, 0x01c0, 0x01bd, 0x01ba, 0x01b7, 0x01b4, 0x01b2, 0x01af, 0x01ac,
	0x01a9, 0x01a6, 0x01a4, 0x01a1, 0x019e, 0x019c, 0x0199, 0x0197, 0x0194,
	0x0192, 0x018f, 0x018d, 0x018a, 0x0188, 0x0186, 0x0183, 0x0181, 0x017f,
	0x017d, 0x017a, 0x0178, 0x0176, 0x0174, 0x0172, 0x0170, 0x016e, 0x016c,
	0x016a, 0x0168, 0x0166, 0x0164, 0x0162, 0x0160, 0x015e, 0x015c, 0x015a,
	0x0158, 0x0157, 0x0155, 0x0153, 0x0151, 0x0150, 0x014e, 0x014c, 0x014a,
	0x0149, 0x0147, 0x0146, 0x0144, 0x0142, 0x0141, 0x013f, 0x013e, 0x013c,
	0x013b, 0x0139, 0x0138, 0x0136, 0x0135, 0x0133, 0x0132, 0x0130, 0x012f,
	0x012e, 0x012c, 0x012b, 0x0129, 0x0128, 0x0127, 0x0125, 0x0124, 0x0123,
	0x0121, 0x0120, 0x011f, 0x011e, 0x011c, 0x011b, 0x011a, 0x0119, 0x0118,
	0x0116, 0x0115, 0x0114, 0x0113, 0x0112, 0x0111, 0x010f, 0x010e, 0x010d,
	0x010c, 0x010b, 0x010a, 0x0109, 0x0108, 0x0107, 0x0106, 0x0105, 0x0104,
	0x0103, 0x0102, 0x0101, };

//ANDROID_NDK bitmap ANDROID_BITMAP_FORMAT_RGBA_8888: [R][G][B][A]

void zglImage::negative()
{
	if (m_pData == NULL)
	{
		return;
	}

	if (m_format != PIXEL_RGBA8888 && m_format != PIXEL_RGB888) //TODO RGBA8888 only
	{
		return;
	}

	int step;
	if (m_format == PIXEL_RGBA8888)
	{
		step = 4;
	}
	else
	{
		step = 3;
	}

	unsigned char * p_src = m_pData;
	for (int i = 0; i < m_height_origin; i++)
	{
		unsigned char * p = p_src;
		for (int j = 0; j < m_width_origin; j++)
		{
			*p = 255 - *p;
			*(p + 1) = 255 - *(p + 1);
			*(p + 2) = 255 - *(p + 2);
			p += step;
		}

		p_src += m_stride;
	}
}

void zglImage::grayscale()
{
	if (m_pData == NULL)
	{
		return;
	}

	if (m_format != PIXEL_RGBA8888 && m_format != PIXEL_RGB888) //TODO RGBA8888 only
	{
		return;
	}

	fixed f_w1 = ftofx(0.299f);
	fixed f_w2 = ftofx(0.587f);
	fixed f_w3 = ftofx(0.114f);

	int step = m_format == PIXEL_RGBA8888 ? 4 : 3;

	unsigned char * p_src = m_pData;

	for (int i = 0; i < m_height_origin; i++)
	{
		unsigned char * p = p_src;
		for (int j = 0; j < m_width_origin; j++)
		{
			unsigned char r = *p;
			unsigned char g = *(p + 1);
			unsigned char b = *(p + 2);

			unsigned short f_value = (f_w1 * r + f_w2 * g + f_w3 * b) >> 8;

			*p = f_value; //RGBA
			*(p + 1) = f_value;
			*(p + 2) = f_value;
			p += step;
		}

		p_src += m_stride;
	}
}

void zglImage::sepia(int depth)
{
	if (m_pData == NULL)
	{
		return;
	}

	if (m_format != PIXEL_RGBA8888 && m_format != PIXEL_RGB888) //TODO RGBA8888 only
	{
		return;
	}

	fixed f_w1 = ftofx(0.299f);
	fixed f_w2 = ftofx(0.587f);
	fixed f_w3 = ftofx(0.114f);

	int _depth = depth > 100 ? 100 : depth;
	_depth = _depth > 0 ? _depth : 0;

	fixed f_depth = itofx(_depth);
	fixed f_depth2 = Mulfx(itofx(2),f_depth);

	int step = m_format == PIXEL_RGBA8888 ? 4 : 3;

	unsigned char * p_src = m_pData;

	for (int i = 0; i < m_height_origin; i++)
	{
		unsigned char * p = p_src;
		for (int j = 0; j < m_width_origin; j++)
		{
			unsigned char r = *p;
			unsigned char g = *(p + 1);
			unsigned char b = *(p + 2);

			fixed f_value = f_w1 * r + f_w2 * g + f_w3 * b;

			unsigned char red = (unsigned char) fxtoi(f_value+f_depth2);
			if (red < ((_depth << 1) - 1))
				red = 255;
			unsigned char green = (unsigned char) fxtoi(f_value+f_depth);
			if (green < (_depth - 1))
				green = 255;
			unsigned char blue = (unsigned char) fxtoi(f_value);

			*p = red; //RGBA
			*(p + 1) = green;
			*(p + 2) = blue;
			p += step;
		}

		p_src += m_stride;
	}
}

void zglImage::emboss()
{
	if (m_pData == NULL)
	{
		return;
	}
}

void zglImage::engrave()
{
	if (m_pData == NULL)
	{
		return;
	}
}

void zglImage::pixelize(int size)
{
	if (m_pData == NULL)
	{
		return;
	}
}

/**
 * The matrix is below:
 *
 * | 1 1 1 |
 * | 1 1 1 |
 * | 1 1 1 |
 */
unsigned char* zglImage::blur()
{
	if (m_pData == NULL
			|| (m_format != PIXEL_RGBA8888 && m_format != PIXEL_RGB888)) //TODO RGBA8888 only
	{
		return NULL;
	}

	const unsigned int f_9 = 0x1C71;

	int pixel_size;
	if (m_format == PIXEL_RGBA8888)
	{
		pixel_size = 4;
	}
	else
	{
		pixel_size = 3;
	}

	int total_pixel = m_width * m_height * pixel_size;
	unsigned char * m_pData_new = (unsigned char*) malloc(total_pixel);

	if (m_pData_new == NULL)
	{
		return NULL;
	}

	memcpy(m_pData_new, m_pData, total_pixel);

	unsigned char * p_src = m_pData;
	unsigned char * p_dst = m_pData_new;

	for (int i = 1; i < m_height_origin - 1; i++)
	{
		unsigned char * p0 = p_src;
		unsigned char * p1 = p_dst;

		for (int j = 1; j < m_width_origin - 1; j++)
		{
			*p1 = ((*(p0 - pixel_size) + *p0 + *(p0 + pixel_size)
					+ *(p0 - pixel_size - m_stride) + *(p0 - m_stride)
					+ *(p0 - m_stride + pixel_size)
					+ *(p0 - pixel_size + m_stride) + *(p0 + m_stride)
					+ *(p0 + m_stride + pixel_size)) * f_9) >> 16;
			p0++;
			p1++;

			*p1 = ((*(p0 - pixel_size) + *p0 + *(p0 + pixel_size)
					+ *(p0 - pixel_size - m_stride) + *(p0 - m_stride)
					+ *(p0 - m_stride + pixel_size)
					+ *(p0 - pixel_size + m_stride) + *(p0 + m_stride)
					+ *(p0 + m_stride + pixel_size)) * f_9) >> 16;
			p0++;
			p1++;

			*p1 = ((*(p0 - pixel_size) + *p0 + *(p0 + pixel_size)
					+ *(p0 - pixel_size - m_stride) + *(p0 - m_stride)
					+ *(p0 - m_stride + pixel_size)
					+ *(p0 - pixel_size + m_stride) + *(p0 + m_stride)
					+ *(p0 + m_stride + pixel_size)) * f_9) >> 16;
			p0++;
			p1++;

			if (pixel_size == 4)
			{
				*p1 = 255; //ALPHA
				p0++;
				p1++;
			}
		}

		p_src += m_stride;
		p_dst += m_stride;
	}

	//free(m_pData);

	//m_pData = m_pData_new;

	return m_pData_new;
}

#if 0
void zglImage::blur()
{
	if (m_pData == NULL || (m_colorDepth != 32 && m_colorDepth != 24)) //TODO RGBA8888 only

	{
		return;
	}

	long time1 = zglGetCurrentTime();

	int total_pixel = m_width * m_height * (m_colorDepth >> 3);
	unsigned char * m_pData_new = (unsigned char*) malloc(total_pixel);

	if(m_pData_new == NULL)
	{
		return;
	}

	//memcpy(m_pData_new, m_pData, total_pixel);

	unsigned char * p_src = m_pData;
	unsigned char * p_dst = m_pData_new;

	//int sx, tx;
	//int sy, ty;

	for (int i = 0; i < m_height_origin; i++)
	{
		unsigned char * p0 = p_src;
		unsigned char * p1 = p_dst;

		for (int j = 0; j < m_width_origin; j++)
		{
			if(i == 0)
			{
				if(j==0)
				{
					//sx = 0; tx = 4;
					//sy = 0; ty = m_stride;

					*p1 = (*p0 + *(p0+1) + *(p0+m_stride) + *(p0+m_stride+1)) >> 2;
					p0 ++;
					p1 ++;

					*p1 = (*p0 + *(p0+1) + *(p0+m_stride) + *(p0+m_stride+1)) >> 2;
					p0 ++;
					p1 ++;

					*p1 = (*p0 + *(p0+1) + *(p0+m_stride) + *(p0+m_stride+1)) >> 2;
					p0 ++;
					p1 ++;
				}
				else if(j == m_width_origin - 1)
				{
					//sx = -4; tx = 0;
					//sy = 0; ty = m_stride;

					*p1 = (*p0 + *(p0-1) + *(p0+m_stride) + *(p0+m_stride-1)) >> 2;
					p0 ++;
					p1 ++;

					*p1 = (*p0 + *(p0-1) + *(p0+m_stride) + *(p0+m_stride-1)) >> 2;
					p0 ++;
					p1 ++;

					*p1 = (*p0 + *(p0-1) + *(p0+m_stride) + *(p0+m_stride-1)) >> 2;
					p0 ++;
					p1 ++;
				}
				else
				{
					//sx = -4; tx = 4;
					//sy = 0; ty = m_stride;

					*p1 = ( *(p0-1) + *p0 + *(p0+1)
							+ *(p0-1 + m_stride) + *(p0+m_stride) + *(p0+m_stride+1) )/6;
					p0 ++;
					p1 ++;

					*p1 = ( *(p0-1) + *p0 + *(p0+1)
							+ *(p0-1 + m_stride) + *(p0+m_stride) + *(p0+m_stride+1) )/6;
					p0 ++;
					p1 ++;

					*p1 = ( *(p0-1) + *p0 + *(p0+1)
							+ *(p0-1 + m_stride) + *(p0+m_stride) + *(p0+m_stride+1) )/6;
					p0 ++;
					p1 ++;
				}
			}
			else if(i == m_height_origin - 1)
			{
				if(j==0)
				{
					//sx = 0; tx = 4;
					//sy = -m_stride; ty = 0;

					*p1 = (*p0 + *(p0+1) + *(p0-m_stride) + *(p0-m_stride+1)) >> 2;
					p0 ++;
					p1 ++;

					*p1 = (*p0 + *(p0+1) + *(p0-m_stride) + *(p0-m_stride+1)) >> 2;
					p0 ++;
					p1 ++;

					*p1 = (*p0 + *(p0+1) + *(p0-m_stride) + *(p0-m_stride+1)) >> 2;
					p0 ++;
					p1 ++;
				}
				else if(j == m_width_origin - 1)
				{
					//sx = -4; tx = 0;
					//sy = -m_stride; ty = 0;

					*p1 = (*p0 + *(p0-1) + *(p0-m_stride) + *(p0-m_stride-1)) >> 2;
					p0 ++;
					p1 ++;

					*p1 = (*p0 + *(p0-1) + *(p0-m_stride) + *(p0-m_stride-1)) >> 2;
					p0 ++;
					p1 ++;

					*p1 = (*p0 + *(p0-1) + *(p0-m_stride) + *(p0-m_stride-1)) >> 2;
					p0 ++;
					p1 ++;
				}
				else
				{
					//sx = -4; tx = 4;
					//sy = -m_stride; ty = 0;

					*p1 = ( *(p0-1) + *p0 + *(p0+1)
							+ *(p0-1 - m_stride) + *(p0-m_stride) + *(p0-m_stride+1) )/6;
					p0 ++;
					p1 ++;

					*p1 = ( *(p0-1) + *p0 + *(p0+1)
							+ *(p0-1 - m_stride) + *(p0-m_stride) + *(p0-m_stride+1) )/6;
					p0 ++;
					p1 ++;

					*p1 = ( *(p0-1) + *p0 + *(p0+1)
							+ *(p0-1 - m_stride) + *(p0-m_stride) + *(p0-m_stride+1) )/6;
					p0 ++;
					p1 ++;
				}
			}
			else
			{
				if(j==0)
				{
					//sx = 0; tx = 4;
					//sy = -m_stride; ty = m_stride;

					*p1 = ( *p0 + *(p0+1)
							+ *(p0+m_stride) + *(p0+m_stride+1)
							+ *(p0-m_stride) + *(p0-m_stride+1))/6;
					p0 ++;
					p1 ++;

					*p1 = ( *p0 + *(p0+1)
							+ *(p0+m_stride) + *(p0+m_stride+1)
							+ *(p0-m_stride) + *(p0-m_stride+1))/6;
					p0 ++;
					p1 ++;

					*p1 = ( *p0 + *(p0+1)
							+ *(p0+m_stride) + *(p0+m_stride+1)
							+ *(p0-m_stride) + *(p0-m_stride+1))/6;
					p0 ++;
					p1 ++;
				}
				else if(j == m_width_origin - 1)
				{
					//sx = -4; tx = 0;
					//sy = -m_stride; ty = m_stride;

					*p1 = ( *p0 + *(p0-1)
							+ *(p0+m_stride) + *(p0+m_stride-1)
							+ *(p0-m_stride) + *(p0-m_stride-1))/6;
					p0 ++;
					p1 ++;

					*p1 = ( *p0 + *(p0+1)
							+ *(p0+m_stride) + *(p0+m_stride-1)
							+ *(p0-m_stride) + *(p0-m_stride-1))/6;
					p0 ++;
					p1 ++;

					*p1 = ( *p0 + *(p0+1)
							+ *(p0+m_stride) + *(p0+m_stride-1)
							+ *(p0-m_stride) + *(p0-m_stride-1))/6;
					p0 ++;
					p1 ++;
				}
				else
				{
					//sx = -4; tx =  4;
					//sy = -m_stride; ty =  m_stride;

					*p1 = ( *(p0-4) + *p0 + *(p0+4)
							+ *(p0-4 - m_stride) + *(p0-m_stride) + *(p0-m_stride+4)
							+ *(p0-4 + m_stride) + *(p0+m_stride) + *(p0+m_stride+4) )/9;
					p0 ++;
					p1 ++;

					*p1 = ( *(p0-4) + *p0 + *(p0+4)
							+ *(p0-4 - m_stride) + *(p0-m_stride) + *(p0-m_stride+4)
							+ *(p0-4 + m_stride) + *(p0+m_stride) + *(p0+m_stride+4) )/9;
					p0 ++;
					p1 ++;

					*p1 = ( *(p0-4) + *p0 + *(p0+4)
							+ *(p0-4 - m_stride) + *(p0-m_stride) + *(p0-m_stride+4)
							+ *(p0-4 + m_stride) + *(p0+m_stride) + *(p0+m_stride+4) )/9;
					p0 ++;
					p1 ++;
				}
			}

#if 0
			for (int z = 0; z < 3; z++) //RGB

			{
				int pixels = 0;
				unsigned int p = 0;

				for (int y = sy; y <= ty; y += m_stride)
				{
					for (int x = sx; x <= tx; x += 4)
					{
						p += *(p0 + x + y);
						pixels ++;
					}
				}

				*p1 = (p)/pixels;

				p0 ++;
				p1 ++;

			}
#endif
			*p1 = 255; //ALPHA

			p0 ++;
			p1 ++;
		}

		p_src += m_stride;
		p_dst += m_stride;
	}

	free(m_pData);
	m_pData = m_pData_new;

	long time2 = zglGetCurrentTime();
	LOGI("zglImage::blur(%d,%d) -> %d(ms)", m_width_origin, m_height_origin, (int)(time2 - time1) );
}
#endif

/**
 * The matrix is below:
 *
 * | 1 2 1 |
 * | 2 4 2 |
 * | 1 2 1 |
 */
unsigned char* zglImage::gaussianBlur()
{
	if (m_pData == NULL
			|| (m_format != PIXEL_RGBA8888 && m_format != PIXEL_RGB888)) //TODO RGBA8888 only
	{
		return NULL;
	}

	int step;
	if (m_format == PIXEL_RGBA8888)
	{
		step = 4;
	}
	else
	{
		step = 3;
	}

	int total_pixel = m_width * m_height * step;
	unsigned char * m_pData_new = (unsigned char*) malloc(total_pixel);

	if (m_pData_new == NULL)
	{
		return NULL;
	}

	memcpy(m_pData_new, m_pData, total_pixel);

	unsigned char * p_src = m_pData;
	unsigned char * p_dst = m_pData_new;

	for (int i = 1; i < m_height_origin - 1; i++)
	{
		unsigned char * p0 = p_src;
		unsigned char * p1 = p_dst;

		for (int j = 1; j < m_width_origin - 1; j++)
		{

			*p1 = (((*(p0 - step)) << 1) + ((*p0) << 2) + ((*(p0 + step)) << 1)
					+ *(p0 - step - m_stride) + ((*(p0 - m_stride)) << 1)
					+ *(p0 - m_stride + step) + *(p0 - step + m_stride)
					+ ((*(p0 + m_stride)) << 1) + *(p0 + m_stride + step)) >> 4;
			p0++;
			p1++;

			*p1 = (((*(p0 - step)) << 1) + ((*p0) << 2) + ((*(p0 + step)) << 1)
					+ *(p0 - step - m_stride) + ((*(p0 - m_stride)) << 1)
					+ *(p0 - m_stride + step) + *(p0 - step + m_stride)
					+ ((*(p0 + m_stride)) << 1) + *(p0 + m_stride + step)) >> 4;
			p0++;
			p1++;

			*p1 = (((*(p0 - step)) << 1) + ((*p0) << 2) + ((*(p0 + step)) << 1)
					+ *(p0 - step - m_stride) + ((*(p0 - m_stride)) << 1)
					+ *(p0 - m_stride + step) + *(p0 - step + m_stride)
					+ ((*(p0 + m_stride)) << 1) + *(p0 + m_stride + step)) >> 4;
			p0++;
			p1++;

			if (step)
			{
				*p1 = 255; //ALPHA
				p0++;
				p1++;
			}
		}

		p_src += m_stride;
		p_dst += m_stride;
	}

	//free(m_pData);

	//m_pData = m_pData_new;
	return m_pData_new;
}

unsigned char* zglImage::gaussBlur(int radiaus)
{
	if (m_pData == NULL
			|| (m_format != PIXEL_RGBA8888 && m_format != PIXEL_RGB888)) //TODO RGBA8888 only
	{
		return NULL;
	}

	if (radiaus < 1 || radiaus > 7)
	{
		return NULL;
	}

	unsigned char * p = NULL;

	int step;
	if (m_format == PIXEL_RGBA8888)
	{
		step = 4;
	}
	else
	{
		step = 3;
	}

	int total_pixel = m_width * m_height * step;
	unsigned char * m_pData_new = (unsigned char*) malloc(total_pixel);

	if (m_pData_new == NULL)
	{
		return NULL;
	}

	memcpy(m_pData_new, m_pData, total_pixel);

	int* sumRed = (int*) malloc(m_width_origin * sizeof(int));
	int* sumGreen = (int*) malloc(m_width_origin * sizeof(int));
	int* sumBlue = (int*) malloc(m_width_origin * sizeof(int));
	int* sumAlpha = (int*) malloc(m_width_origin * sizeof(int));
	int* sumPixel = (int*) malloc(m_width_origin * sizeof(int));

	memset(sumRed, 0, sizeof(int) * m_width_origin);
	memset(sumGreen, 0, sizeof(int) * m_width_origin);
	memset(sumBlue, 0, sizeof(int) * m_width_origin);
	memset(sumAlpha, 0, sizeof(int) * m_width_origin);
	memset(sumPixel, 0, sizeof(int) * m_width_origin);

	//!< (0,0)
	for (int x = 0; x <= radiaus; x++)
	{
		for (int y = 0; y <= radiaus; y++)
		{
			p = m_pData + m_stride * y + x * 4;
			sumRed[0] += p[0]; //R
			sumGreen[0] += p[1]; //G
			sumBlue[0] += p[2]; //B
			sumAlpha[0] += p[3]; //A
			sumPixel[0]++;
		}
	}

	m_pData_new[0] = (sumRed[0] * g_factor[sumPixel[0] - 1]) >> 16;
	m_pData_new[1] = (sumGreen[0] * g_factor[sumPixel[0] - 1]) >> 16;
	m_pData_new[2] = (sumBlue[0] * g_factor[sumPixel[0] - 1]) >> 16;
	m_pData_new[3] = (sumAlpha[0] * g_factor[sumPixel[0] - 1]) >> 16;

	//!< (x~, 0)
	for (int x = 1; x < m_width_origin; x++)
	{
		int left = x - radiaus - 1;
		int right = x + radiaus;

		sumRed[x] = sumRed[x - 1];
		sumGreen[x] = sumGreen[x - 1];
		sumBlue[x] = sumBlue[x - 1];
		sumAlpha[x] = sumAlpha[x - 1];
		sumPixel[x] = sumPixel[x - 1];

		for (int y = 0; y <= radiaus; y++)
		{
			if (y < m_height_origin)
			{
				//Remove left part
				if (left >= 0)
				{
					p = m_pData + m_stride * y + left * 4;
					sumRed[x] -= p[0];
					sumGreen[x] -= p[1];
					sumBlue[x] -= p[2];
					sumAlpha[x] -= p[3];
					sumPixel[x]--;
				}

				//Add right part
				if (right < m_width_origin)
				{
					p = m_pData + m_stride * y + right * 4;
					sumRed[x] += p[0]; //R
					sumGreen[x] += p[1]; //G
					sumBlue[x] += p[2]; //B
					sumAlpha[x] += p[3]; //A
					sumPixel[x]++;
				}
			}
		}

		//Update the dest
		int z = x * 4;

		m_pData_new[z + 0] = (sumRed[x] * g_factor[sumPixel[x] - 1]) >> 16;
		m_pData_new[z + 1] = (sumGreen[x] * g_factor[sumPixel[x] - 1]) >> 16;
		m_pData_new[z + 2] = (sumBlue[x] * g_factor[sumPixel[x] - 1]) >> 16;
		m_pData_new[z + 3] = (sumAlpha[x] * g_factor[sumPixel[x] - 1]) >> 16;
	}

	//y 1 ->  m_height_origin
	for (int y = 1; y < m_height_origin; y++)
	{
		int top = y - radiaus - 1;
		int bottom = y + radiaus;

		int sumRowRed = 0;
		int sumRowGreen = 0;
		int sumRowBlue = 0;
		int sumRowAlpha = 0;

		unsigned char * pTempT = m_pData + m_stride * top;
		unsigned char * pTempB = m_pData + m_stride * bottom;

		unsigned char * pLineDest = m_pData_new + m_stride * y;

		//The first pixel of current line
		for (int x = 0; x <= radiaus; x++)
		{
			if (x < m_width_origin)
			{
				if (top >= 0)
				{
					p = pTempT + x * 4;
					sumRowRed -= p[0];
					sumRowGreen -= p[1];
					sumRowBlue -= p[2];
					sumRowAlpha -= p[3];

					sumPixel[0]--;
				}

				if (bottom < m_height_origin)
				{
					p = pTempB + x * 4;
					sumRowRed += p[0];
					sumRowGreen += p[1];
					sumRowBlue += p[2];
					sumRowAlpha += p[3];

					sumPixel[0]++;
				}
			}
		}

		sumRed[0] += sumRowRed; //R
		sumGreen[0] += sumRowGreen; //G
		sumBlue[0] += sumRowBlue; //B
		sumAlpha[0] += sumRowAlpha; //A

		//Update the dest
		p = pLineDest;
		p[0] = (sumRed[0] * g_factor[sumPixel[0] - 1]) >> 16;
		p[1] = (sumGreen[0] * g_factor[sumPixel[0] - 1]) >> 16;
		p[2] = (sumBlue[0] * g_factor[sumPixel[0] - 1]) >> 16;
		p[3] = (sumAlpha[0] * g_factor[sumPixel[0] - 1]) >> 16;

		//The remaining part of current line
		for (int x = 1; x < m_width_origin; x++)
		{
#if 0
			int left = x - radiaus;
			int right = x + radiaus;

			sumRowRed = 0;
			sumRowGreen = 0;
			sumRowBlue = 0;

			for (int i = left; i <= right; i++)
			{
				if (i >= 0 && i < m_width_origin)
				{
					if (top >= 0)
					{
						p = pTempT + left * 4;
						sumRowRed -= p[0];
						sumRowGreen -= p[1];
						sumRowBlue -= p[2];

						sumPixel[x]--;
					}

					if (bottom < m_height_origin)
					{
						p = pTempB + left * 4;
						sumRowRed += p[0];
						sumRowGreen += p[1];
						sumRowBlue += p[2];
						sumPixel[x]++;
					}
				}
			}

			sumRed[x] += sumRowRed; //R
			sumGreen[x] += sumRowGreen;//G
			sumBlue[x] += sumRowBlue;//B

			p = pLineDest + x * 4;
			p[0] = (sumRed[x] * factor[sumPixel[x] - 1]) >> 16;
			p[1] = (sumGreen[x] * factor[sumPixel[x] - 1]) >> 16;
			p[2] = (sumBlue[x] * factor[sumPixel[x] - 1]) >> 16;

#else

			int left = x - radiaus - 1;

			if (left >= 0)
			{
				if (top >= 0)
				{
					p = pTempT + left * 4;
					sumRowRed += p[0];
					sumRowGreen += p[1];
					sumRowBlue += p[2];
					sumRowAlpha += p[3];
				}

				if (bottom < m_height_origin)
				{
					p = pTempB + left * 4;
					sumRowRed -= p[0];
					sumRowGreen -= p[1];
					sumRowBlue -= p[2];
					sumRowAlpha -= p[3];
				}
			}

			if (top < 0)
			{
				if ((x >= radiaus) && (x < m_width_origin - radiaus))
				{
					sumPixel[x] += (radiaus * 2 + 1);
				}
				else if (x < radiaus)
				{
					sumPixel[x] += (radiaus + x + 1);
				}
				else
				{
					sumPixel[x] += (radiaus + m_width_origin - x);
				}
			}

			if (bottom > m_height_origin)
			{
				if ((x >= radiaus) && (x < m_width_origin - radiaus))
				{
					sumPixel[x] -= (radiaus * 2 + 1);
				}
				else if (x < radiaus)
				{
					sumPixel[x] -= (radiaus + x + 1);
				}
				else
				{
					sumPixel[x] -= (radiaus + m_width_origin - x);
				}
			}

			int right = x + radiaus;
			if (right < m_width_origin)
			{
				if (top >= 0)
				{
					p = pTempT + right * 4;
					sumRowRed -= p[0];
					sumRowGreen -= p[1];
					sumRowBlue -= p[2];
					sumRowAlpha -= p[3];
				}

				if (bottom < m_height_origin)
				{
					p = pTempB + right * 4;
					sumRowRed += p[0];
					sumRowGreen += p[1];
					sumRowBlue += p[2];
					sumRowAlpha += p[3];
				}
			}

			sumRed[x] += sumRowRed; //R
			sumGreen[x] += sumRowGreen; //G
			sumBlue[x] += sumRowBlue; //B
			sumAlpha[x] += sumRowAlpha; //A

			p = pLineDest + x * 4;
			p[0] = (sumRed[x] * g_factor[sumPixel[x] - 1]) >> 16;
			p[1] = (sumGreen[x] * g_factor[sumPixel[x] - 1]) >> 16;
			p[2] = (sumBlue[x] * g_factor[sumPixel[x] - 1]) >> 16;
			p[3] = (sumAlpha[x] * g_factor[sumPixel[x] - 1]) >> 16;

			if (p[0] + 0x10 < 255)
				p[0] += 0x10;
			else
				p[0] = 255;

			if (p[1] + 0x10 < 255)
				p[1] += 0x10;
			else
				p[1] = 255;

			if (p[2] + 0x10 < 255)
				p[2] += 0x10;
			else
				p[2] = 255;
#endif
		}
	}

	//free( m_pData);

	//m_pData = m_pData_new;

	free(sumPixel);
	free(sumAlpha);
	free(sumBlue);
	free(sumGreen);
	free(sumRed);

	return m_pData_new;
}

void zglImage::sharp()
{

}

void zglImage::colorize(int color)
{

}

// ---------------------------------------------------------

zglImage::Buffer::Buffer()
{
	m_buffer = NULL;
	m_buffer_length = 0;
}

zglImage::Buffer::~Buffer()
{
	if (m_buffer != NULL)
	{
		free(m_buffer);
		m_buffer = NULL;

		m_buffer_length = 0;
	}
}

int zglImage::Buffer::getLength()
{
	return m_buffer_length;
}

void * zglImage::Buffer::load()
{
	return m_buffer;
}

void zglImage::Buffer::save(void * buffer, int length)
{
	if (length <= 0)
	{
		return;
	}

	if (m_buffer != NULL)
	{
		if (m_buffer_length != length)
		{
			free(m_buffer);
			m_buffer = NULL;

			m_buffer_length = 0;
		}
		else
		{
			memset(m_buffer, 0, m_buffer_length);
		}
	}

	if (m_buffer == NULL)
	{
		m_buffer = malloc(length);
		m_buffer_length = length;
	}

	memcpy(m_buffer, buffer, m_buffer_length);
}

void zglImage::Buffer::clear()
{
	if (m_buffer != NULL)
	{
		free(m_buffer);
		m_buffer = NULL;

		m_buffer_length = 0;
	}
}

// ---------------------------------------------------------

zglImage::MatrixBuffer::MatrixBuffer()
{
	m_buffer = NULL;
	m_buffer_header = NULL;
	m_buffer_width = 0;
	m_buffer_height = 0;
	m_buffer_unit = 0;

	m_buffer_stride = 0;
}

zglImage::MatrixBuffer::MatrixBuffer(int width, int height, int unitSize)
{
	m_buffer = NULL;
	m_buffer_header = NULL;
	m_buffer_width = 0;
	m_buffer_height = 0;
	m_buffer_unit = 0;

	m_buffer_stride = 0;

	allocate(width, height, unitSize);
}

zglImage::MatrixBuffer::~MatrixBuffer()
{
	if (m_buffer != NULL)
	{
		free(m_buffer);
		m_buffer = NULL;
	}

	if (m_buffer_header != NULL)
	{
		free(m_buffer_header);
		m_buffer_header = NULL;
	}
	m_buffer_width = 0;
	m_buffer_height = 0;
	m_buffer_unit = 0;
}

bool zglImage::MatrixBuffer::allocate(int width, int height, int unitSize)
{
	int stride = width * unitSize;
	int total = stride * height;

	if (total <= 0)
	{
		return false;
	}

	//memory allocate the whole buffer.
	unsigned char * buffer = (unsigned char*) malloc(total);
	if (buffer == NULL)
	{
		return false;
	}

	//memory allocate the buffer index.
	unsigned char ** buffer_header = (unsigned char**) malloc(
			height * sizeof(unsigned char*));
	if (buffer_header == NULL)
	{
		free(buffer);
		buffer = NULL;
		return false;
	}

	int offset = 0;
	for (int i = 0; i < height; i++)
	{
		buffer_header[i] = &buffer[offset];
		offset += stride;
	}

	//Free the previous memory.
	if (m_buffer != NULL)
	{
		free(m_buffer);
		m_buffer = NULL;
	}

	if (m_buffer_header != NULL)
	{
		free(m_buffer_header);
		m_buffer_header = NULL;
	}

	//Update the memory members.
	m_buffer_width = width;
	m_buffer_height = height;
	m_buffer_unit = unitSize;

	m_buffer_stride = stride;

	m_buffer = buffer;
	m_buffer_header = buffer_header;

	return true;
}

void zglImage::MatrixBuffer::reset(int value)
{
	if (m_buffer != NULL)
	{
		memset(m_buffer, value,
				m_buffer_width * m_buffer_height * m_buffer_unit);
	}
}

void zglImage::MatrixBuffer::setValues(int x, int y, unsigned char* value,
		int length)
{
	unsigned char * p = m_buffer_header[y] + x * m_buffer_unit;
	while (length > 0)
	{
		*p++ = *value++;
		length--;
	}
}

unsigned char* zglImage::MatrixBuffer::getValues(int x, int y)
{
	unsigned char * p = m_buffer_header[y] + x * m_buffer_unit;
	return p;
}

//!< 低位在前，高位在后
void zglImage::MatrixBuffer::setIntValue(int x, int y, int value)
{
	unsigned char * p = m_buffer_header[y] + x * m_buffer_unit;

	switch (m_buffer_unit)
	{
	case 1:
		{
			*p = (unsigned char) ((value) & 0xff);
		}
		break;

	case 2:
		{
			*p = (unsigned char) ((value) & 0xff);
			*(p + 1) = (unsigned char) ((value >> 8) & 0xff);
		}
		break;

	case 3:
		{
			*p = (unsigned char) ((value) & 0xff);
			*(p + 1) = (unsigned char) ((value >> 8) & 0xff);
			*(p + 2) = (unsigned char) ((value >> 16) & 0xff);
		}
		break;

	case 4:
		{
			*p = (unsigned char) ((value) & 0xff);
			*(p + 1) = (unsigned char) ((value >> 8) & 0xff);
			*(p + 2) = (unsigned char) ((value >> 16) & 0xff);
			*(p + 3) = (unsigned char) ((value >> 24) & 0xff);
		}
		break;
	default:
		break;
	}
}

//!< 低位在前，高位在后, 0x12345678 --> 78 56 34 12
int zglImage::MatrixBuffer::getIntValue(int x, int y)
{
	unsigned char * p = m_buffer_header[y] + x * m_buffer_unit;

	int ret = 0;
	switch (m_buffer_unit)
	{
	case 1:
		{
			unsigned char v = *p;
			ret = v;
		}
		break;

	case 2:
		{
			unsigned short v = *p;
			ret = v;
		}
		break;

	case 3:
		{
			ret = (*p) + ((*p + 1) >> 8) + ((*p + 2) >> 16);
		}
		break;

	case 4:
		{
			ret = (*p) + ((*p + 1) >> 8) + ((*p + 2) >> 16) + ((*p + 3) >> 24);
		}
		break;
	default:
		break;
	}

	return ret;
}

void zglImage::MatrixBuffer::clear()
{
	if (m_buffer != NULL)
	{
		free(m_buffer);
		m_buffer = NULL;
	}

	if (m_buffer_header != NULL)
	{
		free(m_buffer_header);
		m_buffer_header = NULL;
	}
	m_buffer_width = 0;
	m_buffer_height = 0;
	m_buffer_unit = 0;
}

bool zglImage::MatrixBuffer::saveBitmap(const unsigned char * buffer,
		unsigned int width, unsigned int height, int format,
		unsigned int stride, unsigned int x, unsigned int y)
{
	if (x >= m_buffer_width || y >= m_buffer_height)
	{
		return false;
	}

	if (m_buffer_unit == 2)
	{
		if (format == 4) //ANDROID_BITMAP_FORMAT_RGB_565
		{
			unsigned char * p = NULL;
			unsigned char* p_srcLine = (unsigned char*) buffer;

			for (unsigned int yy = 0; yy < height; yy++)
			{
				if (yy + y >= m_buffer_height)
				{
					break;
				}

				p = m_buffer_header[yy + y] + x * m_buffer_unit;

				unsigned char * p_src = p_srcLine;
				for (unsigned int xx = 0; xx < width; xx++)
				{
					if (x + xx >= m_buffer_width)
					{
						break;
					}

					*p++ = *p_src++; //!<
					*p++ = *p_src++; //!<
				}

				p_srcLine += stride;
			}

			return true;
		}
	}
	else if (m_buffer_unit == 4)
	{
		if (format == 1) //ANDROID_BITMAP_FORMAT_RGBA_8888
		{
			unsigned char * p = NULL;
			unsigned char* p_srcLine = (unsigned char*) buffer;

			for (unsigned int yy = 0; yy < height; yy++)
			{
				if (yy + y >= m_buffer_height)
				{
					break;
				}

				p = m_buffer_header[yy + y] + x * m_buffer_unit;

				unsigned char * p_src = p_srcLine;
				for (unsigned int xx = 0; xx < width; xx++)
				{
					if (x + xx >= m_buffer_width)
					{
						break;
					}

					*p++ = *p_src++; //!< r
					*p++ = *p_src++; //!< g
					*p++ = *p_src++; //!< b
					*p++ = *p_src++; //!< a
				}

				p_srcLine += stride;
			}

			return true;
		}
		else if (format == 4) //ANDROID_BITMAP_FORMAT_RGB_565
		{
			unsigned char * p = NULL;
			unsigned char* p_srcLine = (unsigned char*) buffer;
			unsigned short pixel;

			for (unsigned int yy = 0; yy < height; yy++)
			{
				if (yy + y >= m_buffer_height)
				{
					break;
				}

				p = m_buffer_header[yy + y] + x * m_buffer_unit;

				unsigned short * p_src = (unsigned short *) (p_srcLine);
				for (unsigned int xx = 0; xx < width; xx++)
				{
					if (x + xx >= m_buffer_width)
					{
						break;
					}

					pixel = *p_src;

					*p++ = (pixel >> 8) & 0xf8;
					*p++ = (pixel >> 3) & 0xfc;
					*p++ = (pixel << 3) & 0xf8;
					*p++ = 0xff;

					p_src++;
				}

				p_srcLine += stride;
			}

			return true;
		}
	}

	return false;
}

bool zglImage::MatrixBuffer::loadBitmap(unsigned char * buffer,
		unsigned int width, unsigned int height, int format,
		unsigned int stride, unsigned int x, unsigned int y)
{
	if (x >= m_buffer_width || y >= m_buffer_height)
	{
		return false;
	}

	int bmp_total = height * stride;
	memset(buffer, 0, bmp_total);

	if (m_buffer_unit == 2)
	{
		if (format == 1) //ANDROID_BITMAP_FORMAT_RGBA_8888
		{
			unsigned char * p_dstLine = (unsigned char*) buffer;

			for (unsigned int yy = 0; yy < height; yy++)
			{
				if (yy + y >= m_buffer_height)
				{
					break;
				}

				unsigned char * p_dst = p_dstLine;

				unsigned short * p = (unsigned short *)(m_buffer_header[yy + y] + x * m_buffer_unit);
				for (unsigned int xx = 0; xx < width; xx++)
				{
					if (x + xx >= m_buffer_width)
					{
						break;
					}

					*p_dst++ = ((*p) >> 8) & 0xf8;//r
					*p_dst++ = ((*p) >> 3) & 0xfc;//g
					*p_dst++ = ((*p) << 3) & 0xf8;//b
					*p_dst++ = 0xff;//a
				}

				p_dstLine += stride;
			}
			return true;
		}
		else if (format == 4) //ANDROID_BITMAP_FORMAT_RGB_565
		{
			unsigned char * p_dstLine = (unsigned char*) buffer;

			for (unsigned int yy = 0; yy < height; yy++)
			{
				if (yy + y >= m_buffer_height)
				{
					break;
				}

				unsigned short * p_dst = (unsigned short *) p_dstLine;
				unsigned short * p = (unsigned short *) (m_buffer_header[yy + y] + x * m_buffer_unit);
				for (unsigned int xx = 0; xx < width; xx++)
				{
					if (x + xx >= m_buffer_width)
					{
						break;
					}

					*p_dst++ = *p++;
				}

				p_dstLine += stride;
			}
			return true;
		}

	}
	else if (m_buffer_unit == 4)
	{
		if (format == 1) //ANDROID_BITMAP_FORMAT_RGBA_8888
		{
			unsigned char * p = NULL;
			unsigned char* p_dstLine = (unsigned char*) buffer;

			for (unsigned int yy = 0; yy < height; yy++)
			{
				unsigned char * p_dst = p_dstLine;

				if (yy + y >= m_buffer_height)
				{
					break;
				}

				p = m_buffer_header[yy + y] + x * m_buffer_unit;
				for (unsigned int xx = 0; xx < width; xx++)
				{
					if (x + xx >= m_buffer_width)
					{
						break;
					}

					*p_dst++ = *p++;
					*p_dst++ = *p++;
					*p_dst++ = *p++;
					*p_dst++ = *p++;
				}

				p_dstLine += stride;
			}

			return true;
		}
		else if (format == 4) //ANDROID_BITMAP_FORMAT_RGB_565
		{
			unsigned char * p = NULL;
			unsigned char* p_dstLine = (unsigned char*) buffer;
			unsigned short pixel;

			for (unsigned int yy = 0; yy < height; yy++)
			{
				if (yy + y >= m_buffer_height)
				{
					break;
				}

				p = m_buffer_header[yy + y] + x * m_buffer_unit;

				unsigned short * p_dst = (unsigned short *) (p_dstLine);
				for (unsigned int xx = 0; xx < width; xx++)
				{
					if (x + xx <= m_buffer_width)
					{
						break;
					}

					pixel = ((*p) >> 3 << 11) | ((*p + 1) >> 2 << 5) | ((*p + 2) >> 3);
					*p_dst = pixel;

					p_dst++;
				}
			}
			return true;
		}
	}

	return false;
}

