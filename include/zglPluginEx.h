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

#ifndef __zgl_plugin_ex_h__
#define __zgl_plugin_ex_h__

#include "zglPlugin.h"

#include "zglUtil.h"

/**
 * @brief A typical plugin with TouchEvent parsing.
 *
 * The TouchEvent will be parsed into several gesture, such as flingX, flingY, etc.
 */
class zglPluginEx: public zglPlugin
{

protected:
	/**
	 * @brief interface of plug-in touch handler, called by #zglEngine.
	 */
	virtual bool handleTouchEvent(int type, int x, int y, long time);

protected:

	/**
	 * @brief Handle the gesture when the touch down the screen the first time.
	 *
	 *
	 * @param x The touch down position.
	 * @param y The touch down position.
	 * @param time The touch down time-line.
	 * @return True if the event is handled.
	 */
	virtual bool onGestureTouchDown(float x, float y, long time);

	/**
	 * @brief Handle the gesture of moving horizontal with the screen.
	 *
	 * @param x The moving position.
	 * @param time The moving time-line.
	 *
	 * @return True if the event is handled.
	 */
	virtual bool onGestureScrollX(float x, long time);

	/**
	 * @brief Handle the gesture of moving vertical with the screen.
	 *
	 * @param y The moving position.
	 * @param time The moving time-line.
	 *
	 * @return True if the event is handled.
	 */
	virtual bool onGestureScrollY(float y, long time);

	/**
	 * @brief Handle the gesture of moving vertical with the screen.
	 *
	 * @param x The moving position.
	 * @param y The moving position.
	 * @param time The moving time-line.
	 *
	 * @return True if the event is handled.
	 */
	virtual bool onGestureMove(float x, float y, long time);

	/**
	 * @brief Handle the gesture when the click the screen.
	 *
	 * The gesture with the finger quickly hitting and leave the screen
	 * without moving.
	 *
	 * @param x The click up position.
	 * @param y The click up position.
	 * @param time The click time-line.
	 * @return True if the event is handled.
	 */
	virtual bool onGestureClick(float x, float y, long time);

	/**
	 * @brief Handle the gesture when the click the screen.
	 *
	 * The gesture with the finger fling horizontal with the screen.
	 *
	 * The speed can be obtained by #m_tracker.
	 *
	 * @param time The fling time-line.
	 * @return True if the event is handled.
	 */
	virtual bool onGestureFlingX(long time);

	/**
	 * @brief Handle the gesture when the click the screen.
	 *
	 * The gesture with the finger fling vertical with the screen.
	 * The speed can be obtained by #m_tracker.
	 *
	 * @param time The fling time-line.
	 * @return True if the event is handled.
	 */
	virtual bool onGestureFlingY(long time);

	/**
	 * @brief Handle the gesture when the long press the screen.
	 *
	 * @return True if the event is handled.
	 */
	virtual bool onGestureLongPress();

protected:

	//!< Fling speed track.
	zglVelocityTrack m_tracker;

	/**
	 * @brief get the velocity of the touch speed.
	 * @param unit the multiple factor of the speed.
	 */
	inline int getVelocityX(int unit) { return m_tracker.getVelocityX(unit); }

	/**
	 * @brief get the velocity of the touch speed.
	 * @param unit the multiple factor of the speed.
	 */
	inline int getVelocityY(int unit) { return m_tracker.getVelocityY(unit); }

	int m_motion_x; //!< The touch down position
	int m_motion_y; //!< The touch down position

	int m_last_x; //!< The last position of touch moving.
	int m_last_y; //!< The last position of touch moving.

	/**
	 * @brief The touch move slop.
	 *
	 * If the finger moves not too far, little than this value, the engine will
	 * not think the finger moving.
	 */
	int m_motion_max;

	int m_gesture; //!< motion gesture.

	static const int GESTURE_NONE = 0; //!< No gesture
	static const int GESTURE_DOWN = 1; //!< gesture of hit the screen first time.
	static const int GESTURE_MOVE_X = 2; //!< gesture of moving the screen horizontal.
	static const int GESTURE_MOVE_Y = 3; //!< gesture of moving the screen vertical.
};



#endif

