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
#include "zglTexture.h"

zglTexture::~zglTexture()
{
#if !defined (ANDROID)
	//!< On Android, The GLThread will free the GL resource automatically.
	if (m_tex_id != 0)
	{
		zglCore::deleteTexture( m_tex_id);
	}
#endif
}

void zglTexture::clearTexture()
{
	if (m_tex_id != 0)
	{
		zglCore::deleteTexture( m_tex_id);
		m_tex_id = 0;
	}
}

#if 0
zglTexture& zglTexture::operator=(zglTexture& tex)
{
	this->m_height = tex.m_height;
	this->m_width = tex.m_width;
	this->m_tex_id = tex.m_tex_id;

	return *this;
}
#endif



