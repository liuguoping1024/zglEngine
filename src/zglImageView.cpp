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

#include "zglImageView.h"

#include "zglEventType.h"

#include "zglUtil.h"
#include "zglImage.h"
#include "zglWidget.h"

/**
 *  0 -- 2
 *  |   /|
 *  |  / |
 *  | /  |
 *  1 -- 3
 */
zglImageView::zglImageView()
{
	m_width = 128.0f;
	m_height = 128.0f;

	//!< UV members for effector.
	m_uv_left = 0.0f;
	m_uv_right = 1.0f;
	m_uv_top = 0.0f;
	m_uv_bottom = 1.0f;

	m_shear_dy = 0.0f;
	m_shear_dx = 0.0f;

	m_antialias = false;

	for (int i = 0; i < 4; i++)
	{
		m_alpha[i] = m_vertex[i].a = 0xff;
		m_offset_x[i] = 0.0f;
		m_offset_y[i] = 0.0f;
		m_offset_z[i] = 0.0f;
	}

	m_effect_type = EFFECT_NONE;
	m_action_type = ACTION_FLIP_NONE;

	m_map_flag = FLAG_MAPPING_STATIC;
}

static const unsigned short g_index[36] = {
		0, 4, 5, 1,
		1, 5, 6, 2,
		2, 6, 7, 3,

		4, 8, 9, 5,
		5, 9, 10, 6,
		6, 10, 11, 7,

		8, 12, 13, 9,
		9, 13, 14, 10,
		10, 14, 15, 11
};

void zglImageView::init(zglTexture * texture, unsigned int width, unsigned int height, bool antialias)
{
	zglWidget3D::init();

	if(antialias)
	{
		//Left-top corner
		m_vertex[0].u = m_uv_left;
		m_vertex[0].v = m_uv_top;
		m_vertex[0].a = 0x00;

		m_vertex[1].u = m_uv_left;
		m_vertex[1].v = m_uv_top;
		m_vertex[1].a = 0x00;

		m_vertex[4].u = m_uv_left;
		m_vertex[4].v = m_uv_top;
		m_vertex[4].a = 0x00;

		m_vertex[5].u = m_uv_left;
		m_vertex[5].v = m_uv_top;
		m_vertex[AA_LEFT_TOP].a = m_alpha[0];

		//Left-bottom corner
		m_vertex[8].u = m_uv_left;
		m_vertex[8].v = m_uv_bottom;
		m_vertex[8].a = 0x00;

		m_vertex[9].u = m_uv_left;
		m_vertex[9].v = m_uv_bottom;
		m_vertex[AA_LEFT_BOTTOM].a = m_alpha[1];

		m_vertex[12].u = m_uv_left;
		m_vertex[12].v = m_uv_bottom;
		m_vertex[12].a = 0x00;

		m_vertex[13].u = m_uv_left;
		m_vertex[13].v = m_uv_bottom;
		m_vertex[13].a = 0x00;

		//Right-top corner
		m_vertex[2].u = m_uv_right;
		m_vertex[2].v = m_uv_top;
		m_vertex[2].a = 0x00;

		m_vertex[3].u = m_uv_right;
		m_vertex[3].v = m_uv_top;
		m_vertex[3].a = 0x00;

		m_vertex[6].u = m_uv_right;
		m_vertex[6].v = m_uv_top;
		m_vertex[AA_RIGHT_TOP].a = m_alpha[2];

		m_vertex[7].u = m_uv_right;
		m_vertex[7].v = m_uv_top;
		m_vertex[7].a = 0x00;

		//Right-bottom corner
		m_vertex[10].u = m_uv_right;
		m_vertex[10].v = m_uv_bottom;
		m_vertex[AA_RIGHT_BOTTOM].a = m_alpha[3];

		m_vertex[11].u = m_uv_right;
		m_vertex[11].v = m_uv_bottom;
		m_vertex[11].a = 0x00;

		m_vertex[14].u = m_uv_right;
		m_vertex[14].v = m_uv_bottom;
		m_vertex[14].a = 0x00;

		m_vertex[15].u = m_uv_right;
		m_vertex[15].v = m_uv_bottom;
		m_vertex[15].a = 0x00;

		m_prim.init(DRAW_QUADS, m_vertex, 16, (unsigned short *)g_index, 36, texture);
		//m_prim.init(DRAW_LINE_LOOP, m_vertex, 16, (unsigned short *)g_index, 36, texture);
	}
	else
	{
		m_vertex[0].setUV(m_uv_left, m_uv_top);
		m_vertex[1].setUV(m_uv_left, m_uv_bottom);
		m_vertex[2].setUV(m_uv_right, m_uv_top);
		m_vertex[3].setUV(m_uv_right, m_uv_bottom);

		m_prim.init(DRAW_TRI_STRIP, m_vertex, 4, texture);
	}

	m_antialias = antialias;

	m_prim.setBlendType(BLEND_HALF);

	//!< Update the dimension, and request updating the vertex.
	m_width = width;
	m_height = height;

	applyVertexUpdate();
}

