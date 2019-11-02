
#include "zglStaticView.h"

zglStaticView::zglStaticView()
{
	m_map_flag = FLAG_MAPPING_STATIC;
}

void zglStaticView::init(zglTexture * tex, unsigned int width, unsigned int height)
{
	zglWidget2D::init();

	m_prim.init(DRAW_TRI_STRIP, m_vertex, 4, (zglTexture *) tex);
	m_prim.setBlendType(BLEND_HALF);

	//!< Update the dimension, and request updating the vertex.
	m_width = width;
	m_height = height;

	applyDimensionUpdate();
}

void zglStaticView::setTexCoord(float left, float top, float right, float bottom)
{
	m_vertex[0].setUV(left, top);
	m_vertex[1].setUV(left, bottom);
	m_vertex[2].setUV(right, top);
	m_vertex[3].setUV(right, bottom);

	applyVertexUpdate();
}

void zglStaticView::setColor(unsigned int color)
{
	unsigned char r = (color & 0xff000000) >> 24;
	unsigned char g = (color & 0x00ff0000) >> 16;
	unsigned char b = (color & 0x0000ff00) >> 8;
	unsigned char a = (color & 0x000000ff);

	m_vertex[0].r = m_vertex[1].r = m_vertex[2].r = m_vertex[3].r = r;
	m_vertex[0].g = m_vertex[1].g = m_vertex[2].g = m_vertex[3].g = g;
	m_vertex[0].b = m_vertex[1].b = m_vertex[2].b = m_vertex[3].b = b;
	m_vertex[0].a = m_vertex[1].a = m_vertex[2].a = m_vertex[3].a = a;
}

void zglStaticView::setAlpha(unsigned char alpha)
{

}

void zglStaticView::updateDimension()
{
	float width2 = m_width * 0.5f;
	float height2 = m_height * 0.5f;

	m_vertex[0].setPos(-width2, height2, 0.0f);
	m_vertex[1].setPos(-width2, -height2, 0.0f);
	m_vertex[2].setPos(width2, height2, 0.0f);
	m_vertex[3].setPos(width2, -height2, 0.0f);;
}

