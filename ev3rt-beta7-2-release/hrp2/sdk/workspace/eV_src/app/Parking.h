/******************************************************************************
 *  Tracer.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Class Tracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_PARKING_H_
#define EV3_APP_PARKING_H_

#include "LineMonitor.h"
#include "PidController.h"

#include "Odmetry.h"
#include "RunManager.h"
#include "app_config.h"
#include "Tracer.h"
#include "DriveController.h"
#include "CommandMaker.h"


class Parking {

public:

    Parking();
    virtual ~Parking();

    void run(void);

private:


};

#endif  // EV3_APP_PARKING_H_
