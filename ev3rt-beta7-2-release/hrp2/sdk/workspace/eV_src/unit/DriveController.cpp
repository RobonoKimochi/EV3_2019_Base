#include "DriveController.h"
#include "app.h"


/**
 * コンストラクタ
 */
DriveController::DriveController(LineMonitor* lineMonitor,Tracer* Tracer)
: mLineMonitor(lineMonitor),
  mTracer(Tracer)
{
}

/**
 * デストラクタ
 */
DriveController::~DriveController()
{
}

bool DriveController::TurnN(float TargetTheta, float R_PWM, float L_PWM)
{

	bool FinishFlag = false;
	float AbsThetaDif = 0.0;
	/* 初回には目標Θをセットする */
	if(mNInitFlag == false) {
		mNInitTheta = mOdmetry->getTheta();
    	mBackTargetBri = mLineMonitor->rgbTobright();
    	mBackInitFlag = true;
    	mNInitFlag = true;
	}

	mNTheta = mOdmetry->getTheta();
	AbsThetaDif = ABS(mNTheta - mNInitTheta);

    if (AbsThetaDif < TargetTheta) {
    	/* 目標Θ未達の場合はモータに回転の指示 */
        /* 左右輪に出力 */
    	mRightWheel.setPWM(R_PWM);
    	mLeftWheel.setPWM(L_PWM);
    } else {
//    	/* 目標Θ達成の場合は完了フラグをtrueにする */
    	FinishFlag = true;
    	mNInitFlag = false;
    }

    return (FinishFlag);
}

bool DriveController::TurnL(bool FowFlag, float R_PWM, float L_PWM)
{

	bool FinishFlag = false;
	/* 初回には目標Θをセットする */
	if(mLftInitFlag == false) {
		mLftTargetTheta = mOdmetry->getTheta();

		if (FowFlag == true) {
			mLftTargetTheta += QTURN;
		} else {
			mLftTargetTheta += QTURNB;
		}

    	mBackTargetBri = mLineMonitor->rgbTobright();
    	mBackInitFlag = true;
		mLftInitFlag = true;
		OffsetFlag = false;
	}

	mLftTheta = mOdmetry->getTheta();

    if (mLftTheta < mLftTargetTheta) {
    	/* 目標Θ未達の場合はモータに回転の指示 */
        /* 左右輪に出力 */
    	mRightWheel.setPWM(R_PWM);
    	mLeftWheel.setPWM(L_PWM);

    	FinishFlag = false;
    } else {
//    	/* 目標Θ達成の場合は完了フラグをtrueにする */
//    	mRightWheel.setPWM(0);
//    	mLeftWheel.setPWM(0);
    	FinishFlag = true;
    	mLftInitFlag = false;
    }

    return (FinishFlag);
}

bool DriveController::TurnR(bool FowFlag, float R_PWM, float L_PWM)
{

	bool FinishFlag = false;
	/* 初回には目標Θをセットする */
	if(mRgtInitFlag == false) {
		mRgtTargetTheta = mOdmetry->getTheta();
		if (FowFlag == true) {
			mRgtTargetTheta -= QTURN;
		} else {
			mRgtTargetTheta -= QTURNB;
		}
		mRgtInitFlag = true;
		OffsetFlag = true;
	}

	mRgtTheta = mOdmetry->getTheta();

    if (mRgtTheta > mRgtTargetTheta) {
    	/* 目標Θ未達の場合はモータに回転の指示 */
        /* 左右輪に出力 */
    	mRightWheel.setPWM(R_PWM);
    	mLeftWheel.setPWM(L_PWM);

    	FinishFlag = false;
    } else {
    	/* 目標Θ達成の場合は完了フラグをtrueにする */
//    	mRightWheel.setPWM(0);
//    	mLeftWheel.setPWM(0);
    	FinishFlag = true;
    	mRgtInitFlag = false;
    }

    return (FinishFlag);
}

bool DriveController::Forward(float targetdistance)
{

	bool FinishFlag = false;

	FinishFlag = SetPWMForward(targetdistance,FORWARDSPEED,FORWARDSPEED);

    return (FinishFlag);
}

