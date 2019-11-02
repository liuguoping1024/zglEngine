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

#include <stdlib.h>

#include "zglAnimation.h"

zglAnimation::zglAnimation()
{
	m_next = NULL;
	m_prev = NULL;

	m_widget = NULL;
	m_interpolator = NULL;

	m_state = ANIM_STATE_NONE;

	m_tick = 0;
	m_tick_max = 0;

	m_dur_time = 1000;
	m_delay_time = 0;
}

zglAnimation::~zglAnimation()
{
}

void zglAnimation::bindWidget(zglWidget * widget)
{
	m_widget = widget;
}

void zglAnimation::setDurTime(long time)
{
	m_dur_time = time;
}

void zglAnimation::setInterpolator(zglInterpolator * interpolator)
{
	m_interpolator = interpolator;
}

float zglAnimation::getValue(float t)
{
	if (m_interpolator == NULL)
	{
		return t;
	}
	else
	{
		return m_interpolator->getValue(t);
	}
}

void zglAnimation::reset()
{
	m_state = ANIM_STATE_NONE;

	m_tick = 0;
	m_tick_max = 0;

	m_delay_time = 0;
}

void zglAnimation::start(long time, long delay)
{
	m_start_time = m_last_time = time;
	m_delay_time = delay;
	m_state = ANIM_STATE_RUNNING;
}

void zglAnimation::stop()
{
	m_state = ANIM_STATE_NONE;
}

bool zglAnimation::isRunning()
{
	return m_state == ANIM_STATE_RUNNING;
}

bool zglAnimation::isStopped()
{
	return m_state == ANIM_STATE_NONE;
}

bool zglAnimation::update(long time)
{
	m_last_time = time;
	return false;
}





