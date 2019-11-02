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


#ifndef __zgl_vertex_h__
#define __zgl_vertex_h__

/**
 * @brief The vertex class.
 *
 * If the normal array needed, just create a new child-class plz.
 */
class zglVertex
{
public:
	zglVertex();

	float x; //!< x-coordinate of the vertex
	float y; //!< y-coordinate of the vertex
	float z; //!< z-coordinate of the vertex
	unsigned char r; //!< color of the vertex
	unsigned char g; //!< color of the vertex
	unsigned char b; //!< color of the vertex
	unsigned char a; //!< color of the vertex
	float u; //!< texture coordinate of the vertex
	float v; //!< texture coordinate of the vertex

	/**
	 * @brief Set the values to initialize a vertex.
	 *
	 * @param x x coordinate of the vertex.
	 * @param y y coordinate of the vertex.
	 * @param z z coordinate of the vertex.
	 * @param color color of the vertex.
	 * @param u x texture coordinate.
	 * @param v y texture coordinate.
	 *
	 * The method's function can be implemented by other methods.
	 * - #setPos()
	 * - #setColor()
	 * - #setUV()
	 */
	void init(float x, float y, float z, float u, float v,
			unsigned int color = 0xffffffff);

	/**
	 * @brief Set the values to initialize a vertex.
	 *
	 * @param x x coordinate of the vertex.
	 * @param y y coordinate of the vertex.
	 * @param z z coordinate of the vertex.
	 * @param color color of the vertex.
	 *
	 * The method's function can be implemented by other methods.
	 * - #setPos()
	 * - #setColor()
	 * - #setUV()
	 */
	void init(float x, float y, float z, unsigned int color = 0xffffffff);

	/**
	 * @brief set the position of the vertex.
	 *
	 * @param x x coordinate of the vertex.
	 * @param y y coordinate of the vertex.
	 * @param z z coordinate of the vertex.
	 */
	void setPos(float x, float y, float z);

	/**
	 * @brief set the color of the vertex.
	 * @param color color of the vertex.
	 */
	void setColor(unsigned int color);

	/**
	 * @brief set the texture coordinate of the vertex.
	 *
	 * @param u x texture coordinate.
	 * @param v y texture coordinate.
	 */
	void setUV(float u, float v);
};

/**
 * @brief Special vertex class with normal array information.
 *
 */
class zglVertexEx: public zglVertex
{
public:
	zglVertexEx();

	/**
	 * Set the normal of the vertex.
	 * @param nx x normal of the vertex.
	 * @param ny y normal of the vertex.
	 * @param nz z normal of the vertex.
	 */
	void setNormal(float nx, float ny, float nz);

	float nx; //!< x normal of the vertex.
	float ny; //!< y normal of the vertex.
	float nz; //!< z normal of the vertex.
};

#endif

