/******************************************************************************
 *  Calibration.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/06/12
 *  Implementation of the Class BalancingWalker
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "Calibration.h"


// 定数宣言
const int Calibration::NUM_OF_GYRO_CALIBRATION =  30;
const int Calibration::NUM_OF_BLACK_CALIBRATION = 30;
const int Calibration::NUM_OF_WHITE_CALIBRATION = 30;

/**
 * コンストラクタ
 * @param colorSensor カラーセンサ
 * @param gyroSensor  ジャイロセンサ
 * @param lineMonitor ラインモニタ
 */
Calibration::Calibration(const ev3api::ColorSensor& colorSensor,
                  ev3api::GyroSensor& gyroSensor,
                  LineMonitor* lineMonitor,
				  const Button* Button)
    : mColorSensor(colorSensor),
      mGyroSensor(gyroSensor),
      mLineMonitor(lineMonitor),
	  mButton(Button),
      mIsStartedGyro(false),
      mIsStartedBlack(false),
      mIsStartedWhite(false),
	  CalibrationCompFlag(false),
      mCalCount(0),
	  mCalibrationState(UNDEFINED),
      mSum(0) {
}

/**
 * デストラクタ
 */
Calibration::~Calibration() {
}


/**
 * キャリブレーションに必要なものをリセットする
 */
void Calibration::init()
{

    mGyroSensor.setOffset(0);                           // ジャイロセンサオフセット初期化
}

/**
 * キャリブレーションの実行
 */
bool Calibration::RunCalibration()
{

    char buf[256];

    mColorSensor.getRawColor(mRGB);
    mHSV = RGBtoHSV(mRGB);
    mbright = mColorSensor.getBrightness();
	ev3_lcd_set_font(EV3_FONT_SMALL);
    sprintf( buf, "l = %03d", mbright);          // 光センサ値を表示
    ev3_lcd_draw_string( buf, 0, 90);
    sprintf( buf, "r = %03d", mRGB.r);          // 光センサ値を表示
    ev3_lcd_draw_string( buf, 0, 100);
    sprintf( buf, "g = %03d", mRGB.g);          // 光センサ値を表示
    ev3_lcd_draw_string( buf, 0, 110);
    sprintf( buf, "b = %03d", mRGB.b);          // 光センサ値を表示
    ev3_lcd_draw_string( buf, 0, 120);

    sprintf( buf, "h = %03d", mHSV.h);          // 光センサ値を表示
    ev3_lcd_draw_string( buf, 100, 100);
    sprintf( buf, "s = %03d", mHSV.s);          // 光センサ値を表示
    ev3_lcd_draw_string( buf, 100, 110);
    sprintf( buf, "v = %03d", mHSV.v);          // 光センサ値を表示
    ev3_lcd_draw_string( buf, 100, 120);

    switch (mCalibrationState) {
    case UNDEFINED:
    	mCalibrationState = CALIBRATION_ARM;
        CalibrationCompFlag = false;
        break;
    case CALIBRATION_ARM:
        execCalibrationArm();
    	ev3_lcd_set_font(EV3_FONT_MEDIUM);
    	ev3_lcd_draw_string(" ARM   ", 100, 50);
        CalibrationCompFlag = false;
        break;
//    case CALIBRATION_GYRO:
//        execCalibrationGyro();
//        CalibrationCompFlag = false;
//        break;
    case CALIBRATION_BLACK:
    	ev3_lcd_set_font(EV3_FONT_MEDIUM);
    	ev3_lcd_draw_string(" BLACK ", 90, 50);
        execCalibrationBlack();
        CalibrationCompFlag = false;
        break;
    case CALIBRATION_WHITE:
    	ev3_lcd_set_font(EV3_FONT_MEDIUM);
    	ev3_lcd_draw_string(" WHITE ", 90, 50);
        execCalibrationWhite();
        CalibrationCompFlag = false;
        break;
    case CALIBRATION_BLUE:
    	ev3_lcd_set_font(EV3_FONT_MEDIUM);
    	ev3_lcd_draw_string(" BLUE  ", 90, 50);
    	execCalibrationBlue();
        CalibrationCompFlag = false;
        break;
    case CALIBRATION_RED:
    	ev3_lcd_set_font(EV3_FONT_MEDIUM);
    	ev3_lcd_draw_string(" RED   ", 90, 50);
    	execCalibrationRed();
        CalibrationCompFlag = false;
        break;
    case CALIBRATION_GREEN:
    	ev3_lcd_set_font(EV3_FONT_MEDIUM);
    	ev3_lcd_draw_string(" GREEN ", 90, 50);
    	execCalibrationGreen();
        CalibrationCompFlag = false;
        break;
    case CALIBRATION_YELLOW:
    	ev3_lcd_set_font(EV3_FONT_MEDIUM);
    	ev3_lcd_draw_string(" YELLOW", 90, 50);
    	execCalibrationYellow();
        CalibrationCompFlag = false;
        break;
    case CALIBRATION_SET_THRESHOLD:
    	ev3_lcd_set_font(EV3_FONT_MEDIUM);
    	ev3_lcd_draw_string("COMPLITE", 90, 50);
    	calibrateLineThreshold();
    	CalibrationCompFlag = true;
        break;
    case CALIBRATION_COMPLITE:
    	CalibrationCompFlag = true;
        break;

    default:
        break;
    }

	return CalibrationCompFlag;
}

