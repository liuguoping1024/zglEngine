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

#include <stdlib.h>	//!< NULL
#include "zglEngine.h"

#include "zglCore.h"	//!< Render
#include "zglPrim.h"	//!< Node
#include "zglPrim3D.h"	//!< Prim render
#include "zglPrim2D.h"	//!< Prim2D render
#include "zglMatrix.h"	//!< Matrix of the GL
#include "zglBasicType.h"
#include "zglEventType.h"	//!< event dispatch
#include "zglPlugin.h"

#ifdef _DEBUG_
#include "zglEngineDebugger.h"
#endif

/**
 * TODO
 * - 1. Add screen to support multi-viewport
 */

static zglEngine* glEngine = (zglEngine*) NULL;

zglEngine* zglEngine::getEngine(void)
{
	if (glEngine == NULL)
	{
		glEngine = new zglEngine();
	}

	return glEngine;
}

void zglEngine::destroyEngine(void)
{
	if (glEngine != NULL)
	{
		delete glEngine;
		glEngine = NULL;
	}
}

zglEngine::zglEngine()
{
	m_plugin = NULL;

	m_width = 0;
	m_height = 0;

	m_near_dist = 1.0f;
	m_far_dist = 1000.0f;

	m_focus_dist = 990.0f;

	m_disp_x = 0;
	m_disp_y = 0;

	m_disp_w = 320;
	m_disp_h = 240;

	m_view = 1.0f;

	m_clear_color[0] = 0;
	m_clear_color[1] = 0;
	m_clear_color[2] = 0;
	m_clear_color[3] = 0;

	m_screen_changed = true;
	m_update_needed = true;

#ifdef _DEBUG_
	m_debugger = NULL;
#endif
}

zglEngine::~zglEngine()
{
#ifdef _DEBUG_
	if(m_debugger != NULL)
	{
		delete m_debugger;
		m_debugger = NULL;
	}
#endif
}

void zglEngine::setSize(int width, int height)
{
	m_width = width;
	m_height = height;

	m_screen_changed = true;
}

void zglEngine::setViewport(short x, short y, unsigned short width,
		unsigned short height)
{
	m_disp_x = x;
	m_disp_y = y;

	m_disp_w = width;
	m_disp_h = height;

	m_screen_changed = true;
}

void zglEngine::setFocusDistance(float focus)
{
	m_focus_dist = focus;

	m_screen_changed = true;
}

void zglEngine::setClearColor(unsigned char r, unsigned char g, unsigned char b,
		unsigned char a)
{
	m_clear_color[0] = r;
	m_clear_color[1] = g;
	m_clear_color[2] = b;
	m_clear_color[3] = a;
}

zglMatrix& zglEngine::world()
{
	//m_world_changed = true;
	return m_view;
}

zglMatrix& zglEngine::modelview()
{
	return m_world_to_view;
}

void zglEngine::setClipLength(float near, float far)
{
	m_near_dist = near;
	m_far_dist = far;

	m_screen_changed = true;
}

void zglEngine::initiate()
{
	zglCore::reset();

	m_global_timestamp = 0;

	if (m_plugin != NULL)
	{
		m_plugin->onInit();
	}

#ifdef _DEBUG_
	if(m_debugger == NULL)
	{
		m_debugger = new zglEngineDebugger();
	}
	m_debugger->onInit();
#endif
}

void zglEngine::finish()
{
#ifdef _DEBUG_
	if(m_debugger != NULL)
	{
		m_debugger->onFini();
	}
#endif
	if (m_plugin != NULL)
	{
		m_plugin->onFini();
	}

	m_widget_list.reset();
	m_node_list.reset();
}

void zglEngine::resize(int w, int h)
{
	setSize(w, h);
	setViewport(0, 0, w, h);

	//zglCore::reset();

	if (m_plugin != NULL)
	{
		m_plugin->onResize(w, h);
	}
}

/**
 * If the graphics elements changed by the user or animation, return true.
 * then the engine will try to render the screen.
 */
bool zglEngine::update(long time_val)
{
	bool bRet = false;
	bRet = m_widget_list.update(time_val);

	if (m_plugin != NULL)
	{
		bRet = m_plugin->onUpdate(time_val) || bRet;
	}

	bRet = bRet || m_update_needed;

	m_update_needed = false;

	return bRet;
}

bool zglEngine::onTouchEvent(int event_type, int x, int y, long time)
{
	bool bRet = false;

#ifdef _DEBUG_
	bRet = m_debugger->handleTouchEvent(event_type, x, y, time);
#endif

	bRet = m_widget_list.dispatchTouchEvent(event_type, x, y, time) || bRet;

	if (bRet)
	{
		m_update_needed = true;
		return bRet;
	}

	if (m_plugin != NULL)
	{
		bRet = m_plugin->handleTouchEvent(event_type, x, y, time) || bRet;
	}

	m_update_needed = bRet;

	return bRet;
}

