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

#include <math.h>
#include "zglInterpolator.h"
#include "zglBasicType.h"


//!< --------------------------------------------------------------------------
float zglInterpolator::getValue(float t)
{
	return t;
}

//!< --------------------------------------------------------------------------
float zglLinearInterpolator::getValue(float t)
{
	return t;
}

//!< --------------------------------------------------------------------------
float zglAccelerateInterpolator::getValue(float t)
{
	if (m_factor == 1.0f)
	{
		return t * t;
	}
	else
	{
		return (float) pow(t, m_factor * 2);
	}
}

//!< --------------------------------------------------------------------------
float zglDecelerateInterpolator::getValue(float t)
{
	if (m_factor == 1.0f)
	{
		return (float) (1.0f - (1.0f - t) * (1.0f - t));
	}
	else
	{
		return (float) (1.0f - pow((1.0f - t), m_factor * 2.0f));
	}
}

//!< --------------------------------------------------------------------------
float zglAccelerateDecelerateInterpolator::getValue(float t)
{
	return (float) (cos((t + 1) * ZGL_PI) / 2.0f) + 0.5f;
}

//!< --------------------------------------------------------------------------
float zglAnticipateInterpolator::getValue(float t)
{
	return t * t * ((m_tension + 1) * t - m_tension);
}

//!< --------------------------------------------------------------------------
float zglAnticipateOvershootInterpolator::getValue(float t)
{
	if (t < 0.5f)
		return 0.5f * a(t * 2.0f, m_tension);
	else
		return 0.5f * (o(t * 2.0f - 2.0f, m_tension) + 2.0f);
}

float zglAnticipateOvershootInterpolator::a(float t, float s)
{
	return t * t * ((s + 1) * t - s);
}

float zglAnticipateOvershootInterpolator::o(float t, float s)
{
	return t * t * ((s + 1) * t + s);
}

//!< --------------------------------------------------------------------------
float zglBounceInterpolator::getValue(float t)
{
	t *= 1.1226f;

	if (t < 0.3535f)
		return bounce(t);
	else if (t < 0.7408f)
		return bounce(t - 0.54719f) + 0.7f;
	else if (t < 0.9644f)
		return bounce(t - 0.8526f) + 0.9f;
	else
		return bounce(t - 1.0435f) + 0.95f;
}

float zglBounceInterpolator::bounce(float t)
{
	return t * t * 0.8f;
}

//!< --------------------------------------------------------------------------
float zglCycleInterpolator::getValue(float t)
{
	return (float) sin(2 * m_cycle * t * ZGL_PI);
}

//!< --------------------------------------------------------------------------
float zglOvershootInterpolator::getValue(float t)
{
	t -= 1.0f;

	return t * t * ((m_tension + 1) * t + m_tension) + 1.0f;
}