bool DriveController::Backward(float targetdistance)
{

	bool FinishFlag = false;

	FinishFlag = SetPWMForward(targetdistance,-FORWARDSPEED,-FORWARDSPEED);

    return (FinishFlag);
}

/**
 * ブロックならべライントレース
 */
bool DriveController::TraceForward(float targetdistance)
{

	bool FinishFlag = false;
    float turn;
    float RightPWM;
    float LeftPWM;

	mTracer->setBlockPID();

    /* PID制御によりturn値を算出 */
    turn = mTracer->calcDirectionBlock();

    RightPWM = FORWARDSPEED - turn;
    LeftPWM = FORWARDSPEED + turn;


    FinishFlag = SetPWMForward(targetdistance,RightPWM,LeftPWM);

	return (FinishFlag);
}

bool DriveController::SetForward(float targetdistance)
{

//	bool FinishFlag = false;

//    FinishFlag = SetPWMForward(targetdistance,FORWARDSPEED,FORWARDSPEED);

	bool FinishFlag = false;
	float x,y,Distance;
	/* 初回には目標Θをセットする */
	if(mFowInitFlag == false) {
		mFowInitX = mOdmetry->getX();
		mFowInitY = mOdmetry->getY();
		mFowInitFlag = true;
	}

	x = mOdmetry->getX();
	y = mOdmetry->getY();

	Distance = DIST2POINT(x,mFowInitX,y,mFowInitY);

    if (Distance < targetdistance) {
    	/* 目標未達の場合はモータに回転の指示 */
        /* 左右輪に出力 */
    	mRightWheel.setPWM(FORWARDSPEED);
    	mLeftWheel.setPWM(FORWARDSPEED);

    } else {
//    	/* 目標達成の場合は完了フラグをtrueにする */
    	FinishFlag = true;
    	mFowInitFlag = false;
    }

	return (FinishFlag);
}

bool DriveController::SetPWMForward(float targetdistance, float R_PWM, float L_PWM)
{

	bool FinishFlag = false;
	float x,y,Distance;

	if(mFowInitFlag == false) {
		mFowInitX = mOdmetry->getX();
		mFowInitY = mOdmetry->getY();
		mFowInitFlag = true;
	}

	x = mOdmetry->getX();
	y = mOdmetry->getY();

	Distance = DIST2POINT(x,mFowInitX,y,mFowInitY);

    if (Distance < targetdistance) {
    	/* 目標未達の場合はモータに回転の指示 */
        /* 左右輪に出力 */
    	mRightWheel.setPWM(R_PWM);
    	mLeftWheel.setPWM(L_PWM);

    } else {
//    	/* 目標達成の場合は完了フラグをtrueにする */
//    	mRightWheel.setPWM(0);
//    	mLeftWheel.setPWM(0);
    	FinishFlag = true;
    	mFowInitFlag = false;
    }

    if (MINGUARDDISTANCE < Distance) {
    	/* サークルに乗っているか判定する */
    	if ((COUNTERNUMTHRESH <= Bcount) || (COUNTERNUMTHRESH <= Gcount) || (COUNTERNUMTHRESH <= Ycount) || (COUNTERNUMTHRESH <= Rcount)) {

    		FinishFlag = true;
    		mFowInitFlag = false;
    	}
    }

    return (FinishFlag);
}

bool DriveController::ThroughCircle(void)
{

	bool FinishFlag = false;
	float x,y,Distance;
	/* 初回には目標Θをセットする */
	if(mThrInitFlag == false) {
		mThrInitX = mOdmetry->getX();
		mThrInitY = mOdmetry->getY();
		mThrInitFlag = true;
	}

	x = mOdmetry->getX();
	y = mOdmetry->getY();

	Distance = DIST2POINT(x,mThrInitX,y,mThrInitY);

    FinishFlag = UpdateFowColorState();

    if (Distance > CIRCLEDISTANCE) {
    	FinishFlag = true;
    	mCircleState = CS_FirstLine;

    }

    if (FinishFlag == true) {

    	mThrInitFlag = false;
    }

    return (FinishFlag);
}

bool DriveController::RgtTurnCircle(void)
{

	bool FinishFlag = false;
#if RUN_COURSE == RUN_LEFT_COURSE
    FinishFlag = UpdateFarColorState();
#else
    FinishFlag = UpdateNearColorState();
#endif

    return (FinishFlag);
}

