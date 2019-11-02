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

#include "zglPluginEx.h"

bool zglPluginEx::handleTouchEvent(int type, int x, int y, long time)
{
	switch (type)
	{
	case TOUCH_DOWN:
		{
			m_gesture = GESTURE_DOWN;

			m_motion_x = x;
			m_motion_y = y;

			m_last_x = x;
			m_last_y = y;

			m_tracker.clear();

			bool ret = onGestureTouchDown(x, y, time);

			return ret;
		}

	case TOUCH_UP:
		{
			bool ret = false;
			switch (m_gesture)
			{
			default:
			case GESTURE_NONE:
				break;

			case GESTURE_DOWN:
				ret = onGestureClick(x, y, time);
				break;

			case GESTURE_MOVE_X:
				ret = onGestureFlingX(time);
				break;

			case GESTURE_MOVE_Y:
				ret = onGestureFlingY(time);
				break;
			}

			m_gesture = GESTURE_NONE;
			return ret;
		}

	case TOUCH_MOVE:
		{
			bool ret = false;

			m_tracker.addMoveMent((int)x, (int)y, time);

			switch (m_gesture)
			{
			case GESTURE_DOWN:
				{
					int dx = x - m_motion_x;
					dx = dx > 0 ? dx : -dx;

					int dy = y - m_motion_y;
					dy = dy > 0 ? dy : -dy;

					if ((dx >= m_motion_max) || (dy >= m_motion_max))
					{
						if (dx > dy)
						{
							m_gesture = GESTURE_MOVE_X;
						}
						else
						{
							m_gesture = GESTURE_MOVE_Y;
						}
					}
				}

				break;


			case GESTURE_MOVE_X:
				{
					ret = onGestureScrollX(x, time);
				}
				break;

			case GESTURE_MOVE_Y:
				{
					ret = onGestureScrollY(y, time);
				}
				break;

			default:
				break;
			}

			if (!ret)
			{
				ret = onGestureMove(x, y, time);
			}
			m_last_x = x;
			m_last_y = y;

			return ret;
		}
	}

	return false;
}

bool zglPluginEx::onGestureTouchDown(float x, float y, long time)
{
	return false;
}

bool zglPluginEx::onGestureClick(float x, float y, long time)
{
	return false;
}

bool zglPluginEx::onGestureFlingX(long time)
{
	return false;
}

bool zglPluginEx::onGestureFlingY(long time)
{
	return false;
}

bool zglPluginEx::onGestureScrollX(float x, long time)
{
	return false;
}

bool zglPluginEx::onGestureScrollY(float y, long time)
{
	return false;
}

bool zglPluginEx::onGestureMove(float x, float y, long time)
{
	return false;
}

bool zglPluginEx::onGestureLongPress()
{
	return false;
}




