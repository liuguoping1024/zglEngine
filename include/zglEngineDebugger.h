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


#ifndef __zgl_engine_debugger_h__
#define __zgl_engine_debugger_h__

#ifdef _DEBUG_

#include "zglPlugin.h"

#include "zglVertex.h"
#include "zglPrim2D.h"
#include "zglDebugText.h"

/**
 * The plug-in support part of the online debug function.
 */
class zglEngineDebugger : public zglPlugin
{
public:

	void onInit(void);

	void onFini(void);

	void onResize(int w, int h);

	bool onUpdate(long time_val);

	bool handleTouchEvent(int type, int x, int y, long time);

	virtual ~zglEngineDebugger()
	{
	}

private:
	zglPrim2D m_debug_outline;
	zglDebugText m_debug_text;

	zglVertex m_debug_vertex[4];//TODO
	int m_debug_vertex_count;



	char m_text[256];
};

#endif

#endif