bool DriveController::LftTurnCircle(void)
{

	bool FinishFlag = false;
#if RUN_COURSE == RUN_LEFT_COURSE
    FinishFlag = UpdateNearColorState();
#else
    FinishFlag = UpdateFarColorState();
#endif

    return (FinishFlag);
}

bool DriveController::UTurnCircle(void)
{

	bool FinishFlag = false;

//    FinishFlag = UpdateColorState(TM_UTurn);

    return (FinishFlag);
}

bool DriveController::UpdateFowColorState()
{

	bool FinishFlag    = false;

    switch (mCircleState) {
    case CS_FirstLine: /* 黒ラインに乗っている状態 */

    	/* 色サークルに乗っているか判定する */
        if ((COUNTERNUMTHRESH <= Bcount) || (COUNTERNUMTHRESH <= Gcount) || (COUNTERNUMTHRESH <= Ycount) || (COUNTERNUMTHRESH <= Rcount)) {

            mCircleState = CS_FirstColor;
        }

        /* 左右輪に出力 */
    	mRightWheel.setPWM(FORWARDSPEED);
    	mLeftWheel.setPWM(FORWARDSPEED);

        break;
    case CS_FirstColor: /* 色サークルに乗っている状態 */

    	/* 白い円に入ったか判定する */
    	/* 色サークルに乗っているか判定する */
        if (COUNTERNUMTHRESH <= Wcount) {
            mCircleState = CS_SecondWhite;
        }

        /* 左右輪に出力 */
    	mRightWheel.setPWM(FORWARDSPEED);
    	mLeftWheel.setPWM(FORWARDSPEED);

        break;
    case CS_SecondWhite: /* 白い円に入った状態 */

    	/* 再び色サークルに乗っているか判定する */
        if ((COUNTERNUMTHRESH <= Bcount) || (COUNTERNUMTHRESH <= Gcount) || (COUNTERNUMTHRESH <= Ycount) || (COUNTERNUMTHRESH <= Rcount)) {

            mCircleState = CS_SecondColor;
        }

        /* 左右輪に出力 */
    	mRightWheel.setPWM(FORWARDSPEED);
    	mLeftWheel.setPWM(FORWARDSPEED);

        break;
    case CS_SecondColor:

    	/* 黒ラインに乗ったか判定する */
        if (COUNTERNUMTHRESH <= Wcount || colorNum == COLOR_NONE) {

            mCircleState = CS_SecondLine;

        } else {

            /* 左右輪に出力 */
        	mRightWheel.setPWM(FORWARDSPEED);
        	mLeftWheel.setPWM(FORWARDSPEED);
        }


    	break;
    case CS_SecondLine: /* サークルを超えてラインに到達。次回のために初期化処理を行う */

        /* 次のためにfirstlineに戻す */
        mCircleState = CS_FirstLine;
    	FinishFlag = true;
    	break;
    default:
        break;
    }

    return(FinishFlag);

}

