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
#include "zglEaseInterpolator.h"
#include "zglBasicType.h"

typedef float (*getValueFunc)(float input);

/* quadratic */
static float ease_in_quad(float input);
static float ease_out_quad(float input);
static float ease_in_out_quad(float input);

/* cubic */
static float ease_in_cubic(float input);
static float ease_out_cubic(float input);
static float ease_in_out_cubic(float input);

/* quartic */
static float ease_in_quart(float input);
static float ease_out_quart(float input);
static float ease_in_out_quart(float input);

/* quintic */
static float ease_in_quint(float input);
static float ease_out_quint(float input);
static float ease_in_out_quint(float input);

/* sinusoidal */
static float ease_in_sine(float input);
static float ease_out_sine(float input);
static float ease_in_out_sine(float input);

/* exponential */
static float ease_in_expo(float input);
static float ease_out_expo(float input);
static float ease_in_out_expo(float input);

/* circular */
static float ease_in_circ(float input);
static float ease_out_circ(float input);
static float ease_in_out_circ(float input);

/* elastic */
static float ease_in_elastic(float input);
static float ease_out_elastic(float input);
static float ease_in_out_elastic(float input);

/* overshooting cubic */
static float ease_in_back(float input);
static float ease_out_back(float input);
static float ease_in_out_back(float input);

/* exponentially decaying parabolic  */
static float ease_in_bound(float input);
static float ease_out_bound(float input);
static float ease_in_out_bound(float input);

static const struct {
	int mode;
	getValueFunc func;
} g_interpolates[] =
{
	/* quadratic */
	{ EASE_IN_QUAD, ease_in_quad },
	{ EASE_OUT_QUAD, ease_out_quad },
	{ EASE_IN_OUT_QUAD, ease_in_out_quad },

	/* cubic */
	{ EASE_IN_CUBIC, ease_in_cubic},
	{ EASE_OUT_CUBIC, ease_out_cubic},
	{ EASE_IN_OUT_CUBIC, ease_in_out_cubic},

	/* quartic */
	{ EASE_IN_QUART, ease_in_quart},
	{ EASE_OUT_QUART, ease_out_quart},
	{ EASE_IN_OUT_QUART, ease_in_out_quart},

	/* quintic */
	{ EASE_IN_QUINT, ease_in_quint},
	{ EASE_OUT_QUINT, ease_out_quint},
	{ EASE_IN_OUT_QUINT, ease_in_out_quint},

	/* sinusoidal */
	{ EASE_IN_SINE, ease_in_sine},
	{ EASE_OUT_SINE, ease_out_sine},
	{ EASE_IN_OUT_SINE, ease_in_out_sine},

	/* exponential */
	{ EASE_IN_EXPO, ease_in_expo},
	{ EASE_OUT_EXPO, ease_out_expo},
	{ EASE_IN_OUT_EXPO, ease_in_out_expo},

	/* circular */
	{ EASE_IN_CIRC, ease_in_circ},
	{ EASE_OUT_CIRC, ease_out_circ},
	{ EASE_IN_OUT_CIRC, ease_in_out_circ},

	/* elastic */
	{ EASE_IN_ELASTIC, ease_in_elastic },
	{ EASE_OUT_ELASTIC, ease_out_elastic },
	{ EASE_IN_OUT_ELASTIC, ease_in_out_elastic },

	/* overshooting cubic */
	{ EASE_IN_BACK, ease_in_back },
	{ EASE_OUT_BACK, ease_out_back },
	{ EASE_IN_OUT_BACK, ease_in_out_back },

	/* exponentially decaying parabolic */
	{ EASE_IN_BOUNCE, ease_in_bound },
	{ EASE_OUT_BOUNCE, ease_out_bound },
	{ EASE_IN_OUT_BOUNCE, ease_in_out_bound },
};


float zglEaseInterpolator::getValue(float input)
{
	if(m_mode > EASE_MAX )
	{
		return input;
	}

	return g_interpolates[m_mode].func(input);
}

///////////////////////////////////////////////////////////////////////////////
/* quadratic */
static float ease_in_quad(float t)
{
	return (float) t * t;
}
static float ease_out_quad(float t)
{
	return -(float) t * (t - 2.0f);
}

static float ease_in_out_quad(float t)
{
	double st = t * 2.0;

	if (st < 1.0)
		return (float) (0.5 * st * st);

	st -= 1.0;
	return (float) (-0.5 * (st * (st - 2) - 1));
}

/* cubic */
static float ease_in_cubic(float t)
{
	return (float) t * t * t;
}

static float ease_out_cubic(float t)
{
	float st = t - 1.0f;
	return (float) st * st * st + 1.0f;
}

static float ease_in_out_cubic(float t)
{
	float st = t * 2;
	if (st < 1)
		return 0.5 * st * st * st;

	st -= 2.0f;

	return 0.5 * (st * st * st + 2);
}

/* quartic */
static float ease_in_quart(float t)
{
	return (float) t * t * t * t;
}

