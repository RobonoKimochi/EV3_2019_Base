/******************************************************************************
 *  Tracer.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/26
 *  Implementation of the Class Tracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "Tracer.h"

/**
 * コンストラクタ
 * @param lineMonitor     ライン判定
 * @param balancingWalker 倒立走行
 */
Tracer::Tracer(LineMonitor* lineMonitor,
                       PidController* pidController)
    : mforward(CONSTANT_FORWARD_VAL),
	  mturn(0),
      mLineMonitor(lineMonitor),
      mPidController(pidController),
      mIsInitialized(false),
      mIsFinished(false),
	  mSection(RunManager::STRAIGHT_ZONE),
      percent(1.0) {
}

/**
 * デストラクタ
 */
Tracer::~Tracer() {
}

/**
 * ライントレースする
 */
void Tracer::run()
{

    // コース状況を判断する
    mSection = mRunManager->determineCourse();

#if USE_DEBUG_MODE
#if USE_CONSTANT_PID
    /* デバッグ用処理 */
    /* 走行区間によるPIDの切り替えを無効化 */
    mSection = RunManager::STRAIGHT_ZONE;
#endif
#endif

    /* セクションによりPIDとforward値を選定 */
    set_PID_forward();

#if USE_DEBUG_MODE
    /* デバッグ用処理 */
    /* PID設定 */
//    mPidController->setPID(0.1, 0, 12);

#if USE_GRAY_THRESH
    mLineMonitor->LineThresholdGray();
#endif
#if LOOKUPGATE_TEST_MODE
    mIsFinished = true;
#endif
#endif

    /* PID制御によりturn値を算出 */
    mturn = calcDirection();

#if !USE_LINE_TRACE
    mturn = 0;
#endif

    /* ★★★削除予定★★★ */
    //両輪のPwm値がマイナスの時に符号を逆にする。主にスタート用
    mturn = -mturn / 2;
    mturn = mturn * (1.0f - percent);
    percent = percent * 0.9f;

    /* ★★★削除予定★★★ */
    /* ターン値ガード */
    if (mturn > TURN_GUARD)
    {
    	mturn = TURN_GUARD;
    }
    else if (mturn < -TURN_GUARD)
    {
    	mturn = -TURN_GUARD;
    }

    /* 左右輪に出力 */
	mRightWheel.setPWM(mforward - mturn);
	mLeftWheel.setPWM(mforward + mturn);

}

bool Tracer::detectGray()
{

	return true;
}


int Tracer::calcDirection()
{
    // ★PID走行
   return mPidController->calControlledVariable(mLineMonitor->getDeviation());
}

void Tracer::set_PID_forward()
{

    switch (mSection) {
    case RunManager::STRAIGHT_ZONE:
    	mPidController->setPID(1.0, 0, 9.0);
        break;
    case RunManager::CURB_ZONE:
    	mPidController->setPID(0.42, 0, 12.0);
        break;
    case RunManager::LOOSE_CURVE_ZONE:
    	mPidController->setPID(0.39, 0, 20.0);
        break;
    case RunManager::TIGHT_CURVE_ZONE:
    	mPidController->setPID(0.57, 0, 20.0);
        break;
    case RunManager::SLOW:
    	mLineMonitor->LineThresholdGray();
    	mPidController->setPID(0.5, 0, 40.0);
    	break;
    case RunManager::FINISHED:
        mIsFinished = true;
    	mLineMonitor->LineThresholdGray();
#if RUN_COURSE == RUN_LEFT_COURSE
    	mPidController->setPID(1.0, 0, 10.0);
#endif

#if RUN_COURSE == RUN_RIGHT_COURSE
    	mPidController->setPID(1.0, 0, 10.0);
        mBalancingWalker->setCommand(20, direction);	//■■速度は暫定
        mBalancingWalker->run();
#endif
    	break;

    default:
        break;
    }

}

/**
 * 走行区間が終了したか戻り値で返す
 */
bool Tracer::isFinished()
{
    if( mIsFinished == true)
    {
        return true;
    }
    else
    {
        return false;
    }
}
