
#include "zglStatic.h"

zglStatic::zglStatic()
{
	m_flag |= FLAG_DISABLE; //!< Disable the Static

	m_map_flag = FLAG_MAPPING_STATIC;
}

void zglStatic::init(zglTexture * tex, unsigned int width, unsigned int height)
{
	zglWidget2D::init();

	m_prim.init(DRAW_TRI_STRIP, m_vertex, 4, (zglTexture *) tex);
	m_prim.setBlendType(BLEND_HALF);

	//m_prim.setSortOffset(0x70); //!< Move to background

	//!< Update the dimension, and request updating the vertex.
	m_width = width;
	m_height = height;
}

void zglStatic::setTexCoord(float left, float top, float right, float bottom)
{
	m_vertex[0].setUV(left, top);
	m_vertex[1].setUV(left, bottom);
	m_vertex[2].setUV(right, top);
	m_vertex[3].setUV(right, bottom);

	applyVertexUpdate();
}

bool zglStatic::onTouchEvent(zglTouchEvent *event)
{
	return false;
}

bool zglStatic::hitTest(float x, float y)
{
	return false;
}

void zglStatic::updateDimension()
{
	float width2 = m_width * 0.5f;
	float height2 = m_height * 0.5f;

	m_vertex[0].setPos(-width2, height2, 0.0f);
	m_vertex[1].setPos(-width2, -height2, 0.0f);
	m_vertex[2].setPos(width2, height2, 0.0f);
	m_vertex[3].setPos(width2, -height2, 0.0f);;
}

