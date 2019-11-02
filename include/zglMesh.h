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

#ifndef __zgl_mesh_h__
#define __zgl_mesh_h__

#include "zglTexture.h"
#include "zglPrim3D.h"
#include "zglVertex.h"

/**
 * @brief A Mesh holder and render for 3D graphics, the mesh can be used for
 * NURBS or other curve faces, or 3D models.
 */
class zglMesh: public zglPrim3D
{
public:
	zglMesh();
	virtual ~zglMesh();

	/**
	 * @brief Initialize and Prepare enough resource for the new mesh.
	 *
	 * @param vertex the count of the vertex
	 * @param indices the count of the index
	 * @param texture the texture of the mesh. (Just for passing the value.)
	 */
	void init(int vertex, int indices, zglTexture * texture);

	/**
	 * @brief Set the Mesh type.
	 *
	 * The supported type is list below:
	 * -# MESH_TEXTURE
	 * -# MESH_WIREFRAME
	 * -# MESH_POINTS
	 *
	 * default value is MESH_TEXTURE.
	 */
	virtual void setMeshType(unsigned char type);

	/**
	 * @brief get the vertex array of mesh.
	 */
	inline zglVertexEx* getMesh() const
	{
		return (zglVertexEx*)m_vertex;
	}

	/**
	 * @brief get the index array of mesh.
	 */
	inline unsigned short * getIndices() const
	{
		return m_indices;
	}

	/**
	 * @brief Check the mesh is well prepared or not.
	 *
	 * The workload of the mesh calculation always is very heavy,
	 * so the way to check the mesh state is supplied.
	 *
	 * Here is an example:
	 * @code
	 * 		Mesh * pMesh = &m_mesh_xxx;
	 * 		if(pMesh->isAvailable())
	 * 		{
	 * 			pMesh->setVisible(true);
	 * 		}
	 * 		else
	 * 		{
	 * 			m_mesh_xxx.init(...);
	 * 		}
	 * @endcode
	 */
	bool isAvailable();

protected:
	/**
	 * @brief resize the indices array.
	 */
	void resizeIndices(int indices);

	/**
	 * @brief Update the vertex and indices information into the GPU hardware
	 * buffer.
	 */
	virtual void updateHwBuffers();

	/**
	 * @brief Create the hardware buffer
	 */
	virtual bool createHwBuffers();

protected:
	unsigned char m_mesh_type;//!< mesh types, @see
};
#endif
