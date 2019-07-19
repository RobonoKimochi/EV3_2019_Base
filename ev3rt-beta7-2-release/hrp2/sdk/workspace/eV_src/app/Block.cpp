/******************************************************************************
 *  Tracer.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/26
 *  Implementation of the Class Tracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "Block.h"

/**
 * コンストラクタ
 *
 *
 */
Block::Block(DriveController* drivecontroller)
    : mdrivecontroller(drivecontroller){
}

/**
 * デストラクタ
 */
Block::~Block() {
}

/**
 * ブロックならべメイン処理開始
 */
void Block::run(void)
{

	bool FinishFlag = false;

    switch (mCommand[mCommand_index]) {
    case CommandMaker::BC_Forward:
    	FinishFlag = mdrivecontroller->TraceForward(TARGETDISTANCE);
        break;
    case CommandMaker::BC_Half_For:
    	FinishFlag = mdrivecontroller->TraceForward(HALFDISTANCE);
        break;
    case CommandMaker::BC_Rturn:
    	FinishFlag = mdrivecontroller->TurnR();
        break;
    case CommandMaker::BC_Ltrun:
    	FinishFlag = mdrivecontroller->TurnL();
    	break;
    case CommandMaker::BC_PassCircle:
    	FinishFlag = mdrivecontroller->ThroughCircle();
    	break;
    case CommandMaker::BC_RturnCircle:
    	FinishFlag = mdrivecontroller->RgtTurnCircle();
    	break;
    case CommandMaker::BC_LTurnCircle:
    	FinishFlag = mdrivecontroller->LftTurnCircle();
    	break;
    case CommandMaker::BC_UTurnCircle:
    	FinishFlag = mdrivecontroller->UTurnCircle();
    	break;
    case CommandMaker::BC_Set:
    	FinishFlag = mdrivecontroller->SetToBin();
    	break;
    case CommandMaker::BC_BackShallow:
    	FinishFlag = mdrivecontroller->BackSallow();
    	break;
    case CommandMaker::BC_BackDeep:
    	FinishFlag = mdrivecontroller->BackDeep();
    	break;
    case CommandMaker::BC_End:
    	EndFlag = true;
    	break;
    default:
        break;
    }

    if (FinishFlag == true) {
    	mCommand_index++;
    }

}