void zglImageView::setDimension(unsigned int width, unsigned int height)
{
	m_width = width;
	m_height = height;

	//updateDimension();
	applyDimensionUpdate();
}

void zglImageView::setTexCoord(const float coord[])
{
	setTexCoord(coord[0], coord[1], coord[2], coord[3]);
}

void zglImageView::setTexCoord(float left, float top, float right, float bottom)
{
	m_uv_left = left;
	m_uv_right = right;
	m_uv_top = top;
	m_uv_bottom = bottom;

	if(m_antialias)
	{
		//Left-top corner
		m_vertex[0].u = left;
		m_vertex[0].v = top;

		m_vertex[1].u = left;
		m_vertex[1].v = top;

		m_vertex[4].u = left;
		m_vertex[4].v = top;

		m_vertex[5].u = left;
		m_vertex[5].v = top;

		//Left-bottom corner
		m_vertex[8].u = left;
		m_vertex[8].v = bottom;

		m_vertex[9].u = left;
		m_vertex[9].v = bottom;

		m_vertex[12].u = left;
		m_vertex[12].v = bottom;

		m_vertex[13].u = left;
		m_vertex[13].v = bottom;

		//Right-top corner
		m_vertex[2].u = right;
		m_vertex[2].v = top;

		m_vertex[3].u = right;
		m_vertex[3].v = top;

		m_vertex[6].u = right;
		m_vertex[6].v = top;

		m_vertex[7].u = right;
		m_vertex[7].v = top;

		//Right-bottom corner
		m_vertex[10].u = right;
		m_vertex[10].v = bottom;

		m_vertex[11].u = right;
		m_vertex[11].v = bottom;

		m_vertex[14].u = right;
		m_vertex[14].v = bottom;

		m_vertex[15].u = right;
		m_vertex[15].v = bottom;
	}
	else
	{
		m_vertex[0].setUV(left, top);
		m_vertex[1].setUV(left, bottom);
		m_vertex[2].setUV(right, top);
		m_vertex[3].setUV(right, bottom);
	}

	applyVertexUpdate();
}

void zglImageView::setCoordOffset(unsigned int vertexIndex, float dx, float dy,
		float dz)
{
	if (vertexIndex > RIGHT_BOTTOM)
		return;

	m_offset_x[vertexIndex] = dx;
	m_offset_y[vertexIndex] = dy;
	m_offset_z[vertexIndex] = dz;

	applyDimensionUpdate();
}

