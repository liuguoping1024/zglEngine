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


#ifndef __zgl_widget_particle_h__
#define __zgl_widget_particle_h__

#include "zglWidget.h"
#include "zglPrimParticle.h"

#include "zglTexture.h"
#include "zglVector.h"


/**
 * @brief  Animation information of the Particle widgets.
 *
 * @note the widget list will not update this widget.
 */
struct zglParticleInfo
{
	/**
	 * @brief speed(velocity) of the particle.
	 */
	zglVector m_velocity;

	/**
	 * @brief gravity of the particle. a negative value will make the
	 * particle move upwards.
	 */
	float m_gravity;

	/**
	 * @brief Radial acceleration. a larger value will make the particle
	 * move far away more quickly.
	 */
	float m_radialAccel;

	/**
	 * @brief Tangential acceleration. The particle will rotate with the
	 * center if the value is set.
	 */
	float m_tangentialAccel;

	/**
	 * @brief The spin delta value. A none-zero value will make the particle
	 * rotate with its own center.
	 */
	float m_spinDelta;

	/**
	 * @brief The size delta value. A none-zero value will change the size.
	 */
	float m_sizeDelta;

	float m_colorR; //!< The current R color of the particle.
	float m_colorG; //!< The current G color of the particle.
	float m_colorB; //!< The current B color of the particle.
	float m_colorA; //!< The current A color of the particle.

	float m_colorRDelta; //!< The current R delta color of the particle.
	float m_colorGDelta; //!< The current R delta color of the particle.
	float m_colorBDelta; //!< The current R delta color of the particle.
	float m_colorADelta; //!< The current R delta color of the particle.

	long m_age; //!< The current age of the current particle.

	/**
	 * @brief The max age of the current particle.
	 * If the #m_age is larger than #m_ageMax, the particle will dead.
	 */
	long m_ageMax;
};

/**
 * @brief Animation information of the Particle.
 *
 * @note the widget list will not update this widget.
 */
//typedef struct zglSParticleInfo zglParticleInfo;

/**
 * @brief A typical particle system.
 * @note the widget list will not update this widget. The developer MUST update
 * the widget in #onUpdate().
 */
class zglWidgetParticle: public zglWidget
{
public:
	zglWidgetParticle();
	virtual ~zglWidgetParticle();

	/**
	 * @brief initial a new particle widget.
	 *
	 * @param totalCount the total count of the particles of the widget.
	 * @param initCount the initial count of the particles.
	 * @param tex the texture of all the particles of the widget.
	 * @param width single particle's width.
	 * @param height single particle's height.
	 */
	void init(int totalCount, int initCount, zglTexture * tex,
			float width, float height);

	/**
	 * @brief Set texture to the widget.
	 *
	 * The texture will be applied to all the particles.
	 * @param tex the texture of all the particles of the widget.
	 */
	void setTexture(zglTexture * tex);

	/**
	 * @brief Set the texture coordinate to widget.
	 *
	 * The texture coordinate will be applied to all the particles.
	 * @param left the left coordinate.
	 * @param top the top coordinate.
	 * @param right the right coordinate.
	 * @param bottom the bottom coordinate.
	 */
	void setTexCoord(float left, float top, float right, float bottom);

	/**
	 * Update the widget.
	 *
	 * The widget list will not update this special widget. so a new
	 * interface is designed for updating the widget.
	 */
	bool update(long time);

	/**
	 * @brief Set the visibility of the widget.
	 *
	 * All the particles of the widget will be affected.
	 *
	 * @param visible true to be visible, and false to make invisible.
	 */
	virtual void setVisible(bool visible);

	// ------------------------------------------------------------------------

	/**
	 * @brief set the life time of the widget.
	 *
	 * If the life time of the widget is larger than #m_lifeTimeMax, the whole
	 * widget will dead.
	 */
	inline void setGlobalLifeTime(long life) { m_lifeTimeMax = life; }

	/**
	 * @brief set the initial direction of a new particle.
	 *
	 * @param dir a radian value (0 -- 2pi).
	 */
	inline void setDirection(float dir) { m_direction = dir; }

	/**
	 * @brief set the spread value of the widgets.
	 *
	 * The spread value will make the particle move at the angle around the
	 * direction angle(#m_direction)
	 */
	inline void setSpread(float spread) { m_spread = spread; }

	/**
	 * @brief set the max and min life time of a new particle.
	 *
	 * The life time of the particle will be a random value between the min
	 * and the max value.
	 */
	inline void setLifeTime(long min, long max)
	{
		m_particalLifeMax = max;
		m_particalLifeMin = min;
	}

	/**
	 * @brief set the max and min velocity of a new particle.
	 *
	 * The initial velocity of the new particle will be a random value between
	 * the min and the max velocity value.
	 */
	inline void setVelocity(float min, float max)
	{
		m_velocityMax = max;
		m_velocityMin = min;
	}

	/**
	 * @brief set the max and min gravity of a new particle.
	 *
	 * The initial gravity of the new particle will be a random value between
	 * the min and the max gravity value.
	 */
	inline void setGravity(float min, float max)
	{
		m_gravityMax = max;
		m_gravityMin = min;
	}