/**
 * アームモータのキャリブレーション
 */
void Calibration::execCalibrationArm()
{
    if (calibrateArm(mButton->isPushed()) == true)
    {
#if !RUN_COLOR_CALIBRATION
	mCalibrationState = CALIBRATION_COMPLITE;
	mSound->trumpet();
#else
	mCalibrationState = CALIBRATION_BLACK;
    mSound->ok();
#endif
    }
}

/**
 * ジャイロセンサのキャリブレーション
 */
//void Calibration::execCalibrationGyro()
//{
//    if (calibrateGyro(mButton->isPushed()) == true)
//    {
//    	mCalibrationState = CALIBRATION_BLACK;
//        mSound->ok();
//    }
//#if !RUN_COLOR_CALIBRATION
//	mCalibrationState = CALIBRATION_COMPLITE;
//	mSound->trumpet();
//#endif
//}

/**
 * 黒キャリブレーション
 */
void Calibration::execCalibrationBlack()
{
    if (calibrateBlack(mButton->isPushed()) == true)
    {
    	mCalibrationState = CALIBRATION_WHITE;
        mSound->ok();
    }

}

/**
 * 白キャリブレーション
 */
void Calibration::execCalibrationWhite()
{
    if (calibrateWhite(mButton->isPushed()) == true)
    {
    	mCalibrationState = CALIBRATION_BLUE;
        mSound->ok();
    }

}

/**
 * 青キャリブレーション
 */
void Calibration::execCalibrationBlue()
{
    if (calibrateBlue(mButton->isPushed()) == true)
    {
    	mCalibrationState = CALIBRATION_RED;
        mSound->ok();
    }

}

/**
 * 赤キャリブレーション
 */
void Calibration::execCalibrationRed()
{
    if (calibrateRed(mButton->isPushed()) == true)
    {
    	mCalibrationState = CALIBRATION_GREEN;
        mSound->ok();
    }

}
/**
 * 緑キャリブレーション
 */
void Calibration::execCalibrationGreen()
{
    if (calibrateGreen(mButton->isPushed()) == true)
    {
    	mCalibrationState = CALIBRATION_YELLOW;
        mSound->ok();
    }

}

/**
 * 黄色キャリブレーション
 */
void Calibration::execCalibrationYellow()
{
    if (calibrateYellow(mButton->isPushed()) == true)
    {
    	mCalibrationState = CALIBRATION_SET_THRESHOLD;
        mSound->trumpet();
    }

}
/**
 * ライントレースの閾値をキャリブレーション
 */
void Calibration::execCalibrateLineThreshold()
{

	calibrateLineThreshold();
	mCalibrationState = CALIBRATION_COMPLITE;

}

/**
 * アームモータのキャリブレーションする
 * ＜戻り値＞
 *    false: キャリブレーション未完了
 *    true : キャリブレーション完了
 */