//TODO some bug for the 16 vertex effect.
void zglImageView::setEffect(unsigned int effect)
{
	//if (effect == m_effect_type)
	//	return;

	m_effect_type = effect;

	switch (effect)
	{
	case EFFECT_NONE:
		{
			if (m_antialias)
			{
				//Left-top corner
				m_vertex[0].u = m_uv_left;
				m_vertex[0].v = m_uv_top;

				m_vertex[1].u = m_uv_left;
				m_vertex[1].v = m_uv_top;

				m_vertex[4].u = m_uv_left;
				m_vertex[4].v = m_uv_top;

				m_vertex[5].u = m_uv_left;
				m_vertex[5].v = m_uv_top;

				//Left-bottom corner
				m_vertex[8].u = m_uv_left;
				m_vertex[8].v = m_uv_bottom;

				m_vertex[9].u = m_uv_left;
				m_vertex[9].v = m_uv_bottom;

				m_vertex[12].u = m_uv_left;
				m_vertex[12].v = m_uv_bottom;

				m_vertex[13].u = m_uv_left;
				m_vertex[13].v = m_uv_bottom;

				//Right-top corner
				m_vertex[2].u = m_uv_right;
				m_vertex[2].v = m_uv_top;

				m_vertex[3].u = m_uv_right;
				m_vertex[3].v = m_uv_top;

				m_vertex[6].u = m_uv_right;
				m_vertex[6].v = m_uv_top;

				m_vertex[7].u = m_uv_right;
				m_vertex[7].v = m_uv_top;

				//Right-bottom corner
				m_vertex[10].u = m_uv_right;
				m_vertex[10].v = m_uv_bottom;

				m_vertex[11].u = m_uv_right;
				m_vertex[11].v = m_uv_bottom;

				m_vertex[14].u = m_uv_right;
				m_vertex[14].v = m_uv_bottom;

				m_vertex[15].u = m_uv_right;
				m_vertex[15].v = m_uv_bottom;
			}
			else
			{
				m_vertex[0].setUV(m_uv_left, m_uv_top);
				m_vertex[1].setUV(m_uv_left, m_uv_bottom);
				m_vertex[2].setUV(m_uv_right, m_uv_top);
				m_vertex[3].setUV(m_uv_right, m_uv_bottom);

				m_vertex[0].a = m_alpha[0];
				m_vertex[1].a = m_alpha[1];
				m_vertex[2].a = m_alpha[2];
				m_vertex[3].a = m_alpha[3];
			}
			applyVertexUpdate();
		}
		break;

	case EFFECT_MIRROR:
		{
			if (m_antialias)
			{
				//Left-top corner, m_uv_left, m_uv_bottom
				m_vertex[0].u = m_uv_left;
				m_vertex[0].v = m_uv_bottom;

				m_vertex[1].u = m_uv_left;
				m_vertex[1].v = m_uv_bottom;

				m_vertex[4].u = m_uv_left;
				m_vertex[4].v = m_uv_bottom;

				m_vertex[5].u = m_uv_left;
				m_vertex[6].v = m_uv_bottom;

				//Left-bottom corner, m_uv_left, m_uv_top
				m_vertex[8].u = m_uv_left;
				m_vertex[8].v = m_uv_top;

				m_vertex[9].u = m_uv_left;
				m_vertex[9].v = m_uv_top;

				m_vertex[12].u = m_uv_left;
				m_vertex[12].v = m_uv_top;

				m_vertex[13].u = m_uv_left;
				m_vertex[13].v = m_uv_top;

				//Right-top corner, m_uv_right, m_uv_bottom
				m_vertex[2].u = m_uv_right;
				m_vertex[2].v = m_uv_bottom;

				m_vertex[3].u = m_uv_right;
				m_vertex[3].v = m_uv_bottom;

				m_vertex[6].u = m_uv_right;
				m_vertex[6].v = m_uv_bottom;

				m_vertex[7].u = m_uv_right;
				m_vertex[7].v = m_uv_bottom;

				//Right-bottom corner, m_uv_right, m_uv_top
				m_vertex[10].u = m_uv_right;
				m_vertex[10].v = m_uv_top;

				m_vertex[11].u = m_uv_right;
				m_vertex[11].v = m_uv_top;

				m_vertex[14].u = m_uv_right;
				m_vertex[14].v = m_uv_top;

				m_vertex[15].u = m_uv_right;
				m_vertex[15].v = m_uv_top;

				m_vertex[AA_LEFT_TOP].a = 0xcc;
				m_vertex[AA_LEFT_BOTTOM].a = 0x00;
				m_vertex[AA_RIGHT_TOP].a = 0xcc;
				m_vertex[AA_RIGHT_BOTTOM].a = 0x00;
			}
			else
			{
				m_vertex[0].setUV(m_uv_left, m_uv_bottom);
				m_vertex[1].setUV(m_uv_left, m_uv_top);
				m_vertex[2].setUV(m_uv_right, m_uv_bottom);
				m_vertex[3].setUV(m_uv_right, m_uv_top);

				m_vertex[0].a = 0xcc;
				m_vertex[1].a = 0x00;
				m_vertex[2].a = 0xcc;
				m_vertex[3].a = 0x00;
			}
			applyVertexUpdate();
		}
		break;

	case EFFECT_FLIP_X:
		{
			if (m_antialias)
			{
				//Left-top corner, m_uv_left, m_uv_bottom
				m_vertex[0].u = m_uv_left;
				m_vertex[0].v = m_uv_bottom;

				m_vertex[1].u = m_uv_left;
				m_vertex[1].v = m_uv_bottom;

				m_vertex[4].u = m_uv_left;
				m_vertex[4].v = m_uv_bottom;

				m_vertex[5].u = m_uv_left;
				m_vertex[6].v = m_uv_bottom;

				//Left-bottom corner, m_uv_left, m_uv_top
				m_vertex[8].u = m_uv_left;
				m_vertex[8].v = m_uv_top;

				m_vertex[9].u = m_uv_left;
				m_vertex[9].v = m_uv_top;

				m_vertex[12].u = m_uv_left;
				m_vertex[12].v = m_uv_top;

				m_vertex[13].u = m_uv_left;
				m_vertex[13].v = m_uv_top;

				//Right-top corner, m_uv_right, m_uv_bottom
				m_vertex[2].u = m_uv_right;
				m_vertex[2].v = m_uv_bottom;

				m_vertex[3].u = m_uv_right;
				m_vertex[3].v = m_uv_bottom;

				m_vertex[6].u = m_uv_right;
				m_vertex[6].v = m_uv_bottom;

				m_vertex[7].u = m_uv_right;
				m_vertex[7].v = m_uv_bottom;

				//Right-bottom corner, m_uv_right, m_uv_top
				m_vertex[10].u = m_uv_right;
				m_vertex[10].v = m_uv_top;

				m_vertex[11].u = m_uv_right;
				m_vertex[11].v = m_uv_top;

				m_vertex[14].u = m_uv_right;
				m_vertex[14].v = m_uv_top;

				m_vertex[15].u = m_uv_right;
				m_vertex[15].v = m_uv_top;
			}
			else
			{
				m_vertex[0].setUV(m_uv_left, m_uv_bottom);
				m_vertex[1].setUV(m_uv_left, m_uv_top);
				m_vertex[2].setUV(m_uv_right, m_uv_bottom);
				m_vertex[3].setUV(m_uv_right, m_uv_top);
			}

			applyVertexUpdate();
		}
		break;

	case EFFECT_FLIP_Y:
		{
			if (m_antialias)
			{
				//Left-top corner, m_uv_right, m_uv_top
				m_vertex[0].u = m_uv_right;
				m_vertex[0].v = m_uv_top;

				m_vertex[1].u = m_uv_right;
				m_vertex[1].v = m_uv_top;

				m_vertex[4].u = m_uv_right;
				m_vertex[4].v = m_uv_top;

				m_vertex[5].u = m_uv_right;
				m_vertex[6].v = m_uv_top;

				//Left-bottom corner, m_uv_right, m_uv_bottom
				m_vertex[8].u = m_uv_right;
				m_vertex[8].v = m_uv_bottom;

				m_vertex[9].u = m_uv_right;
				m_vertex[9].v = m_uv_bottom;

				m_vertex[12].u = m_uv_right;
				m_vertex[12].v = m_uv_bottom;

				m_vertex[13].u = m_uv_right;
				m_vertex[13].v = m_uv_bottom;

				//Right-top corner, m_uv_left, m_uv_top
				m_vertex[2].u = m_uv_left;
				m_vertex[2].v = m_uv_top;

				m_vertex[3].u = m_uv_left;
				m_vertex[3].v = m_uv_top;

				m_vertex[6].u = m_uv_left;
				m_vertex[6].v = m_uv_top;

				m_vertex[7].u = m_uv_left;
				m_vertex[7].v = m_uv_top;

				//Right-bottom corner, m_uv_left, m_uv_bottom
				m_vertex[10].u = m_uv_left;
				m_vertex[10].v = m_uv_bottom;

				m_vertex[11].u = m_uv_left;
				m_vertex[11].v = m_uv_bottom;

				m_vertex[14].u = m_uv_left;
				m_vertex[14].v = m_uv_bottom;

				m_vertex[15].u = m_uv_left;
				m_vertex[15].v = m_uv_bottom;
			}
			else
			{
				m_vertex[0].setUV(m_uv_right, m_uv_top);
				m_vertex[1].setUV(m_uv_right, m_uv_bottom);
				m_vertex[2].setUV(m_uv_left, m_uv_top);
				m_vertex[3].setUV(m_uv_left, m_uv_bottom);
			}

			applyVertexUpdate();
		}
		break;
	}
}

