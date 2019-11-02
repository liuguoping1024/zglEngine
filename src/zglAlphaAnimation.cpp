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

#include "zglWidget.h"
#include "zglAlphaAnimation.h"

zglAlphaAnimation::zglAlphaAnimation()
{
	m_from = 0.0f;
	m_to = 1.0f;
}

bool zglAlphaAnimation::update(long time)
{
	if (m_widget == NULL)
	{
		m_state = ANIM_STATE_NONE;
		return false;
	}

	long dur = time - m_start_time - m_delay_time;

	if(dur < 0)
	{
		return true;//Return true to make the engine continue.
	}

	float t = dur / float(m_dur_time);

	if (dur >= m_dur_time)
	{
		setAlpha(1.0f, true);

		m_state = ANIM_STATE_NONE;
	}
	else
	{
		float it = getValue(t);

		setAlpha(it, false);
	}

	return true;
}

void zglAlphaAnimation::init(float from, float to)
{
	m_from = from;
	if (m_from < 0.0f)
		m_from = 0.0f;
	if (m_from > 1.0f)
		m_from = 1.0f;

	m_to = to;
	if (m_to < 0.0f)
		m_to = 0.0f;
	if (m_to > 1.0f)
		m_to = 1.0f;
}

void zglAlphaAnimation::setAlpha(float it, bool lastFrame)
{
	float a;
	int alpha;
	if (lastFrame)
	{
		a = m_to;
	}
	else
	{
		a = it * (m_to - m_from) + m_from;
	}

	alpha = a * 0xff;

	if (alpha < 0x00)
		alpha = 0x00;
	if (alpha > 0xff)
		alpha = 0xff;

	m_widget->setAlpha(alpha);
}


