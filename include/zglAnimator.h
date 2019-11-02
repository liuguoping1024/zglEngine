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

#ifndef __zgl_widget_animator_h__
#define __zgl_widget_animator_h__

/**
 * @brief zglAnimator is a special animation unit, which will be applied to
 * multi-object at the same time.
 */
class zglAnimator
{
public:
	zglAnimator();
	virtual ~zglAnimator();

	/**
	 * @brief add widget to the animator.
	 */
	void addWidget(Widget * w);

};


#endif
