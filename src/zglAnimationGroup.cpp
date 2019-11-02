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
#include "zglAnimationGroup.h"

zglAnimationGroup::zglAnimationGroup()
{
	m_head = NULL;
	m_tail = NULL;
}

void zglAnimationGroup::reset()
{
	zglAnimation* a = m_head;
	while (a != NULL)
	{
		a->reset();
		a = a->m_next;
	}
	zglAnimation::reset();
}

void zglAnimationGroup::start(long time, long delay)
{
	zglAnimation* a = m_head;
	while (a != NULL)
	{
		a->start(time);
		a = a->m_next;
	}

	zglAnimation::start(time, delay);
}

void zglAnimationGroup::stop()
{
	zglAnimation* a = m_head;
	while (a != NULL)
	{
		a->stop();
		a = a->m_next;
	}
	zglAnimation::stop();
}

bool zglAnimationGroup::isRunning()
{
	zglAnimation* a = m_head;
	while (a != NULL)
	{
		if (a->isRunning())
		{
			return true;
		}
		a = a->m_next;
	}

	return false;
}

bool zglAnimationGroup::isStopped()
{
	zglAnimation* a = m_head;
	while (a != NULL)
	{
		if (a->isStopped())
		{
			return true;
		}
		a = a->m_next;
	}

	return false;
}

bool zglAnimationGroup::update(long time)
{
	bool bRet = false;

	zglAnimation* a = m_head;
	while (a != NULL)
	{
		if (a->isRunning())
		{
			bRet = a->update(time) || bRet;
		}
		a = a->m_next;
	}

	return bRet;
}

void zglAnimationGroup::bindWidget(zglWidget * widget)
{
	zglAnimation* a = m_head;
	while (a != NULL)
	{
		a->bindWidget(widget);
		a = a->m_next;
	}
	zglAnimation::bindWidget(widget);
}

void zglAnimationGroup::addAnimation(zglAnimation * anim)
{
	if (anim == NULL)
		return;

	if (m_head == NULL && m_tail == NULL)
	{
		m_head = anim;
		m_tail = anim;

		anim->m_next = NULL;
		anim->m_prev = NULL;
	}
	else
	{
		zglAnimation *a = m_tail;

		anim->m_next = NULL;
		anim->m_prev = a;

		a->m_next = anim;
	}

	anim->stop();
	if (m_widget != NULL)
	{
		anim->bindWidget(m_widget);
	}
}

void zglAnimationGroup::removeAnimation(zglAnimation * anim)
{
	if (anim == NULL)
		return;

	if (anim == m_head)
	{
		m_head = anim->m_next;
		if (m_head != NULL)
		{
			m_head->m_prev = NULL;
		}
		else
		{
			m_tail = NULL;
		}
	}
	else if (anim == m_tail)
	{
		m_tail = anim->m_prev;
		if (m_tail != NULL)
		{
			m_tail->m_next = NULL;
		}
	}
	else
	{
		anim->m_next->m_prev = anim->m_prev;
		anim->m_prev->m_next = anim->m_next;
	}

	anim->m_next = NULL;
	anim->m_prev = NULL;
	anim->bindWidget(NULL);
}

void zglAnimationGroup::removeAllAnimation()
{
	zglAnimation* a = NULL;
	while (m_head != NULL)
	{
		a = m_head;
		m_head = m_head ->m_next;
		a->m_next = NULL;
		a->m_prev = NULL;
		a->bindWidget(NULL);
	}
}







