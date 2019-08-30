/******************************************************************************
 *  StateManager.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/26
 *  Implementation of the Class StateManager
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "StateManager.h"
#include "ev3api.h"
#include "MotorDriver.h"
#include "app.h"
#include "TailMotor.h"
/**
 * コンストラクタ
 * @param Tracer ライントレーサ
 * @param starter    スタータ
 */
 StateManager::StateManager(Tracer* Tracer,
                                const Button* Button,
                                Calibration* calibration,
 								Remote*     Remote,
 								MeasureDistance *measureDistance)
    : TailInit(false),
      LookUpCompFlag(false),
      mTracer(Tracer),
      mButton(Button),
      mCalibration(calibration),
      mRemote(Remote),
      mMeasureDistance(measureDistance),
      mState(UNDEFINED),
      TimeCount(0),
      mStartSignal(false)
      {
}

/**
 * デストラクタ
 */
StateManager::~StateManager() {
}

/**
 * ライントレースする
 */
void StateManager::run()
{

    switch (mState) {
    case UNDEFINED:
        execUndefined();
        break;
    case CALIBRATION:
    	execCalibration();
        break;
    case WAITING_FOR_START:
        execWaitingForStart();
        break;
    case WALKING:
        execWalking();
        break;
    case REMOTE:
    	remote();
    	break;
    case LOOKUPGATE:
    	execLookUpGate();
    	break;
    case GRAY_DETECT:
        execGrayDetect();
        break;
	case SEESAW:
		execSeesaw();
		break;
    case GARAGE:
    	execGarage();
    	break;
    default:
        break;
    }
}

/**
 * 未定義状態の処理
 */
void StateManager::execUndefined()
{

	ev3_lcd_set_font(EV3_FONT_MEDIUM);
#if RUN_COURSE == RUN_LEFT_COURSE
	ev3_lcd_draw_string("LeftCourse", 70, 90);
#elif RUN_COURSE == RUN_RIGHT_COURSE
	ev3_lcd_draw_string("RightCourse", 70, 90);
#endif

#if RUN_EDGE == RUN_LEFT_EDGE
	ev3_lcd_draw_string("LeftEdge", 70, 110);
#elif RUN_EDGE == RUN_RIGHT_EDGE
	ev3_lcd_draw_string("RightEdge", 70, 110);
#endif


	mTailMotor->init(0);
//    mCalibration->init();
    mState = CALIBRATION;
}

void StateManager::execCalibration()
{

 //   if (mCalibration->RunCalibration() == true)
    {
    	mState = WAITING_FOR_START;
    }

}


/**
 * 開始待ち状態の処理
 */
void StateManager::execWaitingForStart() {


    if (mButton->isPushed()|| mStartSignal == true)
    {
        mState = WALKING;
    }
}

/**
 * 走行中状態の処理
 */
void StateManager::execWalking()
{
	if(mRemote->RemoteState() == true)
	{
		mState = REMOTE;
	}
	else
	{
        mTracer->run();
        ev3_led_set_color(LED_GREEN); /* 通常走行は緑 */
#if RUN_COURSE == RUN_RIGHT_COURSE
        if( mTracer->isFinished() == true)
        {

            mState = FINISH;
            GoalFlag = true;
#elif RUN_COURSE == RUN_LEFT_COURSE
        if( mTracer->isFinished() == true)
        {

            mState = FINISH;
        	GoalFlag = true;
#endif
		}
	}
}

/**
 * ルックアップゲート状態の処理
 */
void StateManager::execLookUpGate()
{
	if(mRemote->RemoteState() == true)
	{
		mState = REMOTE;
	}
	else
	{

		if(LookUpCompFlag == true)
		{
			// mState = GARAGE;
            mState = GRAY_DETECT;
		}
	}
}
/**
 * シーソー制御の処理
 */
void StateManager::execSeesaw()
{
	ev3_led_set_color(LED_GREEN);		/* 通常走行は緑	*/

}
/**
 * 車庫入れ状態の処理
 */
void StateManager::execGarage()
{
	ev3_led_set_color(LED_ORANGE);
	if(mRemote->RemoteState() == true)
	{
		mState = REMOTE;
	}
	else
	{

	}
}
/**
 * 灰色検知してガレージに渡す処理
 */
void StateManager::execGrayDetect()
{
	ev3_led_set_color(LED_OFF);
	if(mRemote->RemoteState() == true)
	{
		mState = REMOTE;
	}
	else
	{
		if( mTracer->detectGray() == true)
		{
            mState = GARAGE;
        }
	}
}
/**
 * リモート中の処理
 */
void StateManager::remote()
{
	if(mRemote->RemoteState() == false)
	{
		mState = WALKING;
	}
	else
	{
		mRemote->RemoteControl();
	}
}
