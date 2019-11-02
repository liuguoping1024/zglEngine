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

#ifndef __zgl_bitmap_textview_h_
#define __zgl_bitmap_textview_h_

#include "zglWidget2D.h"

/**
 * @brief Bitmap font configuration information.
 */
class zglFontInfo
{
public:
	zglFontInfo();
    unsigned short m_first;//!< First character of the Bitmap font
    unsigned short m_last;//!< last character of the Bitmap font

    unsigned short m_size; //!< Font size (height)
    static const int m_charCount = 96;//!< Character count of the Bitmap font.

    unsigned short m_x[m_charCount]; //!< X offset of the characters
    unsigned short m_y[m_charCount]; //!< Y offset of the characters
    unsigned short m_w[m_charCount]; //!< width of the characters

public:
    /**
     * @brief Load the bitmap font information from the files.
     * @param file file name.
     */
	bool loadFontInfo(const char* file);

	/**
	 * @brief Load the bitmap font information from buffer.
	 * @param buffer the bitmap font buffer.
	 */
	bool loadFontInfoBuffer(const char* buffer);

	/**
	 * @brief The font information is loaded.
	 */
	bool available();

	/**
	 * @brief get the size(height) of the bitmap font.
	 * @return the size of the bitmap font.
	 */
	int getHeight();
};

/**
 * @brief Text alignment
 */
enum
{
	TEXTVIEW_ALIGN_LEFT = 0, //!< align left
	TEXTVIEW_ALIGN_CENTER = 1, //!< align center
	TEXTVIEW_ALIGN_RIGHT = 2, //!< align right
};

/**
 * @brief Bitmap TextView:
 * - 1. Font of the same width. Only one fixed dimension bitmap file needed.
 * - 2. Font of the different width. One bitmap and one bitmap config file needed.
 */
class zglBitmapTextView: public zglWidget2D
{
	static const int m_default_offset = -32;
public:
	zglBitmapTextView();

	/**
	 * @brief Initialize the variable width font.
	 *
	 * @param texture the texture binded the bitmap of the font.
	 * @param fontInfo the font information binded the bitmap of the font.
	 * @param align alignment of the string.
	 */
	void init(const zglTexture * texture, const zglFontInfo * fontInfo,
			int align = TEXTVIEW_ALIGN_LEFT);

	/**
	 * @brief Set the color to all the vertex of the image view.
	 * @param color the color will set to the all of the vertex of the image view.
	 */
	void setColor(unsigned int color);

	/**
	 * @brief Set alignment of the Bitmap Text.
	 *
	 * @param align alignment.
	 * @see TEXTVIEW_ALIGN_LEFT
	 * @see TEXTVIEW_ALIGN_CENTER
	 * @see TEXTVIEW_ALIGN_RIGHT
	 */
	void setAlign(int align);

	/**
	 * @brief Print string at the appointed position.
	 *
	 * @param x x-position of the text.
	 * @param y y-position of the text.
	 * @param text the contents of the string.
	 */
	void layout(int x, int y, const char * text);

protected:
	virtual void updateDimension();

	virtual bool hitTest(float x, float y);

private:
	void updateDimension(const char * text);

private:
	static const int m_max_font = 256;
	static const float m_stride;

	//max length is 256
	zglVertex m_text_vertex[m_max_font * 4];

	zglFontInfo * m_fontInfo;//!< Bitmap config information
	zglTexture * m_texture;//!< Bitmap binded texture

	int m_used_length;//!<
	int m_text_length;

	int m_aligh;//!< Alignment, @see

	int m_t_width;
	int m_t_height;

	int m_start_x;//!< Start position
	int m_start_y;//!< Start position
};
#endif
