/******************************************************************************
 *  Calibration.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/06/12
 *  Implementation of the Class BalancingWalker
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_UNIT_CALIBLATION_H_
#define EV3_UNIT_CALIBLATION_H_

#include "GyroSensor.h"
#include "ColorSensor.h"
#include "LineMonitor.h"
#include "Sound.h"
#include "ev3api.h"
#include "app.h"
#include "Button.h"


class Calibration
{
public:

    Calibration(const ev3api::ColorSensor& colorSensor,
                  ev3api::GyroSensor& gyroSensor,
                  LineMonitor* lineMonitor,
				  const Button* Button);
    virtual ~Calibration();

    void init();
    bool RunCalibration();
    bool calibrateGyro(bool startTrigger);
    bool calibrateBlack(bool startTrigger);
    bool calibrateWhite(bool startTrigger);
    bool calibrateBlue(bool startTrigger);
    bool calibrateRed(bool startTrigger);
    bool calibrateGreen(bool startTrigger);
    bool calibrateYellow(bool startTrigger);
    void calibrateLineThreshold();

private:
    enum CalibrationState {
        UNDEFINED,
        CALIBRATION_GYRO,
        CALIBRATION_BLACK,
		CALIBRATION_WHITE,
		CALIBRATION_BLUE,
		CALIBRATION_RED,
		CALIBRATION_GREEN,
		CALIBRATION_YELLOW,
		CALIBRATION_SET_THRESHOLD,
		CALIBRATION_COMPLITE
    };

    static const int NUM_OF_GYRO_CALIBRATION;
    static const int NUM_OF_BLACK_CALIBRATION;
    static const int NUM_OF_WHITE_CALIBRATION;

    void execCalibrationGyro();
    void execCalibrationBlack();
    void execCalibrationWhite();
    void execCalibrateLineThreshold();
    void execCalibrationBlue();
    void execCalibrationRed();
    void execCalibrationGreen();
    void execCalibrationYellow();
//    void execCalibrationGyroAndTail();

    const ev3api::ColorSensor& mColorSensor;
    ev3api::GyroSensor& mGyroSensor;
    LineMonitor* mLineMonitor;
    const Button* mButton;
    Sound *mSound = Sound::getInstance();
    bool mIsStartedGyro;
    bool mIsStartedBlack;
    bool mIsStartedWhite;
    bool mIsStartedBlue = false;
    bool mIsStartedRed = false;
    bool mIsStartedGreen = false;
    bool mIsStartedYellow = false;
    bool CalibrationCompFlag;
    int16_t  mCalCount;
    CalibrationState mCalibrationState;
    int32_t  mSum;
    Hsv  mHSVSum;

};

#endif  // EV3_UNIT_CALIBLATION_H_
