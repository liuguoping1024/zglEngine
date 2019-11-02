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


#ifndef __zgl_node_list_h__
#define __zgl_node_list_h__

#include "zglPrim.h"

/**
 * @brief Container for all the graphics primitives.
 *
 * The primitives in the primitive list will has the chance to be draw
 * on the screen. The primitives outside will not be drawn until it was set
 * to visible and put into the list.
 *
 * The primitives are grouped into three groups:
 * 1. 3D objects, perspective projection;
 * 2. 2D objects, parallel projection.
 * 3. Particle object, for improving performance.
 */
class zglPrimList
{
public:
	zglPrimList();
	virtual ~zglPrimList();

private:
	/**
	 * @brief Put the node into the sortList.
	 * @see calcSortDepth
	 */
	void queueSortList(zglPrim* prim);

	/**
	 * Restore the zglPrimList to original state.
	 * Designed for the Android. Do not use it if you do not master it.
	 * @deprecated
	 */
	void reset();

	/**
	 * lock the primitive list for security access.
	 */
	void lock();

	/**
	 * unlock the primitive list for security.
	 */
	void unlock();

private:
	/**
	 * Render the Prim3D without alpha blending.
	 */
	void renderPrim();

	/**
	 * Render the Prim3D with alpha blending
	 */
	void renderPrimAlpha();

	/**
	 * Render the Prim2D
	 */
	void renderPrim2D();

	/**
	 * Sprite is a special 3D primitive in the Computer Graphics.
	 * Sprite is an important type in the 3D Games.
	 *
	 * 1. The sprite will look forward to user forever.
	 * 2. All the sprite together will use the same texture.
	 * 3. ...
	 */
	void renderSprite();

private:
	//!< Header and tail holder for the primitives. Double list.
	zglPrim m_prim_list_head[zglPrim::TYPE_NUM]; //!< Header node of the prim
	zglPrim m_prim_list_tail[zglPrim::TYPE_NUM]; //!< Tail node of the prim.

	static const int m_sort_list_max = 256;
	/**
	 * Tempt holder for the widgets header,
	 * for example, render the transparent primitives.
	 */
	zglPrim * m_sort_list[m_sort_list_max];

	friend class zglPrim;
	friend class zglEngine;
};
#endif