void zglImageView::setAction(unsigned int action)
{
	m_action_type = action;
}

void zglImageView::setShear(float dx, float dy)
{
	m_offset_x[0] = dx;
	m_offset_x[1] = -dx;
	m_offset_x[2] = dx;
	m_offset_x[3] = -dx;

	m_offset_y[0] = dy;
	m_offset_y[1] = dy;
	m_offset_y[2] = -dy;
	m_offset_y[3] = -dy;

	applyVertexUpdate();
}

void zglImageView::updateDimension()
{
	float width2 = m_width * 0.5f;
	float height2 = m_height * 0.5f;

	if (m_antialias)
	{
		float x, y;

		const float offset = 0.50f;

		width2 -= offset;
		height2 -= offset;

		//Left-top corner
		x = -width2 + m_offset_x[0];
		y = height2 + m_offset_y[0];
#if 1
		m_vertex[0].setPos(x - offset, y + offset, m_offset_z[0]);
		m_vertex[1].setPos(x, y + offset, m_offset_z[0]);
		m_vertex[4].setPos(x - offset, y, m_offset_z[0]);
		m_vertex[5].setPos(x, y, m_offset_z[0]);
#else

		m_vertex[0].setPos(x, y, m_offset_z[0]);
		m_vertex[1].setPos(x, y, m_offset_z[0]);
		m_vertex[4].setPos(x, y, m_offset_z[0]);
		m_vertex[5].setPos(x, y, m_offset_z[0]);
#endif
		//Left-bottom corner
		x = -width2 + m_offset_x[1];
		y = -height2 + m_offset_y[1];

#if 1
		m_vertex[8].setPos(x - offset, y, m_offset_z[1]);
		m_vertex[9].setPos(x, y, m_offset_z[1]);
		m_vertex[12].setPos(x - offset, y - offset, m_offset_z[1]);
		m_vertex[13].setPos(x, y - offset, m_offset_z[1]);
#else

		m_vertex[8].setPos(x, y, m_offset_z[1]);
		m_vertex[9].setPos(x, y, m_offset_z[1]);
		m_vertex[12].setPos(x, y, m_offset_z[1]);
		m_vertex[13].setPos(x, y, m_offset_z[1]);
#endif
		//Right-top corner
		x = width2 + m_offset_x[2];
		y = height2 + m_offset_y[2];

#if 1
		m_vertex[2].setPos(x, y + offset, m_offset_z[2]);
		m_vertex[3].setPos(x + offset, y + offset, m_offset_z[2]);
		m_vertex[6].setPos(x, y, m_offset_z[2]);
		m_vertex[7].setPos(x + offset, y, m_offset_z[2]);
#else
		m_vertex[2].setPos(x, y, m_offset_z[2]);
		m_vertex[3].setPos(x, y, m_offset_z[2]);
		m_vertex[6].setPos(x, y, m_offset_z[2]);
		m_vertex[7].setPos(x, y, m_offset_z[2]);
#endif
		//Right-Bottom corner
		x = width2 + m_offset_x[3];
		y = -height2 + m_offset_y[3];
#if 1
		m_vertex[10].setPos(x, y, m_offset_z[3]);
		m_vertex[11].setPos(x + offset, y, m_offset_z[3]);
		m_vertex[14].setPos(x, y - offset, m_offset_z[3]);
		m_vertex[15].setPos(x + offset, y - offset, m_offset_z[3]);
#else
		m_vertex[10].setPos(x, y, m_offset_z[3]);
		m_vertex[11].setPos(x, y, m_offset_z[3]);
		m_vertex[14].setPos(x, y, m_offset_z[3]);
		m_vertex[15].setPos(x, y, m_offset_z[3]);
#endif
	}
	else
	{
		m_vertex[0].setPos(-width2 + m_offset_x[0], height2 + m_offset_y[0],
				m_offset_z[0]);
		m_vertex[1].setPos(-width2 + m_offset_x[1], -height2 + m_offset_y[1],
				m_offset_z[1]);
		m_vertex[2].setPos(width2 + m_offset_x[2], height2 + m_offset_y[2],
				m_offset_z[2]);
		m_vertex[3].setPos(width2 + m_offset_x[3], -height2 + m_offset_y[3],
				m_offset_z[3]);
	}
}

