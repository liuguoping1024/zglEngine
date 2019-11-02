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

#ifndef __zgl_prim_quadrangle_h__
#define __zgl_prim_quadrangle_h__

#include "zglTexture.h"
#include "zglPrim3D.h"
#include "zglVertex.h"

/**
 * @brief This is a utility primitive for the widget.
 *
 * For normal widgets, the widget can be divide into small quadrangle, every
 * quadrangle can hold one texture. The #zglPrimQuad is a primitive composed
 * of quadrangle.
 *
 * To make full use of this primitive, the developer should prepare the vertex
 * array, and the textures array.
 *
 * The amount of the vertex should be (4 * n), the n is the amount of the quadrangle
 * of the widgets. Excluding the background textures, developers should prepare
 * the texture array. The amount of the texture array should be (n - 1), because
 * the background texture is stored in the Prim3D.
 *
 */
class zglPrimQuad: public zglPrim3D
{
public:
	zglPrimQuad();
	virtual ~zglPrimQuad();


	/**
	 * Initialize the Primitive-QUAD.
	 *
	 * @note
	 * 	- The function will NOT check the length of the array.
	 * 	- More primitive can share the same vertex array.
	 * 	- More primitive can share the same texture.
	 *
	 * @param type the zglDrawType, LINES, or LOOPS, etc.
	 * @param vertex the vertex array of the primitive.
	 * @param vert_num the vertex array length.
	 * @param texture the <B>background</B> texture bind to the primitive.
	 *
	 */
	void init(zglDrawType type, zglVertex* vertex, int vert_num,
			zglTexture * texture = NULL);

	//TODO TBD
	void init(zglDrawType type, zglVertex* vertex, int vert_num,
			unsigned short * indices, int index_num,
			zglTexture * texture = NULL);

	/**
	 * @brief Set the extra textures for the primitive.
	 *
	 * @param texs The texture array of the primitive. The length should be
	 * 	(n), where n is the count of the quadrangle.
	 *
	 */
	void setTextures(zglTexture ** texs);

protected:
	/**
	 * Render the 3D objects to the screen.
	 */
	virtual void render();

private:
	zglTexture ** m_tex_group;

};


#endif