bool DriveController::UpdateFarColorState()
{

	bool FinishFlag    = false;

    switch (mCircleState) {
    case CS_FirstLine: /* 黒ラインに乗っている状態 */

    	/* 色サークルに乗っているか判定する */
        if ((COUNTERNUMTHRESH <= Bcount) || (COUNTERNUMTHRESH <= Gcount) || (COUNTERNUMTHRESH <= Ycount) || (COUNTERNUMTHRESH <= Rcount)) {

            mCircleState = CS_FirstColor;
        } else if ((colorid_t::COLOR_BLUE <=colorPre) && (colorPre <= colorid_t::COLOR_RED)) {
        	mCircleState = CS_FirstColor;
        }

        /* 左右輪に出力 */
    	mRightWheel.setPWM(FORWARDSPEED);
    	mLeftWheel.setPWM(FORWARDSPEED);

        break;
    case CS_FirstColor: /* 色サークルに乗っている状態 */

    	/* 白い円に入ったか判定する */
    	/* 色サークルに乗っているか判定する */
        if (COUNTERNUMTHRESH <= Wcount || colorNum == COLOR_NONE) {

            mCircleState = CS_SecondWhite;
        }

        /* 左右輪に出力 */
        mRightWheel.setPWM(FORWARDSPEED);
        mLeftWheel.setPWM(FORWARDSPEED);

        break;
    case CS_FirstWhite: /* 白い円に入った状態で一定距離前進 */
//
//
//    	if (FowardFinFlag == true) {
//    		mCircleState = CS_SecondWhite;
//    	}
//
//    	if (turnMode == TM_RgtTurn){
//    		/* 左右輪に出力 */
//    		mRightWheel.setPWM(-TURNSLOW);
//    		mLeftWheel.setPWM(FORWARDSPEED);
//    	} else {
//    		/* 左右輪に出力 */
//    		mRightWheel.setPWM(FORWARDSPEED);
//    		mLeftWheel.setPWM(-TURNFAST);
//    	}
//
//        break;
    case CS_Turn: /* 白い円で旋回する状態 */

//    	if (turnMode == TM_RgtTurn) {
//    		TrunFinFlag = TurnR();
//    	} else if (turnMode == TM_LftTurn){
//    		TrunFinFlag = TurnL();
//    	} else {
//
//    		/* Uターンする場合 */
//    		/* 右旋回 → 直進(トレースエッジ変更) →右旋回の順番で処理を実施 */
//
//    	    switch (mUTurnState) {
//    	    case US_FirstTurn: /* 最初の旋回 */
//
//    	    	UTTurnFlag = TurnR();
//
//    	    	if (UTTurnFlag == true) {
//    	    		mUTurnState = US_Forward;
//    	    	}
//
//    	    	break;
//    	    case US_Forward: /* トレースエッジ分の移動 */
//
//    	    	UTForFlag = Forward(EDGEDISTANCE);
//
//    	    	if (UTForFlag == true) {
//    	    		mUTurnState = US_SecondTurn;
//    	    	}
//
//    	    	break;
//    	    case US_SecondTurn: /* 2回目の旋回 */
//
//    	    	UTTurnFlag = TurnR();
//
//    	    	if (UTTurnFlag == true) {
//    	    		mUTurnState = US_FirstTurn;
//    	    		TrunFinFlag = true;
//    	    	}
//
//    	    	break;
//    	    default:
//    	        break;
//    	    }
//    	}
//
//    	if (TrunFinFlag == true) {
//    		mCircleState = CS_SecondWhite;
//    	}
//
//        break;
    case CS_SecondWhite: /* 白い円に入った状態 */

    	/* 再び色サークルに乗っているか判定する */
        if ((COUNTERNUMTHRESH <= Bcount) || (COUNTERNUMTHRESH <= Gcount) || (COUNTERNUMTHRESH <= Ycount) || (COUNTERNUMTHRESH <= Rcount)) {

            mCircleState = CS_SecondColor;
        }

        /* 左右輪に出力 */
#if RUN_COURSE == RUN_LEFT_COURSE
        /* Lコースでは右折 */
    	mRightWheel.setPWM(1);
    	mLeftWheel.setPWM(20);
#else
    	/* Rコースでは右左折 */
    	mRightWheel.setPWM(20);
    	mLeftWheel.setPWM(1);
#endif

        break;
    case CS_SecondColor:

    	/* 黒ラインに乗ったか判定する */
        if (COUNTERNUMTHRESH <= Wcount) {

//            mCircleState = CS_AdjustAng;
            mSecondWFlag = true;
        }

        if (mSecondWFlag == true) {
        	if (mLineMonitor->rgbTobright() < (mLineMonitor->mBlockLineThreshold + 20)) {
//        		mCircleState = CS_AdjustAng;
        		mCircleState = CS_FirstLine;
        		mSecondWFlag = false;
        		FinishFlag = true;
        	}
        }

        /* 左右輪に出力 */
#if RUN_COURSE == RUN_LEFT_COURSE
        /* Lコースでは右折 */
        mRightWheel.setPWM(-7);
        mLeftWheel.setPWM(10);
#else
        /* Rコースでは左折 */
        mRightWheel.setPWM(10);
        mLeftWheel.setPWM(-7);
#endif

    	break;
    default:
        break;
    }

    return(FinishFlag);

}

