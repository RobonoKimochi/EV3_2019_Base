/******************************************************************************
 *  Tracer.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Definition of the Class Tracer
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_Tracer_H_
#define EV3_APP_Tracer_H_

#include "LineMonitor.h"
#include "PidController.h"

#include "Odmetry.h"
#include "Rotator.h"
#include "RunManager.h"
#include "app_config.h"

#define TURN_GUARD 30

class Tracer {
public:
    Tracer(LineMonitor* lineMonitor,
               PidController* pidController);
    virtual ~Tracer();

    void run();
    void runWithoutBalancing();
    bool isFinished();
	int calcDirection();

    float mforward;
    float mturn;
private:
    LineMonitor* mLineMonitor;
    PidController* mPidController;
    Rotator* mRotator = new Rotator();
    bool mIsInitialized;
    bool mIsFinished;

	void  set_PID_forward();

	ev3api::Motor mRightWheel = MotorDriver::getInstance().getRightWheel();
    ev3api::Motor mLeftWheel = MotorDriver::getInstance().getLeftWheel();
    Odmetry *mOdmetry = Odmetry::getInstance();
    RunManager *mRunManager = RunManager::getInstance();
    RunManager::Section mSection;
    float percent;

    void setPID(float kp, float ki, float kd);
public:
    bool detectGray();
};

#endif  // EV3_APP_Tracer_H_
