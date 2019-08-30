/******************************************************************************
 *  LineMonitor.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Class LineMonitor
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/


#include "LineMonitor.h"

// 定数宣言
const int8_t LineMonitor::INITIAL_THRESHOLD = 20;  // 黒色の光センサ値

/**
 * コンストラクタ
 * @param colorSensor カラーセンサ
 */
LineMonitor::LineMonitor(const ev3api::ColorSensor& colorSensor)
    : mColorSensor(colorSensor),
      mLineThreshold((DEFAULT_COLOR_WHITE + DEFAULT_COLOR_BLACK) / 2),
      mWhiteThresh(DEFAULT_COLOR_WHITE),
      mBlackThresh(DEFAULT_COLOR_BLACK) {
}

/**
 * デストラクタ
 */
LineMonitor::~LineMonitor() {
}

void LineMonitor::getBright(void)
{
    // カラーセンサ値を取得
	mBright = mColorSensor.getBrightness();
}


/**
 * ライン上か否かを判定する
 * @retval true  ライン上
 * @retval false ライン外
 */
bool LineMonitor::isOnLine() const
{
    // 光センサからの取得値を見て
    // 黒以上であれば「true」を、
    // そうでなければ「false」を返す
    if (mBright >= mLineThreshold)
    {
        return true;
    }
    else
    {
        return false;
    }
}

/**
 * ライン閾値を設定する。
 * @param threshold ライン閾値
 */
void LineMonitor::setLineThreshold(int8_t threshold)
{
    mLineThreshold = threshold;
}

/**
 * ライン閾値からの偏差を取得する
 */
int8_t LineMonitor::getDeviation() const
{
    return (mLineThreshold - mBright);
}


/**
 * ライン閾値を算出して設定する
 */
void LineMonitor::calLineThreshold()
{
    int16_t  cal;

    cal = (mBlackThresh + mWhiteThresh) / 2;
    setLineThreshold(cal);
}

/**
 * グレー用閾値設定
 */
void LineMonitor::LineThresholdGray()
{
    int16_t  cal;

    cal = (mBlackThresh + mWhiteThresh) / 2;
	cal = cal * 1.5;
    setLineThreshold(cal);
}


/**
 * 黒色の閾値を設定する
 */
void LineMonitor::setBlackThreshold(int8_t threshold)
{
    mBlackThresh = threshold;
}


/**
 * 白色の閾値を設定する
 */
void LineMonitor::setWhiteThreshold(int8_t threshold)
{
    mWhiteThresh = threshold;
}

/**
 * 青色の閾値を設定する
 */
void LineMonitor::setBlueThreshold(Hsv in)
{
	mBlueHSV = in;
}

/**
 * 赤色の閾値を設定する
 */
void LineMonitor::setRedThreshold(Hsv in)
{
	mRedHSV = in;
}

/**
 * 緑色の閾値を設定する
 */
void LineMonitor::setGreenThreshold(Hsv in)
{
	mGreenHSV = in;
}

/**
 * 黄色の閾値を設定する
 */
void LineMonitor::setYellowThreshold(Hsv in)
{
	mYellowHSV = in;
}

void LineMonitor::LeanModecalLineThreshold()
{
    int16_t  cal;

    cal = (mLeanBlackThresh + mLeanWhiteThresh) / 2;
    setLineThreshold(cal);
}

/**
 * ライン閾値からの偏差が減少しているか判断する
 * TRUE   目標値との差が減少した
 * FALSE  目標値との差が減少していない
 */
bool LineMonitor::DecreaseDif()
{

	bool ret_ ;
	if (ABS(getDeviation()) < ABS(mDecPreDevietion))
	{

		if (mDecDecCount < (EDGE_THRESHOLD))
		{
			mDecDecCount++;
		}

		mDecIncCount = 0;
	}
	else if (ABS(getDeviation()) > ABS(mDecPreDevietion))
	{

		if (mDecIncCount < (EDGE_THRESHOLD))
		{
			mDecIncCount++;
		}

		mDecDecCount = 0;

	}

	if (mDecDecCount >= EDGE_THRESHOLD) {

		ret_ = true;
	}
	else
	{
		ret_ = false;
	}

	mDecPreDevietion = getDeviation();

    return (ret_);
}


/**
 * ライン閾値からの偏差が増加しているか判断する
 * TRUE   目標値との差が増加した
 * FALSE  目標値との差が増加していない
 */
bool LineMonitor::IncreaseDif()
{

	bool ret_ ;
	if (ABS(getDeviation()) < ABS(mIncPreDevietion))
	{

		if (mIncDecCount < (EDGE_THRESHOLD))
		{
			mIncDecCount++;
		}

		mIncIncCount = 0;
	}
	else if (ABS(getDeviation()) > ABS(mIncPreDevietion))
	{

		if (mIncIncCount < (EDGE_THRESHOLD))
		{
			mIncIncCount++;
		}

		mIncDecCount = 0;

	}

	if (mIncIncCount >= EDGE_THRESHOLD) {

		ret_ = true;
	}
	else
	{
		ret_ = false;
	}

	mIncPreDevietion = getDeviation();

    return (ret_);
}

