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

#include <stdio.h> //!< File open
#include <stdlib.h> //!< NULL
#include <string.h>
#include "zglBitmapTextView.h"

const float zglBitmapTextView::m_stride = 1.0f / 16.0f;

zglBitmapTextView::zglBitmapTextView()
{
	m_text_length = 0;
	m_used_length = 0;

	m_fontInfo = NULL;
	m_texture = NULL;

	m_aligh = TEXTVIEW_ALIGN_LEFT;

	m_x = 0;
	m_y = 0;

	m_t_width = 0;
	m_t_height = 0;
	m_start_x = 0;
	m_start_y = 0;

	m_flag |= FLAG_DISABLE; //!< Disable the Text

	m_map_flag = FLAG_MAPPING_STATIC;
}

void zglBitmapTextView::init(const zglTexture * texture, const zglFontInfo * fontInfo,
		int align)
{
	zglWidget2D::init();

	m_texture = (zglTexture*) texture;
	m_fontInfo = (zglFontInfo*) fontInfo;

	m_aligh = align;
}

void zglBitmapTextView::setColor(unsigned int color)
{
	int r = (color >> 24) & 0xff;
	int g = (color >> 16) & 0xff;
	int b = (color >> 8) & 0xff;
	int a = (color) & 0xff;
	for (int i = 0; i < m_max_font * 4; i++)
	{
		m_text_vertex[i].r = r;
		m_text_vertex[i].g = g;
		m_text_vertex[i].b = b;
		m_text_vertex[i].a = a;
	}

	applyVertexUpdate();
}

void zglBitmapTextView::updateDimension()
{
}

bool zglBitmapTextView::hitTest(float x, float y)
{
	return false;
}

void zglBitmapTextView::setAlign(int align)
{
	m_aligh = align;
}

void zglBitmapTextView::layout(int x, int y, const char * text)
{
	m_x = x;
	m_y = y;

	updateDimension(text);

	switch(m_aligh)
	{
	default:
	case TEXTVIEW_ALIGN_LEFT:
		m_start_x = x;
		break;
	case TEXTVIEW_ALIGN_CENTER:
		m_start_x = x - (m_t_width >> 1);
		break;

	case TEXTVIEW_ALIGN_RIGHT:
		m_start_x = x - m_t_width;
		break;
	}

	m_start_y = y + (m_t_height >> 1);

	int length = strlen(text);

	if(length >= m_max_font)
	{
		length = m_max_font;
	}

	char c;
	int startx = m_start_x;

	//!< 1--> 2
	//!< |    |
	//!< 0 <--3
	if((m_fontInfo != NULL) && m_fontInfo->available())
	{
		if(m_texture == NULL) return;

		float w = (float)m_texture->getWidth();
		float h = (float)m_texture->getHeight();

		for(int i=0; i<length; i++)
		{
			int v = i << 2;
			c = text[i];
			if(c > 0x7f || c < ' ') c = '?';

			int offset = c - ' ';

			m_text_vertex[v + 0].x = startx;
			m_text_vertex[v + 0].y = m_start_y;
			m_text_vertex[v + 0].z = 0.0f;
			m_text_vertex[v + 0].u = m_fontInfo->m_x[offset]/w;
			m_text_vertex[v + 0].v = m_fontInfo->m_y[offset]/h;

			m_text_vertex[v + 1].x = startx;
			m_text_vertex[v + 1].y = m_start_y - m_fontInfo->m_size;
			m_text_vertex[v + 1].z = 0.0f;

			m_text_vertex[v + 1].u = m_fontInfo->m_x[offset]/w;
			m_text_vertex[v + 1].v = (m_fontInfo->m_y[offset] + m_fontInfo->m_size)/h;


			m_text_vertex[v + 2].x = startx + m_fontInfo->m_w[offset];
			m_text_vertex[v + 2].y = m_start_y - m_fontInfo->m_size;
			m_text_vertex[v + 2].z = 0.0f;
			m_text_vertex[v + 2].u = (m_fontInfo->m_x[offset] + m_fontInfo->m_w[offset])/w;
			m_text_vertex[v + 2].v = (m_fontInfo->m_y[offset] + m_fontInfo->m_size)/h;

			m_text_vertex[v + 3].x = startx + m_fontInfo->m_w[offset];
			m_text_vertex[v + 3].y = m_start_y;
			m_text_vertex[v + 3].z = 0.0f;
			m_text_vertex[v + 3].u = (m_fontInfo->m_x[offset] + m_fontInfo->m_w[offset])/w;
			m_text_vertex[v + 3].v = m_fontInfo->m_y[offset]/h;

			startx += m_fontInfo->m_w[offset];
		}
	}
	else
	{
		for(int i=0; i<length; i++)
		{
			int v = i << 2;

			c = text[i];
			if(c > 0x7f || c < ' ') c = '?';
			c -= ' ';

			float cx = (float) (c % 16) / 16.0f;
			float cy = (float) (c / 16) / 16.0f;

			m_text_vertex[v + 0].x = startx;
			m_text_vertex[v + 0].y = m_start_y;
			m_text_vertex[v + 0].z = 0.0f;
			m_text_vertex[v + 0].u = cx;
			m_text_vertex[v + 0].v = cy;

			m_text_vertex[v + 1].x = startx;
			m_text_vertex[v + 1].y = m_start_y - 16;
			m_text_vertex[v + 1].z = 0.0f;
			m_text_vertex[v + 1].u = cx;
			m_text_vertex[v + 1].v = cy + m_stride;

			m_text_vertex[v + 2].x = startx + 16;
			m_text_vertex[v + 2].y = m_start_y - 16;
			m_text_vertex[v + 2].z = 0.0f;
			m_text_vertex[v + 2].u = cx + m_stride;
			m_text_vertex[v + 2].v = cy + m_stride;

			m_text_vertex[v + 3].x = startx + 16;
			m_text_vertex[v + 3].y = m_start_y;
			m_text_vertex[v + 3].z = 0.0f;
			m_text_vertex[v + 3].u = cx + m_stride;
			m_text_vertex[v + 3].v = cy;

			startx += 16;
		}
	}

	m_prim.init(DRAW_QUADS, m_text_vertex, (length << 2), m_texture);
	m_prim.setBlendType(BLEND_HALF);
	m_prim.setSortOffset(m_default_offset);//!< Make the default Text floating before the others.

	applyVertexUpdate();
}

