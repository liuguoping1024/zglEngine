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

#ifndef __zgl_image_h__
#define __zgl_image_h__

#include "zglBasicType.h"

//!< TODO
//!< - 1. abstract this class, and expand to other format
//!< - 2. Support RGB16(RGB565,RGB5551,etc)

/**
 *  @brief Image processing operation.
 *
 * 1. Fixed the dimension to 2^n.
 * 2. Typical image process.
 */
class zglImage
{
public:
	zglImage();

	virtual ~zglImage();

	/**
	 * @brief Load external bitmap pixels to 32-bit bitmap. (For Android NDK Bitmap)
	 * (便于图像处理)
	 *
	 * The external pixels can be abandoned after the function return.
	 *
	 * @param width the width of external resource.
	 * @param height the height of external resource.
	 * @param format Pixel format.
	 * @param stride the stride of external resource.
	 * @param pixel the pixel data of external resource.
	 * @param shrink if the external resource is too large and the flag set
	 * 			to true, shrink the large resource to a smaller one.
	 *
	 * @return true if loading success.
	 */
	bool loadPixel(int width, int height, int format, int stride,
			const char * pixel, bool shrink = false);

	/**
	 *  @brief Load external bitmap pixels. (For Android NDK Bitmap) and
	 *  keep the original format.(便于生成纹理，并且节省内存开销)
	 */
	bool autoLoadPixel(int width, int height, int format, int stride,
			const char * pixel);

	/**
	 * @brief Create a image by the templates, and bind the pixel buffer
	 * with current new image.
	 */
	void bindBuffer(zglImage * templates, const char * pixel);

	/**
	 *  @brief Get the texture coordinate.
	 */
	void getTexCoord(float *u, float* v);

	/**
	 * @brief Get the texture coordinate.
	 * @deprecated
	 */
	inline float getTexCoordWidth()
	{
		return m_width_normalized;
	}

	/**
	 * @brief Get the texture coordinate.
	 */
	inline float getU()
	{
		return m_width_normalized;
	}

	/**
	 * @brief Get the texture coordinate.
	 * @deprecated
	 */
	inline float getTexCoordHeight()
	{
		return m_height_normalized;
	}

	/**
	 * @brief Get the texture coordinate.
	 */
	inline float getV()
	{
		return m_height_normalized;
	}

	/**
	 * @brief get the width of the image.
	 * @return the width of the image.
	 */
	inline int getWidth() const
	{
		return m_width;
	}

	/**
	 * @brief get the height of the image.
	 * @return the height of the image.
	 */
	inline int getHeight() const
	{
		return m_height;
	}

	/**
	 * @brief get the stride of the image.
	 * @return the stride of the image
	 */
	inline int getStride() const
	{
		return m_stride;
	}

	/**
	 * @brief get the pixel format of the image.
	 *
	 * @return PIXEL_RGBA8888
	 */
	inline zglPixelFormat getFormat() const
	{
		return m_format;
	}

	/**
	 * @brief get the pixel data of the image.
	 * @return the pixel buffer of the image.
	 */
	inline const char* getImageData() const
	{
		return (const char*) m_pData;
	}

	/**
	 * @brief get the original width of the image.
	 * @return the width of the image.
	 */
	inline int getOriginWidth() const
	{
		return m_width_origin;
	}

	/**
	 * @brief get the original height of the image.
	 * @return the height of the image.
	 */
	inline int getOriginHeight() const
	{
		return m_height_origin;
	}

	/**
	 * @brief Free the image data and reset the state of the image.
	 */
	void clear();

	/**
	 * @brief reset the pixel color.
	 */
	void reset(int color = 0);

	/**
	 * @brief image processing
	 * @see http://www.codeproject.com/KB/graphics/CBitmapEx.aspx
	 */
	void negative();

	/**
	 * @brief image processing
	 * @see http://www.codeproject.com/KB/graphics/CBitmapEx.aspx
	 */
	void grayscale();

	/**
	 * @brief image processing
	 * @see http://www.codeproject.com/KB/graphics/CBitmapEx.aspx
	 */
	void sepia(int depth = 34);

	/**
	 * @brief image processing
	 * @see http://www.codeproject.com/KB/graphics/CBitmapEx.aspx
	 */
	void emboss();

	/**
	 * @brief image processing
	 * @see http://www.codeproject.com/KB/graphics/CBitmapEx.aspx
	 */
	void engrave();

