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
#include <math.h>
#include <string.h>

#include "zglMatrix.h"
#include "zglVector.h"

#include "zglUtil.h"

zglMatrix::zglMatrix()
{
#if 0
	m11 = 0.0f;  m21 = 0.0f; m31 = 0.0f; m41 = 0.0f;
	m12 = 0.0f;  m22 = 0.0f; m32 = 0.0f; m42 = 0.0f;
	m13 = 0.0f;  m23 = 0.0f; m33 = 0.0f; m43 = 0.0f;
	m14 = 0.0f;  m24 = 0.0f; m34 = 0.0f; m44 = 0.0f;
#endif
}

zglMatrix::zglMatrix(float v)
{
	//!< m44 = 1.0f ?
	m11 = v;     m21 = 0.0f; m31 = 0.0f; m41 = 0.0f;
	m12 = 0.0f;  m22 = v;    m32 = 0.0f; m42 = 0.0f;
	m13 = 0.0f;  m23 = 0.0f; m33 = v;    m43 = 0.0f;
	m14 = 0.0f;  m24 = 0.0f; m34 = 0.0f; m44 = v;
}

zglMatrix::zglMatrix(
		float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44)
{
	this->m11 = m11;	this->m21 = m12;	this->m31 = m13;	this->m41 = m14;
	this->m12 = m21;	this->m22 = m22;	this->m32 = m23;	this->m42 = m24;
	this->m13 = m31;	this->m23 = m32;	this->m33 = m33;	this->m43 = m34;
	this->m14 = m41;	this->m24 = m42;	this->m34 = m43;	this->m44 = m44;
}

zglVector& zglMatrix::X() const
{
	return *((zglVector*) &m11);
}

zglVector& zglMatrix::Y() const
{
	return *((zglVector*) &m21);
}

zglVector& zglMatrix::Z() const
{
	return *((zglVector*) &m31);
}

zglVector& zglMatrix::T() const
{
	return *((zglVector*) &m41);
}

void zglMatrix::operator()(float m11, float m12, float m13, float m14,
		float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34,
		float m41, float m42, float m43, float m44)
{
	this->m11 = m11;    this->m21 = m12;    this->m31 = m13;    this->m41 = m14;
	this->m12 = m21;    this->m22 = m22;    this->m32 = m23;    this->m42 = m24;
	this->m13 = m31;    this->m23 = m32;    this->m33 = m33;    this->m43 = m34;
	this->m14 = m41;    this->m24 = m42;    this->m34 = m43;    this->m44 = m44;

	++ m_timestamp;
}

zglMatrix& zglMatrix::operator=(float v)
{
	m11 = v;     m21 = 0.0f; m31 = 0.0f; m41 = 0.0f;
	m12 = 0.0f;  m22 = v;    m32 = 0.0f; m42 = 0.0f;
	m13 = 0.0f;  m23 = 0.0f; m33 = v;    m43 = 0.0f;
	m14 = 0.0f;  m24 = 0.0f; m34 = 0.0f; m44 = v;

	++ m_timestamp;
	return *this;
}

zglMatrix& zglMatrix::operator = (const zglMatrix& m)
{
	memcpy(&this->m11, &m.m11, sizeof(float)*16);
	this->m_timestamp ++;
	return *this;
}

zglMatrix zglMatrix::operator*(const zglMatrix& right) const
{
	return zglMatrix(
		m11 * right.m11 + m21 * right.m12 + m31 * right.m13 + m41 * right.m14,
		m11 * right.m21 + m21 * right.m22 + m31 * right.m23 + m41 * right.m24,
		m11 * right.m31 + m21 * right.m32 + m31 * right.m33 + m41 * right.m34,
		m11 * right.m41 + m21 * right.m42 + m31 * right.m43 + m41 * right.m44,

		m12 * right.m11 + m22 * right.m12 + m32 * right.m13 + m42 * right.m14,
		m12 * right.m21 + m22 * right.m22 + m32 * right.m23 + m42 * right.m24,
		m12 * right.m31 + m22 * right.m32 + m32 * right.m33 + m42 * right.m34,
		m12 * right.m41 + m22 * right.m42 + m32 * right.m43 + m42 * right.m44,

		m13 * right.m11 + m23 * right.m12 + m33 * right.m13 + m43 * right.m14,
		m13 * right.m21 + m23 * right.m22 + m33 * right.m23 + m43 * right.m24,
		m13 * right.m31 + m23 * right.m32 + m33 * right.m33 + m43 * right.m34,
		m13 * right.m41 + m23 * right.m42 + m33 * right.m43 + m43 * right.m44,

		m14 * right.m11 + m24 * right.m12 + m34 * right.m13 + m44 * right.m14,
		m14 * right.m21 + m24 * right.m22 + m34 * right.m23 + m44 * right.m24,
		m14 * right.m31 + m24 * right.m32 + m34 * right.m33 + m44 * right.m34,
		m14 * right.m41 + m24 * right.m42 + m34 * right.m43 + m44 * right.m44);
}