/**
 * ライン閾値からの偏差が増加→減少の変化を監視する
 * TRUE   目標値との差が増加→減少に変化した
 * FALSE  上記以外
 */
bool LineMonitor::DetectEdgeOfdif()
{

	bool ret_ = false;
	bool Decerase = DecreaseDif();

	if ((Decerase == true)
	 && (mPreDec == false))
	{
		ret_ = true;
	}

	mPreDec = Decerase;

    return (ret_);
}

/**
 * カラーセンサ値が前回値から変化しているか判定する
 * TRUE   前回値から変化した
 * FALSE  前回値から変化していない
 */
bool LineMonitor::ColorDiffFromPre()
{

	int8_t mBrightness = mBright;
	bool ret_;

	if (mBrightness == mPreBrightness)
	{
		ret_ = false;
	} else {
		ret_ = true;
	}

	mPreBrightness = mBrightness;

    return (ret_);
}

/* カラーセンサ値を取得 */
void LineMonitor::setColor(void)
{

	mColorSensor.getRawColor(RGBdata);
	return;
}

colorid_t LineMonitor::rcgColor(void)
{

	colorid_t color;

	color = mColorSensor.getColorNumber();

	return (color);
}

colorid_t LineMonitor::JdgColorType(void)
{

	colorid_t color = COLOR_NONE;

    Hsv hsv;
    int R_dh;
    int Y_dh;
    hsv = RGBtoHSV(RGBdata);

//    int Hupper;
//    int Hlower;

//    if (mWhiteHSV.h > mBlackHSV.h) {
//    	Hupper = mWhiteHSV.h + GRAY_THR_H + 30;
//    	Hlower = mBlackHSV.h - GRAY_THR_H;
//    } else {
//    	Hupper = mBlackHSV.h + GRAY_THR_H + 30;
//    	Hlower = mWhiteHSV.h - GRAY_THR_H;
//    }
//
//    /* 白黒ライン上判定 */
//	if ( (Hlower < hsv.h) && (hsv.s < Hupper))
//	{
//		color = COLOR_BROWN;
//	}


	if (  (ABS(mWhiteHSV.s - hsv.s) < GRAY_THR_S)
	    &&(ABS(mWhiteHSV.v - hsv.v) < GRAY_THR_V) )
	{
		color = COLOR_BROWN;

	}


	/* 360度またぎ対応 */
	R_dh = mRedHSV.h - hsv.h;
	if(ABS(R_dh) > 180) {
		R_dh = mRedHSV.h - hsv.h - 360;
	}

	if ( (ABS(R_dh) < RED_THR_H)
		&&(ABS(mRedHSV.s - hsv.s) < RED_THR_S)
		&&(ABS(mRedHSV.v - hsv.v) < RED_THR_V) )
	{
		color = COLOR_RED;

	}

	if ( (ABS(mBlueHSV.h - hsv.h) < BLUE_THR_H)
	   &&(ABS(mBlueHSV.s - hsv.s) < BLUE_THR_S)
	   &&(ABS(mBlueHSV.v - hsv.v) < BLUE_THR_V) )
	{
		color = COLOR_BLUE;

	}

	/* 360度またぎ対応 */
	Y_dh = mYellowHSV.h - hsv.h;
	if(ABS(Y_dh) > 180) {
		Y_dh = mYellowHSV.h - hsv.h + 360;
	}

	if ( (ABS(Y_dh) < YELLOW_THR_H)
    	&&(ABS(mYellowHSV.s - hsv.s) < YELLOW_THR_S)
		&&(ABS(mYellowHSV.v - hsv.v) < YELLOW_THR_V) )
	{
		if (ABS(R_dh) > ABS(Y_dh)) {
			color = COLOR_YELLOW;
		}

	}

	if
	    ( (ABS(mGreenHSV.h - hsv.h) < GREEN_THR_H)
		&&(ABS(mGreenHSV.s - hsv.s) < GREEN_THR_S)
		&&(ABS(mGreenHSV.v - hsv.v) < GREEN_THR_V) )

	{
		color = COLOR_GREEN;

	}



	/* ログ用に値を更新する */
	mColorType = color;

	return (color);
}

uint16_t LineMonitor::rgbTobright(void) const
{

	uint16_t bright;
	bright = (RGBdata.b + RGBdata.g + RGBdata.r) / 3;

	return (bright);
}

int8_t LineMonitor::getDeviationBlock(void)
{

	setColor();

    return (mBlockLineThreshold - rgbTobright());
}
