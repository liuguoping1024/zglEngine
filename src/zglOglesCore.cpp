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

#include "zglCore.h"
#include "zglBasicType.h"

#if defined(__ARMEL__) || defined (ANDROID)

#include <GLES/gl.h>

#include "zgl_jni.h"

void zglCore::reset()
{
	glShadeModel(GL_SMOOTH);

	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColorMask(true, true, true, true);
	glClearDepthf(1.0f);

	glDepthFunc(GL_LEQUAL);
	glEnable(GL_DEPTH_TEST);
	glDepthMask(true);

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_SCISSOR_TEST);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glEnable(GL_MULTISAMPLE);

	/*glPointSize(3);
	glLineWidth(3);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
	glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
	glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);*/

	glEnableClientState(GL_VERTEX_ARRAY);
}

void zglCore::flush()
{
	glFlush();
}

void zglCore::setViewport(int x, int y, int width, int height)
{
	glViewport(x, y, width, height);
	glScissor(x, y, width, height);
}

void zglCore::clearBuffer(const unsigned char* clear_color, bool is_clear_color,
		bool is_clear_depth)
{
	if (!is_clear_color && !is_clear_depth)
	{
		return;
	}

	glClearColor(
			(float)clear_color[0] / 255.0f,
			(float)clear_color[1] / 255.0f,
			(float)clear_color[2] / 255.0f,
			(float)clear_color[3] / 255.0f);

	glDepthMask(true);

	glEnable(GL_SCISSOR_TEST);
	glClear((is_clear_color ? GL_COLOR_BUFFER_BIT : 0) | (is_clear_depth ? GL_DEPTH_BUFFER_BIT : 0));
	glDisable(GL_SCISSOR_TEST);
}

void zglCore::setBlendType(zglBlendType blend_type)
{
	switch (blend_type)
	{
		case BLEND_OFF:
		glDisable(GL_ALPHA_TEST);
		glDisable(GL_BLEND);
		break;

		case BLEND_HALF:
		glDisable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA , GL_ONE_MINUS_SRC_ALPHA);
		break;

		case BLEND_ADD:
		glDisable(GL_ALPHA_TEST);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA , GL_ONE);
		break;

		default:
		{
			static GLenum SRC_FACTOR[9] =
			{ GL_ZERO, GL_ONE, GL_DST_COLOR, GL_ONE_MINUS_DST_COLOR,
					GL_SRC_ALPHA_SATURATE, GL_SRC_ALPHA,
					GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
					GL_ONE_MINUS_DST_ALPHA };

			static GLenum DST_FACTOR[8] =
			{ GL_ZERO, GL_ONE, GL_SRC_COLOR, GL_ONE_MINUS_SRC_COLOR,
					GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_DST_ALPHA,
					GL_ONE_MINUS_DST_ALPHA };
			int type = (int)blend_type;
			type -= 3;
			if(type >= 72) type = type%72;
		    int i = type / 8;
		    int j = type % 8;

			glDisable(GL_ALPHA_TEST);
			glEnable(GL_BLEND);
			glBlendFunc(SRC_FACTOR[i], DST_FACTOR[j]);
		}
	}
}

void zglCore::setCullFace(bool enable)
{
	if(enable)
	{
		glEnable(GL_CULL_FACE);
	}
	else
	{
		glDisable(GL_CULL_FACE);
	}
}

void zglCore::setDepthTest(zglDepthType type, bool mask)
{
	switch(type)
	{
		case DEPTH_NEVER: glDepthFunc(GL_NEVER); break;
		default:
		case DEPTH_LESS: glDepthFunc(GL_LESS); break;
		case DEPTH_EQUAL: glDepthFunc(GL_EQUAL); break;
		case DEPTH_LEQUAL: glDepthFunc(GL_LEQUAL); break;
		case DEPTH_GREATER: glDepthFunc(GL_GREATER); break;
		case DEPTH_NOTEQUAL: glDepthFunc(GL_NOTEQUAL); break;
		case DEPTH_GEQUAL: glDepthFunc(GL_GEQUAL); break;
		case DEPTH_ALWAYS: glDepthFunc(GL_ALWAYS); break;
	}

	glDepthMask(mask);
}

void zglCore::resetProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
}

