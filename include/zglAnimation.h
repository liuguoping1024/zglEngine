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

#ifndef __zgl_widget_animation_h__
#define __zgl_widget_animation_h__

#include "zglInterpolator.h"

class zglWidget;


#define ANIM_TYPE_ABSOLUTE 0x0001
#define ANIM_TYPE_RELATIVE 0x0002

#define ANIM_STATE_USER  0x8000

/**
 * @brief zglAnimation is an animation action unit, which will be bind to
 * one widget, and performs animation when the widget is updating.
 *
 * One Widget can bind multiple zglAnimation, but one zglAnimation
 * only can bind with one widget.
 *
 * zglAnimation is both a time-line based animate, and a frame base animate.
 *
 * If a time-line based animation is designed, the animation will count
 * the elapsed time to decide
 *
 *
 *
 * @see zglInterpolator
 * @see zglEaseInterpolator
 */
class zglAnimation
{
public:
	zglAnimation();
	virtual ~zglAnimation();

	/**
	 * Start the animation.
	 *
	 * @param time the animation start time-line. If the animation
	 * 		is frame based, the time-line will be skipped.
	 * @param delay the animation delay time. If the animation is frame
	 * 		based, the value will be skipped.
	 */
	virtual void start(long time, long delay = 0);

	/**
	 * @brief Stop the animation.
	 *
	 * Stop the Animation, and restore the state to initial state(NONE).
	 */
	virtual void stop();

	/**
	 * Simply reset the animation state to PAUSE.
	 *
	 * @note the frame information and the time-line information
	 * 		will not be updated.
	 */
	virtual void reset();

	/**
	 * Check the Animation' state.
	 * @return if the animation is running(including delaying phase),
	 * 		return true.
	 */
	virtual bool isRunning();

	/**
	 * Check the Animation's state.
	 */
	virtual bool isStopped();

	/**
	 * The during time of the Animation.
	 * Default value is 1000 ms. And the delay time is not included.
	 *
	 * @param time the during time.
	 */
	void setDurTime(long time);

	/**
	 * Set the interpolator for the animation.
	 *
	 * Preset interpolator is list below:
	 *
	 * - LinearInterpolator
	 * - AccelerateInterpolator
	 * - DecelerateInterpolator
	 * - AccelerateDecelerateInterpolator
	 * - AnticipateInterpolator
	 * - AnticipateOvershootInterpolator
	 * - BounceInterpolator
	 * - CycleInterpolator
	 * - OvershootInterpolator
	 *
	 * Default interpolator is LinearInterpolator.
	 *
	 * @see #zglInterpolator
	 * @see #zglEaseInterpolator
	 */
	void setInterpolator(zglInterpolator * interpolator);

protected:

	/**
	 * Get the interpolate value.
	 *
	 * @param t input value [0.0 ~ 1.0]
	 * @return the interpolate value. If no interpolator appointed,
	 * 		A default linear interpolated value will be return.
	 */
	float getValue(float t);

	/**
	 * @brief Update the Animation.
	 *
	 * In this procedure, the Animation will try to update the widget by
	 * the parameter interpolated by the time-line.
	 *
	 * @param time current time-line.
	 * @return true if the animation update is success, else return false.
	 */
	virtual bool update(long time);

	/**
	 * Binding the widget.
	 * One Animation can only bind to one Widget.
	 */
	virtual void bindWidget(zglWidget * widget);

protected:

	static const int ANIM_STATE_NONE = 0; //!< initial state
	static const int ANIM_STATE_RUNNING = 1; //!< running state

	/**
	 * @brief Animation state
	 *
	 * @see ANIM_STATE_NONE
	 * @see ANIM_STATE_RUNNING
	 *
	 *@dot
	 * digraph G {
		 node [style=rounded shape=box fontsize="8" height=0.4];

		 "RUNNING" -> "NONE" [label="stop()" fontsize="8"];

		 "RUNNING" -> "NONE" [label="reset()" fontsize="8"];

		 "NONE" -> "RUNNING" [label="start()" fontsize="8"];
	 }
	 @enddot
	 */
	int m_state;

	unsigned int m_tick;//!< Frame based elapsed frame count.
	unsigned int m_tick_max; //!< Frame based total frame count.

	long m_start_time; //!< The start time-line of the animation.
	long m_last_time; //!< The last frame's time-line when needed.
	long m_dur_time; //!< The total duration time of the animation
	long m_delay_time; //!< The Delay time.

	// Link-list animation. Used by AnimationGroup.

	zglAnimation * m_next; //!< Next animation
	zglAnimation * m_prev; //!< Previous animation

	zglWidget * m_widget;//!< The target of the animation

	zglInterpolator* m_interpolator; //!< interpolator(插值器)

	friend class zglAnimationGroup; //!< m_next & m_prev access
	friend class zglWidget;
};


#endif
