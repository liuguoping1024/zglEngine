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

#include <stdio.h> //FILE
#include <stdlib.h>
#include "zglImage.h"
#include "zglImageLoader.h"

#include "zglUtil.h"	//!< Power2 calculation

#if defined(linux) && (defined(i386) || defined(__x86_64))
//#if defined(i386) //!< PC(MINGW & UBUNTU)
/**
 * @notes
 * libpng-1.2.x is supported.
 * Due to the great changes in libpng-1.4.x, the libpng-1.4.x is skipped.
 */
#include <png.h>
#endif

enum BMPMagic
{
	MB = 19778
};

typedef struct
{
	unsigned short type; /* Magic identifier            */
	unsigned int size; /* File size in bytes          */
	unsigned short reserved1, reserved2;
	unsigned int offset; /* Offset to image data, bytes */
} BMPHEADER;

typedef struct
{
	unsigned int size; /* Header size in bytes      */
	int width, height; /* Width and height of image */
	unsigned short int planes; /* Number of colour planes   */
	unsigned short int bits; /* Bits per pixel            */
	unsigned int compression; /* Compression type          */
	unsigned int imagesize; /* Image size in bytes       */
	int xresolution, yresolution; /* Pixels per meter          */
	unsigned int ncolours; /* Number of colours         */
	unsigned int importantcolours; /* Important colours         */
} INFOHEADER;

typedef struct /**** Colormap entry structure ****/
{
	unsigned char b; /* Blue value */
	unsigned char g; /* Green value */
	unsigned char r; /* Red value */
	unsigned char v; /* Reserved */
} RGBQUAD;

zglImageLoader::zglImageLoader()
{
	m_width = 0;
	m_height = 0;
	m_pData = NULL;
	m_stride = 0;
	m_colorDepth = 0;
}

zglImageLoader::~zglImageLoader()
{
	if (m_pData != NULL)
	{
		free( m_pData);
		m_pData = NULL;
	}
}

bool zglImageLoader::isQualifiedTexture()
{
	if (m_pData == NULL)
		return false;

	return zglIsPowerOf2(m_width) && zglIsPowerOf2(m_height);
}

bool zglImageLoader::loadBMP(const char* filename)
{
	FILE *fp = fopen(filename, "rb");
	if (!fp)
		return false;

	clear();

	fseek(fp, 0, SEEK_END);
	size_t filelen = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	if (filelen == 0)
	{
		fclose(fp);
		return false;
	}

	BMPHEADER header;
	INFOHEADER infor;
	size_t read;

	read = fread(&header.type, sizeof(short), 1, fp);
	read = fread(&header.size, sizeof(int), 1, fp);
	read = fread(&header.reserved1, sizeof(short), 1, fp);
	read = fread(&header.reserved2, sizeof(short), 1, fp);
	read = fread(&header.offset, sizeof(int), 1, fp);

	read = fread(&infor, sizeof(INFOHEADER), 1, fp);

	m_width = infor.width;
	m_height = infor.height;
	m_colorDepth = infor.bits;

	//expected
	int s_width;
	int s_size;
	unsigned char* pt;
	unsigned char pv;

	switch (m_colorDepth)
	{
	case 8:
		{
			//8 Bit index -> 32 RGBA8888 (ANDROID_BITMAP_FORMAT_RGBA_8888)
			RGBQUAD bmiColors[256];
			read = fread(bmiColors, sizeof(RGBQUAD), 256, fp); //Read the platte

			int t_width = ((infor.bits * infor.width + 31) & ~31) / 8;

			m_colorDepth = 32;
			s_width = ((m_colorDepth * infor.width + 31) & ~31) / 8;
			s_size = s_width * infor.height;

			m_stride = s_width;

			unsigned char* pTempData = (unsigned char*) malloc(t_width);

			m_pData = (unsigned char*) malloc(s_size);//The output buffer

			unsigned char * p = m_pData + s_width * (infor.height - 1);

			for (int i = 0; i < m_height; i++)
			{
				read = fread(pTempData, t_width, 1, fp);

				pt = (unsigned char*) p;
				for (int j = 0; j < m_width; j++)
				{
					*pt = bmiColors[pTempData[j]].r;
					*(pt + 1) = bmiColors[pTempData[j]].g;
					*(pt + 2) = bmiColors[pTempData[j]].b;
					*(pt + 3) = bmiColors[pTempData[j]].v;
					pt += 4;
				}

				p -= s_width;
			}

			free(pTempData);
		}
		break;

	case 24:
		{
			s_width = ((infor.bits * infor.width + 31) & ~31) / 8;
			s_size = s_width * infor.height;

			m_stride = s_width;

			m_pData = (unsigned char*) malloc(s_size);

			unsigned char * p = m_pData + s_width * (infor.height - 1);
			for (int i = 0; i < m_height; i++)
			{
				read = fread(p, s_width, 1, fp);
				pt = (unsigned char*) p;
				for (int j = 0; j < m_width; j++)
				{
					pv = *pt;
					*pt = *(pt + 2);
					*(pt + 2) = pv;
					pt += 3;
				}

				p -= s_width;
			}
		}
		break;

	case 32:
		{
			s_width = ((infor.bits * infor.width + 31) & ~31) / 8;
			s_size = s_width * infor.height;

			m_stride = s_width;

			m_pData = (unsigned char*) malloc(s_size);

			unsigned char * p = m_pData + s_width * (infor.height - 1);
			for (int i = 0; i < m_height; i++)
			{
				read = fread(p, s_width, 1, fp);
				pt = (unsigned char*) p;
				for (int j = 0; j < m_width; j++)
				{
					pv = *pt;
					*pt = *(pt + 2);
					*(pt + 2) = pv;
					pt += 4;
				}

				p -= s_width;
			}

		}
		break;

	};

	fclose(fp);

	return true;
}