bool DriveController::UpdateNearColorState(void)
{

	bool FinishFlag    = false;
	bool StaFinishFlag    = false;

    switch (mCircleState) {
    case CS_FirstLine: /* 黒ラインに乗っている状態 */

    	/* 色サークルに乗っているか判定する */
        if ((COUNTERNUMTHRESH <= Bcount) || (COUNTERNUMTHRESH <= Gcount) || (COUNTERNUMTHRESH <= Ycount) || (COUNTERNUMTHRESH <= Rcount)) {

            mCircleState = CS_FirstColor;
        } else if ((colorid_t::COLOR_BLUE <=colorPre) && (colorPre <= colorid_t::COLOR_RED)) {
        	mCircleState = CS_FirstColor;
        }

        /* 左右輪に出力 */
    	mRightWheel.setPWM(FORWARDSPEED);
    	mLeftWheel.setPWM(FORWARDSPEED);

        break;
    case CS_FirstColor: /* 色サークルに乗っている状態 */

    	/* 白い円に入ったか判定する */
    	/* 色サークルに乗っているか判定する */
        if (COUNTERNUMTHRESH <= Wcount || colorNum == COLOR_NONE) {

            mCircleState = CS_SecondWhite;
        }


        /* 左右輪に出力 */
    	mRightWheel.setPWM(FORWARDSPEED);
    	mLeftWheel.setPWM(FORWARDSPEED);

        break;

    case CS_SecondWhite: /* 白い円に入った状態 */

    	/* 再び色サークルに乗っているか判定する */
        if ((COUNTERNUMTHRESH <= Bcount) || (COUNTERNUMTHRESH <= Gcount) || (COUNTERNUMTHRESH <= Ycount) || (COUNTERNUMTHRESH <= Rcount)) {

            mCircleState = CS_SecondColor;
        }

        /* 左右輪に出力 */
#if RUN_COURSE == RUN_LEFT_COURSE
        /* Lコースでは左折 */
    	mRightWheel.setPWM(20);
    	mLeftWheel.setPWM(10);
#else
        /* Rコースでは右折 */
    	mRightWheel.setPWM(10);
    	mLeftWheel.setPWM(20);

#endif

        break;
    case CS_SecondColor:

    	/* 白い円に入ったか判定する */
    	/* 色サークルに乗っているか判定する */
        if (COUNTERNUMTHRESH <= Wcount || colorNum == COLOR_NONE) {

            mCircleState = CS_SecondLine;
        }

        /* 左右輪に出力 */
#if RUN_COURSE == RUN_LEFT_COURSE
        /* Lコースでは左折 */
    	mRightWheel.setPWM(10);
    	mLeftWheel.setPWM(-7);
#else
        /* Rコースでは右折 */
    	mRightWheel.setPWM(-7);
    	mLeftWheel.setPWM(10);

#endif

    	break;
    case CS_SecondLine: /* サークルを超えてラインに到達。次回のために初期化処理を行う */

    	/* 黒ラインに乗ったか判定する */
        if (mLineMonitor->rgbTobright() < (mLineMonitor->mBlockBlackThresh + 30)) {

//            mCircleState = CS_AdjustAng;
            mSecondWFlag = true;
        }

        if (mSecondWFlag == true) {
        	if (mLineMonitor->rgbTobright() > (mLineMonitor->mBlockLineThreshold + 5)) {
//        		mCircleState = CS_AdjustAng;
        		mCircleState = CS_FirstLine;
        		mSecondWFlag = false;
        		FinishFlag = true;
        	}
        }

        /* 左右輪に出力 */
#if RUN_COURSE == RUN_LEFT_COURSE
        /* Lコースでは左折 */
       	mRightWheel.setPWM(10);
       	mLeftWheel.setPWM(-10);
#else
        /* Rコースでは右折 */
       	mRightWheel.setPWM(-10);
       	mLeftWheel.setPWM(10);

#endif

    	break;
    case CS_AdjustAng: /* サークルを超えてラインに到達。次回のために初期化処理を行う */

    	StaFinishFlag = TurnN(AdjustAng, 10,-10);

    	if (StaFinishFlag == true) {
    		/* 次のためにfirstlineに戻す */
    		mCircleState = CS_FirstLine;
    		mSecondWFlag = false;
    		FinishFlag = true;
    	}
    	break;
    default:
        break;
    }

    return(FinishFlag);

}

