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

#ifndef __zgl_widget_rotate_animation_h__
#define __zgl_widget_rotate_animation_h__

#include "zglAnimation.h"

/**
 * @brief A simple animation of rotating one widget from one angle to another.
 *
 * @note because of the complex of the rotation and translation of the 3D
 * graphics, there is some shortcoming when using #zglRotateAnimation together
 * with #zglTranslateAnimation. please do not use the animation together if
 * no mastering the two animation classes.
 */
class zglRotateAnimation: public zglAnimation
{

public:
	zglRotateAnimation();

	/**
	 * @brief rotation flags, designed to make the animation more efficiency.
	 */
	enum
	{
		ROTATE_X = 0x01,//!< rotate with the X coordinate only.
		ROTATE_Y = 0x02,//!< rotate with the Y coordinate only.
		ROTATE_Z = 0x04,//!< rotate with the Z coordinate only.

		ROTATE_XY = ROTATE_X | ROTATE_Y,//!< rotate with XY.
		ROTATE_XZ = ROTATE_X | ROTATE_Z,//!< rotate with XZ.
		ROTATE_YZ = ROTATE_Y | ROTATE_Z,//!< rotate with YZ.

		ROTATE_XYZ = ROTATE_X | ROTATE_Y | ROTATE_Z,//!< rotate with XYZ.
	};

	/**
	 *
	 * @brief initial the animation parameters.
	 *
	 * @param fromX rotation from radian angle.
	 * @param toX rotation to radian angle.
	 * @param fromY rotation from radian angle.
	 * @param toY rotation to radian angle.
	 * @param fromZ rotation from radian angle.
	 * @param toZ rotation to radian angle.
	 */
	void init(float fromX, float toX, float fromY, float toY, float fromZ,
				float toZ);

	/**
	 * @brief set the translate flag.
	 *
	 * @see #ROTATE_X
	 * @see #ROTATE_Y
	 * @see #ROTATE_Z
	 */
	void setRotateFlag(int flag);

protected:
	/**
	 * Update the animation.
	 */
	virtual bool update(long time);

private:

	void setAngle(float it, bool lastFrame);

private:
	float m_FromX;
	float m_ToX;

	float m_FromY;
	float m_ToY;

	float m_FromZ;
	float m_ToZ;

	int m_flag;
};

#endif
