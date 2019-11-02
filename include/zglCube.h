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

#ifndef __zgl_cube_h__
#define __zgl_cube_h__

#include "zglVertex.h"
#include "zglMesh.h"
#include "zglTexture.h"
#include "zglBasicType.h"

/**
 * @brief Implement basic 3D shape: Cube.
 */
class zglCube: public zglMesh
{
public:
	zglCube();

	/**
	 * @brief create a new zglCube.
	 *
	 * @param width width of the cube;(x-length)
	 * @param height height of the cube;(y-length)
	 * @param thick thick of the cube;(z-length)
	 * @param texture texture of the cube. The cube array count can be 1, 2, 3, 6.
	 * @param tex_num the count of the texture, the value can be 0, 1, 2, 3, or 6.
	 *
	 * @code
	 *	zglCube cube;
	 *	cube.init(100.0f, 100.0f, 100.0f, tex, 6);
	 * @endcode
	 */
	void init(float width, float height, float thick, zglTexture * texture,
			unsigned int tex_num);

protected:

	/**
	 * @brief render handler of the widget.
	 */
	virtual void render();

private:
	void renderNoVBO();

	void renderVBO();

private:
	static const int m_vertex_max = 24;

	unsigned int m_tex_num;
};

#endif