void zglCore::setProjection(const zglMatrix& mat)
{
	glMatrixMode(GL_PROJECTION);
	glLoadMatrixf((float*)&mat);
}

void zglCore::resetModelView()
{
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void zglCore::setModelView(const zglMatrix& mat)
{
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf((float*)&mat);
}

void zglCore::setVertexPointer2(const float* vert_buf, size_t stride)
{
	glVertexPointer(2, GL_FLOAT, (GLsizei)stride, vert_buf);
}

void zglCore::setVertexPointer3(const float* vert_buf, size_t stride)
{
	glVertexPointer(3, GL_FLOAT, (GLsizei)stride, vert_buf);
}


void zglCore::setDefaultNormal()
{
	glDisableClientState(GL_NORMAL_ARRAY);
	glNormal3f(0.0f, 0.0f, -1.0f);
}

void zglCore::setNormal(const float* normal)
{
	glDisableClientState(GL_NORMAL_ARRAY);
	glNormal3f(normal[0], normal[1], normal[2]);
}

void zglCore::setNormalPointer(const float* normal, size_t stride)
{
	if(normal != NULL)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, stride, normal);
	}
	else
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}
}

void zglCore::setColor4(const unsigned char* color)
{
	glDisableClientState(GL_COLOR_ARRAY);
	glColor4f((float)color[0] / 255.0f, (float)color[1] / 255.0f, (float)color[2] / 255.0f, (float)color[3] / 255.0f);
}

void zglCore::setColorPointer4(const unsigned char* color_buf, size_t stride)
{
	glEnableClientState(GL_COLOR_ARRAY);
	glColorPointer(4, GL_UNSIGNED_BYTE, (GLsizei)stride, color_buf);
}

void zglCore::setTexCoordPointer2(const float* uv_buf, size_t stride)
{
	glTexCoordPointer(2, GL_FLOAT, (GLsizei)stride, uv_buf);
}

void zglCore::setLightParam(int index, const float* ambientColor,
		const float* diffuseColor, const float* specularColor, const float* position)
{
	GLint light;
	switch(index)
	{
	case 0: light = GL_LIGHT0; break;
	case 1: light = GL_LIGHT1; break;
	case 2: light = GL_LIGHT2; break;
	case 3: light = GL_LIGHT3; break;
	case 4: light = GL_LIGHT4; break;
	case 5: light = GL_LIGHT5; break;
	case 6: light = GL_LIGHT6; break;
	case 7: light = GL_LIGHT7; break;
	default:
		return;
	}

	if(ambientColor)
	{
		glLightfv(light, GL_AMBIENT, ambientColor);
	}

	if(diffuseColor)
	{
		glLightfv(light, GL_DIFFUSE, diffuseColor);
	}

	if(specularColor)
	{
		glLightfv(light, GL_SPECULAR, specularColor);
	}

	if(position)
	{
		glLightfv(light, GL_POSITION, position);
	}

	glEnable(light);
}

void zglCore::unSetLightParam(int index)
{
	GLint light;
	switch(index)
	{
	case 0: light = GL_LIGHT0; break;
	case 1: light = GL_LIGHT1; break;
	case 2: light = GL_LIGHT2; break;
	case 3: light = GL_LIGHT3; break;
	case 4: light = GL_LIGHT4; break;
	case 5: light = GL_LIGHT5; break;
	case 6: light = GL_LIGHT6; break;
	case 7: light = GL_LIGHT7; break;
	default:
		return;
	}

	glDisable(light);
}

void zglCore::setLight(bool enable)
{
	if(enable)
	{
		glEnable(GL_LIGHTING);
	}
	else
	{
		glDisable(GL_LIGHTING);
	}
}

void zglCore::setFogParam(int mode, const float* color,
		float density, float start, float end)
{
	switch(mode)
	{
		case FOG_LINEAR:
		glFogx(GL_FOG_MODE, GL_LINEAR);
		break;

		case FOG_EXP:
		glFogx(GL_FOG_MODE, GL_EXP);
		break;

		case FOG_EXP2:
		glFogx(GL_FOG_MODE, GL_EXP2);
		break;

		default:
		break;
	}

	if(color != NULL)
	{
		glFogfv(GL_FOG_COLOR, color);
	}

	glFogf(GL_FOG_DENSITY, density);
	glHint(GL_FOG_HINT, GL_DONT_CARE);
	glFogf(GL_FOG_START, start);
	glFogf(GL_FOG_END, end);
	glEnable(GL_FOG);
}