bool Calibration::calibrateArm(bool startTrigger)
{

    bool finish;
    finish = false;

    if(mbright > maxBrightness) {
    	maxBrightness = mbright;
    }

    if(mbright < preBrightness) {
    	decCount++;

        if(decCount >= 2) {
        	maxflag = true;
        	decBrightness = mbright;
        }

    } else if (mbright > preBrightness){
    	decCount = 0;
    }

    if (maxflag == true) {
//    	if (sensor < maxBrightness) {
//    		mArm->setPWM(0);
//    	} else {
//    		mArm->setPWM(0);
//    	}
    	mArm->setPWM(0);
    } else {
    	mArm->setPWM(4);
    }

    preBrightness = mbright;

    if (startTrigger == true) {
    	mArm->init(0);
    	mArm->setAngle(0);
    	mIsStartedArm =true;
    }

    if (mIsStartedArm == true) {
        mCalCount++;

        if (mCalCount > 20) {
        	finish = true;
        	mCalCount = 0;
        	mIsStartedArm =false;
        }
    }

    return finish;
}

/**
 * ジャイロセンサのオフセット値をキャリブレーションする
 * ＜戻り値＞
 *    false: キャリブレーション未完了
 *    true : キャリブレーション完了
 */
//bool Calibration::calibrateGyro(bool startTrigger)
//{
//
//    int16_t  sensor;
//    int16_t  cal;
//    bool finish;
//    char buf[256];
//
//    finish = false;
//    sensor = mGyroSensor.getAnglerVelocity();
//
//    if (mIsStartedGyro == false)
//    {
//        sprintf( buf, "gyro = %03d", sensor);           // ジャイロセンサ値を表示
//        ev3_lcd_draw_string( buf, 0, 0);
//
//        if (startTrigger == true) {
//
//            mIsStartedGyro = true;
//            mSum = 0;
//            mCalCount = 0;
//        }
//    }
//    else
//    {
//
//        mSum += sensor;                               // ジャイロセンサ値を積算
//        mCalCount++;
//
//        if (mCalCount == NUM_OF_GYRO_CALIBRATION) 		// 規定回数以上積算
//        {
//            cal = mSum / NUM_OF_GYRO_CALIBRATION;       // 平均値 ★キャリブレーション演習修正箇所
//            mGyroSensor.setOffset(cal);
//
////            sprintf( buf, "gyroOffset = %03d", cal);    // ジャイロオフセット値を表示
////            ev3_lcd_draw_string( buf, 0, 50);
//
//            finish = true;                              // 次へ
//        }
//    }
//    return finish;
//}

/**
 * 黒色の閾値をキャリブレーションする
 * ＜戻り値＞
 *    false: キャリブレーション未完了
 *    true : キャリブレーション完了
 */
bool Calibration::calibrateBlack(bool startTrigger)
{

    int16_t  cal;
    bool finish;
    char buf[256];
    finish = false;

	ev3_lcd_set_font(EV3_FONT_SMALL);
    if (mIsStartedBlack == false)
    {

        sprintf( buf, "black    =");       // 黒しきい値を表示
        ev3_lcd_draw_string( buf, 0, 0);

        if (startTrigger == true)
        {

            mIsStartedBlack = true;
            mSum = 0;
            mSumRGB = 0;
            mCalCount = 0;
        }
    }
    else
    {
        mSum += mbright;                                 // 光センサ値を積算
        mSumRGB += ((mRGB.r + mRGB.g + mRGB.b) / 3);
        mCalCount++;

        if (mCalCount == NUM_OF_BLACK_CALIBRATION) 		// 規定回数以上積算
        {
            cal = mSum / NUM_OF_BLACK_CALIBRATION;                                  // 平均値
            mLineMonitor->setBlackThreshold(cal);

            sprintf( buf, "black    = %03d", cal);       // 黒しきい値を表示
            ev3_lcd_draw_string( buf, 0, 0);

            cal = mSumRGB / NUM_OF_BLACK_CALIBRATION;                                // 平均値
            mLineMonitor->mBlockBlackThresh = cal;

            sprintf( buf, "RGBblack = %03d", cal);       // 黒しきい値を表示
            ev3_lcd_draw_string( buf, 0, 10);

            finish = true;                              // 次へ
        }
    }
    return finish;
}

/**
 * 白色の閾値をキャリブレーションする
 * ＜戻り値＞
 *    false: キャリブレーション未完了
 *    true : キャリブレーション完了
 */
