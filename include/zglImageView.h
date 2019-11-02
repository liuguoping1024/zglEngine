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


#ifndef __zgl_image_view_h__
#define __zgl_image_view_h__

#include <stdlib.h>
#include "zglWidget3D.h"

/**
 * @brief Image view in the 3D space.
 *
 * The Image view is the container of one texture, the image view will display
 * the texture just as a sprite does.
 */
class zglImageView: public zglWidget3D
{
public:
	zglImageView();

	/**
	 * @brief initial the imageview and put the imageview into the render queue.
	 *
	 * The image will not be seen until the function #init() is call.
	 *
	 * The function can be invoked multi-times to reset the imageview and
	 * change the position.
	 *
	 * @param texture texture binded to the imageview.
	 * @param width width of the imageview
	 * @param height height of the imageview
	 * @param antialias true for better effect.
	 */

	void init(zglTexture * texture, unsigned int width = 128,
			unsigned int height = 128, bool antialias = false);

	/**
	 * @brief Set the texture for the zglImageView.
	 *
	 * Only one texture is supported.
	 * @param texture the texture will be bind to image view.
	 */
	virtual void setTexture(zglTexture * texture);

	/**
	 * @brief Set the dimension of the image view;
	 */
	void setDimension(unsigned int width, unsigned int height);

	/**
	 * @brief Set special effect of the image view.
	 *
	 *@see
	 * - EFFEACT_NONE
	 * - EFFEACT_MIRROR
	 * - EFFEACT_FLIP_X
	 * - EFFEACT_FLIP_Y
	 */
	void setEffect(unsigned int effect);

	/**
	 * @brief Default action when pressing.
	 * - ACTION_FLIP_BLINK
	 */
	void setAction(unsigned int action);

	/**
	 * @brief Special transform
	 */
	void setShear(float dx, float dy);

	/**
	 * @brief Set the UV value to the vertex of the image view.
	 *
	 * @param top The uv value of the top boundary*
	 * @param left The uv value of the left boundary
	 * @param right The uv value of the right boundary
	 * @param bottom The uv value of the bottom boundary
	 */
	void setTexCoord(float left, float top, float right, float bottom);

	/**
	 * @brief Set the UV value to the vertex of the image view.
	 *
	 * @param coord The coordinate value of the boundary.
	 *
	 * @note The parameter of the array refer to #setTexCoord();
	 */
	void setTexCoord(const float coord[]);

	/**
	 * @brief update the coordinate of the vertex of the imageview.
	 * @see LEFT_TOP
	 * @see LEFT_BOOTOM
	 * @see RIGHT_TOP
	 * @see RIGHT_BOTTOM
	 */
	void setCoordOffset(unsigned int vertexIndex, float dx, float dy, float dz);

	/**
	 * @brief Set the alpha to the vertex of the image view.
	 * @param alpha the color will set to the all of the vertex of the image view.
	 */
	virtual void setAlpha(unsigned char alpha);

	/**
	 * @brief Set the alpha to the vertex of the image view.
	 *
	 * @param alphaLT the alpha will be set to the left-top of the vertex of the image view.
	 * @param alphaRT the alpha will be set to the right-top of the vertex of the image view.
	 * @param alphaLB the alpha will be set to the left-bottom of the vertex of the image view.
	 * @param alphaRB the alpha will be set to the right-bottom of the vertex of the image view.
	 */
	void setAlpha(unsigned char alphaLT, unsigned char alphaRT,
			unsigned char alphaLB, unsigned char alphaRB);

	/**
	 * @brief Set the color to all the vertex of the image view.
	 * @param color the color will set to the all of the vertex of the image view.
	 */
	void setColor(unsigned int color);

	/**
	 * @brief Set the color to the vertex of the image view.
	 * @param colorLT color to the left-top corner vertex
	 * @param colorRT color to the right-top corner vertex
	 * @param colorLB color to the left-bottom corner vertex
	 * @param colorRB color to the right-bottom corner vertex
	 */
	void setColor(unsigned int colorLT, unsigned int colorRT,
			unsigned int colorLB, unsigned int colorRB);

public:
	enum
	{
		EFFECT_NONE = 0, //!< No effect

		EFFECT_MIRROR, //!< Static mode, the upper and down pixel will reversed
				//!< automatic, and the lower
		EFFECT_FLIP_X, //!< Static mode
		EFFECT_FLIP_Y, //!< Static mode
	};

	enum
	{
		ACTION_FLIP_NONE = 0, //!< No action
		ACTION_FLIP_BLINK,
	//!< Dynamic mode, the imageview will decrease the alpha when pressed.
	};

	/**
	 * @brief Position index of the Vertex
	 */
	enum
	{
		LEFT_TOP = 0,
		LEFT_BOOTOM = 1,
		RIGHT_TOP = 2,
		RIGHT_BOTTOM = 3,
	};

protected:

	/**
	 * @brief Update handler of the widget when dimension changed.
	 */
	virtual void updateDimension();

	/**
	 * @brief Update the hit test rectangle.
	 */
	virtual void updateHitRect();

    /**
     * @brief Handle the touch events to the widget.
     *
     * @param event The touch event.
     * @return True if the event was handled, false otherwise.
     */
	virtual bool onTouchEvent(zglTouchEvent *event);

protected:
	static const float m_borderFrac = 0.99f;//!< Anti-alias factor.

	/* The covers are made up of 16 vertices, 9 quads, 18 triangles. The
	 four colors of the center vertices are fully opaque while all the
	 outside vertices are fully transparent. This produces a thin fade out
	 at the edges which avoids anti-aliasing.
		0--1------2--3
		|  |      |  |
		4--5------6--7
		|  |      |  |
		|  |      |  |
		|  |      |  |
		8--9-----10--11
		|  |      |  |
		12-13----14--15
	*/

	zglVertex m_vertex[16]; //!< vertex of the image view

	enum
	{
		AA_LEFT_TOP = 5,
		AA_LEFT_BOTTOM = 9,
		AA_RIGHT_TOP = 6,
		AA_RIGHT_BOTTOM = 10,
	};

	float m_alpha[4];//!< Backup for hit effect.

	float m_offset_x[4];//!< x Offset of the image view, special effect.
	float m_offset_y[4];//!< y Offset of the image view, special effect.
	float m_offset_z[4];//!< z Offset of the image view, special effect.

	unsigned int m_width; //!< Width of the image view.
	unsigned int m_height;//!< Height of the image view.

	float m_uv_left; //!< Texture coordinate.
	float m_uv_right;//!< Texture coordinate.
	float m_uv_top;//!< Texture coordinate.
	float m_uv_bottom;//!< Texture coordinate.

	float m_shear_dy;//!< x coordinate shearing
	float m_shear_dx;//!< y coordinate shearing

	unsigned short m_effect_type;//!< effect type, @see EFFECT_MIRROR
	unsigned short m_action_type;//!< action type. @see ACTION_FLIP_BLINK

	bool m_antialias;//!< Anti-alias function.
};

#endif
