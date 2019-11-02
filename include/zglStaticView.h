#ifndef __zgl_static_view_h__
#define __zgl_static_view_h__


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
class zglStaticView: public zglWidget2D
{
public:
	zglStaticView();

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

	/**
	 * @brief Set the color to the StaticView.
	 */
	virtual void setColor(unsigned int color);

	/**
	 * @brief Set the alpha value of the whole widget.
	 */
	virtual void setAlpha(unsigned char alpha);

protected:

	void updateDimension();

protected:
	zglVertex m_vertex[4]; //!< vertex for the widget.
};

#endif

