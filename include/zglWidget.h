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


#ifndef __zgl_widget_h__
#define __zgl_widget_h__


#include "zglVector.h"
#include "zglVertex.h"

#include "zglAnimation.h"
#include "zglPrim.h"

#include "zglWidgetListener.h"

/**
 * @brief Vertex hardware buffer(VBO) mapping style.
 */
enum
{
	FLAG_MAPPING_NONE, //!< Put the widget's vertex into the memory.
	FLAG_MAPPING_STATIC, //!< Put the widget's vertex into static hardware buffer.
	FLAG_MAPPING_DYNAMIC,//!< Put the widget's vertex into dynamic hardware buffer.
};

/**
 * @brief The widget is the basic organ of the GUI system.
 *
 * A widget can be put into the Widget list of the GUI system automatically by calling
 * init(parent).
 *
 *
 * The main member of the #zglWidget can connect widget with each other. The member
 * #m_next and #m_prev is used by #zglWidgetList to dispatch event, and the member
 * #m_next_sibling and #m_prev_sibling is used by #zglWidgetGroup to form a
 * large, complex widget.
 *
 * @image html zglwidget.png "zglWidget organization diagram"
 *
 */
class zglWidget: public zglWidgetListener
{
public:
	zglWidget();
	virtual ~zglWidget();

	//-------------------------------------------------------------------------
	/**
	 * @brief Set the visibility state of the widget.
	 * @param visible true to make the widget visible.
	 */
	virtual void setVisible(bool visible);

	/**
	 * @brief Enable/disable the widget.
	 *
	 * If the widget is disabled, the widget will not be hit testes, and
	 * will not receive any events.
	 * @param enable true to enable the widget.
	 */
	virtual void setEnable(bool enable);

	/**
	 * @brief Set the alpha value of the whole widget.
	 *
	 * @note The function only take effect after the widget initialized;
	 * If possible, just override the function.
	 */
	virtual void setAlpha(unsigned char alpha);

	/**
	 * @brief Set the color of the whole widget.
	 *
	 * @note The function only take effect after the widget initialized.
	 * If possible, just override the function.
	 */
	virtual void setColor(unsigned int color);

	/**
	 * @brief Set the vertex mapping of the widget.
	 *
	 * This is a performance optimize option.
	 *
	 * If the widget will not use the hardware buffer(VBO), please skip it.
	 * The default setting is not using VBO.
	 *
	 * If the widget set to 'FLAG_MAPPING_STATIC', which means the widget will
	 * not <b>seldom</b> updating it's vertex, including the position, color,
	 * texture coordinate,and alpha information. Then the graphics will make
	 * full use of the VBO(hardware buffer), which will increase the performance
	 *  of the GPU.
	 *
	 * The same as to the indices array.
	 *
	 * If the widget set to 'FLAG_MAPPING_DYNAMIC', which means the widget will
	 * not <b>sometimes</b> updating it's vertex information. That style will
	 * increase the performance of the GPU too.
	 *
	 * If the widget will update the vertex information every frame, such as
	 * FPS text, he VBO will not make the application faster, on the contrary,
	 *  it will bring waste to the GPU resource.
	 *
	 * Under that state, a <i>FLAG_MAPPING_NONE</i> setting is recommended to
	 * the widget.
	 *
	 * @note The transition between the style is not convenient, so just set
	 * the style before the widget visible.
	 */
	virtual void setMappingStyle(unsigned char flag);

	//-------------------------------------------------------------------------

	/**
	 * @brief Set the widget listener to the widget.
	 */
	void setListener(zglWidgetListener * listener);

	/**
	 * @brief Set the animation to the widget.
	 */
	void setAnimation(zglAnimation * anim);

	/**
	 * @brief Set the texture to the widget.
	 */
	virtual void setTexture(zglTexture * tex);

	//-------------------------------------------------------------------------

	/**
	 * @brief offset adjust to make the widgets at the right order.
	 *
	 * The recommend way is to put the widgets at the same z position,
	 * and then apply the offset to them.
	 */
	virtual void setOffset(int offset);

	/**
	 * @brief The Z depth of the widget.
	 *
	 * as to the 2D widget, the default z depth is 128.
	 *
	 * @return the z depth. z = 255 means the widget is located the most far
	 * away from the observer, z = 0 means the widget is very close to observer.
	 */
	virtual unsigned char getZOffset();

	//-------------------------------------------------------------------------