void zglImageView::updateHitRect()
{
	float width2 = m_width * 0.5f;
	float height2 = m_height * 0.5f;

	m_boundary[0].x = - width2 + m_offset_x[0];
	m_boundary[0].y = + height2 + m_offset_y[0];
	m_boundary[0].z = m_offset_z[0];

	m_boundary[1].x = - width2 + m_offset_x[1];
	m_boundary[1].y = - height2 + m_offset_y[1];
	m_boundary[1].z = m_offset_z[1];

	m_boundary[2].x = + width2 + m_offset_x[3];
	m_boundary[2].y = - height2 + m_offset_y[3];
	m_boundary[2].z = m_offset_z[3];

	m_boundary[3].x = + width2 + m_offset_x[2];
	m_boundary[3].y = + height2 + m_offset_y[2];
	m_boundary[3].z = m_offset_z[2];

	getMappingVertex(m_boundary, 4, m_prim.world());
}

void zglImageView::setTexture(zglTexture * texture)
{
	m_prim.setTexture(texture);
}

void zglImageView::setAlpha(unsigned char alpha)
{
	if (m_antialias)
	{
		m_vertex[AA_LEFT_TOP].a = alpha;
		m_vertex[AA_LEFT_BOTTOM].a = alpha;
		m_vertex[AA_RIGHT_TOP].a = alpha;
		m_vertex[AA_RIGHT_BOTTOM].a = alpha;
	}
	else
	{
		m_vertex[0].a = alpha;
		m_vertex[1].a = alpha;
		m_vertex[2].a = alpha;
		m_vertex[3].a = alpha;
	}

	applyVertexUpdate();
}

