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


#include <math.h>	//!< cos() & sin() : TODO Fixed-point algorithm
#include "zglMatrix.h"
#include "zglVector.h"


zglVector::zglVector()
{
	x = y = z = 0.0f;
}

zglVector::zglVector(float v)
{
	x = y = z = v;
}

zglVector::zglVector(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

void zglVector::operator()(float x, float y, float z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}

inline zglVector& zglVector::operator=(float right)
{
	x = y = z = right;
	return *this;
}

zglVector zglVector::operator+(const zglVector& right) const
{
	return zglVector(x + right.x, y + right.y, z + right.z);
}

zglVector zglVector::operator-() const
{
	return zglVector(-x, -y, -z);
}

zglVector zglVector::operator-(const zglVector& right) const
{
	return zglVector(x - right.x, y - right.y, z - right.z);
}

zglVector zglVector::operator*(float right) const
{
	return zglVector(x * right, y * right, z * right);
}

zglVector zglVector::operator/(float right) const
{
	float inv = 1.0f / right;
	return zglVector(x * inv, y * inv, z * inv);
}

void zglVector::operator+=(const zglVector& right)
{
	this->x += right.x;
	this->y += right.y;
	this->z += right.z;
}

void zglVector::operator-=(const zglVector& right)
{
	x -= right.x;
	y -= right.y;
	z -= right.z;
}

void zglVector::operator*=(float right)
{
	x *= right;
	y *= right;
	z *= right;
}

void zglVector::operator/=(float right)
{
	float inv = 1.0f / right;

	x *= inv;
	y *= inv;
	z *= inv;
}

float zglVector::length() const
{
	return sqrt(x * x + y * y + z * z);
}

float zglVector::SqLength() const
{
	return (x * x + y * y + z * z);
}

inline float zglVector::dist(const zglVector& v) const
{
	float diff_x = x - v.x;
	float diff_y = y - v.y;
	float diff_z = z - v.z;

	return sqrt(diff_x * diff_x + diff_y * diff_y + diff_z * diff_z);
}

float zglVector::SqDist(const zglVector& v) const
{
	float diff_x = x - v.x;
	float diff_y = y - v.y;
	float diff_z = z - v.z;

	return (diff_x * diff_x + diff_y * diff_y + diff_z * diff_z);
}

float zglVector::dot(const zglVector& v) const
{
	return x * v.x + y * v.y + z * v.z;
}

zglVector zglVector::Cross(const zglVector& v) const
{
	return zglVector(y * v.z - z * v.y, z * v.x - x * v.z, x * v.y - y * v.x);
}

void zglVector::normalize()
{
	if (x == 0.0f && y == 0.0f && z == 0.0f)
	{
		(*this)(1.0f, 0.0f, 0.0f);
	}
	else
	{
		*this /= length();
	}
}

void zglVector::rotateX(float ang)
{
	float tmp_y = y;
	float tmp_z = z;
	float cos_a = cos(ang);
	float sin_a = sin(ang);

	y = tmp_y * cos_a - tmp_z * sin_a;
	z = tmp_z * cos_a + tmp_y * sin_a;
}

void zglVector::rotateY(float ang)
{
	float tmp_x = x;
	float tmp_z = z;
	float cos_a = cos(ang);
	float sin_a = sin(ang);

	x = tmp_x * cos_a + tmp_z * sin_a;
	z = tmp_z * cos_a - tmp_x * sin_a;
}

void zglVector::rotateZ(float ang)
{
	float tmp_x = x;
	float tmp_y = y;
	float cos_a = cos(ang);
	float sin_a = sin(ang);

	x = tmp_x * cos_a - tmp_y * sin_a;
	y = tmp_y * cos_a + tmp_x * sin_a;
}

///////////////////////////////////////////////////

//TODO  The algrithm
void zglVector::relativePos(const zglMatrix& base)
{
	// TODO
	zglMatrix mat = base;

	mat.noScaleInvert();

	*this = mat * *this;
}

void zglVector::absolutePos(const zglMatrix& base)
{
	*this = base * *this;
}

void zglVector::relativeDir(const zglMatrix& base)
{
	zglMatrix mat = base;
	mat.noTransInvert();
	*this = mat * *this;
}

void zglVector::absoluteDir(const zglMatrix& base)
{
	zglMatrix mat = base;
	mat.T() = 0.0f;
	*this = mat * *this;
}

float zglVector::RelZ(const zglMatrix& base) const
{
	return (*this - base.T()).dot(base.Z());
}

