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
Block::Block(DriveController* drivecontroller, CommandMaker	*commandMaker)
    : mdrivecontroller(drivecontroller),
      mCommandMaker(commandMaker){
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

	/* カラーセンサ値更新 */
	mdrivecontroller->SetColor();

    switch (mCommandMaker->mCommand[mCommandMaker->mCommand_index]) {
    case CommandMaker::BC_Forward:
    	FinishFlag = mdrivecontroller->TraceForward(FOWARDDISTANCE);
        break;
    case CommandMaker::BC_RemainFor:
    	FinishFlag = mdrivecontroller->TraceForward(REMAINTDISTANCE);
        break;
    case CommandMaker::BC_Med_For:
    	FinishFlag = mdrivecontroller->TraceForward(MEDDISTANCE);
        break;
    case CommandMaker::BC_Short_For:
    	FinishFlag = mdrivecontroller->TraceForward(SHORTDISTANCE);
        break;
    case CommandMaker::BC_Half_For:
    	FinishFlag = mdrivecontroller->TraceForward(HALFDISTANCE);
        break;
    case CommandMaker::BC_GetLft:
    	FinishFlag = mdrivecontroller->GetLft();
        break;
    case CommandMaker::BC_GetRgt:
    	FinishFlag = mdrivecontroller->GetRgt();
        break;
    case CommandMaker::BC_Rturn:
    	FinishFlag = mdrivecontroller->TurnN(((float)(2.6015926 / 2)), -TURNSPEED, TURNSPEED);
#if RUN_COURSE == RUN_LEFT_COURSE
    	mdrivecontroller->OffsetFlag = true;
#else
    	mdrivecontroller->OffsetFlag = false;
#endif
        break;
    case CommandMaker::BC_Lturn:
    	FinishFlag = mdrivecontroller->TurnN(((float)(2.6015926 / 2)), TURNSPEED, -TURNSPEED);
#if RUN_COURSE == RUN_LEFT_COURSE
    	mdrivecontroller->OffsetFlag = false;
#else
    	mdrivecontroller->OffsetFlag = true;
#endif
    	break;
    case CommandMaker::BC_PassCircle:
    	FinishFlag = mdrivecontroller->ThroughCircle();
    	break;
    case CommandMaker::BC_RTurnCircle:
    	FinishFlag = mdrivecontroller->RgtTurnCircle();
    	break;
    case CommandMaker::BC_LTurnCircle:
    	FinishFlag = mdrivecontroller->LftTurnCircle();
    	break;
    case CommandMaker::BC_UTurnCircle:
    	FinishFlag = mdrivecontroller->UTurnCircle();
    	break;
    case CommandMaker::BC_SetRgt:
    	FinishFlag = mdrivecontroller->SetRgt();
    	break;
    case CommandMaker::BC_SetLft:
    	FinishFlag = mdrivecontroller->SetLft();
    	break;
    case CommandMaker::BC_BackLft:
    	FinishFlag = mdrivecontroller->BackLft();
    	break;
    case CommandMaker::BC_BackRgt:
    	FinishFlag = mdrivecontroller->BackRgt();
    	break;
    case CommandMaker::BC_End:
    	EndFlag = true;
    	mdrivecontroller->SetPWMZero();
    	break;
    default:
        break;
    }

    if (FinishFlag == true) {
    	mSound->ok_flag = true;
    	mCommandMaker->mCommand_index++;
    }

}
