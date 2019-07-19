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

	mPidController->setSignFacter(KPID_EDGE_FACTOR);
	tPidController->setSignFacter(KPID_THETA_FACTOR);

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
//    mSection = RunManager::STRAIGHT_ZONE;
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

    /*★★★ デバッグ用　シータ制御の検討 ★★★*/
#if THETA_CONTROL

    if (mLineMonitor->ColorDiffFromPre() == true) {
    	mOdmetry->KeepPreTheta();
    }

    if (mLineMonitor->DetectEdgeOfdif() == true) {
    	if (mOdmetry->mPreThetaNum >= PRETHETAKEEPDNUM) {
    		tagetTheta = (mOdmetry->getPreTheta(GETPRETHETANUM) + mOdmetry->getPreTheta(GETPRETHETANUM + 1)) / 2;
    		ThetaStart = true;
    	}
    }

    if (ThetaStart == true) {
    	/* 速度100で適合 */
    	tPidController->setPID(1.2, 0, 0);
    	DeltaTheta = tagetTheta - mOdmetry->getTheta() * 100;
    	tturn = tPidController->calControlledVariable(DeltaTheta);
    }

    /* ガード処理 */
    /* 差分が増加したらシータ制御は禁止する */
    if (mLineMonitor->IncreaseDif() == true) {
    	tturn = 0;
    	tagetTheta = 0;
    	ThetaStart = false;
    }

#endif

#if !USE_LINE_TRACE
    mturn = 0;
#endif
    tturn = 0;

    RightPWM = mforward - mturn - tturn;
    LeftPWM = mforward + mturn + tturn;
//    /* テスト用 後で削除する */
//    /* オドメトリチェックの精度をチェックするためのコード */
//    if (mOdmetry->getTheta() > 6.282) {
//    	RightPWM = 0;
//    	LeftPWM = 0;
//    	mturn = 0;
//    }
//
//    /* オドメトリチェック用 */
//    if (mOdmetry->getTheta() < -6.282) {
//    	RightPWM = 0;
//    	LeftPWM = 0;
//    	mturn = 0;
//    }
//
//    if (mOdmetry->getX() > 300.0) {
//    	RightPWM = 0;
//    	LeftPWM = 0;
//    	mturn = 0;
//    }

//    colorid_t color;
//    color = mLineMonitor->rcgColor();
//
//    if (color == COLOR_YELLOW) {
//    	RightPWM = 0;
//    	LeftPWM = 0;
//    }

    if (mSection == RunManager::FINISHED) {
    	RightPWM = 0;
    	LeftPWM = 0;
    }


    /* 左右輪に出力 */
	mRightWheel.setPWM(RightPWM);
	mLeftWheel.setPWM(LeftPWM);

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
    	/* 速度10で適合、文字読み取り用でゲイン高め */
    	//mPidController->setPID(2.0, 0, 9.0);
    	/* 速度20で適合 */
    	mPidController->setPID(0.5, 0, 9.0);
    	/* 速度100で適合 */
    	//mPidController->setPID(2.0, 0, 15.0);

    	/* 速度100で適合 */
    	//mPidController->setPID(2.0, 0, 17.0);

    	/* 速度100で適合 */
    	//mPidController->setPID(1.6, 0, 15.0);

    	/* 速度100で適合 */
    	//mPidController->setPID(1.6, 0, 17.0);

    	/* 速度100 + シータ制御で適合 */
    	//mPidController->setPID(1.8, 0, 17.0);

    	/* 速度100 + シータ制御で適合 */
    	//mPidController->setPID(2.0, 0, 17.0);

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

int Tracer::calcDirectionBlock(void)
{
    // ★PID走行
   return mPidController->calControlledVariable(mLineMonitor->getDeviationBlock());
}

void Tracer::setBlockPID(void)
{
	mPidController->setPID(0.3, 0, 0.5);
}
