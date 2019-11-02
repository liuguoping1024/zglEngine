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

#ifndef __zgl_sphere_h__
#define __zgl_sphere_h__

#include "zglMesh.h"
#include "zglSphere.h"

/**
 * @brief Basic 3D shape, special Mesh: Sphere.
 *
 * @note the radius is fixed, if a dynamic radius sphere needed, some modification
 * needed for the class.
 */
class zglSphere: public zglMesh
{
public:
	zglSphere();

	/**
	 * @brief create a new Sphere.
	 *
	 * @param radius the radius of the Sphere. the radius should be larger than zero.
	 * @param layer the layer divided on x-z plane,dividing from -z to z.
	 * @param slice the x-y plane division, the slice will divide between 0 to 2pi.
	 * @param texture texture of the Sphere.
	 */
	void init(float radius, unsigned short layer, unsigned short slice,
			zglTexture * texture);

	/**
	 * Set the mesh type.
	 *
	 * @see MESH_TEXTURE
	 * @see MESH_WIREFRAME
	 * @see MESH_POINTS
	 */
	virtual void setMeshType(unsigned char type);

	/**
	 * set the color of the sphere.
	 *
	 * @param color the color of the sphere.
	 */
	void setColor(unsigned int color);

protected:

	/**
	 * @brief render handler of the widget.
	 */
	virtual void render();

	/**
	 * @brief Update the the index array when mesh type changed.
	 */
	void updateMeshType();

	/**
	 * @brief  Update the the vertex array.
	 */
	void updateMeshVertex();

private:
	/**
	 * Render the mesh of No VBO(hardware buffer)
	 */
	void renderNoVBO();

	/**
	 * Render the mesh of VBO(hardware buffer)
	 */
	void renderVBO();

private:
	float m_radius; //!< radius of the sphere.
	unsigned short m_layer;
	unsigned short m_slice;

	unsigned int m_color;
};


#endif