	/**
	 * @brief image processing
	 * @see http://www.codeproject.com/KB/graphics/CBitmapEx.aspx
	 */
	void pixelize(int size = 4);

	/**
	 * @brief image processing
	 *
	 * @return The pixel buffer of the result image.
	 * @see http://www.codeproject.com/KB/graphics/CBitmapEx.aspx
	 */
	unsigned char* blur();

	/**
	 * @brief image processing
	 *
	 * @return The pixel buffer of the result image.
	 * @see http://www.codeproject.com/KB/graphics/CBitmapEx.aspx
	 */
	unsigned char* gaussianBlur();

	/**
	 * @brief image processing
	 *
	 * @return The pixel buffer of the result image.
	 */
	unsigned char* gaussBlur(int radiaus);

	/**
	 * @brief image processing
	 * @see http://www.codeproject.com/KB/graphics/CBitmapEx.aspx
	 *
	 * @return The pixel buffer of the result image.
	 */
	void sharp();

	/**
	 * @brief image processing
	 * @see http://www.codeproject.com/KB/graphics/CBitmapEx.aspx
	 *
	 * @return The pixel buffer of the result image.
	 */
	void colorize(int color);

private:

	unsigned char* m_pData;

	int m_width; //Total width, the value must be {64, 128, 256, 512, 1024,...}
	int m_height; //Total height, the value must be {64, 128, 256, 512, 1024,...}
	int m_stride;
	zglPixelFormat m_format;

	int m_width_origin; //The effective width, the remaining part of the image is empty
	int m_height_origin; //The effective height, the remaining part of the image is empty

	float m_width_normalized; //!< (0.0f, 1.0f]
	float m_height_normalized; //!< (0.0f, 1.0f]

private:

	bool loadPixel32(int width, int height, int stride, const char * pixel);

	bool loadPixel565(int width, int height, int stride, const char * pixel);

	bool loadPixel4444(int width, int height, int stride, const char * pixel);

	void normalize(int width, int height);

public:
	class Buffer
	{
	private:
		void * m_buffer;
		int m_buffer_length;

	public:
		Buffer();
		virtual ~Buffer();

		void * load();
		void save(void * buffer, int length);
		void clear();

		int getLength();
	};


public:
	class MatrixBuffer
	{
	private:
		unsigned char * m_buffer;
		unsigned char ** m_buffer_header;
		unsigned int m_buffer_width;
		unsigned int m_buffer_height;
		unsigned int m_buffer_unit; //!< Unit size

		unsigned int m_buffer_stride;

		unsigned int m_buffer_length;

	public:
		MatrixBuffer();

		MatrixBuffer(int width, int height, int unitSize);
		virtual ~MatrixBuffer();

		//!< Allocate the buffer
		bool allocate(int width, int height, int unitSize);

		//!< Put the bitmap at the position (x,y) of the MatrixBuffer.

		/**
		 *
		 * ANDROID_BITMAP_FORMAT_NONE      = 0,//not supported
		 * ANDROID_BITMAP_FORMAT_RGBA_8888 = 1,
		 * ANDROID_BITMAP_FORMAT_RGB_565   = 4,
		 * ANDROID_BITMAP_FORMAT_RGBA_4444 = 7,//not supported
		 * ANDROID_BITMAP_FORMAT_A_8       = 8,//not supported
		 *
		 */
		bool saveBitmap(const unsigned char * buffer, unsigned int width,
				unsigned int height, int format, unsigned int stride,
				unsigned int x, unsigned int y);

		bool loadBitmap(unsigned char * buffer, unsigned int width,
				unsigned int height, int format, unsigned int stride,
				unsigned int x, unsigned int y);

		void clear();

		void setValues(int x, int y, unsigned char* value, int length);

		unsigned char* getValues(int x, int y);

		void setIntValue(int x, int y, int value);

		int getIntValue(int x, int y);

		/**
		 * Get the logical width of the buffer.
		 */
		inline unsigned int getWidth() const
		{
			return m_buffer_width;
		}

		/**
		 * Get the logical height of the buffer.
		 */
		inline unsigned int getHeight() const
		{
			return m_buffer_height;
		}

		/**
		 * Get the unit size of the buffer.
		 */
		inline unsigned int getUnitSize() const
		{
			return m_buffer_unit;
		}

		/**
		 * Get the buffer address.
		 */
		inline void * getBuffer() const
		{
			return m_buffer;
		}

		/**
		 * Fill the buffer with the byte value.
		 * @param value The byte value.
		 */
		void reset(int value = 0);

	};

};

#endif