void zglImageView::setAlpha(unsigned char alphaLT, unsigned char alphaRT,
		unsigned char alphaLB, unsigned char alphaRB)
{
	if (m_antialias)
	{
		m_vertex[AA_LEFT_TOP].a = alphaLT;
		m_vertex[AA_LEFT_BOTTOM].a = alphaLB;
		m_vertex[AA_RIGHT_TOP].a = alphaRT;
		m_vertex[AA_RIGHT_BOTTOM].a = alphaRB;
	}
	else
	{
		m_vertex[0].a = alphaLT;
		m_vertex[1].a = alphaLB;
		m_vertex[2].a = alphaRT;
		m_vertex[3].a = alphaRB;
	}

	applyVertexUpdate();
}

void zglImageView::setColor(unsigned int color)
{
	unsigned char r = (color & 0xff000000) >> 24;
	unsigned char g = (color & 0x00ff0000) >> 16;
	unsigned char b = (color & 0x0000ff00) >> 8;
	unsigned char a = (color & 0x000000ff);

	if (m_antialias)
	{
		//!< Left-top corner
		m_vertex[0].r = r;
		m_vertex[0].g = g;
		m_vertex[0].b = b;
		m_vertex[0].a = 0;

		m_vertex[1].r = r;
		m_vertex[1].g = g;
		m_vertex[1].b = b;
		m_vertex[1].a = 0;

		m_vertex[4].r = r;
		m_vertex[4].g = g;
		m_vertex[4].b = b;
		m_vertex[4].a = 0;

		m_vertex[5].r = r;
		m_vertex[5].g = g;
		m_vertex[5].b = b;
		m_vertex[AA_LEFT_TOP].a = a;

		//Left-bottom corner
		m_vertex[8].r = r;
		m_vertex[8].g = g;
		m_vertex[8].b = b;
		m_vertex[8].a = 0;

		m_vertex[9].r = r;
		m_vertex[9].g = g;
		m_vertex[9].b = b;
		m_vertex[AA_LEFT_BOTTOM].a = a;

		m_vertex[12].r = r;
		m_vertex[12].g = g;
		m_vertex[12].b = b;
		m_vertex[12].a = 0;

		m_vertex[13].r = r;
		m_vertex[13].g = g;
		m_vertex[13].b = b;
		m_vertex[13].a = 0;

		//Right-top corner
		m_vertex[2].r = r;
		m_vertex[2].g = g;
		m_vertex[2].b = b;
		m_vertex[2].a = 0;

		m_vertex[3].r = r;
		m_vertex[3].g = g;
		m_vertex[3].b = b;
		m_vertex[3].a = 0;

		m_vertex[6].r = r;
		m_vertex[6].g = g;
		m_vertex[6].b = b;
		m_vertex[AA_RIGHT_TOP].a = a;

		m_vertex[7].r = r;
		m_vertex[7].g = g;
		m_vertex[7].b = b;
		m_vertex[7].a = 0;

		//Right-bottom
		m_vertex[10].r = r;
		m_vertex[10].g = g;
		m_vertex[10].b = b;
		m_vertex[AA_RIGHT_BOTTOM].a = a;

		m_vertex[11].r = r;
		m_vertex[11].g = g;
		m_vertex[11].b = b;
		m_vertex[11].a = 0;

		m_vertex[14].r = r;
		m_vertex[14].g = g;
		m_vertex[14].b = b;
		m_vertex[14].a = 0;

		m_vertex[15].r = r;
		m_vertex[15].g = g;
		m_vertex[15].b = b;
		m_vertex[15].a = 0;
	}
	else
	{
		m_vertex[0].r = m_vertex[1].r = m_vertex[2].r = m_vertex[3].r = r;
		m_vertex[0].g = m_vertex[1].g = m_vertex[2].g = m_vertex[3].g = g;
		m_vertex[0].b = m_vertex[1].b = m_vertex[2].b = m_vertex[3].b = b;
		m_vertex[0].a = m_vertex[1].a = m_vertex[2].a = m_vertex[3].a = a;
	}

	applyVertexUpdate();
}