bool DriveController::SetRgt(void)
{

	bool FinishFlag = false;

    FinishFlag = UpdateSetState(false, RGTSETDISTANCE, 0.0, TURNSPEED);

    return (FinishFlag);
}

bool DriveController::SetLft(void)
{

	bool FinishFlag = false;

    FinishFlag = UpdateSetState(true, LFTSETDISTANCE, TURNSPEED, 0.0);

    return (FinishFlag);
}

bool DriveController::Curve(bool Turn_L, bool FowFlag, float R_PWM, float L_PWM)
{
	bool FinishFlag = false;

	if (Turn_L == true) {
		FinishFlag = TurnL(FowFlag, R_PWM, L_PWM);
	} else {
		FinishFlag = TurnR(FowFlag, R_PWM, L_PWM);
	}

    return(FinishFlag);
}

bool DriveController::UpdateSetState(bool Turn_L, float fowDistance, float R_PWM, float L_PWM)
{

	bool FinishFlag = false;
	bool FowFlag    = false;

    switch (mSetState) {
    case SS_Curv: /* 旋回 */

    	FowFlag = Curve(Turn_L, true, R_PWM, L_PWM);

        if(FowFlag == true) {
        	mSetState = SS_Foward;
        }

        break;
    case SS_Foward: /* 前進 */

    	FinishFlag = SetPWMForward(fowDistance, SETSPEED, SETSPEED);

    	if (FinishFlag == true) {
    		mSetState = SS_Curv;
    	}

        break;
    default:
        break;
    }

    return (FinishFlag);
}

bool DriveController::BackLft(void)
{

	bool FinishFlag = false;

    FinishFlag = UpDateBackState(false, LFTBACKDISTANCE, -TURNSPEED, 0.0);

    return (FinishFlag);
}

bool DriveController::BackRgt(void)
{

	bool FinishFlag = false;

	FinishFlag = UpDateBackState(true, RGTBACKDISTANCE, 0.0, -TURNSPEED);

    return (FinishFlag);
}

bool DriveController::UpDateBackState(bool Turn_L, float BackDistance, float R_PWM, float L_PWM)
{

	bool FinishFlag = false;
	bool BackFlag = false;

    switch (mBackState) {
    case BS_Back: /* 後退 */

    	BackFlag = SetPWMForward(BackDistance, -BACKWARDSPEED, -BACKWARDSPEED);

    	if (BackFlag == true) {
    		mBackState = BS_Curv;
    	}
        break;
    case BS_Curv: /* 旋回 */

        FinishFlag = Curve(Turn_L, false, R_PWM, L_PWM);

        if(FinishFlag == true) {
        	mBackState = BS_Back;
        }
        break;
    default:
        break;
    }

    return (FinishFlag);
}

bool DriveController::GetLft(void)
{

	bool FinishFlag = false;
	float distance = LFTGETDISTANCE;

	if (OffsetFlag == true) {
		distance += OFFSETTDISTANCE;
	}

    FinishFlag = UpDateGetState(distance, TURNSPEED, 0.0);

    return (FinishFlag);
}

bool DriveController::GetRgt(void)
{

	bool FinishFlag = false;
	float distance = RGTGETDISTANCE;

	if (OffsetFlag == true) {
		distance += OFFSETTDISTANCE;
	}

    FinishFlag = UpDateGetState(distance, 0.0, TURNSPEED);

    return (FinishFlag);
}

bool DriveController::UpDateGetState(float GetDistance, float R_PWM, float L_PWM)
{

	bool FinishFlag = false;
	bool FowFlag = false;

    switch (mGetState) {
    case GS_Forward: /* 直進 */

    	FowFlag = SetForward(GetDistance);

        if(FowFlag == true) {
        	mGetState = GS_Curv;
        }
        break;
    case GS_Curv: /* 旋回 */

        FinishFlag = TurnN(((float)(2.6015926 / 2)), R_PWM, L_PWM);

        if(FinishFlag == true) {
        	mGetState = GS_Forward;
        }
        break;
    default:
        break;
    }

    return (FinishFlag);
}