zglVector zglMatrix::operator*(const zglVector& right) const
{
	return zglVector(
		m11 * right.x + m21 * right.y + m31 * right.z + m41,
		m12 * right.x + m22 * right.y + m32 * right.z + m42,
		m13 * right.x + m23 * right.y + m33 * right.z + m43);
}

void zglMatrix::operator*=(const zglMatrix& right)
{
	zglMatrix mat(
		m11 * right.m11 + m21 * right.m12 + m31 * right.m13 + m41 * right.m14,
		m11 * right.m21 + m21 * right.m22 + m31 * right.m23 + m41 * right.m24,
		m11 * right.m31 + m21 * right.m32 + m31 * right.m33 + m41 * right.m34,
		m11 * right.m41 + m21 * right.m42 + m31 * right.m43 + m41 * right.m44,

		m12 * right.m11 + m22 * right.m12 + m32 * right.m13 + m42 * right.m14,
		m12 * right.m21 + m22 * right.m22 + m32 * right.m23 + m42 * right.m24,
		m12 * right.m31 + m22 * right.m32 + m32 * right.m33 + m42 * right.m34,
		m12 * right.m41 + m22 * right.m42 + m32 * right.m43 + m42 * right.m44,

		m13 * right.m11 + m23 * right.m12 + m33 * right.m13 + m43 * right.m14,
		m13 * right.m21 + m23 * right.m22 + m33 * right.m23 + m43 * right.m24,
		m13 * right.m31 + m23 * right.m32 + m33 * right.m33 + m43 * right.m34,
		m13 * right.m41 + m23 * right.m42 + m33 * right.m43 + m43 * right.m44,

		m14 * right.m11 + m24 * right.m12 + m34 * right.m13 + m44 * right.m14,
		m14 * right.m21 + m24 * right.m22 + m34 * right.m23 + m44 * right.m24,
		m14 * right.m31 + m24 * right.m32 + m34 * right.m33 + m44 * right.m34,
		m14 * right.m41 + m24 * right.m42 + m34 * right.m43 + m44 * right.m44);

	*this = mat;
}

void zglMatrix::transpose()
{
	float t = 0.0f;

	t = m12; m12 = m21; m21 = t;
	t = m13; m13 = m31; m31 = t;
	t = m14; m14 = m41; m41 = t;
	t = m23; m23 = m32; m32 = t;
	t = m24; m24 = m42; m42 = t;
	t = m34; m34 = m43; m43 = t;

	this->m_timestamp ++;
}

void zglMatrix::rotateX(float ang)
{
#if 1
	float cos_a = cos(ang);
	float sin_a = sin(ang);
#else
	float cos_a = _zcos(ang);
	float sin_a = _zsin(ang);
#endif
/*
	zglMatrix mat(
		1.0f,  0.0f,   0.0f, 0.0f,
		0.0f, cos_a, -sin_a, 0.0f,
		0.0f, sin_a,  cos_a, 0.0f,
		0.0f,  0.0f,   0.0f, 1.0f);

	*this *= mat;
*/

	zglMatrix mat(
		m11,
		m21 * cos_a + m31 * sin_a,
		m21 * (-sin_a) + m31 * cos_a,
		m41,

		m12,
		m22 * cos_a + m32 * sin_a,
		m22 * (-sin_a) + m32 * cos_a,
		m42,

		m13,
		m23 * cos_a + m33 * sin_a,
		m23 * (-sin_a) + m33 * cos_a,
		m43,

		m14,
		m24 * cos_a + m34 * sin_a,
		m24 * (-sin_a) + m34 * cos_a,
		m44);

	*this = mat;
}

void zglMatrix::rotateY(float ang)
{
#if 1
	float cos_a = cos(ang);
	float sin_a = sin(ang);
#else
	float cos_a = _zcos(ang);
	float sin_a = _zsin(ang);
#endif
/*
	zglMatrix mat(
		 cos_a, 0.0f, sin_a, 0.0f,
		  0.0f, 1.0f,  0.0f, 0.0f,
		-sin_a, 0.0f, cos_a, 0.0f,
		  0.0f, 0.0f,  0.0f, 1.0f);

	*this *= mat;
*/

	zglMatrix mat(
		m11 * cos_a + m31 * (-sin_a),
		m21,
		m11 * sin_a + m31 * cos_a,
		m41,

		m12 * cos_a + m32 * (-sin_a),
		m22,
		m12 * sin_a + m32 * cos_a,
		m42,

		m13 * cos_a + m33 * (-sin_a),
		m23,
		m13 * sin_a + m33 * cos_a,
		m43,

		m14 * cos_a + m34 * (-sin_a),
		m24,
		m14 * sin_a + m34 * cos_a,
		m44);

	*this = mat;
}

