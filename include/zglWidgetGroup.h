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

#ifndef __zgl_widget_group_h__
#define __zgl_widget_group_h__

#include "zglWidget.h"
#include "zglWidget3D.h"

#include "zglUtil.h"

/**
 *
 * @brief a container for multiple widget.
 *
 * @note The position of the widget is ........
 *
 *
 * The fastest method to access the first item of the group.
 * @code
 * 	zglWidget * w;
	while(m_listView.getCount() > 0)
	{
		w = m_listView.drop(0);
		m_recycle.insert(w);
	}
 * @endcode
 */
class zglWidgetGroup: public zglWidget
{
public:
	zglWidgetGroup();

	// -------- 8>< -------- background operation -------- ><8 --------

	/**
	 * @brief initial the WidgetGroup and put the WidgetGroup into the render queue.
	 *
	 * The image will not be visible until the function #init() is call.
	 *
	 *
	 * @param texture texture binded to the WidgetGroup.
	 * @param width width of the WidgetGroup
	 * @param height height of the WidgetGroup
	 */
	void init(zglTexture * texture, unsigned int width = 64,
				unsigned int height = 256);

	/**
	 * @brief Set the dimension of the zglWidgetGroup view.
	 *
	 * In fact, the dimension is set to the background of the WidgetGroup.
	 */
	virtual void setDimension(unsigned int width, unsigned int height);

	/**
	 * @brief Set the texture resource to WidgetGroup.
	 *
	 * In fact, the texture is set to the background of the WidgetGroup.
	 */
	virtual void setTexture(zglTexture * texture);

	/**
	 * @brief Set the UV value to the background texture of the view.
	 *
	 * @param top The uv value of the top boundary.
	 * @param left The uv value of the left boundary.
	 * @param right The uv value of the right boundary.
	 * @param bottom The uv value of the bottom boundary.
	 */
	virtual void setTexCoord(float left, float top,
			float right, float bottom);

	/**
	 * @brief Set the color to the background texture of the view.
	 */
	virtual void setColor(unsigned int color);

	/**
	 * @brief Adjust the offset of the widget to adjust the render order
	 *  of the widget.
	 *
	 */
	virtual void setOffset(int offset);

	/**
	 * @brief Set the motion tolerance.
	 *
	 * If moves further then the tolerance, move action will be confirmed.
	 *
	 * The default tolerance is 8.
	 */
	inline void setMotionTolerance(int tolerance)
	{
		m_motion_max = tolerance;
	}

	/**
	 * @brief Set the visibility of the background.
	 */
	virtual void setVisible(bool visible);

	/**
	 * @brief Reset the Coordination setting of the current Widget, and move the
	 * Widget to the new position with x, y, z.
	 */
	virtual void locate(float x, float y, float z = 0.0f);

	// -------- 8>< -------- Item operation procedure -------- ><8 --------

	/**
	 * @brief Insert a new widget to head position of the group.
	 */
	void insert(zglWidget* w);

	/**
	 * @brief Append a new widget to the group tail.
	 *
	 */
	void append(zglWidget* w);

	/**
	 * @brief Add a new widget into the group at the appointed position.
	 *
	 * @param pos The position of the widget in the group. The old widget
	 * 		at that position will be move to the next position.
	 *
	 * @param node The new widget node will be added to the group.
	 */
	void add(int pos, zglWidget* node);

	/**
	 * @brief Drop a widget at the appointed position from the group.
	 *
	 * @param pos The position of the widget in the group.
	 */
	zglWidget* drop(int pos);

	/**
	 * @brief Drop all widget from the group.
	 */
	void dropAll();

	/**
	 * @brief Get the widget count of the group.
	 */
	unsigned int getCount() { return m_count; }

	/**
	 * @brief Get the widget at appointed position.
	 *
	 * @param index The position of the widget.
	 */
	zglWidget * getChildren(unsigned int index);

	/**
	 * @brief Find the appointed widget.
	 *
	 * @param w The widget will be query.
	 * @return The position of the widget. -1 means the widget is not found.
	 */
	int findChild(zglWidget* w);

	/**
	 * @brief Rewind to the begin node of the group, and the the begin node.
	 */
	inline zglWidget* rewind()
	{
		m_current = &m_children;
		return m_current;
	}

	/**
	 * @brief Return the next node of current node, and make the next node as
	 * the current node.
	 */
	inline zglWidget* next()
	{
		m_current = m_current->m_next_sibling;
		return m_current;
	}

	/**
	 * @brief Return the previous node of current node, and make the previous
	 *  node as the current node.
	 */
	inline zglWidget* prev()
	{
		m_current = m_current->m_prev_sibling;
		return m_current;
	}

protected:

	/**
	 * @brief zglBlanket is a special widget to action as background of the widget group.
	 * and send update/touch event to the widget group.
	 */
	class zglBlanket: public zglWidget3D
	{
	public:
		zglBlanket();

		/**
		 * @brief Initialize the Blanket widget.
		 */
		void init(zglTexture * texture, unsigned int width = 128,
				unsigned int height = 128);

		/**
		 * @brief Set the texture for the blanket view.
		 *
		 * Only one texture is supported.
		 * @param texture the texture will be bind to blanket view.
		 */
		void setTexture(zglTexture * texture);

		/**
		 * @brief Set the dimension of the blanket view;
		 */
		void setDimension(unsigned int width, unsigned int height);

		/**
		 * @brief Set the UV value to the vertex of the blanket view.
		 *
		 * @param top the uv value of the top boundary*
		 * @param left the uv value of the left boundary
		 * @param right the uv value of the right boundary
		 * @param bottom the uv value of the bottom boundary
		 */
		void setTexCoord(float left, float top, float right, float bottom);