void zglBitmapTextView::updateDimension(const char * text)
{
	int length = strlen(text);
	if(m_fontInfo == NULL
			|| ((m_fontInfo != NULL) && !m_fontInfo->available()) )
	{
		m_t_width = length * 16;//Fixed size; The Other parameter will not supported.
		m_t_height = 16;//Fixed size;
	}
	else
	{
		char c;
		int len = 0;
		for(int i=0; i<length; i++)
		{
			c = text[i];

			if(c > 0x7f || c < ' ') c = '?';

			c -= ' ';

			len += m_fontInfo->m_w[(int)c];
		}

		m_t_width = len;
		m_t_height = m_fontInfo->m_size;
	}
}

//!< --------------------------------------------------------------------------
zglFontInfo::zglFontInfo()
{
    m_first = m_last = ' ';
    m_size = 0;
}

bool zglFontInfo::loadFontInfo(const char* file)
{
	if (file != NULL)
	{
		FILE * pFile = NULL;
		pFile = fopen(file, "rb");

		int size = fread(this, 1, sizeof(zglFontInfo), pFile);
		fclose(pFile);

		if(size != sizeof(zglFontInfo))
		{
			m_first = m_last = ' ';
			return false;
		}

		return true;
	}
	return false;
}

bool zglFontInfo::loadFontInfoBuffer(const char* buffer)
{
	if(buffer != NULL)
	{
		zglFontInfo* info = (zglFontInfo*)buffer;
		this->m_first = info->m_first;
		this->m_last = info->m_last;
		this->m_size = info->m_size;

		memcpy(this->m_x, info->m_x, sizeof(unsigned short) * info->m_charCount);
		memcpy(this->m_y, info->m_y, sizeof(unsigned short) * info->m_charCount);
		memcpy(this->m_w, info->m_w, sizeof(unsigned short) * info->m_charCount);
	}
	return false;
}

bool zglFontInfo::available()
{
	return m_first != m_last;
}


