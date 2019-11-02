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


#ifndef __zgl_vector_h__
#define __zgl_vector_h__

class zglMatrix;

/**
 * @brief The vector class.
 */
class zglVector
{

public:
	float x; //!< X-coordinate of vector
	float y; //!< Y-coordinate of vector
	float z; //!< Z-coordinate of vector

	zglVector();

	/**
	 * @brief initial a value by one float value.
	 */
	zglVector(float v);

	/**
	 * @brief initial a value by three float value.
	 */
	zglVector(float x, float y, float z);

	/**
	 * @brief set value.(operator)
	 */
	void operator()(float x, float y, float z);

	/**
	 * @brief set value.(operator)
	 */
	zglVector& operator=(float right);

	zglVector operator+(const zglVector& right) const;//!< operator plus
	zglVector operator-() const; //!< operator sub
	zglVector operator-(const zglVector& right) const; //!< operator plus
	zglVector operator*(float right) const;//!< operator multiple
	zglVector operator/(float right) const;//!< operator division

	void operator+=(const zglVector& right);//!< operator plus
	void operator-=(const zglVector& right);//!< operator sub
	void operator*=(float right);//!< operator multiple
	void operator/=(float right);//!< operator division

	float length() const;//!< get the length of the vector.
	float SqLength() const; //!< get the sqrt of the vector.
	float dist(const zglVector& v) const; //!< calculate the distance between vector.
	float SqDist(const zglVector& v) const;//!< calculate the sqrt distance.
	float dot(const zglVector& v) const;//!< Dot multiple of two vector.
	zglVector Cross(const zglVector& v) const;//!< cross multiple of two vector.
	void normalize();//!< Normalize a vector.

	void rotateX(float ang); //!< rotate the vector with angle x.
	void rotateY(float ang); //!< rotate the vector with angle y.
	void rotateZ(float ang); //!< rotate the vector with angle z.

	/**
	 * @brief relative position in a new coordinate system.
	 */
	void relativePos(const zglMatrix& base);

	/**
	 * @brief absolute position in a new coordinate system.
	 */
	void absolutePos(const zglMatrix& base);

	/**
	 * @brief relative direction in a new coordinate system.
	 */
	void relativeDir(const zglMatrix& base);

	/**
	 * @brief absolute direction in a new coordinate system.
	 */
	void absoluteDir(const zglMatrix& base);

	/**
	 * @brief relative z-offset in a new coordinate system.
	 *
	 * @note very important function.
	 */
	float RelZ(const zglMatrix& base) const;

	/**
	 * operator multiple.
	 */
	friend zglVector operator*(float left, const zglVector& right);
};

#endif

