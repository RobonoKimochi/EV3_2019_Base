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
#define GETPRETHETANUM 1
/*★ シータ制御検討用  */
#define THETA_CONTROL 1

#if RUN_EDGE == RUN_LEFT_EDGE
#define KPID_EDGE_FACTOR    (int32_t)(-1)        // ライントレース方向 1 or -1
#elif RUN_EDGE == RUN_RIGHT_EDGE
#define KPID_EDGE_FACTOR    (int32_t)(1)        // ライントレース方向 1 or -1
#endif

/* Θの制御方向はラインのエッジに依らない */
#define KPID_THETA_FACTOR    (int32_t)(-1)        // ライントレース方向 1 or -1

class Tracer {
public:
    Tracer(LineMonitor* lineMonitor,
               PidController* pidController);
    virtual ~Tracer();

    void run();
    void runWithoutBalancing();
    bool isFinished();
	int calcDirection();
	int calcDirectionBlock(void);
	void setBlockPID(void);
	void setStablePID(void);

    float mforward;
    float mturn;
    float RightPWM = 0;
    float LeftPWM = 0;
    float tturn = 0;

#if THETA_CONTROL
    PidController* tPidController = new PidController();

    float tagetTheta = 0;
    bool ThetaStart = false;
    int DeltaTheta = 0;
#endif

private:
    LineMonitor* mLineMonitor;
    PidController* mPidController;
    Rotator* mRotator = new Rotator();

    bool mIsInitialized;
    bool mIsFinished;

    Sound *mSound = Sound::getInstance();

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