void zglImageView::setColor(unsigned int colorLT, unsigned int colorRT,
		unsigned int colorLB, unsigned int colorRB)
{
	if (m_antialias)
	{
		unsigned char r, g, b, a;

		//!< Left-top corner
		r = (colorLT & 0xff000000) >> 24;
		g = (colorLT & 0x00ff0000) >> 16;
		b = (colorLT & 0x0000ff00) >> 8;
		a = (colorLT & 0x000000ff);

		m_vertex[0].r = r;
		m_vertex[0].g = g;
		m_vertex[0].b = b;
		m_vertex[0].a = 0;

		m_vertex[1].r = r;
		m_vertex[1].g = g;
		m_vertex[1].b = b;
		m_vertex[1].a = 0;

		m_vertex[4].r = r;
		m_vertex[4].g = g;
		m_vertex[4].b = b;
		m_vertex[4].a = 0;

		m_vertex[5].r = r;
		m_vertex[5].g = g;
		m_vertex[5].b = b;
		m_vertex[AA_LEFT_TOP].a = a;

		//Left-bottom corner
		r = (colorLB & 0xff000000) >> 24;
		g = (colorLB & 0x00ff0000) >> 16;
		b = (colorLB & 0x0000ff00) >> 8;
		a = (colorLB & 0x000000ff);

		m_vertex[8].r = r;
		m_vertex[8].g = g;
		m_vertex[8].b = b;
		m_vertex[8].a = 0;

		m_vertex[9].r = r;
		m_vertex[9].g = g;
		m_vertex[9].b = b;
		m_vertex[AA_LEFT_BOTTOM].a = a;

		m_vertex[12].r = r;
		m_vertex[12].g = g;
		m_vertex[12].b = b;
		m_vertex[12].a = 0;

		m_vertex[13].r = r;
		m_vertex[13].g = g;
		m_vertex[13].b = b;
		m_vertex[13].a = 0;

		//Right-top corner
		r = (colorRT & 0xff000000) >> 24;
		g = (colorRT & 0x00ff0000) >> 16;
		b = (colorRT & 0x0000ff00) >> 8;
		a = (colorRT & 0x000000ff);

		m_vertex[2].r = r;
		m_vertex[2].g = g;
		m_vertex[2].b = b;
		m_vertex[2].a = 0;

		m_vertex[3].r = r;
		m_vertex[3].g = g;
		m_vertex[3].b = b;
		m_vertex[3].a = 0;

		m_vertex[6].r = r;
		m_vertex[6].g = g;
		m_vertex[6].b = b;
		m_vertex[AA_RIGHT_TOP].a = a;

		m_vertex[7].r = r;
		m_vertex[7].g = g;
		m_vertex[7].b = b;
		m_vertex[7].a = 0;

		//Right-bottom
		r = (colorRB & 0xff000000) >> 24;
		g = (colorRB & 0x00ff0000) >> 16;
		b = (colorRB & 0x0000ff00) >> 8;
		a = (colorRB & 0x000000ff);

		m_vertex[10].r = r;
		m_vertex[10].g = g;
		m_vertex[10].b = b;
		m_vertex[AA_RIGHT_BOTTOM].a = a;

		m_vertex[11].r = r;
		m_vertex[11].g = g;
		m_vertex[11].b = b;
		m_vertex[11].a = 0;

		m_vertex[14].r = r;
		m_vertex[14].g = g;
		m_vertex[14].b = b;
		m_vertex[14].a = 0;

		m_vertex[15].r = r;
		m_vertex[15].g = g;
		m_vertex[15].b = b;
		m_vertex[15].a = 0;
	}
	else
	{
		m_vertex[0].setColor(colorLT);
		m_vertex[1].setColor(colorLB);
		m_vertex[2].setColor(colorRT);
		m_vertex[3].setColor(colorRB);
	}

	applyVertexUpdate();
}