int zglImageLoader::getFormat()
{
	switch (m_colorDepth)
	{
	case 32:
		return PIXEL_RGBA8888;

	case 24:
		return PIXEL_RGB888;

	case 16:
		return PIXEL_RGB565;

	default:
		return PIXEL_NONE;
	}
}

#if defined(linux) && (defined(i386) || defined(__x86_64))
//#if defined(i386) //!< PC(MINGW & UBUNTU)
static void PNGAPI user_error_fn(png_structp png_ptr, png_const_charp message)
{
}

static void PNGAPI user_warning_fn(png_structp png_ptr, png_const_charp message)
{
}
#endif

bool zglImageLoader::loadPNG(const char * filename)
{
#if defined(linux) && (defined(i386) || defined(__x86_64))
//#if defined(i386) //!< PC(MINGW & UBUNTU)
	FILE *fp;
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	png_uint_32 width, height;
	int bit_depth, color_type;

	if((fp = fopen(filename, "rb")) == NULL)
	{
		return false;
	}

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
			NULL, user_error_fn, user_warning_fn);
	if (png_ptr == 0)
	{
		fclose(fp);
		return false;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL)
	{
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return false;
	}

	if (setjmp(png_jmpbuf(png_ptr)))
	{
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		return false;
	}

	//!< Clear the former information of the image holder.
	clear();

	png_init_io(png_ptr, fp);
	png_set_sig_bytes(png_ptr, sig_read);
	png_read_info(png_ptr, info_ptr);

	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,NULL, NULL, NULL);
	m_width = width;
	m_height = height;

	switch(color_type)
	{
		case PNG_COLOR_TYPE_GRAY: m_colorDepth = 8; break;
		case PNG_COLOR_TYPE_GRAY_ALPHA: m_colorDepth = 16; break;
		case PNG_COLOR_TYPE_RGB: m_colorDepth = 24; break;
		case PNG_COLOR_TYPE_RGB_ALPHA: m_colorDepth = 32; break;
	}

	switch(color_type)
	{
		case PNG_COLOR_TYPE_PALETTE:
		{
			png_set_palette_to_rgb(png_ptr);
			m_colorDepth = 24;
		}
		break;

		case PNG_COLOR_TYPE_GRAY:
		{
			if(bit_depth < 8)
			{
				png_set_expand_gray_1_2_4_to_8(png_ptr);
			}
		}
		break;
	}

	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS))
	{
		png_set_tRNS_to_alpha(png_ptr);
		m_colorDepth = 24;
	}

	if (bit_depth == 16)
	{
		png_set_strip_16(png_ptr);
	}
	else if (bit_depth < 8)
	{
		png_set_packing(png_ptr);
	}

	png_uint_32 rowbytes = width * (m_colorDepth>>3);

	m_pData = (unsigned char*)malloc(rowbytes * height);

	for(png_uint_32 y=0; y < height; y++)
	{
		//png_bytep row = m_pData +  rowbytes * (height - y - 1);
		png_bytep row = m_pData + rowbytes * y;
		png_read_rows(png_ptr, &row, NULL, 1);
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
	fclose(fp);

	m_stride = rowbytes;
#endif
	return true;
}

void zglImageLoader::clear()
{
	if (m_pData != NULL)
	{
		free( m_pData);
		m_pData = NULL;
	}

	m_width = 0;
	m_height = 0;
	m_stride = 0;
	m_colorDepth = 0;
}



