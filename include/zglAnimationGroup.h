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


#ifndef __zgl_widget_animation_group_h__
#define __zgl_widget_animation_group_h__

#include "zglAnimation.h"

/**
 * @brief zglAnimationGroup is a container for multiple zglAnimation.
 * Using zglAnimationGroup, more zglAnimations can apply to one widget
 * at the same time.
 */
class zglAnimationGroup: public zglAnimation
{
public:
	zglAnimationGroup();

	/**
	 * @brief Add a new animation to the group.
	 */
	void addAnimation(zglAnimation * anim);

	/**
	 * @brief remove animation.
	 */
	void removeAnimation(zglAnimation * anim);

	/**
	 * @brief remove all the animations.
	 */
	void removeAllAnimation();

	/**
	 * @brief Simply reset the animation state to PAUSE.
	 */
	virtual void reset();

	/**
	 * @brief Start the animation.
	 */
	virtual void start(long time, long delay = 0);

	/**
	 * @brief Stop the animation.
	 */
	virtual void stop();

	virtual bool isRunning();

	virtual bool isStopped();

	virtual void bindWidget(zglWidget * widget);

protected:
	virtual bool update(long time);

private:
	zglAnimation * m_head;
	zglAnimation * m_tail;
};


#endif