bool zglImageView::onTouchEvent(zglTouchEvent *event)
{
	bool ret = false;

	if (ACTION_FLIP_BLINK == m_action_type)
	{
		switch (event->m_event_type)
		{
		case TOUCH_DOWN: //down
			{
				if (m_antialias)
				{
					m_alpha[0] = m_vertex[AA_LEFT_TOP].a;
					m_alpha[1] = m_vertex[AA_LEFT_BOTTOM].a;
					m_alpha[2] = m_vertex[AA_RIGHT_TOP].a;
					m_alpha[3] = m_vertex[AA_RIGHT_BOTTOM].a;
					int a = m_alpha[0] - 0x80;
					if (a < 0) a = 0;
					m_vertex[5].a = a;

					a = m_alpha[1] - 0x80;
					if (a < 0) a = 0;
					m_vertex[9].a = a;

					a = m_alpha[2] - 0x80;
					if (a < 0) a = 0;
					m_vertex[6].a = a;

					a = m_alpha[3] - 0x80;
					if (a < 0) a = 0;
					m_vertex[10].a = a;
				}
				else
				{
					m_alpha[0] = m_vertex[0].a;
					m_alpha[1] = m_vertex[1].a;
					m_alpha[2] = m_vertex[2].a;
					m_alpha[3] = m_vertex[3].a;
					int a = m_alpha[0] - 0x80;
					if (a < 0) a = 0;
					m_vertex[0].a = a;

					a = m_alpha[1] - 0x80;
					if (a < 0) a = 0;
					m_vertex[1].a = a;

					a = m_alpha[2] - 0x80;
					if (a < 0) a = 0;
					m_vertex[2].a = a;

					a = m_alpha[3] - 0x80;
					if (a < 0) a = 0;
					m_vertex[3].a = a;
				}

				applyVertexUpdate();

				ret = true;
			}
			break;

		case TOUCH_CANCEL:
		case TOUCH_UP:
			{
				if (m_antialias)
				{
					m_vertex[AA_LEFT_TOP].a = m_alpha[0];
					m_vertex[AA_LEFT_BOTTOM].a = m_alpha[1];
					m_vertex[AA_RIGHT_TOP].a = m_alpha[2];
					m_vertex[AA_RIGHT_BOTTOM].a = m_alpha[3];
				}
				else
				{
					m_vertex[0].a = m_alpha[0];
					m_vertex[1].a = m_alpha[1];
					m_vertex[2].a = m_alpha[2];
					m_vertex[3].a = m_alpha[3];
				}
				applyVertexUpdate();
			}
			ret = true;
			break;

		case TOUCH_MOVE:
			ret = true;
			break;
		}
	}
	return zglWidget::onTouchEvent(event) || ret;
}


