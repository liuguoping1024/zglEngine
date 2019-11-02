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

#ifndef __zgl_matrix_h_
#define __zgl_matrix_h_

#include "zglVector.h"

/**
 * @brief Matrix basic operation.
 */
class zglMatrix
{
	float m11;//!< matrix member.
	float m12;//!< matrix member.
	float m13;//!< matrix member.
	float m14;//!< matrix member.

	float m21;//!< matrix member.
	float m22;//!< matrix member.
	float m23;//!< matrix member.
	float m24;//!< matrix member.

	float m31;//!< matrix member.
	float m32;//!< matrix member.
	float m33;//!< matrix member.
	float m34;//!< matrix member.

	float m41;//!< matrix member.
	float m42;//!< matrix member.
	float m43;//!< matrix member.
	float m44;//!< matrix member.

	/**
	 * @brief Time stamp of matrix.
	 * The value will changed if the contents update.
	 */
	unsigned short m_timestamp;

public:

	zglMatrix();

	/**
	 * @brief matrix construct
	 *
	 * initial a formal matrix with mxx = v;
	 */
	zglMatrix(float v);

	/**
	 * @brief matrix construct
	 *
	 * initial a matrix with appointed value.
	 */
	zglMatrix(float m11, float m12, float m13, float m14, float m21, float m22,
			float m23, float m24, float m31, float m32, float m33, float m34,
			float m41, float m42, float m43, float m44);

	/**
	 * @brief get the X vector.
	 */
	zglVector& X() const;

	/**
	 * @brief get the Y vector.
	 */
	zglVector& Y() const;

	/**
	 * @brief get the Z vector.
	 */
	zglVector& Z() const;

	/**
	 * @brief get the translate vector.
	 */
	zglVector& T() const;

	/**
	 * @brief Set the value to matrix.
	 */
	void operator()(float m11, float m12, float m13, float m14, float m21,
			float m22, float m23, float m24, float m31, float m32, float m33,
			float m34, float m41, float m42, float m43, float m44);

	zglMatrix& operator=(float scalar); //!< operator

	zglMatrix& operator = (const zglMatrix& m); //!< operator

	zglMatrix operator*(const zglMatrix& right) const; //!< operator

	zglVector operator*(const zglVector& right) const; //!< operator

	void operator*=(const zglMatrix& right); //!< operator

	/**
	 * @brief transpose the matrix.
	 *
	 * exchange the value between Mab and Mba.
	 */
	void transpose();

	/**
	 * @brief rotate the matrix with X axis.
	 *
	 * @param ang radian angle of x rotate.
	 */
	void rotateX(float ang);

	/**
	 * @brief rotate the matrix with Y axis.
	 *
	 * @param ang radian angle of y rotate.
	 */
	void rotateY(float ang);

	/**
	 * @brief rotate the matrix with Z axis.
	 *
	 * @param ang radian angle of z rotate.
	 */
	void rotateZ(float ang);

	/**
	 * @brief rotate the matrix with axis.
	 */
	void rotate(const zglVector& axis, float ang);

	/**
	 * @brief translate the matrix with offset of x, y, and z.
	 */
	void translate(float x, float y, float z);

	/**
	 * @brief Matrix invert
	 */
	void noTransInvert();

	/**
	 * @brief Matrix invert
	 */
	void noScaleInvert();

	/**
	 * @brief Get the time stamp of the matrix.
	 */
	inline unsigned short getTimestamp()
	{
		return m_timestamp;
	}

	friend class zglEngine;
};

#endif

