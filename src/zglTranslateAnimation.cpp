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
#include "zglTranslateAnimation.h"


zglTranslateAnimation::zglTranslateAnimation()
{
	m_FromX = 0.0f;
	m_ToX = 0.0f;

	m_FromY = 0.0f;
	m_ToY = 0.0f;

	m_FromZ = 0.0f;
	m_ToZ = 0.0f;

	m_flag = TRANSLATE_XYZ;
}

void zglTranslateAnimation::init(float fromX, float toX, float fromY,
		float toY, float fromZ, float toZ)
{
	m_FromX = fromX;
	m_ToX = toX;

	m_FromY = fromY;
	m_ToY = toY;

	m_FromZ = fromZ;
	m_ToZ = toZ;
}

void zglTranslateAnimation::setTranslateFlag(int flag)
{
	m_flag = flag & TRANSLATE_XYZ;
}

bool zglTranslateAnimation::update(long time)
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
		moveTo(1.0f, true);

#if 0 //TODO
		if (m_type == ANIM_TYPE_ABSOLUTE)
		{
			moveTo(1.0f, true);
		}
		else
		{
			move(1.0f, true);
		}
#endif
		m_state = ANIM_STATE_NONE;
	}
	else
	{
		float it = getValue(t);

		moveTo(it, false);

#if 0 //TODO
		if (m_type == ANIM_TYPE_ABSOLUTE)
		{
			moveTo(it, false);
		}
		else
		{
			move(it, false);
		}
#endif
	}

	return true;
}

void zglTranslateAnimation::moveTo(float it, bool lastFrame)
{
	float mx = m_widget->getX();
	float my = m_widget->getY();
	float mz = m_widget->getZ();

	if (lastFrame)
	{
		if (m_flag & TRANSLATE_X)
		{
			mx = m_ToX;
		}

		if (m_flag & TRANSLATE_Y)
		{
			my = m_ToY;
		}

		if (m_flag & TRANSLATE_Z)
		{
			mz = m_ToZ;
		}
	}
	else
	{
		if (m_flag & TRANSLATE_X)
		{
			mx = it * (m_ToX - m_FromX) + m_FromX;
		}

		if (m_flag & TRANSLATE_Y)
		{
			my = it * (m_ToY - m_FromY) + m_FromY;
		}

		if (m_flag & TRANSLATE_Z)
		{
			mz = it * (m_ToZ - m_FromZ) + m_FromZ;
		}
	}

	m_widget->locate(mx, my, mz);

	//m_widget->moveTo(mx, my, mz);
}

void zglTranslateAnimation::move(float it, bool lastFrame)
{
	//float mx = m_widget->getX();
	//float my = m_widget->getY();
	//float mz = m_widget->getZ();
	//TODO
	/*
	 if(m_flag | TRANSLATE_X)
	 {
	 mx = it * (m_ToX - m_FromX) + m_FromX;
	 }

	 if(m_flag | TRANSLATE_Y)
	 {
	 my = it * (m_ToY - m_FromY) + m_FromY;
	 }

	 if(m_flag | TRANSLATE_Z)
	 {
	 mz = it * (m_ToZ - m_FromZ) + m_FromZ;
	 }
	 */
}


