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


#ifndef __zgl_translate_animation_h__
#define __zgl_translate_animation_h__


#include "zglAnimation.h"

/**
 * @brief A simple animation of moving one widget from one position to another,
 * or moving one widget by a distance.
 */
class zglTranslateAnimation: public zglAnimation
{
public:
	zglTranslateAnimation();

	/**
	 * @brief Translate flags, designed to make the animation more efficiency.
	 */
	enum
	{
		TRANSLATE_X = 0x01, //!< translate with the X coordinate only.
		TRANSLATE_Y = 0x02, //!< translate with the Y coordinate only.
		TRANSLATE_Z = 0x04, //!< translate with the Z coordinate only.

		TRANSLATE_XY = TRANSLATE_X | TRANSLATE_Y,//!< translate the XY.
		TRANSLATE_XZ = TRANSLATE_X | TRANSLATE_Z,//!< translate the XZ.
		TRANSLATE_YZ = TRANSLATE_Y | TRANSLATE_Z,//!< translate the YZ.

		TRANSLATE_XYZ = TRANSLATE_X | TRANSLATE_Y | TRANSLATE_Z,//!< translate XYZ.
	};

	/**
	 *
	 * @brief initial the animation parameters.
	 *
	 * Under the ANIM_TYPE_ABSOLUTE, the values will be treat as the
	 * coordinate on screen.
	 *
	 * Under the ANIM_TYPE_RELATIVE, the value fromX, fromY, fromZ, will
	 * be treat as zero.
	 */
	void init(float fromX, float toX, float fromY, float toY, float fromZ,
			float toZ);

	/**
	 * @brief set the translate flag.
	 *
	 * @see #TRANSLATE_X
	 * @see #TRANSLATE_Y
	 * @see #TRANSLATE_Z
	 */
	void setTranslateFlag(int flag);

protected:
	/**
	 * Update the animation.
	 */
	virtual bool update(long time);

private:
	void moveTo(float it, bool lastFrame);

	void move(float it, bool lastFrame);

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