static float ease_out_quart(float t)
{
	float st = t - 1.0f;
	return -(float) (st * st * st * st - 1);
}

static float ease_in_out_quart(float t)
{
	float st = t * 2;
	if (st < 1)
		return 0.5 * st * st * st * st;

	st -= 2.0f;

	return -0.5 * (st * st * st * st - 2);
}

/* quintic */
static float ease_in_quint(float t)
{
	return (float) t * t * t * t * t;
}

static float ease_out_quint(float t)
{
	float st = t - 1.0f;
	return (float) (st * st * st * st * st + 1);
}

static float ease_in_out_quint(float t)
{
	float st = t * 2;
	if (st < 1)
		return 0.5 * st * st * st * st * st;

	st -= 2.0f;

	return 0.5 * (st * st * st * st * st + 2);
}

/* sinusoidal */
static float ease_in_sine(float t)
{
	return -(float) cos(t * ZGL_PI2) + 1.0f;
}

static float ease_out_sine(float t)
{
	return (float) sin(t * ZGL_PI2);
}

static float ease_in_out_sine(float t)
{
	return (float) (-0.5 * (cos(ZGL_PI * t) - 1));
}

/* exponential */
static float ease_in_expo(float t)
{
	if (t == 0.0f)
		return 0.0f;
	return (float) pow(2, 10 * (t - 1.0f));
}

static float ease_out_expo(float t)
{
	if (t == 1.0f)
		return 1.0f;
	return (float) (-pow(2, -10 * t) + 1);
}

static float ease_in_out_expo(float t)
{
	if (t == 0.0f)
		return 0.0f;
	else if (t == 1.0f)
		return 1.0f;

	float st = t * 2;

	if (st < 1.0f)
		return 0.5f * pow(2, 10 * (st - 1.0f));

	st -= 1.0f;

	return 0.5f * (-pow(2, -10 * st) + 2);
}

/* circular */
static float ease_in_circ(float t)
{
	return -1.0f * (sqrt(1 - t * t) - 1);
}

static float ease_out_circ(float t)
{
	float st = t - 1.0f;
	return (float)sqrt(1 - st * st);
}

static float ease_in_out_circ(float t)
{
	float st = t * 2;
	if (st < 1)
		return -0.5f * (sqrt(1 - st * st) - 1);

	st -= 2.0f;

	return 0.5f * (sqrt(1 - st * st) + 1);
}

/* elastic */
static float ease_in_elastic(float t)
{
	if( t == 1.0f) return 1.0f;

	t -= 1.0f;

	return -(pow(2, 10 * t) * sin((t * 10.0 / 3.0 - 0.25) * ZGL_2PI));
}

static float ease_out_elastic(float t)
{
	if( t == 1.0f) return 1.0f;

	return pow(2, -10 * t) * sin((t * 10.0 / 3.0 - 0.25) * ZGL_2PI) + 1.0f;
}

static float ease_in_out_elastic(float t)
{
	float st = t * 2;
	if(st < 1)
	{
		st -= 1;
		return -0.5f * (pow(2, 10 * st) * sin( (st * 20.0f / 9 - 0.25f) * ZGL_2PI ) );
	}
	else
	{
		st -= 1;
		return pow(2, -10 * st) * sin( (st * 20.0f / 9 - 0.25f) * ZGL_2PI ) * 0.5f + 1.0f;
	}
	return t;
}

/* overshooting cubic */
static float ease_in_back(float t)
{
	return (float)(t * t * ((1.70158 + 1) * t - 1.70158));
}

static float ease_out_back(float t)
{
	float st = t - 1.0f;
	return st * st * ((1.70158 + 1) * st + 1.70158) + 1.0f;
}

static float ease_in_out_back(float t)
{
	float st = t * 2;
	double s = 1.70158 * 1.525;

	if (st < 1.0f)
		return 0.5f * (st * st * ((s + 1) * st - s));

	st -= 2.0f;

	return 0.5f * (st * st * ((s + 1) * st + s) + 2);
}

/* exponentially decaying parabolic  */

static float ease_bound_internal(float t)
{
	if (t < 1.0f / 2.75)
	{
		return 7.5625 * t * t;
	}
	else if (t < 2.0f / 2.75)
	{
		t -= (1.5f / 2.75);
		return 7.5625 * t * t + 0.75;
	}
	else if (t < 2.5f / 2.75)
	{
		t -= (2.25f / 2.75);
		return 7.5625 * t * t + 0.9375f;
	}
	else
	{
		t -= (2.625f / 2.75);
		return 7.5625 * t * t + 0.984375f;
	}
}

static float ease_in_bound(float t)
{
	return 1.0f - ease_bound_internal(1.0f - t);
}

static float ease_out_bound(float t)
{
	return ease_bound_internal(t);
}

static float ease_in_out_bound(float t)
{
	float st = t * 2;
	if( st < 1)
	{
		return (1.0f - ease_bound_internal(1.0f - st)) * 0.5f;
	}
	else
	{
		return ease_bound_internal(st - 1.0f) * 0.5f + 0.5f;
	}
}



