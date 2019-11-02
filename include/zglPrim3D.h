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

#ifndef __zgl_prim_h__
#define __zgl_prim_h__

#include "zglPrim.h"

#include "zglTexture.h"
#include "zglVertex.h"
#include "zglBasicType.h"

/**
 * @brief The Primitives which will be mapped by the perspective projection.
 * 透视投影
 */
class zglPrim3D: public zglPrim
{
public:
	zglPrim3D();

	/**
	 * Initialize the Primitive-3D, the procedure is list below:
	 * 1. Initialize the Prim3D's parameter;
	 * 2. Reset the local coordinate system matrix;
	 * 3. Insert into the Primitive list.
	 *
	 * @note
	 * 	- The function will NOT check the length of the array.
	 * 	- More primitive can share the same vertex array.
	 * 	- more primitive can share the same texture.
	 *
	 * @param type the zglDrawType, LINES, or LOOPS, etc.
	 * @param vertex the vertex array of the primitive.
	 * @param vert_num the vertex array length.
	 * @param texture the texture bind to the primitive.
	 *
	 * @note
	 * 		The Prim3D will draw all the vertex and index once together.
	 * 		If the Prim3D is too complex, just override the render();
	 */
	void init(zglDrawType type, zglVertex* vertex, int vert_num,
			zglTexture * texture = NULL);

	/**
	 * Initialize the Primitive-3D, the procedure is list below:
	 * 1. Initialize the Prim3D's parameter;
	 * 2. Reset the local coordinate system matrix;
	 * 3. Insert into the Primitive list.
	 *
	 * @note
	 * 	- The function will NOT check the length of the array.
	 * 	- More primitive can share the same vertex/index array.
	 * 	- More primitive can share the same texture.
	 *
	 * @param type the zglDrawType, LINES, or LOOPS, etc.
	 * @param vertex the vertex array of the primitive.
	 * @param vert_num the vertex array length.
	 * @param indices the index array of the primitive.
	 * @param index_num the index array length.
	 * @param texture the texture bind to the primitive.
	 *
	 * @note
	 * 		The Prim3D will draw all the vertex and index once together.
	 * 		If the Prim3D is too complex, just override the render();
	 */
	void init(zglDrawType type, zglVertex* vertex, int vert_num,
			unsigned short * indices, int index_num,
			zglTexture * texture = NULL);

protected:
	/**
	 * Render the 3D objects to the screen.
	 */
	virtual void render();

	/**
	 * Calculate the sort depth, and put the primitive to sort list.
	 */
	virtual void calcSortDepth();

};

#endif