bool Calibration::calibrateWhite(bool startTrigger)
{

    int16_t  cal;
    bool finish;
    char buf[256];
    finish = false;

	ev3_lcd_set_font(EV3_FONT_SMALL);
    if (mIsStartedWhite == false)
    {

        sprintf( buf, "white    =");    // 白しきい値を表示
        ev3_lcd_draw_string( buf, 0, 20);

        if (startTrigger == true)
        {

            mIsStartedWhite = true;
            mSum = 0;
            mSumRGB = 0;
            mCalCount = 0;
        }
    }
    else
    {
        mSum += mbright;                                 // 光センサ値を積算
        mSumRGB += ((mRGB.r + mRGB.g + mRGB.b) / 3);
        mCalCount++;

        if (mCalCount == NUM_OF_WHITE_CALIBRATION) // 規定回数以上積算
        {
            cal = mSum / NUM_OF_WHITE_CALIBRATION;                                  // 平均値
            mLineMonitor->setWhiteThreshold(cal);

            sprintf( buf, "white    = %03d", cal);    // 白しきい値を表示
            ev3_lcd_draw_string( buf, 0, 20);

            cal = mSumRGB / NUM_OF_WHITE_CALIBRATION;                                // 平均値
            mLineMonitor->mBlockWhiteThresh = cal;

            sprintf( buf, "RGBwhite = %03d", cal);    // 白しきい値を表示
            ev3_lcd_draw_string( buf, 0, 30);

            finish = true;                              // 次へ
        }
    }
    return finish;
}

/**
 * 青色の閾値をキャリブレーションする
 * ＜戻り値＞
 *    false: キャリブレーション未完了
 *    true : キャリブレーション完了
 */
bool Calibration::calibrateBlue(bool startTrigger)
{

    Hsv  cal;
    bool finish;
    char buf[256];
    finish = false;

	ev3_lcd_set_font(EV3_FONT_SMALL);
    if (mIsStartedBlue == false)
    {

        sprintf( buf, "Blue     =");       // 白しきい値を表示
        ev3_lcd_draw_string( buf, 0, 40);

        if (startTrigger == true)
        {

        	mIsStartedBlue = true;
        	mHSVSum.h = 0;
        	mHSVSum.s = 0;
        	mHSVSum.v = 0;
            mCalCount = 0;
        }
    }
    else
    {
    	mHSVSum.h += mHSV.h;                                 // 光センサ値を積算
    	mHSVSum.s += mHSV.s;                                 // 光センサ値を積算
    	mHSVSum.v += mHSV.v;                                 // 光センサ値を積算
        mCalCount++;

        if (mCalCount == NUM_OF_WHITE_CALIBRATION) // 規定回数以上積算
        {
            cal.h = mHSVSum.h / NUM_OF_WHITE_CALIBRATION;                                  // 平均値
            cal.s = mHSVSum.s / NUM_OF_WHITE_CALIBRATION;                                  // 平均値
            cal.v = mHSVSum.v / NUM_OF_WHITE_CALIBRATION;                                  // 平均値
            mLineMonitor->setBlueThreshold(cal);

            sprintf( buf, "Blue_h   = %03d", cal.h);       // 白しきい値を表示
            ev3_lcd_draw_string( buf, 0, 40);

            finish = true;                              // 次へ
        }
    }
    return finish;
}

/**
 * 赤色の閾値をキャリブレーションする
 * ＜戻り値＞
 *    false: キャリブレーション未完了
 *    true : キャリブレーション完了
 */
bool Calibration::calibrateRed(bool startTrigger)
{

    Hsv  cal;
    bool finish;
    char buf[256];
    finish = false;

	ev3_lcd_set_font(EV3_FONT_SMALL);
    if (mIsStartedRed == false)
    {

        sprintf( buf, "Red      =");       // 白しきい値を表示
        ev3_lcd_draw_string( buf, 0, 50);

        if (startTrigger == true)
        {

        	mIsStartedRed = true;
        	mHSVSum.h = 0;
        	mHSVSum.s = 0;
        	mHSVSum.v = 0;
            mCalCount = 0;
        }
    }
    else
    {
    	mHSVSum.h += mHSV.h;                                 // 光センサ値を積算
    	mHSVSum.s += mHSV.s;                                 // 光センサ値を積算
    	mHSVSum.v += mHSV.v;                                 // 光センサ値を積算
        mCalCount++;

        if (mCalCount == NUM_OF_WHITE_CALIBRATION) // 規定回数以上積算
        {
            cal.h = mHSVSum.h / NUM_OF_WHITE_CALIBRATION;                                  // 平均値
            cal.s = mHSVSum.s / NUM_OF_WHITE_CALIBRATION;                                  // 平均値
            cal.v = mHSVSum.v / NUM_OF_WHITE_CALIBRATION;                                  // 平均値
            mLineMonitor->setRedThreshold(cal);

            sprintf( buf, "Red_h    = %03d", cal.h);       // 白しきい値を表示
            ev3_lcd_draw_string( buf, 0, 50);

            finish = true;                              // 次へ
        }
    }
    return finish;
}

