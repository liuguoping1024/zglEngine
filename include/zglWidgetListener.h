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

#ifndef __zgl_widget_listener_h__
#define __zgl_widget_listener_h__

#include "zglEventType.h"

/**
 * @brief Listener to the widget.
 */
class zglWidgetListener
{
public:

    /**
     * @brief Handle the touch events to the widget.
     *
     * @param event The touch event.
     * @return True if the event was handled, false otherwise.
     */
	virtual bool onTouchEvent(zglTouchEvent *event);

	zglWidgetListener()
	{
	}

	virtual ~zglWidgetListener()
	{
	}
};


#endif