	/**
	 * @brief Get the current position.
	 *
	 * @param x The x position of the widget.
	 * @param y The y position of the widget.
	 * @param z The z position of the widget.
	 */
	void getPosition(float *x, float *y, float *z);

	/**
	 * @brief Get the current x-position.
	 */
	inline float getX() const
	{
		return m_x;
	}

	/**
	 * @brief Get the current y-position.
	 */
	inline float getY() const
	{
		return m_y;
	}

	/**
	 *@brief  Get the current z-position.
	 */
	inline float getZ() const
	{
		return m_z;
	}

	//-------------------------------------------------------------------------

	/**
	 * @brief Reset the Coordination setting of the current Widget.
	 *
	 * The default position after resetting is (0.0f, 0.0f, 0.0f).
	 */
	virtual void resetWorld();

	/**
	 * @brief Reset the Coordination setting of the current Widget, and move the
	 * Widget to the new position with x, y, z.
	 *
	 * @note the angle of the widget will lost.
	 *
	 * @param x The new position of the widget.
	 * @param y The new position of the widget.
	 * @param z The new position of the widget.
	 *
	 * @see #locatex
	 */
	virtual void locate(float x, float y, float z = 0.0f);

	/**
	 * @brief Reset the Coordination of the current Widget, restore the position
	 * and rotate the widget with angle of ax, ay, az.
	 *
	 * @warning the function conflict with #locate() that this function
	 * may be disturb the coordinate of the widget.
	 *
	 * @param ax The y angle of the widget.
	 * @param ay The y angle of the widget.
	 * @param az The y angle the widget.
	 */
	virtual void locatex(float ax, float ay, float az);

	//-------------------------------------------------------------------------

	/**
	 * @brief rotate the widget around its center x axe.
	 */
	virtual void rotateX(float angle);

	/**
	 * @brief rotate the widget around its center y axe.
	 */
	virtual void rotateY(float angle);

	/**
	 * @brief Rotate the widget around its center z axe.
	 */
	virtual void rotateZ(float angle);

	/**
	 * @brief Move the the widget to the target position.
	 *
	 * @param x The new position of the widget.
	 * @param y The new position of the widget.
	 * @param z The new position of the widget.
	 *
	 * @note the parameter of 'z' is useless for a 2D widget.
	 */
	virtual void moveTo(float x, float y, float z);

	/**
	 * @brief Move the the widget to the target position.
	 *
	 * @param x The new position of the widget.
	 * @param y The new position of the widget.
	 */
	virtual void moveTo(float x, float y);

	/**
	 * @brief Move the center of the widget to a new position by the offset.
	 *
	 * @param dx The offset x position of the widget.
	 * @param dy The offset y position of the widget.
	 * @param dz The offset z position of the widget.
	 *
	 * @note the parameter of 'dz' is useless for a 2D widget.
	 */
	virtual void move(float dx, float dy, float dz);

	/**
	 * @brief Move the the widget to the target x-position.
	 * @param x target x-position
	 */
	virtual void moveXTo(float x);

	/**
	 * @brief Move the the widget to the target y-position.
	 * @param y target y-position
	 */
	virtual void moveYTo(float y);

	/**
	 * @brief Move the the widget to the target z-position.
	 * @param z target z-position
	 */
	virtual void moveZTo(float z);

	//-------------------------------------------------------------------------

	/**
	 * @brief Set the widget ID.
	 *
	 * The Callback in widgetListener will use the ID to notify the widget.
	 * @param id widget ID
	 */
	inline void setID(unsigned short id)
	{
		m_id = id;
	}

	/**
	 * @brief Get the Widget ID
	 */
	inline unsigned short getID() const
	{
		return m_id;
	}

	/**
	 * @brief Get the default graphics primitive of the widget.
	 * If a custom primitive used, the function should be override.
	 */
	inline zglPrim * getPrim()
	{
		return m_prim_ref;
	}

	/**
	 * @brief Set the parent widget to current widget.
	 *
	 * @param parent The parent widget.
	 */
	inline void setParent(zglWidget * parent)
	{
		m_parent = parent;
	}

	//-------------------------------------------------------------------------

	/**
	 * @brief Lock mouse capture
	 *
	 * @deprecated
	 */
	static void lockCapture();

	/**
	 * @brief Unlock mouse capture
	 *
	 * @deprecated
	 */
	static void unlockCapture();


	/**
	 * @brief ...
	 */
	static unsigned short tryHitTest(float x, float y);