void zglCore::unSetFogParam()
{
	glDisable(GL_FOG);
}

void zglCore::setMaterial(const float* ambient,const float* diffuse,
		const float* specular,const float* emission,
		float exponent)
{
	if(ambient)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient);
	}

	if(diffuse)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	}

	if(specular)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	}

	if(emission)
	{
		glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, emission);
	}

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, exponent);
}

void zglCore::setDefaultMaterial()
{
	float data[4];

	data[0] = 0.2f;
	data[1] = 0.2f;
	data[2] = 0.2f;
	data[3] = 1.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, data);

	data[0] = 0.8f;
	data[1] = 0.8f;
	data[2] = 0.8f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, data);

	data[0] = 0.0f;
	data[1] = 0.0f;
	data[2] = 0.0f;
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, data);
	glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, data);

	glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, 0.0f);
}

void zglCore::drawArrays(zglDrawType draw_type, size_t first, size_t count)
{
	const GLenum gl_draw_mode[] =
	{
		GL_POINTS, // DRAW_POINTS
		GL_LINES, // DRAW_LINES
		GL_LINE_STRIP, // DRAW_LINE_STRIP
		GL_LINE_LOOP, // DRAW_LINE_LOOP
		GL_TRIANGLES, // DRAW_TRIANGLES
		GL_TRIANGLE_STRIP, // DRAW_TRI_STRIP
		GL_TRIANGLE_FAN, // DRAW_TRI_FAN
	};

	if (draw_type == DRAW_QUADS)
	{
		for (first = 0; first < count; first += 4)
		{
			glDrawArrays(GL_TRIANGLE_FAN, (GLint)first, 4);
		}
	}
	else
	{
		glDrawArrays(gl_draw_mode[draw_type], (GLint)first, (GLsizei)count);
	}
}

void zglCore::drawElements(zglDrawType draw_type, size_t count, const unsigned short* index_array)
{
	const GLenum gl_draw_mode[] =
	{
		GL_POINTS, // DRAW_POINTS
		GL_LINES, // DRAW_LINES
		GL_LINE_STRIP, // DRAW_LINE_STRIP
		GL_LINE_LOOP, // DRAW_LINE_LOOP
		GL_TRIANGLES, // DRAW_TRIANGLES
		GL_TRIANGLE_STRIP, // DRAW_TRI_STRIP
		GL_TRIANGLE_FAN, // DRAW_TRI_FAN
	};

	if (draw_type == DRAW_QUADS)
	{
		size_t offset;

		for (offset = 0; offset < count; offset += 4)
		{
			glDrawElements(GL_TRIANGLE_FAN, 4, GL_UNSIGNED_SHORT, index_array + offset);
		}
	}
	else
	{
		glDrawElements(gl_draw_mode[draw_type], (GLsizei)count, GL_UNSIGNED_SHORT, index_array);
	}
}

