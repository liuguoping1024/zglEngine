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

#ifndef __zgl_debug_drawer_h_
#define __zgl_debug_drawer_h_

#include "zglPrim2D.h"

/**
 * @brief Special utility for output debug information online.
 * For the users, 'zglBitmapTextView' is suggested.
 */
class zglDebugText
{
public:
	zglDebugText();
	virtual ~zglDebugText();

	/**
	 * @brief initial the debug text
	 *
	 * The debug texture will be prepared,
	 */
	void init();

	/**
	 * @brief print the string at appointed position.
	 *
	 * @param x x-position.
	 * @param y y-position.
	 * @param text string contents.
	 */
	void print(int x, int y, const char * text);

	/**
	 * @brief clear current string.
	 */
	void clear();

private:
	static const int m_debug_text_max = 256;
	//max length is 256
	zglPrim2D m_text[m_debug_text_max];
	zglVertex m_text_vertex[m_debug_text_max * 4];

	//!< Only 64 character supported
	zglTexture m_font;

	int m_used_length;
	int m_text_length;
};

#endif