	//-------------------------------------------------------------------------

	/**
	 * @brief Force update dimension without waiting for the next updating loop.
	 */
	void forceUpdateDimension();

protected:

	/**
	 * @brief VBO(hardware buffer) acceleration assistant.
	 *
	 * Call this function to inform the engine to update the dimension, or
	 * update the VBO buffer.
	 *
	 * If the widget does not care about the VBO, just call the
	 * function when dimension changed.
	 * @code
	 * 		setDirtyFlag(0xffff);
	 * @endcode
	 *
	 * or call the function like this:
	 * @code
	 * 		setDirtyFlag(FLAG_DIM_UPDATE);
	 * @endcode
	 *
	 * If the widget will use the VBO to get more better effect, the
	 * suggestion below should be paid more attention.
	 *
	 * - 1. When the dimension updated.
	 * @code
	 * 		setDirtyFlag(FLAG_DIM_UPDATE);
	 * @endcode
	 * Then the engine will call function #updateDimension() to update
	 * the vertex.
	 *
	 * - 2. After #updateDimension() invoked, the Engine will update the
	 * vertex to the VBO by <i>glBufferSubData</i>. All the updates to the
	 * vertex array or the indices array should be sync to the VBO. So the
	 * updates flags below should be marked carefully. Any action to set the
	 * flags below will all stimulate the action <i>glBufferSubData</i>. But
	 * if no flag set, the updating to the vertex array and indices array will
	 * be skipped.
	 *
	 *		- FLAG_VERTEX_UPDATE: Some call-back functions may update the
	 * vertex bypassing #updateDimension(). When this happened, the flag should
	 * be set. If #FLAG_DIM_UPDATE is set, the flag will no need to be set.
	 *		- FLAG_ALPHA_UPDATE: Alpha updated flag. The same as FLAG_COLOR_UPDATE
	 *		- FLAG_COLOR_UPDATE: Color updated flag. The same as FLAG_ALPHA_UPDATE
	 *		- FLAG_TEXCOORD_UPDATE: Texture coordinate updated flag.
	 *		- FLAG_NORMAL_UPDATE: Normal updated flag.
	 */
#if 0
	inline void setDirtyFlag(unsigned int flag)
	{
		m_dirty_flag |= flag;
	}
#endif

	/**
	 * @brief Update the related flag to make the zglWidget will not forget
	 * updating the dimension and the buffer.
	 *
	 * @note The hardware buffer(VBO) need to be sync from the vertex buffer.
	 */
	inline void applyDimensionUpdate()
	{
		m_dirty_flag |= FLAG_DIM_UPDATE;
	}

	/**
	 * @brief Update the related flag to make the zglWidget will not forget
	 * updating the related buffer.
	 *
	 * The content is list below:
	 *
	 * -# The x, y or z of the vertex is changed.
	 * -# The color is changed.
	 * -# The texture coordinate changed.
	 * -# The alpha value is changed.
	 *
	 * @note The hardware buffer(VBO) need to be sync from the vertex buffer.
	 */
	inline void applyVertexUpdate()
	{
		m_dirty_flag |= FLAG_COLOR_UPDATE;
	}

	/**
	 * @brief getting the screen position of the vertex.
	 *
	 * @param pos the vertex's position, using zglVector.
	 * @param size the array length of the vertex.
	 * @param m local coordinate system matrix of the vertex.
	 *
	 * @note the API will not check the real length of the array.
	 */
	void getMappingVertex(zglVector* pos, int size, zglMatrix & m);

	/**
	 * @brief Parameter Updated flag.
	 *
	 * TODO More consider needed.
	 *
	 */
	enum {
		FLAG_DIM_UPDATE = 0x01, //!< Dimension updated, see #updateDimension

		FLAG_VERTEX_UPDATE = 0x02, //!< zglVertex contents updated
		FLAG_ALPHA_UPDATE = 0x02,
		FLAG_COLOR_UPDATE = 0x02,
		FLAG_TEXCOORD_UPDATE = 0x02,

		FLAG_NORMAL_UPDATE = 0x04,//!< zglVertexEx contents updated

		FLAG_MASK_UPDARE = 0x07,

		FLAG_BOUND_UPDATE = 0x80,
	};

protected:
	unsigned char m_type; //!< Widget type, 2D, 3D, etc
	unsigned char m_flag; //!< Widget flag. VISIBLE, ENABLE, etc
	unsigned char m_map_flag; //!< vertex mapping flag.
	unsigned char m_dirty_flag; //!< parameter updated flag.

