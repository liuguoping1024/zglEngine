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

#ifndef __zgl_engine_h__
#define __zgl_engine_h__

#include "zglPrim.h"
#include "zglPrimList.h"

#include "zglWidget.h"
#include "zglWidgetList.h"

#include "zglMatrix.h"

#include "zglLight.h"

#include "zglPlugin.h"

/**
 * @brief Core of the whole Engine.
 *
 * The class of zglEnine is the main sole of the whole engine. It maintained
 * all the graphics primitive for rendering, and all the widget for GUI event
 * dispatching. At the same time, the zglEngine maintain the basic global
 * graphics environment of the GL, such as global coordination system, the
 * light, fog, etc, to form a virtual world.
 *
 * All the command to the whole engine will be received by the #zglEngine, and
 * then pass to widget.
 *
 */
class zglEngine
{
public:
	zglEngine();
	virtual ~zglEngine();

	// ------------------------------------------------------------------------

	/**
	 * @brief get the global instance of the engine.
	 *
	 * @note If multiple instance needed, try no to use the function.
	 */
	static zglEngine* getEngine(void);

	/**
	 * @brief free the resource of the engine
	 *
	 * @note If multiple instance needed, try no to use the function.
	 */
	static void destroyEngine(void);

	// ------------------------------------------------------------------------

	/**
	 * @brief Update the screen (re-draw the screen)
	 * Render the element one-by-one, the node will be skipped if:
	 * 1. Invisible flag;
	 * 2. TODO The boundary of the Node is out of the viewport;
	 * 3. TODO The z value is out of the scene.
	 */
	void renderScreen();

	/**
	 * @brief Animation and widget update timing..
	 */
	bool update(long time_val);

	// ------------------------------------------------------------------------

	/**
	 * @brief Interface to invoke the plug-in, the plug-in can hook/block
	 * all the event before the event sent to GUI process.
	 *
	 * The plug-in is suit for design small applications.
	 */
	void setPlugin(zglPlugin * plugin);

	/**
	 * @brief Interface when the engine's initial is ready.
	 */
	void initiate();

	/**
	 * @brief Interface when the engine is going to quit.
	 */
	void finish();

	/**
	 * @brief Interface when the screen is going to resize.
	 *
	 * @param w the new size
	 * @param h the new size
	 */
	void resize(int w, int h);

	// ------------------------------------------------------------------------

	//!< Interface when touch event comes.
	bool onTouchEvent(int event_type, int x, int y, long time);

#if 0
	//!< Interface when key event comes.
	bool onKeyEvent(int event_type, int key_value);


	/**
	 * @brief Interface when sensor event comes.
	 *
	 * The interface is mostly based on Android.
	 * @param type sensor type
	 * @param v1 sensor data
	 * @param v2 sensor data
	 * @param v3 sensor data
	 */
	bool onSensorEvent(int type, float v1, float v2, float v3);
#endif

	// ------------------------------------------------------------------------

	/**
	 * Z buffer clip distance.
	 * @see <i>glClearDepth</i>
	 */
	void setClipLength(float near, float far);

	/**
	 * @brief Set the focus distance.
	 *
	 * This is an important feature value.
	 *
	 * The 3D vertex put at the focus distance will be mapped to screen with
	 * the same position of the screen.
	 *
	 * @param focus the focus distance.
	 */
	void setFocusDistance(float focus);

	/**
	 * @brief Set the clear color.
	 * @param r r color, [0, 0xff]
	 * @param g g color, [0, 0xff]
	 * @param b b color, [0, 0xff]
	 * @param a alpha value, [0, 0xff]
	 * @see <i>glClearColor</i>
	 */
	void setClearColor(unsigned char r, unsigned char g, unsigned char b,
			unsigned char a);

	// ------------------------------------------------------------------------

	/**
	 * @brief find the hit widget at the point of (x, y).
	 *
	 * @return If the widget is found, return the widget, else return NULL.
	 */
	/*inline zglWidget * hitTest(int x, int y)
	{
		return m_widget_list.hitTest((float)x, (float)y);
	}*/

	// ------------------------------------------------------------------------

