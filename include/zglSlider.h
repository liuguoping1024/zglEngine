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

#ifndef __zgl_slider_h__
#define __zgl_slider_h__

#include "zglWidget2D.h"
#include "zglTexture.h"
#include "zglVertex.h"

/**
 * @brief Slider component based on 2D primitives.
 *
 * @note the widget is still in developing. The mode of #SLIDER_BAR is not
 * supported.
 */
class zglSlider: public zglWidget2D
{
public:
	zglSlider();

	/**
	 * @brief initial the zglSlider widget.
	 *
	 * @param tex texture of the Slider
	 * @param width the initial width of the widget.
	 * @param height the initial height of the widget.
	 */
	void init(zglTexture * tex, unsigned int width, unsigned int height);

	/**
	 * @brief Set the texture coordinate to widget.
	 *
	 * The texture coordinate will be applied to all the particles.
	 * @param left the left coordinate.
	 * @param top the top coordinate.
	 * @param right the right coordinate.
	 * @param bottom the bottom coordinate.
	 */
	void setTexCoord(float left, float top, float right, float bottom);

	/**
	 * @brief set the mode of the slider
	 *
	 * @param valueMin min value.
	 * @param valueMax max value.
	 * @param mode the mode of the slider.
	 * @see #SLIDER_BAR
	 * @see #SLIDER_BARRELATIVE
	 */
	void setMode(float valueMin, float valueMax, int mode = SLIDER_BAR);

	/**
	 * @brief Slider direction selection.
	 * @param bVertical true for a vertical slider from up to down,
	 * 				false for a vertical from left to right.
	 */
	void setDirection(bool bVertical);

	/**
	 * @brief Set the current value
	 *
	 * @param value the current value. The value should be in the range of
	 * max value and min value set by #setMode
	 */
	void setValue(float value);

	/**
	 * @brief Get the current value
	 *
	 * @return the current value.
	 */
	inline float getValue() const
	{
		return m_value;
	}

public:
	enum
	{
		SLIDER_BAR = 0, //!< Mode 1, TBD
		SLIDER_BARRELATIVE = 1, //!< Mode 2, TBD
	};

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

	/**
	 * @brief Update the hit test rectangle.
	 */
	virtual void updateHitRect();

private:
	void updateVerticalBar(float dy);
	void updateHorizontalBar(float dx);

protected:

	zglVertex m_vertex[4]; //!< default vertex array of the widget.

	int m_mode; //!< slider mode, default value is SLIDER_BAR
	bool m_vertical; //!< slider direction, true for vertical slider.

	bool m_pressed; //!< Button state

	float m_valMax; //!< max value of the slider.
	float m_valMin; //!< min value of the slider.

	float m_value; //!< current value of the slider.
};

#endif
