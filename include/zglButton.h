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

#ifndef __zgl_button_h__
#define __zgl_button_h__

#include "zglPrim2D.h"
#include "zglWidget2D.h"

#include "zglTexture.h"

/**
 * @brief  Press and normal state are using the same Texture with different texture
 * coordinates.
 *
 * To make full use of the class, One texture with both press and normal image should
 * be well prepared.
 *
 * The Texture coordinates of the button are different but with the same dimension,
 * The Offset of the Texture coordinates are defined by <i>tx</i>, and <i>ty</i>.
 */
class zglButton: public zglWidget2D
{
public:
	zglButton();

	/**
	 * @brief  initial the button widget.
	 *
	 * @param tex texture of the button, the texture should has at least two image
	 *  for button.
	 *
	 * @param width the initial width of the widget.
	 * @param height the initial height of the widget.
	 */
	void init(zglTexture * tex, unsigned int width, unsigned int height);

	/**
	 *
	 * @brief set the texture coordinate and offset for button.
	 * The texture coordinates of the Normal state need to be supplied, and
	 * the texture coordinates of the Press state can be calculated by the texture
	 * coordinates offset of tx and ty.
	 *
	 * @param top the normal-texture value of the top boundary.
	 * @param left the normal-texture value of the left boundary.
	 * @param right the normal-texture value of the right boundary.
	 * @param bottom the normal-texture value of the bottom boundary.
	 * @param tx the press-texture x offset.
	 * @param ty the press-texture y offset.
	 */
	void setTexCoord(float left, float top, float right, float bottom,
			float tx = 0.0f, float ty = 0.0f);

protected:
	/**
	 * @brief Update handler of the widget when dimension changed.
	 */
	virtual void updateDimension();

    /**
     * @brief Handle the touch events to the widget.
     *
     * @param event The touch event.
     * @return True if the event was handled, false otherwise.
     */
	virtual bool onTouchEvent(zglTouchEvent *event);

protected:
	zglVertex m_vertex[4]; //!< default vertex array of the widget.

	float m_tx; //!< x texture coordinate offset of the texture
	float m_ty; //!< y texture coordinate offset of the texture
};

#endif




