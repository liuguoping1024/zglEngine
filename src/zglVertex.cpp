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


#include "zglVertex.h"

zglVertex::zglVertex()
{
	this->r = 0xff;
	this->g = 0xff;
	this->b = 0xff;
	this->a = 0xff;
}

void zglVertex::init(float x, float y, float z, float u, float v,
		unsigned int color)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->u = u;
	this->v = v;
	this->r = (unsigned char) ((color >> 24) & 0xff);
	this->g = (unsigned char) ((color >> 16) & 0xff);
	this->b = (unsigned char) ((color >> 8) & 0xff);
	this->a = (unsigned char) ((color >> 0) & 0xff);
}

void zglVertex::init(float x, float y, float z, unsigned int color)
{
	this->x = x;
	this->y = y;
	this->z = z;
	this->r = (unsigned char) ((color >> 24) & 0xff);
	this->g = (unsigned char) ((color >> 16) & 0xff);
	this->b = (unsigned char) ((color >> 8) & 0xff);
	this->a = (unsigned char) ((color >> 0) & 0xff);
}

void zglVertex::setPos(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void zglVertex::setColor(unsigned int color)
{
	r = (unsigned char) ((color >> 24) & 0xff);
	g = (unsigned char) ((color >> 16) & 0xff);
	b = (unsigned char) ((color >> 8) & 0xff);
	a = (unsigned char) ((color >> 0) & 0xff);
}

void zglVertex::setUV(float u, float v)
{
	this->u = u;
	this->v = v;
}

//!< --------------------------------------------------------------------------
zglVertexEx::zglVertexEx()
{
	this->nx = 0.0f;
	this->ny = 0.0f;
	this->nz = -1.0f;
}

void zglVertexEx::setNormal(float nx, float ny, float nz)
{
	this->nx = nx;
	this->ny = ny;
	this->nz = nz;
}





