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


#ifndef __zgl_interpolator_h__
#define __zgl_interpolator_h__

/**
 * @brief Interpolator(插值器) is a very important component for Animation.
 *
 * The <a href="http://en.wikipedia.org/wiki/Interpolation">Interpolator</a>
 * can get an interpolated value based on timeline. So animation can move, scale or
 * transparent due to the time-based value.
 *
 * The Interpolator including(Quote the equator from the Android source code):
 * - #zglLinearInterpolator
 * - #zglAccelerateInterpolator
 * - #zglDecelerateInterpolator
 * - #zglAccelerateDecelerateInterpolator
 * - #zglAnticipateInterpolator
 * - #zglAnticipateOvershootInterpolator
 * - #zglBounceInterpolator
 * - #zglCycleInterpolator
 * - #zglOvershootInterpolator
 *
 */
class zglInterpolator
{
public:
	zglInterpolator()
	{
	}

	virtual ~zglInterpolator()
	{
	}

	/**
	 * @brief get the interpolated value.
	 *
	 * @param input an input value, the value must between 0.0f, and 1.0f.
	 */
	virtual float getValue(float input);
};

/**
 * @brief Linear Interpolator.
 *
 * @note the interpolator is useless. In fact, the animation will return a linear
 * value when no interpolator available.
 */
class zglLinearInterpolator: public zglInterpolator
{
public:
	/**
	 * @brief get the interpolated value.
	 */
	virtual float getValue(float input);
};

/**
 * @brief Accelerate Interpolator.
 *
 * The object will moves/rotates more and more faster.
 */
class zglAccelerateInterpolator: public zglInterpolator
{
public:
	zglAccelerateInterpolator()
	{
		m_factor = 1.0f;
	}

	/**
	 * @brief set the factor.
	 */
	inline void setFactor(float factor)
	{
		m_factor = factor;
	}

	/**
	 * @brief get the interpolated value.
	 */
	virtual float getValue(float input);

private:
	float m_factor;
};

/**
 * @brief Decelerate Interpolator.
 *
 * The object will move/rotate more and more slower.
 */
class zglDecelerateInterpolator: public zglInterpolator
{
public:
	zglDecelerateInterpolator()
	{
		m_factor = 1.0f;
	}

	/**
	 * @brief set the factor.
	 */
	inline void setFactor(float factor)
	{
		m_factor = factor;
	}

	/**
	 * @brief get the interpolated value.
	 */
	virtual float getValue(float input);

private:
	float m_factor;
};

/**
 * @brief Accelerate & Decelerate Interpolator.
 *
 * The Object will move(...) faster, and than move(...) slower.
 */
class zglAccelerateDecelerateInterpolator: public zglInterpolator
{
public:
	/**
	 * @brief get the interpolated value.
	 */
	virtual float getValue(float input);
};

/**
 * @brief Anticipate Interpolator.
 *
 * The Object will move back, and then move faster to target.
 */
class zglAnticipateInterpolator: public zglInterpolator
{
public:
	zglAnticipateInterpolator()
	{
		m_tension = 2.0f;
	}

	/**
	 * @brief get the interpolated value.
	 */
	virtual float getValue(float input);

	/**
	 * @brief set the factor.
	 */
	inline void setFactor(float factor)
	{
		m_tension = factor;
	}

private:
	float m_tension;
};

/**
 * @brief Anticipate & Overshoot Interpolator.
 *
 * The Object will move back, and then move faster to target. when the object
 * get to the value, the object will then keep moves on and moves slower, at
 * the last, the object moves back to target.
 */
class zglAnticipateOvershootInterpolator: public zglInterpolator
{
public:
	zglAnticipateOvershootInterpolator()
	{
		m_tension = 3.0f;
	}

	/**
	 * @brief set the factor.
	 */
	inline void setFactor(float factor)
	{
		m_tension = factor * 1.5f;
	}

	/**
	 * @brief get the interpolated value.
	 */
	virtual float getValue(float input);

private:
	float m_tension;

	float a(float t, float s);
	float o(float t, float s);
};

/**
 * @brief Bounce Interpolator.
 *
 * ...
 */
class zglBounceInterpolator: public zglInterpolator
{
public:
	/**
	 * @brief get the interpolated value.
	 */
	virtual float getValue(float input);

private:
	float bounce(float t);
};

// ----------------------------------------------------------------------------

/**
 * @brief Cycle Interpolator.
 *
 * ...
 */
class zglCycleInterpolator: public zglInterpolator
{
public:
	zglCycleInterpolator()
	{
		m_cycle = 1.0f;
	}

	/**
	 * @brief get the interpolated value.
	 */
	virtual float getValue(float input);

	/**
	 * @brief set the factor.
	 */
	inline void setFactor(float factor)
	{
		m_cycle = factor;
	}

private:
	float m_cycle;
};

/**
 * @brief overshoot Interpolator, a famous interpolator.
 *
 * when the object get to the value, the object will then keep moves on
 * and moves slower, at the last, the object moves back to target.
 */
class zglOvershootInterpolator: public zglInterpolator
{
public:
	zglOvershootInterpolator()
	{
		m_tension = 2.0f;
	}

	/**
	 * @brief set the factor of overshot.
	 */
	inline void setFactor(float factor)
	{
		m_tension = factor;
	}

	/**
	 * @brief get the interpolated value.
	 */
	virtual float getValue(float input);

private:
	float m_tension;
};



#endif
