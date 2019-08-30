/******************************************************************************
 *  Tracer.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Class Tracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_BLOCK_H_
#define EV3_APP_BLOCK_H_

#include "LineMonitor.h"
#include "PidController.h"

#include "Odmetry.h"
#include "Rotator.h"
#include "RunManager.h"
#include "app_config.h"
#include "Tracer.h"
#include "DriveController.h"
#include "CommandMaker.h"

#define FOWARDDISTANCE (30) /* 暫定 */
#define MEDDISTANCE  (15)
#define HALFDISTANCE (24 / 2)
#define SHORTDISTANCE  (6.5) /* 暫定 */
#define REMAINTDISTANCE  (FOWARDDISTANCE - SHORTDISTANCE - 2)

class Block {

public:

    Block(DriveController *drivecontroller, CommandMaker	*commandMaker);
    virtual ~Block();

    void run(void);

    bool EndFlag = false;

private:
    DriveController* mdrivecontroller;
    CommandMaker*   mCommandMaker;

    Sound *mSound = Sound::getInstance();

};

#endif  // EV3_APP_BLOCK_H_
