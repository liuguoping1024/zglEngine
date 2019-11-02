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

#ifndef __zgl_widget_alpha_animation_h__
#define __zgl_widget_alpha_animation_h__

#include "zglAnimation.h"

/**
 * @brief Animation of changing the alpha value of a widget.
 *
 * @note the Animation will update the alpha value of all the vertex of the
 * 		widget. If special requirement need to update the alpha value. A new
 * 		Animation is recommended.
 */
class zglAlphaAnimation: public zglAnimation
{
public:
	zglAlphaAnimation();

	/**
	 * Set the Alpha animation start value and stop value;
	 * @param from the beginning alpha value of the animation.
	 * @param to the ending alpha value of the animation.
	 */
	void init(float from, float to);

protected:
	/**
	 * Update the animation.
	 */
	virtual bool update(long time);

private:
	/**
	 * Set the alpha value to widget due to current interpolated value.
	 */
	void setAlpha(float it, bool lastFrame);

private:
	float m_from; //!< Animation start value(begin alpha value)
	float m_to; //!< Animation stop value(end alpha value)
};
#endif
