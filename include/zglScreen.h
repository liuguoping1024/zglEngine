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


#ifndef __zgl_screen_h__
#define __zgl_screen_h__

/**
 * @brief The viewport of the system.
 *
 * @see Multiple Viewports
 * http://nehe.gamedev.net/data/lessons/lesson.asp?lesson=42
 *
 * @deprecated Multiple viewport is not suitable for Embeded Systems.
 */
class zglScreen
{
public:
	zglScreen()
	{
	}

	virtual ~zglScreen()
	{
	}
};


#endif
