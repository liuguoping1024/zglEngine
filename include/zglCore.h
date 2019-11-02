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

#ifndef __zgl_core_h__
#define __zgl_core_h__

#include <unistd.h>
#include <stdint.h>

#include "zglMatrix.h"
#include "zglBasicType.h"

/** 
 * @brief An abstract layer of the Graphics layer, such as OpenGL, OpenGL es,
 * may be DirectX in the future.
 */
class zglCore
{

public:
	/**
	 * @brief reset the core parameters.
	 */
	static void reset();

	/**
	 * @brief flush the contents to screen.
	 */
	static void flush();

	/**
	 * @brief set the view port(small screen)
	 *
	 * Only one view port is recommended for the embedded system.
	 */
	static void setViewport(int x, int y, int width, int height);

	/**
	 * @brief clear the background or the buffer.
	 */
	static void clearBuffer(const unsigned char* clear_color, bool is_clear_color,
			bool is_clear_depth);

	/**
	 * @brief set the blend type.
	 *
	 * @see zglBlendType
	 */
	static void setBlendType(zglBlendType blend_type);

	/**
	 * @brief set the polygon winding judge type.
	 *
	 * @see <i>GL_CULL_FACE</i>
	 */
	static void setCullFace(bool enable);

	/**
	 * @brief set the depth test method.
	 *
	 * @see <i>GL_DEPTH_TEST</i>
	 */
	static void setDepthTest(zglDepthType type, bool mask);

	/**
	 * @brief reset project matrix to identity.
	 */
	static void resetProjection();

	/**
	 * @brief set project matrix.
	 */
	static void setProjection(const zglMatrix& mat);

	/**
	 * @brief reset model view matrix to identity.
	 */
	static void resetModelView();

	/**
	 * @brief set model view matrix.
	 */
	static void setModelView(const zglMatrix& mat);

	/**
	 * @brief vertex pointer
	 *
	 * @see glVertexPointer
	 * @see GL_VERTEX_ARRAY
	 */
	static void setVertexPointer2(const float* vert_buf, size_t stride);

	/**
	 * @brief vertex pointer
	 *
	 * @see glVertexPointer
	 * @see GL_VERTEX_ARRAY
	 */
	static void setVertexPointer3(const float* vert_buf, size_t stride);

	/**
	 * @brief disable normal array, and reset to default normal.
	 */
	static void setDefaultNormal();

	/**
	 * @brief set normal.
	 */
	static void setNormal(const float* normal);

	/**
	 * @brief set normal array.
	 */
	static void setNormalPointer(const float* normal, size_t stride);

	/**
	 * @brief set color.
	 */
	static void setColor4(const unsigned char* color);

	/**
	 * @brief set color array.
	 */
	static void setColorPointer4(const unsigned char* color_buf, size_t stride);

	/**
	 * @brief set texture coordinate array.
	 *
	 * @see GL_TEXTURE_COORD_ARRAY
	 */
	static void setTexCoordPointer2(const float* uv_buf, size_t stride);

	/**
	 * @brief Set light parameters.
	 */
	static void setLightParam(int index, const float* ambientColor,
			const float* diffuseColor, const float* specularColor, const float* position);

	/**
	 * @brief disable light parameters.
	 */
	static void unSetLightParam(int index);

	/**
	 * @brief enable/disable light function of the whole system.
	 */
	static void setLight(bool enable);

	/**
	 * @brief Set fog parameters.
	 */
	static void setFogParam(int mode, const float* color,
			float density, float start, float end);

	/**
	 * @brief disable fog parameters.
	 */
	static void unSetFogParam();

	/**
	 * @brief Set current material parameters.
	 */
	static void setMaterial(const float* ambient,const float* diffuse,
			const float* specular,const float* emission, float exponent);

	/**
	 * @brief restore to default parameters.
	 */
	static void setDefaultMaterial();

	/**
	 * @brief draw arrays
	 *
	 * @see <i>glDrawArrays</i>
	 */
	static void drawArrays(zglDrawType draw_type, size_t first, size_t count);

	/**
	 * @brief draw elements
	 *
	 * @see <i>glDrawElements</i>
	 */
	static void drawElements(zglDrawType draw_type, size_t count, const unsigned short* index_array);

	/**
	 * @brief create a new texture
	 *
	 */
	static unsigned int genTexture(unsigned short width, unsigned short height,
			zglPixelFormat pixel_format, const char* image);

	/**
	 * @brief remove a texture
	 */
	static void deleteTexture(unsigned int tex_obj);

	/**
	 * @brief choose texture, and make it current.
	 */
	static void setTexture(unsigned int tex_obj, bool is_linear);

	/**
	 * @brief create a new hardware buffer.
	 */
	static unsigned int genBuffers(zglBufferType buffer,
			const void* data_buffer, size_t count, bool dynamic);

	/**
	 * @brief clear hardware buffer.
	 */
	static void deleteBuffers(unsigned int buffer_obj);

	/**
	 * @brief updating and sync hardware buffer.
	 */
	static bool updateBuffer(zglBufferType buffer, unsigned int buffer_obj,
			const void* data_buffer, size_t count);

	/**
	 * @brief choose hardware buffer, and make it current.
	 */
	static void setBuffer(zglBufferType buffer, unsigned int buffer_obj);

	/**
	 * @brief query the hardware supported capability of the system.
	 */
	static bool isBufferSupported();

	/**
	 * @brief read pixel
	 *
	 * TODO
	 */
	static void readPixel(int x, int y, unsigned int w, unsigned h,
			unsigned char pixel_format, unsigned char pixel_type, unsigned char * pixels);

	/**
	 * @brief pixel parameters
	 * TODO: Extend to more setting
	 * X11: format = GL_RGBA, type = GL_UNSIGNED_BYTE
	 * ZII: format = GL_RGB,  type = GL_UNSIGNED_BYTE
	 */
	static void readPixelParam(unsigned int *pixel_format, unsigned int *pixel_type,
			unsigned int * pixel_size);

	/**
	 * @brief get pixel
	 * TODO
	 */
	static void getPixel(int x, int y, unsigned int w, unsigned int h, unsigned char * pixels);

private:
	zglCore();
	~zglCore();
};

#endif

