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


#ifndef __zgl_ease_interpolator_h__
#define __zgl_ease_interpolator_h__

#include "zglInterpolator.h"

/**
 * @brief enum of ease animation.
 */
enum zglEaseType
{
	  /* quadratic */
	  EASE_IN_QUAD,
	  EASE_OUT_QUAD,
	  EASE_IN_OUT_QUAD,

	  /* cubic */
	  EASE_IN_CUBIC,
	  EASE_OUT_CUBIC,
	  EASE_IN_OUT_CUBIC,

	  /* quartic */
	  EASE_IN_QUART,
	  EASE_OUT_QUART,
	  EASE_IN_OUT_QUART,

	  /* quintic */
	  EASE_IN_QUINT,
	  EASE_OUT_QUINT,
	  EASE_IN_OUT_QUINT,

	  /* sinusoidal */
	  EASE_IN_SINE,
	  EASE_OUT_SINE,
	  EASE_IN_OUT_SINE,

	  /* exponential */
	  EASE_IN_EXPO,
	  EASE_OUT_EXPO,
	  EASE_IN_OUT_EXPO,

	  /* circular */
	  EASE_IN_CIRC,
	  EASE_OUT_CIRC,
	  EASE_IN_OUT_CIRC,

	  /* elastic */
	  EASE_IN_ELASTIC,
	  EASE_OUT_ELASTIC,
	  EASE_IN_OUT_ELASTIC,

	  /* overshooting cubic */
	  EASE_IN_BACK,
	  EASE_OUT_BACK,
	  EASE_IN_OUT_BACK,

	  /* exponentially decaying parabolic */
	  EASE_IN_BOUNCE,
	  EASE_OUT_BOUNCE,
	  EASE_IN_OUT_BOUNCE,

	  EASE_MAX = EASE_IN_OUT_BOUNCE
};

/**
 * @brief Interpolator of ease package.
 *
 * The ease animation is widely used in <b>adobe FLASH</b>.
 */
class zglEaseInterpolator : public zglInterpolator
{
public:
	zglEaseInterpolator()
	{
		m_mode = 0;
	}

	/**
	 * @brief Get the interpolated value.
	 * @param input input value, [0.0f, 1.0f].
	 */
	virtual float getValue(float input);

	/**
	 * @brief set the mode of the ease animation
	 * @param mode animation mode
	 *
	 * @see zglEaseType
	 */
	inline void setMode(unsigned int mode)
	{
		m_mode = (unsigned int)mode;
	}

private:
	unsigned int m_mode;

};

#endif
