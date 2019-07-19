/******************************************************************************
 *  Calibration.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/06/12
 *  Implementation of the Class BalancingWalker
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "Calibration.h"


// 定数宣言
const int Calibration::NUM_OF_GYRO_CALIBRATION =  100; // ★キャリブレーション演習修正箇所
const int Calibration::NUM_OF_BLACK_CALIBRATION = 100; // ★キャリブレーション演習修正箇所
const int Calibration::NUM_OF_WHITE_CALIBRATION = 100; // ★キャリブレーション演習修正箇所

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
	  mCalibrationState(CALIBRATION_GYRO),
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

    switch (mCalibrationState) {
    case UNDEFINED:
    	mCalibrationState = CALIBRATION_GYRO;
        CalibrationCompFlag = false;
        break;
    case CALIBRATION_GYRO:
        execCalibrationGyro();
        CalibrationCompFlag = false;
        break;
    case CALIBRATION_BLACK:
        execCalibrationBlack();
        CalibrationCompFlag = false;
        break;
    case CALIBRATION_WHITE:
        execCalibrationWhite();
        CalibrationCompFlag = false;
        break;
    case CALIBRATION_BLUE:
    	execCalibrationBlue();
        CalibrationCompFlag = false;
        break;
    case CALIBRATION_RED:
    	execCalibrationRed();
        CalibrationCompFlag = false;
        break;
    case CALIBRATION_GREEN:
    	execCalibrationGreen();
        CalibrationCompFlag = false;
        break;
    case CALIBRATION_YELLOW:
    	execCalibrationYellow();
        CalibrationCompFlag = false;
        break;
    case CALIBRATION_SET_THRESHOLD:
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

/* テール、アームのキャリブレーションは別途検討 */
//void Calibration::execCalibrationGyroAndTail() {
//
//    if (isGyroInit == false && calibrateGyro(mButton->isPushed()) == true) {
//        isGyroInit = true;
//    }
//
//    if(mButton->isPushed() == true) {
//		mTailMotor->setPWM(0);
//		mTailMotor->init(0);
//		TailInit = true;
//	}
//
//	if(TailInit == false) {
//		mTailMotor->setPWM(-2);
//	}
//
//	if(TailInit == true) {
//		TimeCount++;
//	}
//
//	if(isTailInit == false && TimeCount > 100) {
//		mTailMotor->setAngle(92);	// 開始待ち時尻尾91°
//        isTailInit = true;
//	}
//
//    if(isTailInit && isGyroInit) {
//        mSound->ok();
//        mCalibrationState = CALIBRATION_BLACK;
//    }
//}


/**
 * ジャイロセンサのキャリブレーション
 */
void Calibration::execCalibrationGyro()
{
    if (calibrateGyro(mButton->isPushed()) == true)
    {
    	mCalibrationState = CALIBRATION_BLACK;
        mSound->ok();
    }
#if !RUN_COLOR_CALIBRATION
	mCalibrationState = CALIBRATION_COMPLITE;
	mSound->trumpet();
#endif
}

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
    	mCalibrationState = CALIBRATION_COMPLITE;
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
 * ジャイロセンサのオフセット値をキャリブレーションする
 * ＜戻り値＞
 *    false: キャリブレーション未完了
 *    true : キャリブレーション完了
 */
bool Calibration::calibrateGyro(bool startTrigger)
{

    int16_t  sensor;
    int16_t  cal;
    bool finish;
    char buf[256];

    finish = false;
    sensor = mGyroSensor.getAnglerVelocity();

    if (mIsStartedGyro == false)
    {
        sprintf( buf, "gyro = %03d", sensor);           // ジャイロセンサ値を表示
        ev3_lcd_draw_string( buf, 0, 0);

        if (startTrigger == true) {

            mIsStartedGyro = true;
            mSum = 0;
            mCalCount = 0;
        }
    }
    else
    {

        mSum += sensor;                               // ジャイロセンサ値を積算
        mCalCount++;

        if (mCalCount == NUM_OF_GYRO_CALIBRATION) 		// 規定回数以上積算
        {
            cal = mSum / NUM_OF_GYRO_CALIBRATION;       // 平均値 ★キャリブレーション演習修正箇所
            mGyroSensor.setOffset(cal);

//            sprintf( buf, "gyroOffset = %03d", cal);    // ジャイロオフセット値を表示
//            ev3_lcd_draw_string( buf, 0, 50);

            finish = true;                              // 次へ
        }
    }
    return finish;
}

