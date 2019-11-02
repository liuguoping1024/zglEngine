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

#ifndef __zgl_widget_2d_h__
#define __zgl_widget_2d_h__

#include "zglWidget.h"

#include "zglPrim2D.h"
#include "zglVector.h"

/**
 * @brief A 2D widget base.
 *
 * The 2D widget is implemented by Prim2D, all the 2D widget will be floating
 * above the 3D widgets.
 *
 * The 2D widget is suit for GUI Components.
 */
class zglWidget2D: public zglWidget
{
public:
	zglWidget2D();

	/**
	 * @brief Set the dimension of the 2D widgets.
	 */
	void setDimension(unsigned int width, unsigned int height);

protected:
	/**
	 * @brief Hit test interface.
	 *
	 * @param x The hit pointer.
	 * @param y The hit pointer.
	 *
	 * @return Return true if current widget is hit.
	 */
	virtual bool hitTest(float x, float y);

	/**
	 * @brief Update the hit test rectangle.
	 */
	virtual void updateHitRect();

protected:
	unsigned int m_width;//!< Height of the 2D widget.
	unsigned int m_height;//!< Width of the 2D widget.

	zglPrim2D m_prim; //!< A default primitive for the 2D widget.

	/**
	 * @brief The hit test rectangle.
	 * left-top, right-down
	 */
	zglVector m_boundary[2];
	unsigned short m_local_timestamp; //!< The time stamp of the widget's local matrix.

public:

public:
#ifdef _DEBUG_
	virtual void getBound(zglVertex * v, int *count);
#endif
};
#endif
