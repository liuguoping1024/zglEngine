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

#ifndef __zgl_node_h__
#define __zgl_node_h__

#include "zglBasicType.h"
#include "zglMatrix.h"
#include "zglTexture.h"
#include "zglVertex.h"

#include "zglMaterial.h" //!< zglMaterial


/**
 * @brief attribute of the primitive nodes
 */
enum zglPrimNodeFlag
{
	FLAG_INVISIBLE = 0x00000001, //!< visibility
	FLAG_BILLBOARD = 0x00000002, //!< bill board effect
	FLAG_CULL_FACE = 0x00000004, //!< Cull face enable
	FLAG_TEX_POINT = 0x00000008, //!< Sprite point texture

	FLAG_NO_NORMAL = 0x00000010, //!< No normal needed.
	FLAG_NO_FOG    = 0x00000020, //!< No fog needed.

	FLAG_VBO_DYNAMIC  = 0x40000000, //!< dynamic hardware buffer enable
	FLAG_VBO_ENABLE  = 0x80000000 //!< hardware buffer enable
};

/**
 * @brief Graphics Primitive is the  basic part of the graphics layer.
 *
 * All the objects on the screen will be reprensented by the primitives.
 */
class zglPrim
{
public:
	zglPrim();
	virtual ~zglPrim();

	// ------------------------------------------------------------------------
	/**
	 * @brief Set A new texture to the primitive.
	 * @param texture the new texture
	 */
	inline void setTexture(zglTexture * texture)
	{
		m_texture = texture;
	}

	/**
	 * @brief Set A new material to the primitive.
	 * @param material new material to the primitives.
	 */
	inline void setMaterial(zglMaterial * material)
	{
		m_material = material;
	}

	// ------------------------------------------------------------------------
	/**
	 * Set the visible of the primitive.
	 *
	 * @param visible <i>true</i> to visible and <i>false</i> to invisible.
	 */
	inline void setVisible(bool visible)
	{
		if (visible)
		{
			resetFlag(FLAG_INVISIBLE);
		}
		else
		{
			setFlag(FLAG_INVISIBLE);
		}
	}

	/** 
	 * @brief set the blend type of the node.
	 * @see zglBlendType
	 */
	inline void setBlendType(zglBlendType blend_type)
	{
		m_blend_type = blend_type;
	}

	// ------------------------------------------------------------------------
	/**
	 * @brief Get the local coordinate system matrix of the primitive.
	 */
	inline zglMatrix & world()
	{
		return m_world;
	}

	// ------------------------------------------------------------------------
	/**
	 * @brief set the sort offset.
	 *
	 * The sort offset will adjust the sort depth (z-buffer) of the primitive to
	 * changed the render order of the primitives.
	 *
	 * @param sort_offset value of sort offset.
	 */
	inline void setSortOffset(short sort_offset)
	{
		m_sort_offset = sort_offset;
	}

	/**
	 * @brief get the sort value(z-buffer) of the primitives.
	 */
	inline unsigned char getSortValue() const
	{
		return m_sort_value & 0xff;
	}

	// ------------------------------------------------------------------------
	/**
	 * @brief Set the primitive flag of the primitive.
	 *
	 * @see zglPrimNodeFlag
	 */
	virtual void setFlag(unsigned int obj_flag);

	/**
	 * @brief Reset the primitive flag of the primitive.
	 *
	 * @see zglPrimNodeFlag
	 */
	virtual void resetFlag(unsigned int obj_flag);

	/**
	 * @brief Get the primitive flag of the primitive
	 *
	 * @return current primitive flag.
	 * @see zglPrimNodeFlag
	 */
	inline unsigned int getFlag()
	{
		return m_flag;
	}

	// ------------------------------------------------------------------------

	/**
	 * @brief update the vertex and indices array information to hardware buffer
	 * (VBO) to increase the performance.
	 *
	 * If the vertex and indices array had not been submit to hardware buffer,
	 * a new hardware buffer will be create for the primitive; else if the vertex
	 * and indices array had been submit to the hardware buffer, the hardware
	 * buffer will sync with the input arrays.
	 *
	 * If the flag FLAG_VBO_ENABLE is not been set, the function will had no
	 * effect to the hardware buffer.
	 *
	 * If the flag FLAG_VBO_ENABLE is been set, and FLAG_VBO_DYNAMIC not been
	 * set, the hardware buffer will create a new buffer when the function called
	 * first time; the hardware will not sync with the input the buffer.
	 *
	 * If the flag FLAG_VBO_ENABLE and FLAG_VBO_DYNAMIC are all set, the hardware
	 * buffer will create new buffer and sync the input buffer when the function
	 * called every time.
	 *
	 * @see FLAG_VBO_DYNAMIC
	 * @see FLAG_VBO_ENABLE
	 */
	virtual void applyHwBuffers(bool enable = true);

protected:
	/**
	 * @brief node type
	 *
	 * The node is designed to apply different projection.
	 * -# TYPE_PRIM3D will use the perspective projection.
	 * -# TYPE_PRIM2D will use the parellal projection.
	 */
	enum zglPrimType
	{
		//TYPE_MODEL,
		TYPE_PRIM3D, //!< Basic 3D graphics primitive.
		TYPE_PRIM2D, //!< Basic 2D graphics primitive.
		TYPE_SPRITE, //!< Basic sprite.
		TYPE_NUM,  //!< node type count.
	};

protected:

	// ------------------------------------------------------------------------
	/**
	 * @brief Animation or frame update
	 */
	virtual bool update(long time);

	/**
	 * @brief Render interface, the engine will invoke it to render the nodes.
	 */
	virtual void render();

	/**
	 * @brief Prepare action before render procedure.
	 *
	 * - Set the ModelView
	 * - update the FLAG_CULL_FACE
	 * - update the BLEND setting
	 */
	void preRender();

	/**
	 * @brief post action after render procedure.
	 *
	 * - update the FLAG_CULL_FACE
	 * - Restore the normal setting.
	 */
	void postRender();


	// ------------------------------------------------------------------------
	/**
	 * @brief The node's z-depth will be updated and  will be put into a
	 * z-buffer list.
	 *
	 * The node with same type(TYPE_PRIM3D, TYPE_PRIM2D ..) will be render by
	 * the depth.
	 *
	 * @see #getSortValue
	 */
	virtual void calcSortDepth();


	// ------------------------------------------------------------------------
	/** 
	 * @brief put the graphics primitive into the Nodelist.
	 */
	void queue();

	/** 
	 * @brief Drop the graphics primitive from the Nodelist.
	 */
	void dequeue();

	/** 
	 * @brief check the queued state of the Node.
	 */
	bool isQueued();

protected:
	enum {
		HWBUFFER_NONE = 0, //!< Initial state

		HWBUFFER_AVAILABLE, //!< Hwbuffer OK
		HWBUFFER_FAIL, //!< HwBuffer fail??
	};

	/**
	 * @brief check the hardware buffer enable state.
	 */
	inline bool isHwBufferEnable() const
	{
		return (m_flag & FLAG_VBO_ENABLE) != 0;
	}

	/**
	 * @brief Update the vertex and indices information into the GPU hardware
	 *  buffer.
	 */
	virtual void updateHwBuffers();

	/**
	 * @brief Create the hardware buffer
	 */
	virtual bool createHwBuffers();

	/**
	 * @brief Free the hardware buffer.
	 */
	virtual void destroyHwBuffers();

	/**
	 * @brief Query the hardware buffer state.
	 */
	inline bool isHwBufferAvailable() const
	{
		return m_hwbuffer_type == HWBUFFER_AVAILABLE;
	}

	/**
	 * @brief check the buffer empty state.
	 */
	inline bool isHwBufferEmpty() const
	{
		return m_hwbuffer_type == HWBUFFER_NONE;
	}

	/**
	 * @brief update the hardware buffer state.
	 */
	inline void postHwBuffer(bool success)
	{
		m_hwbuffer_type = success ? HWBUFFER_AVAILABLE : HWBUFFER_FAIL;
	}

protected:

	unsigned char m_blend_type; //!< Blend type, @see zglBlendType
	unsigned char m_hwbuffer_type; //!< hardware buffer

	unsigned int m_flag; //!< flags of the primitive, @see zglPrimNodeFlag

	/**
	 * @brief The primitive type, such as TRIANGLE, TRIANGLE_FAN.
	 *
	 * @see zglDrawType
	 */
	zglDrawType m_draw_type;
	zglPrimType m_type; //!< Primitive type, 2D, 3D, or sprite, @see zglPrimType

	unsigned int m_sort_value; //!< use to sort the sort-list
	short m_sort_offset;//!< offset adjustment

	zglMatrix m_world; //!< Local Coordinate matrix of current primitive.

	zglTexture * m_texture; //!< Texture binded to the primitive.

	zglMaterial * m_material; //!< Material binded to the primitive.

	zglVertex * m_vertex;//!< zglVertex or, zglVertexEx.
	unsigned short * m_indices; //!< Index Array, @see glDrawElements.
	unsigned short m_vertex_num; //!< the zglVertex or, zglVertexEx count.
	unsigned short m_indices_num; //!< The indices array count.

	// hardware buffer(VBO)
	unsigned int m_vbo_vertex; //!< The VBO resource for the vertex array.
	unsigned int m_vbo_index; //!< The VBO index for the indices array.

	// linked node in the primitive list
	zglPrim* m_prev; //!< The previous Prim-node in the PrimList
	zglPrim* m_next; //!< The next Prim-node in the PrimList

	zglPrim* m_next_sort;//!< The next node with the same sort value(z buffer).

	friend class zglEngine;
	friend class zglPrimList;
	friend class zglWidget;
};

#endif