		/**
		 * @brief Set the color to all the vertex of the blanket view.
		 * @param color the color will set to the all of the vertex of the blanket view.
		 */
		void setColor(unsigned int color);

	protected:

		/**
		 * @brief Update handler of the widget when dimension changed.
		 */
		virtual void updateDimension();

		/**
		 * @brief Interface to check the widget is hit or not.
		 */
		virtual bool hitTest(float x, float y);

		/**
		 * @brief Update event handler of the widget.
		 *
		 * @param time The time line of the update event.
		 * @return True if the event was handled, false otherwise.
		 */
		virtual bool onUpdate(long time);


	private:
		void updateVisible();

	private:
		zglVertex m_vertex[4]; //!< vertex of the blanket

		zglTexture * m_my_texture;

		unsigned int m_my_color;
		unsigned int m_width;
		unsigned int m_height;
	};

protected:
    /**
     * @brief Handle the touch events to the widget.
     *
     * @param event The touch event.
     * @return True if the event was handled, false otherwise.
     */
	virtual bool onTouchEvent(zglTouchEvent *event);

protected:

	/**
	 * @brief Velocity tracker, to calculate the fling speed.
	 */
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

	int m_motion_x; //!< Touch click down position.
	int m_motion_y; //!< Touch click down position.

	int m_last_x; //!< Last moving position.
	int m_last_y; //!< Last moving position.

	int m_motion_max; //!< The max offset of moving slop.

	long m_motion_timeline; //!< The touch down time-line for long press.
	long m_motion_hold_dur; //!< The touch down during time for long press.

	int m_gesture; //!< Gusture

	static const int GESTURE_NONE = 0; //!< No gesture
	static const int GESTURE_DOWN = 1; //!< gesture of hit the screen first time.
	static const int GESTURE_MOVE_X = 2; //!< gesture of moving the screen horizontal.
	static const int GESTURE_MOVE_Y = 3; //!< gesture of moving the screen vertical.
	static const int GESTURE_HOLD = 4;//!< gesture of Long pressing.

protected :
	/**
	 * @brief Handle the gesture when the touch down the screen the first time.
	 *
	 * @param event The touchEvent.
	 *
	 * @return True if the event is handled.
	 */
	virtual bool onGestureTouchDown(zglTouchEvent *event);

	/**
	 * @brief Handle the gesture of moving horizontal with the screen.
	 *
	 * @param event The touchEvent.
	 *
	 * @return True if the event is handled.
	 */
	virtual bool onGestureScrollX(zglTouchEvent *event);

	/**
	 * @brief Handle the gesture of moving vertical with the screen.
	 *
	 * @param event The touchEvent.
	 *
	 * @return True if the event is handled.
	 */
	virtual bool onGestureScrollY(zglTouchEvent *event);

	/**
	 * @brief Handle the gesture of moving vertical with the screen.
	 *
	 * @param event The touchEvent.
	 *
	 * @return True if the event is handled.
	 */
	virtual bool onGestureMove(zglTouchEvent *event);

	/**
	 * @brief Handle the gesture when the click the screen.
	 *
	 * The gesture with the finger quickly hitting and leave the screen
	 * without moving.
	 *
	 * @param event The touchEvent.
	 *
	 * @return True if the event is handled.
	 */
	virtual bool onGestureClick(zglTouchEvent *event);

	/**
	 * @brief Handle the gesture when the click the screen.
	 *
	 * The gesture with the finger fling horizontal with the screen.
	 *
	 * The speed can be obtained by #m_tracker.
	 *
	 * @param event The touchEvent.
	 *
	 * @return True if the event is handled.
	 */
	virtual bool onGestureFlingX(zglTouchEvent *event);

	/**
	 * @brief Handle the gesture when the click the screen.
	 *
	 * The gesture with the finger fling vertical with the screen.
	 * The speed can be obtained by #m_tracker.
	 *
	 * @param event The touchEvent.
	 *
	 * @return True if the event is handled.
	 */
	virtual bool onGestureFlingY(zglTouchEvent *event);

	/**
	 * @brief Handle the gesture when the long press the screen.
	 *
	 * @return True if the event is handled.
	 */
	virtual bool onGestureLongPress();

private:

	/**
	 * @brief Find the widget at the appointed position.
	 *
	 *
	 * @param pos The postion of the widget. The value must be valid [0, m_count).
	 */
	zglWidget * findWidgetByPosition(int pos);

protected:
	zglWidget m_children; //!< The pointer to all the widget children.

	zglWidget * m_current; //!< Current node for #rewind(), #next(), #prev().

	unsigned short m_count; //!< Children count.

	/**
	 * @brief The background of the widget group.
	 *
	 * The Engine will not dispatch any event to zglWidgetGroup, and will not
	 * update the zglWidgetGroup.
	 *
	 * The zglBlanket also will action as Event hooker to pass the event to
	 * zglWidgetGroup.
	 */
	zglBlanket m_background;

	/**
	 * @brief The logical width of the zglWidgetGroup.
	 *
	 * The zglWidgetGroup is a logical container, not a real widget, so the
	 * dimension of the zglWidgetGroup is a fake dimension.
	 */
	unsigned int m_width;

	/**
	 * @brief The logical height of the zglWidgetGroup.
	 *
	 * The zglWidgetGroup is a logical container, not a real widget, so the
	 * dimension of the zglWidgetGroup is a fake dimension.
	 */
	unsigned int m_height;
};

#endif