void zglMatrix::rotateZ(float ang)
{
#if 1
	float cos_a = cos(ang);
	float sin_a = sin(ang);
#else
	float cos_a = _zcos(ang);
	float sin_a = _zsin(ang);
#endif
/*
	zglMatrix mat(
		cos_a, -sin_a, 0.0f, 0.0f,
		sin_a,  cos_a, 0.0f, 0.0f,
		 0.0f,   0.0f, 1.0f, 0.0f,
		 0.0f,   0.0f, 0.0f, 1.0f);

	*this *= mat;	
*/

	zglMatrix mat(
		m11 * cos_a + m21 * sin_a,
		m11 * (-sin_a) + m21 * cos_a,
		m31,
		m41,

		m12 * cos_a + m22 * sin_a,
		m12 * (-sin_a) + m22 * cos_a,
		m32,
		m42,

		m13 * cos_a + m23 * sin_a,
		m13 * (-sin_a) + m23 * cos_a,
		m33,
		m43,

		m14 * cos_a + m24 * sin_a,
		m14 * (-sin_a) + m24 * cos_a,
		m34,
		m44);

	*this = mat;;
}

inline void zglMatrix::rotate(const zglVector& axis, float ang)
{
#if 0
	float cos_a = cos(ang);
	float sin_a = sin(ang);
#else
	float cos_a = _zcos(ang);
	float sin_a = _zsin(ang);
#endif

	float icos = 1.0f - cos_a;
	float xx1c = axis.x * axis.x * icos;
	float xy1c = axis.x * axis.y * icos;
	float xz1c = axis.x * axis.z * icos;
	float yy1c = axis.y * axis.y * icos;
	float yz1c = axis.y * axis.z * icos;
	float zz1c = axis.z * axis.z * icos;

	float xs = axis.x * sin_a;
	float ys = axis.y * sin_a;
	float zs = axis.z * sin_a;

	zglMatrix mat(
		xx1c + cos_a,	 xy1c - zs,	   xz1c + ys,  0.0f,
		   xy1c + zs, yy1c + cos_a,	   yz1c - xs,  0.0f,
		   xz1c - ys,	 yz1c + xs, zz1c + cos_a,  0.0f,
				0.0f,		  0.0f,			0.0f,  1.0f);

	*this *= mat;
}

//TODO optimize
void zglMatrix::translate(float x, float y, float z)
{
   /*
	zglMatrix mat(
        1.0f, 0.0f, 0.0f, x,
        0.0f, 1.0f, 0.0f, y,
        0.0f, 0.0f, 1.0f, z,
        0.0f, 0.0f, 0.0f, 1.0f);

    *this *= mat;
    *this */

	zglMatrix mat(
		m11,
		m21,
		m31,
		m11 * x + m21 * y + m31 * z + m41,

		m12,
		m22,
		m32,
		m12 * x + m22 * y + m32 * z + m42,

		m13,
		m23,
		m33,
		m13 * x + m23 * y + m33 * z + m43,

		m14,
		m24,
		m34,
		m14 * x + m24 * y + m34 * z + m44);

	*this = mat;
}

///////////////////////////////////
//TODO
void zglMatrix::noTransInvert()
{
	zglMatrix mat = *this;

	m12 = mat.m21;
	m13 = mat.m31;
	m14 = 0.0f;

	m21 = mat.m12;
	m23 = mat.m32;
	m24 = 0.0f;

	m31 = mat.m13;
	m32 = mat.m23;
	m34 = 0.0f;

	m41 = 0.0f;
	m42 = 0.0f;
	m43 = 0.0f;
	m44 = 1.0f;
}

void zglMatrix::noScaleInvert()
{
	zglMatrix mat = *this;

	m12 = mat.m21;
	m13 = mat.m31;
	m14 = 0.0f;

	m21 = mat.m12;
	m23 = mat.m32;
	m24 = 0.0f;

	m31 = mat.m13;
	m32 = mat.m23;
	m34 = 0.0f;

	m41 = -(mat.X().dot(mat.T()));
	m42 = -(mat.Y().dot(mat.T()));
	m43 = -(mat.Z().dot(mat.T()));
	m44 = 1.0f;
}

