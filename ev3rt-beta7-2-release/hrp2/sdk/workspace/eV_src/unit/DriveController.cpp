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

bool DriveController::TurnL(void)
{

	bool FinishFlag = false;
	/* 初回には目標Θをセットする */
	if(mLftInitFlag == false) {
		mLftTargetTheta = mOdmetry->getTheta();
		mLftTargetTheta += QTURN;
		mLftInitFlag = true;
	}

	mLftTheta = mOdmetry->getTheta();

    if (mLftTheta < mLftTargetTheta) {
    	/* 目標Θ未達の場合はモータに回転の指示 */
        /* 左右輪に出力 */
    	mRightWheel.setPWM(TURNSPEED);
    	mLeftWheel.setPWM(-TURNSPEED);

    	FinishFlag = false;
    } else {
    	/* 目標Θ達成の場合は完了フラグをtrueにする */
    	mRightWheel.setPWM(0);
    	mLeftWheel.setPWM(0);
    	FinishFlag = true;
    	mLftInitFlag = false;
    }

    return (FinishFlag);
}

bool DriveController::TurnR(void)
{

	bool FinishFlag = false;
	/* 初回には目標Θをセットする */
	if(mRgtInitFlag == false) {
		mRgtTargetTheta = mOdmetry->getTheta();
		mRgtTargetTheta -= QTURN;
		mRgtInitFlag = true;
	}

	mRgtTheta = mOdmetry->getTheta();

    if (mRgtTheta > mRgtTargetTheta) {
    	/* 目標Θ未達の場合はモータに回転の指示 */
        /* 左右輪に出力 */
    	mRightWheel.setPWM(-TURNSPEED);
    	mLeftWheel.setPWM(TURNSPEED);

    	FinishFlag = false;
    } else {
    	/* 目標Θ達成の場合は完了フラグをtrueにする */
    	mRightWheel.setPWM(0);
    	mLeftWheel.setPWM(0);
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

bool DriveController::SetPWMForward(float targetdistance, float R_PWM, float L_PWM)
{

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
    	/* 目標Θ未達の場合はモータに回転の指示 */
        /* 左右輪に出力 */
    	mRightWheel.setPWM(R_PWM);
    	mLeftWheel.setPWM(L_PWM);

    	FinishFlag = false;
    } else {
    	/* 目標Θ達成の場合は完了フラグをtrueにする */
    	mRightWheel.setPWM(0);
    	mLeftWheel.setPWM(0);
    	FinishFlag = true;
    	mFowInitFlag = false;
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

    if (Distance > CIRCLEDISTANCE) {
    	FinishFlag = true;
    }

    FinishFlag = UpdateColorState(TM_Forward, 0.0);


    if (FinishFlag == true) {

    	mThrInitFlag = false;
    } else {

    }

    return (FinishFlag);
}

bool DriveController::RgtTurnCircle(void)
{

	bool FinishFlag = false;

    FinishFlag = UpdateColorState(TM_RgtTurn, RGTTURNCIRCLEDISTANCE);

    return (FinishFlag);
}

bool DriveController::LftTurnCircle(void)
{

	bool FinishFlag = false;

    FinishFlag = UpdateColorState(TM_LftTurn, LFTTURNCIRCLEDISTANCE);

    return (FinishFlag);
}

bool DriveController::UTurnCircle(void)
{

	bool FinishFlag = false;

    FinishFlag = UpdateColorState(TM_UTurn, UTURNCIRCLEDISTANCE);

    return (FinishFlag);
}

bool DriveController::UpdateColorState(TurnMode turnMode, float MedWhiteDistance)
{

	bool FinishFlag    = false;
	bool FowardFinFlag = false;
	bool TrunFinFlag   = false;
	bool UTTurnFlag    = false;
	bool UTForFlag     = false;

    colorid_t color = mLineMonitor->JdgColorType();

    /* カラーカウントを更新*/
    SetColorCount(color);

    switch (mCircleState) {
    case CS_FirstLine: /* 黒ラインに乗っている状態 */

    	/* 色サークルに乗っているか判定する */
        if ((COUNTERNUMTHRESH <= Bcount) || (COUNTERNUMTHRESH <= Gcount) || (COUNTERNUMTHRESH <= Ycount) || (COUNTERNUMTHRESH <= Rcount)) {
            ev3_led_set_color(LED_RED);
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
            ev3_led_set_color(LED_OFF);
            if (turnMode == TM_Forward) {
            	mCircleState = CS_SecondWhite;
            } else {
            	mCircleState = CS_FirstWhite;
            }
        }

        /* 左右輪に出力 */
    	mRightWheel.setPWM(FORWARDSPEED);
    	mLeftWheel.setPWM(FORWARDSPEED);

        break;
    case CS_FirstWhite: /* 白い円に入った状態で一定距離前進 */


    	FowardFinFlag = Forward(MedWhiteDistance);

    	if (FowardFinFlag == true) {
    		mCircleState = CS_Turn;
    	}

        break;
    case CS_Turn: /* 白い円で旋回する状態 */

    	if (turnMode == TM_RgtTurn) {
    		TrunFinFlag = TurnR();
    	} else if (turnMode == TM_LftTurn){
    		TrunFinFlag = TurnL();
    	} else {

    		/* Uターンする場合 */
    		/* 右旋回 → 直進(トレースエッジ変更) →右旋回の順番で処理を実施 */

    	    switch (mUTurnState) {
    	    case US_FirstTurn: /* 最初の旋回 */

    	    	UTTurnFlag = TurnR();

    	    	if (UTTurnFlag == true) {
    	    		mUTurnState = US_Forward;
    	    	}

    	    	break;
    	    case US_Forward: /* トレースエッジ分の移動 */

    	    	UTForFlag = Forward(EDGEDISTANCE);

    	    	if (UTForFlag == true) {
    	    		mUTurnState = US_SecondTurn;
    	    	}

    	    	break;
    	    case US_SecondTurn: /* 2回目の旋回 */

    	    	UTTurnFlag = TurnR();

    	    	if (UTTurnFlag == true) {
    	    		mUTurnState = US_FirstTurn;
    	    		TrunFinFlag = true;
    	    	}

    	    	break;
    	    default:
    	        break;
    	    }
    	}

    	if (TrunFinFlag == true) {
    		mCircleState = CS_SecondWhite;
    	}

        break;
    case CS_SecondWhite: /* 白い円に入った状態 */

    	/* 再び色サークルに乗っているか判定する */
        if ((COUNTERNUMTHRESH <= Bcount) || (COUNTERNUMTHRESH <= Gcount) || (COUNTERNUMTHRESH <= Ycount) || (COUNTERNUMTHRESH <= Rcount)) {
            ev3_led_set_color(LED_RED);
            mCircleState = CS_SecondColor;
        }

        /* 左右輪に出力 */
    	mRightWheel.setPWM(FORWARDSPEED);
    	mLeftWheel.setPWM(FORWARDSPEED);

        break;
    case CS_SecondColor:

    	/* 黒ラインに乗ったか判定する */
        if (COUNTERNUMTHRESH <= Wcount) {
            ev3_led_set_color(LED_GREEN);
            mCircleState = CS_SecondLine;
        	mRightWheel.setPWM(0);
        	mLeftWheel.setPWM(0);
        } else {

            /* 左右輪に出力 */
        	mRightWheel.setPWM(FORWARDSPEED);
        	mLeftWheel.setPWM(FORWARDSPEED);
        }


    	break;
    case CS_SecondLine: /* サークルを超えてラインに到達。次回のために初期化処理を行う */

        /* 次のためにfirstlineに戻す */
        mCircleState = CS_FirstLine;
    	mRightWheel.setPWM(0);
    	mLeftWheel.setPWM(0);
    	FinishFlag = true;
    	break;
    default:
        break;
    }

    return(FinishFlag);

}

bool DriveController::SetToBin(void)
{

	bool FinishFlag = false;

    FinishFlag = UpdateSetState(SETDISTANCE);

    return (FinishFlag);
}

bool DriveController::UpdateSetState(float setDistance)
{

	bool FinishFlag = false;
	bool FowardFinFlag = false;

    colorid_t color = mLineMonitor->JdgColorType();

    if (mBackInitFlag == false) {
    	mBackTargetBri = mLineMonitor->rgbTobright();
    	mBackInitFlag = true;
    }

    /* カラーカウントを更新*/
    SetColorCount(color);

   /* 色サークルに乗っているか判定する */
   if ((COUNTERNUMTHRESH <= Bcount) || (COUNTERNUMTHRESH <= Gcount) || (COUNTERNUMTHRESH <= Ycount) || (COUNTERNUMTHRESH <= Rcount)) {
       ev3_led_set_color(LED_RED);
       FinishFlag = true;
   }

   FowardFinFlag = Forward(setDistance);

   if (FowardFinFlag == true) {
	   FinishFlag = true;
   }

   if (FinishFlag == true) {
       /* 左右輪に出力 */
	   mRightWheel.setPWM(0);
	   mLeftWheel.setPWM(0);
	   FinishFlag = true;
   }

    return(FinishFlag);
}

bool DriveController::BackSallow(void)
{

	bool FinishFlag = false;

    FinishFlag = BackToLine(BM_Shallow, BACKSHALLOWDISTANCE);

    return (FinishFlag);
}

bool DriveController::BackDeep(void)
{

	bool FinishFlag = false;

    FinishFlag = BackToLine(BM_Deep, BACKDEEPDISTANCE);

    return (FinishFlag);
}

bool DriveController::BackToLine(BackMode backMode, float setDistance)
{

	bool FinishFlag = false;

    switch (mBackState) {
    case BT_Noline: /* ラインに乗る前 */

    	Backward(setDistance);

    	if (mLineMonitor->rgbTobright() < (mBackTargetBri + BRITHEOFF)) {

    		if (backMode == BM_Deep) {
    			mBackState = BT_Line;
    		} else {
    			FinishFlag = true;
    		}
    	}

        break;
    case BT_Line: /* トレースするエッジに移動 */

    	Backward(setDistance);

    	if (mLineMonitor->rgbTobright() > mBackTargetBri) {
    		FinishFlag = true;
    	}

        break;
    default:
        break;
    }

    if (FinishFlag == true) {

    	mBackState = BT_Noline;
    	mBackInitFlag = false;
    	mFowInitFlag = false;

        /* 左右輪に出力 */
    	mRightWheel.setPWM(0);
    	mLeftWheel.setPWM(0);
    }

    return(FinishFlag);

}

void DriveController::SetColorCount(colorid_t color)
{

    switch (color) {
        case colorid_t::COLOR_BLUE:
        	mSound->punch();
        	Bcount++;
        	Gcount = 0;
        	Ycount = 0;
        	Rcount = 0;
        	Wcount = 0;
            break;
        case colorid_t::COLOR_GREEN:
        	mSound->dryy();
        	Bcount = 0;
        	Gcount++;
        	Ycount = 0;
        	Rcount = 0;
        	Wcount = 0;
            break;
        case colorid_t::COLOR_YELLOW:
        	mSound->kinoko();
        	Bcount = 0;
        	Gcount = 0;
        	Ycount++;
        	Rcount = 0;
        	Wcount = 0;
            break;
        case colorid_t::COLOR_RED:
        	mSound->nya();
        	Bcount = 0;
        	Gcount = 0;
        	Ycount = 0;
        	Rcount++;
        	Wcount = 0;
            break;
        case colorid_t::COLOR_BROWN:
        	mSound->pon();
        	Bcount = 0;
        	Gcount = 0;
        	Ycount = 0;
        	Rcount = 0;
        	Wcount++;
            break;

        default:
        	Bcount = 0;
        	Gcount = 0;
        	Ycount = 0;
        	Rcount = 0;
        	Wcount = 0;
            break;
    }

}