	/**
	 * @brief get the global coordinate system matrix(global world matrix).
	 *
	 * @note The global world matrix will affect the camera, so that it seems
	 * the whole world is changing.
	 */
	zglMatrix& world();

	/**
	 * @brief get the global model-view matrix(global model-view).
	 *
	 * The model View of the primitives should be :
	 * 	- (global model-view Matrix * local model-view Matrix)
	 *
	 * @note The global model-view matrix will be affected by the global world matrix.
	 *
	 * @see GL_MODELVIEW, glMatrixMode()
	 */
	zglMatrix& modelview();

	// ------------------------------------------------------------------------


	// ------------------------------------------------------------------------
#if 0
	/**
	 * @brief get the primitive list.
	 */
	inline zglPrimList * getPrimNodeList()
	{
		return &m_node_list;
	}

	/**
	 * @brief get the widget list.
	 */
	inline zglWidgetList * getWidgetList()
	{
		return &m_widget_list;
	}
#endif

	/**
	 * @brief get the global light resource.
	 */
	inline zglLight * getLight()
	{
		return &m_light;
	}

	/**
	 * @brief get the fog resource.
	 */
	inline zglFog * getFog()
	{
		return &m_fog;
	}

private:
	/**
	 * @brief The Global Light of the Engine. Only 8 light supported.
	 */
	zglLight m_light;

	/**
	 * @brief The Global Fog of the Engine.
	 */
	zglFog m_fog;

	/**
	 * @brief The Primitive list. All the visible primitives are hold in the list.
	 */
	zglPrimList m_node_list;

	/**
	 * @brief The Widget list. All the Widgets are contained in the list.
	 */
	zglWidgetList m_widget_list;

protected:
	/**
	 * @brief calculate the projection matrix.
	 */
	void calcProjection();

	/**
	 * @brief calculate the vector to screen coordinate
	 * @param pos [in/out]vector will be calculated.
	 * @param local the local coordinate system of the vector.
	 */
	void calcWorldTo2D(zglVector& pos, zglMatrix & local);

	/**
	 * @brief calculate the vector to screen coordinate
	 * @param pos [in/out]vector will be calculated.
	 * @param size vector array size
	 * @param local the local coordinate system of the vector.
	 */
	void calcWorldTo2D(zglVector* pos, int size, zglMatrix & local);

	/**
	 * @brief calculate the 2D coordinate to screen coordinate
	 * @param pos [in/out]vector will be calculated.
	 */
	void calc2DToScreen(zglVector& pos);

	/**
	 * @brief Set the viewport of the GL. The range may be smaller than the framebuffer size.
	 * @code
	 * 		setViewport(0, 0, 400, 400);
	 * @endcode
	 * @see <i>glViewport</i>
	 */
	void setViewport(short x, short y, unsigned short width,
			unsigned short height);

	/**
	 * @brief Set the framebuffer(device) size.
	 * For example: Android screen resolution may be 480x800.
	 * just call setSize(480, 800);
	 */
	void setSize(int width, int height);

	zglPlugin * m_plugin; //!< plug-in of applications.

private:
	float m_width;
	float m_height;

	float m_near_dist;
	float m_far_dist;
	float m_focus_dist;

	short m_disp_x;
	short m_disp_y;
	unsigned short m_disp_w;
	unsigned short m_disp_h;

	bool m_screen_changed;
	bool m_update_needed;//!< Force update

	unsigned short m_global_timestamp; //!< Time stamp of the global matrix.

	zglMatrix m_view_to_screen; //!< projection of 3D primitive(perspective).
	zglMatrix m_world_to_view; //!< global model-view.
	zglMatrix m_world_to_screen; //!< vertex world position to screen translate.
	zglMatrix m_2d_to_screen;//!< projection of 2D primitive(parellal).

	zglMatrix m_view;

	unsigned char m_clear_color[4];

	friend class zglPrim;
	friend class zglPrim3D;
	friend class zglPrimParticle;
	friend class zglMesh;
	friend class zglWidget;
	friend class zglWidgetList;

private:
#ifdef _DEBUG_
	zglPlugin* m_debugger;
	friend class zglEngineDebugger;
#endif

};

#endif

