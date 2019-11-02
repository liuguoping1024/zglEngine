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

#ifndef __zgl_prim_type_h__
#define __zgl_prim_type_h__

#include <stdlib.h>

/**
 * Basic types of the graphics primitives.
 */

#define ZGL_PI  3.14159265358979323846
#define ZGL_2PI 6.28318530717958647692
#define ZGL_PI2 1.57079632679489661923

enum zglDrawType
{
	DRAW_POINTS,
	DRAW_LINES,
	DRAW_LINE_STRIP,
	DRAW_LINE_LOOP,
	DRAW_TRIANGLES,
	DRAW_TRI_STRIP,
	DRAW_TRI_FAN,
	DRAW_QUADS,
};


/**
 * @brief Pixel Format for the Engine.
 *
 * Refer to OpenGL-ES spec, p69, Table 3.4.
 *
 *
 * @code
		enum AndroidBitmapFormat {
			ANDROID_BITMAP_FORMAT_NONE      = 0,
			ANDROID_BITMAP_FORMAT_RGBA_8888 = 1,
			ANDROID_BITMAP_FORMAT_RGB_565   = 4,
			ANDROID_BITMAP_FORMAT_RGBA_4444 = 7,
			ANDROID_BITMAP_FORMAT_A_8       = 8,
		};
 * @endcode
 */
enum zglPixelFormat
{
	PIXEL_NONE = 0,
	PIXEL_RGBA8888 = 1, //!< GL_RGBA, UNSIGNED_BYTE, size = 4(*Android)
	PIXEL_RGB888 = 2, 	//!< GL_RGB, UNSIGNED_BYTE, size = 3

	PIXEL_RGBA5551 = 3, //!< GL_RGBA, UNSIGNED_SHORT_5_5_5_1, size = 2
	PIXEL_RGB565 = 4, 	//!< GL_RGB, UNSIGNED_SHORT_5_6_5, size = 2(*Android)

	PIXEL_RA88 = 5,		//!< GL_LUMINANCE_ALPHA, UNSIGNED_BYTE, size = 2
	PIXEL_R8 = 6,		//!< GL_LUMINANCE, UNSIGNED_BYTE, size = 1

	PIXEL_RGBA4444 = 7, //!< GL_RGBA, UNSIGNED_SHORT_4_4_4_4, size = 2(*Android)
	PIXEL_A8 = 8,		//!< GL_ALPHA, UNSIGNED_BYTE, size = 1(*Android)
};

enum zglPixelType
{
	PIXEL_BYTE,			//!< GL_UNSIGNED_BYTE
	PIXEL_SHORT4444,	//!< GL_UNSIGNED_SHORT_4444
	PIXEL_SHORT5551,	//!< GL_UNSIGNED_SHORT_5551
	PIXEL_SHORT565,		//!< GL_UNSIGNED_SHORT_565
};

/**
 * @brief Blend type
 */
enum zglBlendType
{
	BLEND_OFF = 0, //!< No blend, alpha will have no effects.
	BLEND_HALF, //!< Blend half
	BLEND_ADD, //!< Blend Add
};

enum zglBufferType
{
	VERTEX_BUFFER = 0,
	INDEX_BUFFER,
};

enum zglFogMode
{
	FOG_LINEAR = 0,
	FOG_EXP,
	FOG_EXP2
};

enum zglMaterialType
{
	MATERIAL_NONE = 0,
	MATERIAL_DEFAULT,
	MATERIAL_BRASS,
	MATERIAL_BRONZE,
	MATERIAL_POLISHED_BRONZE,
	MATERIAL_CHROME,
	MATERIAL_COPPER,
	MATERIAL_POLISHED_COPPER,
	MATERIAL_GOLD,
	MATERIAL_POLISHED_GOLD,
	MATERIAL_PEWTER,
	MATERIAL_SILVER,
	MATERIAL_POLISHED_SILVER,
	MATERIAL_EMERALD,
	MATERIAL_JADE,
	MATERIAL_OBSIDIAN,
	MATERIAL_PEARL,
	MATERIAL_RUBY,
	MATERIAL_TURQUOISE,
	MATERIAL_BLACK_PLASTIC,
	MATERIAL_BLACK_RUBBER,
};

enum zglMeshFlag
{
	MESH_TEXTURE = 0x01,
	MESH_WIREFRAME = 0x02,
	MESH_POINTS = 0x04,
	//MESH_FLATSHADE = 0x08,
	//MESH_SMOOTHSHADE = 0x10,
	//MESH_NORMAL = 0x20,
	//TODO
};

enum zglDepthType
{
	DEPTH_NEVER,
	DEPTH_LESS,
	DEPTH_EQUAL,
	DEPTH_LEQUAL,
	DEPTH_GREATER,
	DEPTH_NOTEQUAL,
	DEPTH_GEQUAL,
	DEPTH_ALWAYS,
};

#endif