unsigned int zglCore::genTexture(unsigned short width, unsigned short height,
		zglPixelFormat pixel_format, const char* image)
{
	unsigned int tex_obj;
	GLenum errno;
	glEnable(GL_TEXTURE_2D);

	//ANDROID_BITMAP_FORMAT_RGBA_8888:
	//ANDROID_BITMAP_FORMAT_RGB_565:
	//ANDROID_BITMAP_FORMAT_RGBA_4444:
	//ANDROID_BITMAP_FORMAT_A_8:

	glGenTextures(1, (GLuint*)&tex_obj);
	glBindTexture(GL_TEXTURE_2D, tex_obj);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);

	//glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//GLenum dataFormat;
	switch(pixel_format)
	{
		default: return 0;

		case PIXEL_A8:
		{
			//dataFormat = GL_ALPHA;
			glTexImage2D(GL_TEXTURE_2D,
					0,
					GL_ALPHA,
					width,
					height,
					0,
					GL_ALPHA,
					GL_UNSIGNED_BYTE,
					image);
		}
		break;

		//TODO: only RGB or RGBA support nowadays.(24bit, and 32 bits)
		case PIXEL_RGB888:
		{
			//dataFormat = GL_RGB;
			glTexImage2D(GL_TEXTURE_2D,
					0,
					GL_RGB,
					width,
					height,
					0,
					GL_RGB,
					GL_UNSIGNED_BYTE,
					image);
		}
		break;

		case PIXEL_RGBA8888:
		{
			//dataFormat = GL_RGBA;
			glTexImage2D(GL_TEXTURE_2D,
					0,
					GL_RGBA,
					width,
					height,
					0,
					GL_RGBA,
					GL_UNSIGNED_BYTE,
					image);
		}
		break;

		case PIXEL_RGB565:
		{
			glTexImage2D(GL_TEXTURE_2D,
					0,
					GL_RGB,
					width,
					height,
					0,
					GL_RGB,
					GL_UNSIGNED_SHORT_5_6_5,
					image);
		}
		break;
	}

	/*glTexImage2D(GL_TEXTURE_2D,
			0,
			dataFormat,
			width,
			height,
			0,
			dataFormat,
			GL_UNSIGNED_BYTE,
			image);
*/

	errno = glGetError();
	if(errno != GL_NO_ERROR)
	{
		switch(errno){
		case GL_OUT_OF_MEMORY:
			LOGE("glTexImage2D: GL_OUT_OF_MEMORY");
			return 0;

		case GL_INVALID_ENUM:
			LOGE("glTexImage2D: GL_INVALID_ENUM");
			return 0;

		case GL_INVALID_VALUE:
			LOGE("glTexImage2D: GL_INVALID_VALUE");
			return 0;

		case GL_INVALID_OPERATION:
			LOGE("glTexImage2D: GL_INVALID_OPERATION");
			return 0;

		default:
			LOGE("glTexImage2D: error = %d",errno);
			return 0;
		}
	}


#if 0
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 8, 8, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterx(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexEnvx(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
#endif
	return tex_obj;
}

void zglCore::deleteTexture(unsigned int tex_obj)
{
	glDeleteTextures(1, (const GLuint*)&tex_obj);
}

void zglCore::setTexture(unsigned int tex_obj, bool is_linear)
{
	if (tex_obj == 0)
	{
		glDisable(GL_TEXTURE_2D);
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}
	else
	{
		glEnable(GL_TEXTURE_2D);
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);

		glBindTexture(GL_TEXTURE_2D, tex_obj);

		if (is_linear)
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		}
		else
		{
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		}
	}
}

unsigned int zglCore::genBuffers(zglBufferType buffer, const void* data_buffer, size_t count, bool dynamic)
{
	GLuint buffer_obj = 0;
	GLenum target = buffer == INDEX_BUFFER ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
	GLenum usage = dynamic ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW;

	glGenBuffers(1, &buffer_obj);
	glBindBuffer(target, buffer_obj);

	glBufferData(target, count, data_buffer, usage);
	glBindBuffer(target, 0);

	GLenum err = glGetError();
	if(err != GL_NO_ERROR)
	{
		//GL_OUT_OF_MEMORY
		return 0;
	}

	return (unsigned int)buffer_obj;
}

void zglCore::deleteBuffers(unsigned int buffer_obj)
{
	glDeleteBuffers(1, (const GLuint*)&buffer_obj);
}

bool zglCore::updateBuffer(zglBufferType buffer, unsigned int buffer_obj,
			const void* data_buffer, size_t count)
{
	GLenum target = buffer == INDEX_BUFFER ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
	glBindBuffer(target, (GLuint)buffer_obj);
	glBufferSubData(target, 0, count, data_buffer);
	glBindBuffer(target, 0);

	GLenum err = glGetError();
	if(err != GL_NO_ERROR)
	{
		//GL_OUT_OF_MEMORY
		return false;
	}

	return true;
}

void zglCore::setBuffer(zglBufferType buffer, unsigned int buffer_obj)
{
	GLenum target = buffer == INDEX_BUFFER ? GL_ELEMENT_ARRAY_BUFFER : GL_ARRAY_BUFFER;
	glBindBuffer(target, (GLuint)buffer_obj);
}

bool zglCore::isBufferSupported()
{
	return true;
}

#endif