/**
 * 緑色の閾値をキャリブレーションする
 * ＜戻り値＞
 *    false: キャリブレーション未完了
 *    true : キャリブレーション完了
 */
bool Calibration::calibrateGreen(bool startTrigger)
{

    Hsv  cal;
    bool finish;
    char buf[256];
    finish = false;

	ev3_lcd_set_font(EV3_FONT_SMALL);
    if (mIsStartedGreen == false)
    {

        sprintf( buf, "green    =");       // 白しきい値を表示
        ev3_lcd_draw_string( buf, 0, 60);

        if (startTrigger == true)
        {

        	mIsStartedGreen = true;
        	mHSVSum.h = 0;
        	mHSVSum.s = 0;
        	mHSVSum.v = 0;
            mCalCount = 0;
        }
    }
    else
    {
    	mHSVSum.h += mHSV.h;                                 // 光センサ値を積算
    	mHSVSum.s += mHSV.s;                                 // 光センサ値を積算
    	mHSVSum.v += mHSV.v;                                 // 光センサ値を積算
        mCalCount++;

        if (mCalCount == NUM_OF_WHITE_CALIBRATION) // 規定回数以上積算
        {
            cal.h = mHSVSum.h / NUM_OF_WHITE_CALIBRATION;                                  // 平均値
            cal.s = mHSVSum.s / NUM_OF_WHITE_CALIBRATION;                                  // 平均値
            cal.v = mHSVSum.v / NUM_OF_WHITE_CALIBRATION;                                  // 平均値
            mLineMonitor->setGreenThreshold(cal);

            sprintf( buf, "green_h  = %03d", cal.h);       // 白しきい値を表示
            ev3_lcd_draw_string( buf, 0, 60);

            finish = true;                              // 次へ
        }
    }
    return finish;
}

/**
 * 黄色の閾値をキャリブレーションする
 * ＜戻り値＞
 *    false: キャリブレーション未完了
 *    true : キャリブレーション完了
 */
bool Calibration::calibrateYellow(bool startTrigger)
{

    Hsv  cal;
    bool finish;
    char buf[256];
    finish = false;
	ev3_lcd_set_font(EV3_FONT_SMALL);
    if (mIsStartedYellow == false)
    {

        sprintf( buf, "Yellow   =");       // 白しきい値を表示
        ev3_lcd_draw_string( buf, 0, 70);

        if (startTrigger == true)
        {

        	mIsStartedYellow = true;
        	mHSVSum.h = 0;
        	mHSVSum.s = 0;
        	mHSVSum.v = 0;
            mCalCount = 0;
        }
    }
    else
    {
    	mHSVSum.h += mHSV.h;                                 // 光センサ値を積算
    	mHSVSum.s += mHSV.s;                                 // 光センサ値を積算
    	mHSVSum.v += mHSV.v;                                 // 光センサ値を積算
        mCalCount++;

        if (mCalCount == NUM_OF_WHITE_CALIBRATION) // 規定回数以上積算
        {
            cal.h = mHSVSum.h / NUM_OF_WHITE_CALIBRATION;                                  // 平均値
            cal.s = mHSVSum.s / NUM_OF_WHITE_CALIBRATION;                                  // 平均値
            cal.v = mHSVSum.v / NUM_OF_WHITE_CALIBRATION;                                  // 平均値
            mLineMonitor->setYellowThreshold(cal);

            sprintf( buf, "Yellow_h = %03d", cal.h);       // 白しきい値を表示
            ev3_lcd_draw_string( buf, 0, 70);

            finish = true;                              // 次へ
        }
    }
    return finish;
}

/**
 * ライントレースしきい値を設定
 */
void Calibration::calibrateLineThreshold()
{
    mLineMonitor->calLineThreshold();
    mLineMonitor->mBlockLineThreshold = (mLineMonitor->mBlockBlackThresh + mLineMonitor->mBlockWhiteThresh) / 2;
}
