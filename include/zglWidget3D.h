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

#ifndef __zgl_widget_3d_h__
#define __zgl_widget_3d_h__

#include "zglWidget.h"

#include "zglPrim3D.h"

/**
 * @brief A 3D widget base.
 *
 * @note if the 3D widget is put at the focus distance of the Engine.
 * 		The mapped position is the same as the 3D position. for example,
 * 		if the focus distance is 990.0f, the position (200.0f, 200.0f, 990.0f)
 * 		will be mapped to (200, 200) of the screen coordinate.
 */
class zglWidget3D: public zglWidget
{
public:
	zglWidget3D();

	/**
	 * @brief get the screen position of the widget3D.
	 * @return the mapped position of the center coordinate of the widget
	 * on the screen.
	 */
	zglVector getScreenPosition();

protected:
	/**
	 * @brief Update the hit test rectangle.
	 */
	virtual void updateHitRect();

	/**
	 * Interface to check the widget is hit or not.
	 */
	virtual bool hitTest(float x, float y);

protected:
	zglPrim3D m_prim; //!< A default primitive for the 3D widget.

	/**
	 * @brief The hit test boundary rectangle of the widget.
	 *
	 * The boundary rectangle is used to store the hit test rectangle information to
	 * decrease the calculation of the operation #hitTest().
	 *
	 * As to #zglWidget3D, the boundary rectangle will need to be updated when
	 * these situation occurred:
	 * - The dimension changed.
	 * - The local matrix changed, such as move, rotate, scale, etc
	 * - The global matrix changed.
	 *
	 * The flag #FLAG_VERTEX_UPDATE should be considered too, but the flag is only
	 * changed for zglBitmapTextView, so we just skip the flag.
	 *
	 * Order : left-top, left-down, right-down, right top
	 *
	 */
	zglVector m_boundary[4];
	unsigned short m_local_timestamp; //!< The time stamp of the local matrix
	unsigned short m_global_timestamp; //!< The time stamp of the global matrix

public:
#ifdef _DEBUG_
	virtual void getBound(zglVertex * v, int *count);
#endif
};


#endif