	/**
	 * @brief set the max and min radial acceleration of a new particle.
	 *
	 * The initial radial acceleration of the new particle will be a random
	 * value between the min and the max radial acceleration value.
	 */
	inline void setRadialAccel(float min, float max)
	{
		m_radialAccelMax = max;
		m_radialAccelMin = min;
	}

	/**
	 * @brief set the max and min Tangential acceleration of a new particle.
	 *
	 * The initial Tangential acceleration of the new particle will be a
	 * random value between the min and the max Tangential acceleration value.
	 */
	inline void setTangAccel(float min, float max)
	{
		m_tangentialAccelMax = max;
		m_tangentialAccelMin = min;
	}

	/**
	 * @brief set the start and end size of a new particle.
	 *
	 * The initial size of the new particle will be a random value between
	 * the start and the end size value.
	 *
	 * No matter what the initial size is, the end size will be the same.
	 */
	inline void setSize(float start, long end)
	{
		m_sizeStart = start;
		m_sizeEnd = end;
	}

	/**
	 * @brief set the start and end Spin of a new particle.
	 *
	 * The initial Spin of the new particle will be a random value between
	 * the start and the end Spin value.
	 *
	 * No matter what the initial Spin is, the end Spin will be the same.
	 */
	inline void setSpin(float start, long end)
	{
		m_spinStart = start;
		m_spinEnd = end;
	}

	/**
	 * @brief set the start and end Red of a new particle.
	 *
	 * The initial Red of the new particle will be a random value between
	 * the start and the end Red value.
	 *
	 * No matter what the initial Red is, the end Red will be the same.
	 */
	inline void setRed(float start, long end)
	{
		m_colorRStart = start;
		m_colorREnd = end;
	}

	/**
	 * @brief set the start and end Green of a new particle.
	 *
	 * The initial Green of the new particle will be a random value between
	 * the start and the end Green value.
	 *
	 * No matter what the initial Green is, the end Green will be the same.
	 */
	inline void setGreen(float start, long end)
	{
		m_colorGStart = start;
		m_colorGEnd = end;
	}

	/**
	 * @brief set the start and end Blue of a new particle.
	 *
	 * The initial Blue of the new particle will be a random value between
	 * the start and the end Blue value.
	 *
	 * No matter what the initial Blue is, the end Blue will be the same.
	 */
	inline void setBlue(float start, long end)
	{
		m_colorBStart = start;
		m_colorBEnd = end;
	}

	/**
	 * @brief set the start and end alpha of a new particle.
	 *
	 * The initial alpha of the new particle will be a random value between
	 * the start and the end alpha value.
	 *
	 * No matter what the initial alpha is, the end alpha will be the same.
	 */
	inline void setAlpha(float start, long end)
	{
		m_colorAStart = start;
		m_colorAEnd = end;
	}

protected:
	/**
	 * @brief Update event handler of the widget.
	 *
	 * @param time The time line of the update event.
	 * @return True if the event was handled, false otherwise.
	 */
	virtual bool onUpdate(long time);

protected:
	long m_lifeTime; //!< Current Life time of the zglWidgetParticle.
	long m_lifeTimeMax;  //!< Max Life time of the zglWidgetParticle.

	long m_particalLifeMax; //!< Particle Node's max life time (ms).
	long m_particalLifeMin; //!< Particle Node's min life time (ms).

	float m_direction; //!< particle initial direction.

	float m_spread; //!< particle spread direction.

	float m_velocityMax;//!< Particle's max speed.
	float m_velocityMin;//!< Particle's min speed.

	float m_gravityMax;//!< Particle's max gravity.
	float m_gravityMin;//!< Particle's min gravity.

	float m_radialAccelMax;//!< Particle's max radial acceleration.
	float m_radialAccelMin;//!< Particle's min radial acceleration.

	float m_tangentialAccelMax;//!< Particle's max tangential acceleration.
	float m_tangentialAccelMin;//!< Particle's min tangential acceleration.

	float m_sizeStart;//!< Size animation range
	float m_sizeEnd;//!< Size animation range

	float m_spinStart;//!< Spin animation range.
	float m_spinEnd;//!< Spin animation range.

	float m_colorRStart;//!< R Color animation range.
	float m_colorGStart;//!< G Color animation range.
	float m_colorBStart;//!< B Color animation range.
	float m_colorAStart;//!< A Color animation range.

	float m_colorREnd;//!< R Color animation range.
	float m_colorGEnd;//!< G Color animation range.
	float m_colorBEnd;//!< B Color animation range.
	float m_colorAEnd;//!< A Color animation range.

	long m_lastTick;//!< Animation tick

private:
	//zglTexture* m_texture;

	zglPrimParticle m_particlePrim;

	zglParticleInfo * m_particleInfo;
	zglParticleNode * m_particleNodeInfo;
	unsigned short m_particleCount;
	unsigned short m_aliveCount;

	float m_width;//!< Height of the particle.
	float m_height;//!< Width of the particle.
};



#endif