/**
 * 黒色の閾値をキャリブレーションする
 * ＜戻り値＞
 *    false: キャリブレーション未完了
 *    true : キャリブレーション完了
 */
bool Calibration::calibrateBlack(bool startTrigger)
{

    int8_t  sensor;
    int16_t  cal;
    bool finish;
    char buf[256];

    finish = false;
    sensor = mColorSensor.getBrightness();

    if (mIsStartedBlack == false)
    {
        sprintf( buf, "black = %03d", sensor);          // 光センサ値を表示
        ev3_lcd_draw_string( buf, 0, 20);

        if (startTrigger == true)
        {

            mIsStartedBlack = true;
            mSum = 0;
            mCalCount = 0;
        }
    }
    else
    {
        mSum += sensor;                                 // 光センサ値を積算
        mCalCount++;

        if (mCalCount == NUM_OF_BLACK_CALIBRATION) 		// 規定回数以上積算
        {
            cal = mSum / NUM_OF_BLACK_CALIBRATION;                                  // 平均値 ★キャリブレーション演習修正箇所
            mLineMonitor->setBlackThreshold(cal);

            sprintf( buf, "blackTh = %03d", cal);       // 黒しきい値を表示
            ev3_lcd_draw_string( buf, 0, 20);

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

    int8_t  sensor;
    int16_t  cal;
    bool finish;
    char buf[256];

    finish = false;
    sensor = mColorSensor.getBrightness();

    if (mIsStartedWhite == false)
    {
        sprintf( buf, "white = %03d", sensor);          // 光センサ値を表示
        ev3_lcd_draw_string( buf, 0, 30);

        if (startTrigger == true)
        {

            mIsStartedWhite = true;
            mSum = 0;
            mCalCount = 0;
        }
    }
    else
    {
        mSum += sensor;                                 // 光センサ値を積算
        mCalCount++;

        if (mCalCount == NUM_OF_WHITE_CALIBRATION) // 規定回数以上積算
        {
            cal = mSum / NUM_OF_WHITE_CALIBRATION;                                  // 平均値 ★キャリブレーション演習修正箇所
            mLineMonitor->setWhiteThreshold(cal);

            sprintf( buf, "whiteTh = %03d", cal);       // 白しきい値を表示
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
    rgb_raw_t rgb;
    Hsv hsv;

    finish = false;
    mColorSensor.getRawColor(rgb);
    hsv = RGBtoHSV(rgb);

    if (mIsStartedBlue == false)
    {
        sprintf( buf, "blue_r = %03d", rgb.r);          // 光センサ値を表示
        ev3_lcd_draw_string( buf, 0, 40);
        sprintf( buf, "blue_g = %03d", rgb.g);          // 光センサ値を表示
        ev3_lcd_draw_string( buf, 0, 50);
        sprintf( buf, "blue_b = %03d", rgb.b);          // 光センサ値を表示
        ev3_lcd_draw_string( buf, 0, 60);

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
    	mHSVSum.h += hsv.h;                                 // 光センサ値を積算
    	mHSVSum.s += hsv.s;                                 // 光センサ値を積算
    	mHSVSum.v += hsv.v;                                 // 光センサ値を積算
        mCalCount++;

        if (mCalCount == NUM_OF_WHITE_CALIBRATION) // 規定回数以上積算
        {
            cal.h = mHSVSum.h / NUM_OF_WHITE_CALIBRATION;                                  // 平均値 ★キャリブレーション演習修正箇所
            cal.s = mHSVSum.s / NUM_OF_WHITE_CALIBRATION;                                  // 平均値 ★キャリブレーション演習修正箇所
            cal.v = mHSVSum.v / NUM_OF_WHITE_CALIBRATION;                                  // 平均値 ★キャリブレーション演習修正箇所
            mLineMonitor->setBlueThreshold(cal);

            sprintf( buf, "Blue h = %03d", cal.h);       // 白しきい値を表示
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
    rgb_raw_t rgb;
    Hsv hsv;

    finish = false;
    mColorSensor.getRawColor(rgb);
    hsv = RGBtoHSV(rgb);


    if (mIsStartedRed == false)
    {
        sprintf( buf, "red = %03d", hsv.h);          // 光センサ値を表示
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
    	mHSVSum.h += hsv.h;                                 // 光センサ値を積算
    	mHSVSum.s += hsv.s;                                 // 光センサ値を積算
    	mHSVSum.v += hsv.v;                                 // 光センサ値を積算
        mCalCount++;

        if (mCalCount == NUM_OF_WHITE_CALIBRATION) // 規定回数以上積算
        {
            cal.h = mHSVSum.h / NUM_OF_WHITE_CALIBRATION;                                  // 平均値 ★キャリブレーション演習修正箇所
            cal.s = mHSVSum.s / NUM_OF_WHITE_CALIBRATION;                                  // 平均値 ★キャリブレーション演習修正箇所
            cal.v = mHSVSum.v / NUM_OF_WHITE_CALIBRATION;                                  // 平均値 ★キャリブレーション演習修正箇所
            mLineMonitor->setRedThreshold(cal);

            sprintf( buf, "Red h = %03d", cal.h);       // 白しきい値を表示
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
    rgb_raw_t rgb;
    Hsv hsv;

    finish = false;
    mColorSensor.getRawColor(rgb);
    hsv = RGBtoHSV(rgb);

    if (mIsStartedGreen == false)
    {
        sprintf( buf, "green = %03d", hsv.h);          // 光センサ値を表示
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
    	mHSVSum.h += hsv.h;                                 // 光センサ値を積算
    	mHSVSum.s += hsv.s;                                 // 光センサ値を積算
    	mHSVSum.v += hsv.v;                                 // 光センサ値を積算
        mCalCount++;

        if (mCalCount == NUM_OF_WHITE_CALIBRATION) // 規定回数以上積算
        {
            cal.h = mHSVSum.h / NUM_OF_WHITE_CALIBRATION;                                  // 平均値 ★キャリブレーション演習修正箇所
            cal.s = mHSVSum.s / NUM_OF_WHITE_CALIBRATION;                                  // 平均値 ★キャリブレーション演習修正箇所
            cal.v = mHSVSum.v / NUM_OF_WHITE_CALIBRATION;                                  // 平均値 ★キャリブレーション演習修正箇所
            mLineMonitor->setGreenThreshold(cal);

            sprintf( buf, "green h = %03d", cal.h);       // 白しきい値を表示
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
    rgb_raw_t rgb;
    Hsv hsv;

    finish = false;
    mColorSensor.getRawColor(rgb);
    hsv = RGBtoHSV(rgb);


    if (mIsStartedYellow == false)
    {
        sprintf( buf, "yellow = %03d", hsv.h);          // 光センサ値を表示
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
    	mHSVSum.h += hsv.h;                                 // 光センサ値を積算
    	mHSVSum.s += hsv.s;                                 // 光センサ値を積算
    	mHSVSum.v += hsv.v;                                 // 光センサ値を積算
        mCalCount++;

        if (mCalCount == NUM_OF_WHITE_CALIBRATION) // 規定回数以上積算
        {
            cal.h = mHSVSum.h / NUM_OF_WHITE_CALIBRATION;                                  // 平均値 ★キャリブレーション演習修正箇所
            cal.s = mHSVSum.s / NUM_OF_WHITE_CALIBRATION;                                  // 平均値 ★キャリブレーション演習修正箇所
            cal.v = mHSVSum.v / NUM_OF_WHITE_CALIBRATION;                                  // 平均値 ★キャリブレーション演習修正箇所
            mLineMonitor->setYellowThreshold(cal);

            sprintf( buf, "Yellow = %03d", cal.h);       // 白しきい値を表示
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
}