bool DriveController::BackToLine(BackMode backMode, float setDistance)
{

	bool FinishFlag = false;

//    switch (mBackState) {
//    case BT_Deley: /* 待つ */
//
//    	DeleyCoute++;
//
//    	if (DeleyCoute > 30) {
//    		mBackState = BT_Color;
//    		DeleyCoute = 0;
//    	}
//
//        break;
//    case BT_Color: /* 白地に行く前 */
//
//    	Backward(setDistance);
//
//    	if (mLineMonitor->rgbTobright() > (mLineMonitor->mBlockWhiteThresh - BRITHEOFF)) {
//    		mBackState = BT_White;
//    	}
//
//        break;
//    case BT_White: /* ラインに乗る前 */
//
//    	Backward(setDistance);
//
//    	if (mLineMonitor->rgbTobright() < (mLineMonitor->mBlockLineThreshold + BRITHEOFF)) {
//
//    		if (backMode == BM_Deep) {
//    			mBackState = BT_Line;
//    		} else {
//    			FinishFlag = true;
//    		}
//    	}
//
//        break;
//    case BT_Line: /* トレースするエッジに移動 */
//
//    	Backward(setDistance);
//
//    	if (mLineMonitor->rgbTobright() > mLineMonitor->mBlockLineThreshold) {
//    		FinishFlag = true;
//    	}
//
//        break;
//    default:
//        break;
//    }
//
//    char buf[256];
//    sprintf( buf, "%03d", mBackTargetBri);          // 光センサ値を表示
//    ev3_lcd_draw_string( buf, 0, 70);
//    sprintf( buf, "%03d", mLineMonitor->rgbTobright());          // 光センサ値を表示
//    ev3_lcd_draw_string( buf, 0, 90);
//
//    if (FinishFlag == true) {
//
//    	mBackState = BT_Deley;
//    	mBackInitFlag = false;
//    	mFowInitFlag = false;
//
//        /* 左右輪に出力 */
////    	mRightWheel.setPWM(0);
////    	mLeftWheel.setPWM(0);
//    }

    return(FinishFlag);

}

void DriveController::SetColorCount(colorid_t color)
{

    switch (color) {
        case colorid_t::COLOR_BLUE:
//        	mSound->punch();
            ev3_led_set_color(LED_ORANGE);
        	Bcount++;
        	Gcount = 0;
        	Ycount = 0;
        	Rcount = 0;
        	Wcount = 0;
            break;
        case colorid_t::COLOR_GREEN:
//        	mSound->dryy();
            ev3_led_set_color(LED_GREEN);
        	Bcount = 0;
        	Gcount++;
        	Ycount = 0;
        	Rcount = 0;
        	Wcount = 0;
            break;
        case colorid_t::COLOR_YELLOW:
//        	mSound->kinoko();
        	LEDColor::getInstance()->LEDYellow(); /* デバッグ用に光らせる */
        	Bcount = 0;
        	Gcount = 0;
        	Ycount++;
        	Rcount = 0;
        	Wcount = 0;
            break;
        case colorid_t::COLOR_RED:
//        	mSound->nya();
            ev3_led_set_color(LED_RED);
        	Bcount = 0;
        	Gcount = 0;
        	Ycount = 0;
        	Rcount++;
        	Wcount = 0;
            break;
        case colorid_t::COLOR_BROWN:
//        	mSound->pon();
        	ev3_led_set_color(LED_OFF);
        	Bcount = 0;
        	Gcount = 0;
        	Ycount = 0;
        	Rcount = 0;
        	Wcount++;
            break;

        default:
        	ev3_led_set_color(LED_OFF);
        	Bcount = 0;
        	Gcount = 0;
        	Ycount = 0;
        	Rcount = 0;
        	Wcount = 0;
            break;
    }

	colorPre = colorNum;
	colorNum = color;

}

void DriveController::SetColor()
{

	/* カラーセンサ値を更新 */
	mLineMonitor->setColor();

	/* 色判定 */
    colorid_t color = mLineMonitor->JdgColorType();

    /* カラーカウントを更新*/
    SetColorCount(color);

}

void DriveController::SetPWMZero(void)
{

    mRightWheel.setPWM(0);
    mLeftWheel.setPWM(0);

}
