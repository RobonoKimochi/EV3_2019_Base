/******************************************************************************
 *  LineMonitor.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Class LineMonitor
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_UNIT_LINEMONITOR_H_
#define EV3_UNIT_LINEMONITOR_H_

#include "ColorSensor.h"
#include "app_config.h"
#include "app.h"
#include "COM.h"

#define INITIAL_THRESHOLD_BLOCK ((INITIAL_WHITE_BLOCK + INITIAL_BLACK_BLOCK) / 2)
#define INITIAL_WHITE_BLOCK     (180)
#define INITIAL_BLACK_BLOCK     (7)

#define BLUE_THR_H    (4)
#define BLUE_THR_S    (20)
#define BLUE_THR_V    (20)
#define RED_THR_H     (30)
#define RED_THR_S     (40)
#define RED_THR_V     (30)
#define GREEN_THR_H   (20)
#define GREEN_THR_S   (20)
#define GREEN_THR_V   (20)
#define YELLOW_THR_H  (30)
#define YELLOW_THR_S  (30)
#define YELLOW_THR_V  (30)
#define GRAY_THR_H    (10)
#define GRAY_THR_S    (20)
#define GRAY_THR_V    (40)

#if PERPAR_MODE
#define BLUE_INIT_H    (227)
#define BLUE_INIT_S    (83)
#define BLUE_INIT_V    (65)
#define RED_INIT_H     (358)
#define RED_INIT_S     (80)
#define RED_INIT_V     (63)
#define GREEN_INIT_H   (214)
#define GREEN_INIT_S   (69)
#define GREEN_INIT_V   (28)
#define YELLOW_INIT_H  (21)
#define YELLOW_INIT_S  (68)
#define YELLOW_INIT_V  (69)
#define WHITE_INIT_H   (254)
#define WHITE_INIT_S   (50)
#define WHITE_INIT_V   (116)
#define BLACK_INIT_H   (262)
#define BLACK_INIT_S   (56)
#define BLACK_INIT_V   (5)
#else
#define BLUE_INIT_H    (230)
#define BLUE_INIT_S    (91)
#define BLUE_INIT_V    (72)
#define RED_INIT_H     (358)
#define RED_INIT_S     (84)
#define RED_INIT_V     (64)
#define GREEN_INIT_H   (140)
#define GREEN_INIT_S   (62)
#define GREEN_INIT_V   (30)
#define YELLOW_INIT_H  (28)
#define YELLOW_INIT_S  (79)
#define YELLOW_INIT_V  (64)
#define WHITE_INIT_H   (254)
#define WHITE_INIT_S   (50)
#define WHITE_INIT_V   (116)
#define BLACK_INIT_H   (262)
#define BLACK_INIT_S   (56)
#define BLACK_INIT_V   (5)

#endif


// 定義
class LineMonitor {
public:
    explicit LineMonitor(const ev3api::ColorSensor& colorSensor);
    virtual ~LineMonitor();

    void getBright(void);
    bool isOnLine() const;
    void setLineThreshold(int8_t threshold);
    int8_t getDeviation() const;
    void calLineThreshold();
    void setBlackThreshold(int8_t threshold);
    void setWhiteThreshold(int8_t threshold);
    void setBlueThreshold(Hsv in);
    void setRedThreshold(Hsv in);
    void setGreenThreshold(Hsv in);
    void setYellowThreshold(Hsv in);
	void LeanModecalLineThreshold();
	void LineThresholdGray();
	bool DecreaseDif();
	bool IncreaseDif();
	bool DetectEdgeOfdif();
	bool ColorDiffFromPre();
	void setColor(void);
	colorid_t rcgColor(void);
	uint16_t rgbTobright(void) const;
	int8_t getDeviationBlock(void);
	colorid_t JdgColorType(void);
    int16_t mBlockLineThreshold = INITIAL_THRESHOLD_BLOCK;
    int16_t mBlockWhiteThresh = INITIAL_WHITE_BLOCK;
    int16_t mBlockBlackThresh = INITIAL_BLACK_BLOCK;
    int8_t mBright = 0;
    colorid_t mColorType = COLOR_NONE;
    rgb_raw_t RGBdata ={};

    const ev3api::ColorSensor& mColorSensor;
    int8_t mLineThreshold;
    int8_t mWhiteThresh;
    int8_t mBlackThresh;
    Hsv    mBlueHSV = {BLUE_INIT_H,BLUE_INIT_S,BLUE_INIT_V};
    Hsv    mRedHSV = {RED_INIT_H,RED_INIT_S,RED_INIT_V};
    Hsv    mGreenHSV = {GREEN_INIT_H,GREEN_INIT_S,GREEN_INIT_V};
    Hsv    mYellowHSV = {YELLOW_INIT_H,YELLOW_INIT_S,YELLOW_INIT_V};
    Hsv    mWhiteHSV = {WHITE_INIT_H,WHITE_INIT_S,WHITE_INIT_V};
    Hsv    mBlackHSV = {BLACK_INIT_H,BLACK_INIT_S,BLACK_INIT_V};
private:
    static const int8_t INITIAL_THRESHOLD;
    static const int8_t EDGE_THRESHOLD = 2;
    static const int8_t UPPER_GUARD = 1;
    int8_t mEdgeCount = 0;
    int8_t mDecIncCount = 0;
    int8_t mDecDecCount = 0;
    int8_t mIncIncCount = 0;
    int8_t mIncDecCount = 0;
    int8_t mPreBrightness = 0;
    bool  mPreDec = false;
    float mDecPreDevietion = 0;
    float mIncPreDevietion = 0;

	const int8_t mLeanWhiteThresh = 10;
	const int8_t mLeanBlackThresh = 0;
};

#endif  // EV3_UNIT_LINEMONITOR_H_