	unsigned short m_id; //!< widget id, reserve for special use

	float m_x;//!< Center x-position of the widget.
	float m_y;//!< Center y-position of the widget.
	float m_z;//!< Center z-position of the widget.

	/**
	 * The primitive binded to the widget. Normally, one primitive can meet
	 * the most requirements. If one primitive is not enough, add more
	 * primitives.
	 *
	 * One primitive is highly recommended for the sake of the matrix operation.
	 */
	zglPrim * m_prim_ref;

	zglWidget * m_next; //!< the next widget in the widget list or the children list.
	zglWidget * m_prev; //!< the prev widget in the widget list or the children list.

	zglWidget * m_parent; //!< the parent of current widget. NULL if not exist.

	zglWidget * m_next_sibling; //!< the sibling of the current widget. NULL if not exist.
	zglWidget * m_prev_sibling; //!< the sibling of the current widget. NULL if not exist.

	zglWidgetListener * m_listener; //!< Event listener of current widget.

	zglAnimation * m_animation; //!< Animation of the widget, @see zglAnimation

	/**
	 * @brief Widget type
	 */
	enum zglWidgetType
	{
		TYPE_WIDGET2D, //!< Parallel projection widgets, the widget will float at the top layer.
		TYPE_WIDGET3D, //!< perspective projection widgets.
		TYPE_NUM,
		TYPE_WIDGETGROUP, //!< Not used
		TYPE_PARTICLE, //!< Partical system widget, special widget, @see zglWidgetParticle
	};

	/**
	 * @brief Widget attribute flags, such as invisible, etc
	 */
	enum zglWidgetFlag
	{
		FLAG_DISABLE = 0x00000001, //!< No response to touch, chance to update(animation).
		FLAG_INVISIBLE = 0x00000002, //!< invisible, no response to touch, no chance to update.
	};

protected:

	/**
	 * @brief Initial the widget and put into the widget list.
	 */
	void init();

	/**
	 * @brief Update event handler of the widget.
	 *
	 * @param time The time line of the update event.
	 * @return True if the event was handled, false otherwise.
	 */
	virtual bool onUpdate(long time);

    /**
     * @brief Handle the touch events to the widget.
     *
     * @param event The touch event.
     * @return True if the event was handled, false otherwise.
     */
	virtual bool onTouchEvent(zglTouchEvent *event);

	/**
	 * @brief Hit test, check the widget is hit or not.
	 *
	 * @return true if the widget is hit.
	 */
	virtual bool hitTest(float x, float y);

	/**
	 * @brief Update dimension.
	 *
	 * The vertex's position will be changed after invoke.
	 * Override needed.
	 */
	virtual void updateDimension();

	//-------------------------------------------------------------------------

	/**
	 * @brief put the widget into the widget list
	 */
	void queue();

	/**
	 * @brief drop the widget from the widget list
	 */
	void dequeue();

	/**
	 * check the queue state of the widget
	 */
	bool isQueued();

	//-------------------------------------------------------------------------

	/**
	 * @brief set the widget flags.
	 */
	void setFlag(zglWidgetFlag obj_flag);

	/**
	 * @brief clear the widget flags.
	 */
	void resetFlag(zglWidgetFlag obj_flag);

private:
	/**
	 * @brief Update the hardware buffer.
	 *
	 * The interface is only invoked when the vertex changed, especially
	 * dimension changed.
	 */
	void updateHardwareBuffer();

	/**
	 * @brief  Updating the hardware setting.
	 *
	 * Mapping the #FLAG_MAPPING_NONE, #FLAG_MAPPING_STATIC and
	 * #FLAG_MAPPING_DYNAMIC to #FLAG_VBO_DYNAMIC and #FLAG_VBO_ENABLE of the
	 * Primitive flags.
	 *
	 * @see #FLAG_MAPPING_NONE
	 * @see #FLAG_MAPPING_STATIC
	 * @see #FLAG_MAPPING_DYNAMIC
	 * @see #FLAG_VBO_DYNAMIC
	 * @see #FLAG_VBO_ENABLE
	 */
	void updateHardwareSetting();

	friend class zglWidgetList;
	friend class zglWidgetGroup;

public:
#ifdef _DEBUG_
	virtual void getBound(zglVertex * v, int *count);
#endif
};

#endif

