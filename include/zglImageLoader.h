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

#ifndef __zgl_image_loader_h__
#define __zgl_image_loader_h__


/**
 * @brief Image Loader for the X11 & Win32 platform.
 */
class zglImageLoader
{
public:
	zglImageLoader();

	virtual ~zglImageLoader();

	/**
	 * @brief Load a bitmap resource
	 * @return true if loading success.
	 */
	bool loadBMP(const char* filename);

	/**
	 * @brief Load a PNG resource
	 * @return true if loading success.
	 */
	bool loadPNG(const char * filename);

	/**
	 * @brief check the width of the image.
	 *
	 * @return return true if the width and height is 64, 128, 256, 512, ...
	 */
	bool isQualifiedTexture();

	/**
	 * @brief get the pixel format of the image.
	 */
	int getFormat();

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
	 * @brief get color depth of the image.
	 * @return the color depth of the image
	 */
	inline int getColorDepth() const
	{
		return m_colorDepth;
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
	 * @brief get the pixel data of the image.
	 * @return the pixel data of the image.
	 */
	inline const char* getImageData() const
	{
		return (const char*) m_pData;
	}

	/**
	 * @brief Free the image data and reset the state of the image.
	 */
	void clear();

private:
	unsigned char* m_pData;
	int m_width;
	int m_height;
	int m_colorDepth;
	int m_stride;
};

#endif
