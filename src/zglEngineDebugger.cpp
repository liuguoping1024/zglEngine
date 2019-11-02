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

#ifdef _DEBUG_

#include <stdio.h>
#include <stdlib.h>

#include "zglEngineDebugger.h"
#include "zglEngine.h"

#include "zglCore.h"

#include "zglUtil.h"

#include "zglEventType.h"

#include <math.h>
#include <time.h>

/************************************************************************/
void zglEngineDebugger::onFini(void)
{
}

void zglEngineDebugger::onResize(int w, int h)
{
}

void zglEngineDebugger::onInit()
{
	//debug
	m_debug_vertex[0].init(-100.0f, 100.0f, 0, 0.0f, 0.0f, 0xffffffff);
	m_debug_vertex[1].init(-100.0f, -100.0f, 0, 0.0f, 1.0f, 0xffffffff);
	m_debug_vertex[2].init( 100.0f, -100.0f, 0, 1.0f, 1.0f, 0xffffffff);
	m_debug_vertex[3].init( 100.0f, 100.0f, 0, 1.0f, 0.0f, 0xffffffff);

	m_debug_outline.init(DRAW_LINE_LOOP, m_debug_vertex, 4, NULL);

	m_debug_outline.setVisible(false);

	m_debug_text.init();
}

bool zglEngineDebugger::handleTouchEvent(int type, int x, int y, long time)
{
	zglEngine * glEngine = zglEngine::getEngine();
	zglWidget * w = glEngine->m_widget_list.hitTest(x, y);

	switch(type)
	{
		case TOUCH_DOWN:
		case TOUCH_MOVE:
		m_debug_text.clear();
		if(w != 0)
		{
			w->getBound(m_debug_vertex, &m_debug_vertex_count);
			m_debug_outline.init(DRAW_LINE_LOOP, m_debug_vertex, m_debug_vertex_count, NULL);

			sprintf(m_text, "[z = %d]", w->getZOffset());
			m_debug_text.print(0, 0, m_text);
		}
		else
		{
			//m_debug_outline.setFlag(zglPrim::FLAG_INVISIBLE);
			m_debug_outline.setVisible(false);
			sprintf(m_text, "( %d, %d )", (int)x, (int)y);
			m_debug_text.print(0, 0, m_text);
		}

		break;

		case TOUCH_UP:
		{
			m_debug_outline.setVisible(false);
			m_debug_text.clear();
		}
		break;
	}

	return true;
}

bool zglEngineDebugger::onUpdate(long time_val)
{
	return true;
}

#endif //#ifdef _DEBUG_