#if 0
bool zglEngine::onKeyEvent(int event_type, int key_value)
{
	bool bRet = false;
	if(m_plugin != NULL)
	{
		switch(event_type)
		{
			case KEY_DOWN://down
			bRet = m_plugin->onKeyDown(key_value);
			break;

			case KEY_UP: //up
			bRet = m_plugin->onKeyUp(key_value);
			break;
		}

		if(bRet)
		{
			return true;
		}
	}

	//Pass the event to the GUI system (widget list)
	bRet = m_widget_list.dispatchKeyEvent(event_type, key_value);

	return bRet;
}

bool zglEngine::onSensorEvent(int type, float v1, float v2, float v3)
{

	bool bRet = false;
	if(m_plugin != NULL)
	{
		bRet = m_plugin->onSensorEvent(type, v1, v2, v3);

		if(bRet)
		{
			return true;
		}
	}

	return false;
}
#endif

void zglEngine::setPlugin(zglPlugin * plugin)
{
	if (plugin != NULL)
	{
		m_plugin = plugin;
	}
}

void zglEngine::calcProjection()
{
	if (m_screen_changed)
	{
		float inv_2w = 2.0f / m_width;
		float inv_2h = 2.0f / m_height;
		float inv_sub = 1.0f / (m_far_dist - m_near_dist);

		/**
		 * http://webcvs.freedesktop.org/mesa/Mesa/src/glu/mesa/project.c
		 */
		m_view_to_screen(
			(m_focus_dist * inv_2w), 	0.0f, 	0.0f, 	0.0f,
			0.0f,  (m_focus_dist * inv_2h), 	0.0f, 	0.0f,
			0.0f,  0.0f, (m_far_dist + m_near_dist) * inv_sub,	-2.0f * m_far_dist * m_near_dist * inv_sub,
			0.0f, 0.0f, 1.0f, 0.0f);

		m_2d_to_screen( inv_2w,   0.0f, 0.0f, 0.0f,
						  0.0f, inv_2h, 0.0f, 0.0f,
						  0.0f,   0.0f, 0.0f, 0.0f,
						  0.0f,   0.0f, 0.0f, 1.0f);

		m_screen_changed = false;

		m_global_timestamp = -1;
	}

	if (m_global_timestamp != m_view.getTimestamp())
	{
		m_world_to_view = m_view;
		m_world_to_view.noScaleInvert();

		m_world_to_screen = m_view_to_screen * m_world_to_view;
		m_global_timestamp = m_view.getTimestamp();
	}
}

void zglEngine::renderScreen()
{
	calcProjection();

	zglCore::setViewport(m_disp_x, m_disp_y, m_disp_w, m_disp_h);
	zglCore::clearBuffer(m_clear_color, true, true);

	zglCore::resetModelView();
	zglCore::resetProjection();

	zglCore::setDefaultNormal();

	if(m_light.m_updated)
	{
		//!< Light env setup, invoke only when parameter changed.
		m_light.appLighting();
	}

	if(m_fog.m_updated)
	{
		m_fog.applyFog();
	}

	zglCore::setCullFace(false);

	zglCore::setProjection( m_view_to_screen);
	zglCore::setDepthTest(DEPTH_LEQUAL, false);

	zglCore::setDefaultNormal();

	m_node_list.renderPrim();

	m_node_list.renderSprite();

	m_node_list.renderPrimAlpha();

	zglCore::setProjection( m_2d_to_screen);
	zglCore::setDepthTest(DEPTH_ALWAYS, false);

	m_node_list.renderPrim2D();
}

void zglEngine::calcWorldTo2D(zglVector& pos, zglMatrix & local)
{
	calcProjection();

	zglMatrix m = m_world_to_screen * local;
	float w = m.m14 * pos.x + m.m24 * pos.y + m.m34 * pos.z + m.m44;

	pos = m * pos;
	pos /= w;

	pos.x *= m_width / 2.0f;
	pos.y *= m_height / 2.0f;
	pos.z = (1.0f - pos.z) / 2.0f;
}

void zglEngine::calcWorldTo2D(zglVector* pos, int size, zglMatrix & local)
{
	calcProjection();

	zglMatrix m = m_world_to_screen * local;

	for (int i = 0; i < size; i++)
	{
		float w = m.m14 * pos[i].x + m.m24 * pos[i].y + m.m34 * pos[i].z
				+ m.m44;

		pos[i] = m * pos[i];
		pos[i] /= w;

		pos[i].x *= m_width / 2.0f;
		pos[i].y *= m_height / 2.0f;
		pos[i].z = (1.0f - pos[i].z) / 2.0f;
	}
}

void zglEngine::calc2DToScreen(zglVector& pos)
{
	calcProjection();
	pos.absolutePos(m_2d_to_screen);
}

// ----------------------------------------------------------------------------




