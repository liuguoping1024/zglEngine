#ifndef __zgl_static_h__
#define __zgl_static_h__


#include "zglWidget2D.h"

/**
 * @brief Static component based on 2D primitives.
 *
 * The widget is designed to display static images, static dialogs.
 *
 * The widget will not response to event dispatching, which is designed to
 * be used as logo, dialog background, etc.
 *
 * @warning the widget can not receive event even clear the flags.
 */
class zglStatic: public zglWidget2D
{
public:
	zglStatic();

	/**
	 * @brief initial the zglStatic widget.
	 *
	 * @param tex texture of the Slider.
	 * @param width the initial width of the widget.
	 * @param height the initial height of the widget.
	 */
	void init(zglTexture * tex, unsigned int width, unsigned int height);

	/**
	 * @brief Set the texture coordinate to widget.
	 *
	 * The texture coordinate will be applied to all the particles.
	 * @param left the left coordinate.
	 * @param top the top coordinate.
	 * @param right the right coordinate.
	 * @param bottom the bottom coordinate.
	 */
	void setTexCoord(float left, float top, float right, float bottom);

protected:
	/**
	 * @brief update the vertex when the dimension changed.
	 */
	virtual void updateDimension();

	virtual bool hitTest(float x, float y);

    /**
     * @brief Handle the touch events to the widget.
     *
     * @param event The touch event.
     * @return True if the event was handled, false otherwise.
     */
	virtual bool onTouchEvent(zglTouchEvent *event);

protected:
	zglVertex m_vertex[4]; //!< vertex for the widget.
};

#endif

