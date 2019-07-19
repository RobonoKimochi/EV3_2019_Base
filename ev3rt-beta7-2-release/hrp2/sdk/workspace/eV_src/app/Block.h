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

#define TARGETDISTANCE (30)
#define HALFDISTANCE (15)

class Block {

public:

    Block(DriveController *drivecontroller);
    virtual ~Block();

    void run(void);

    bool EndFlag = false;

private:
    DriveController* mdrivecontroller;

    unsigned int mCommand_index = 0;

    CommandMaker::block_command mCommand[1000] =
//    {};
      {CommandMaker::BC_PassCircle,
       CommandMaker::BC_Forward,
	   CommandMaker::BC_LTurnCircle,
	   CommandMaker::BC_Forward,
	   CommandMaker::BC_RturnCircle,
	   CommandMaker::BC_End};  //試しに値を設定

};

#endif  // EV3_APP_BLOCK_H_
