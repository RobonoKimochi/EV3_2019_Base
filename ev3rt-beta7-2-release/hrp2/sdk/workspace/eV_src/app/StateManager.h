/******************************************************************************
 *  StateManager.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/26
 *  Implementation of the Class StateManager
 *  Author: Kazuhiro Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_APP_StateManager_H_
#define EV3_APP_StateManager_H_

#include "Button.h"
#include "Tracer.h"
#include "Calibration.h"
#include "Remote.h"
#include "MeasureDistance.h"
#include "Sound.h"
#include "TailMotor.h"
#include "LEDColor.h"

class StateManager {
public:
    StateManager(Tracer* Tracer,
               const Button* Button,
               Calibration* calibration,
               Remote*		   Remote,
               MeasureDistance *measureDistance
               );
    virtual ~StateManager();

    void run();

	bool TailInit;
	bool LookUpCompFlag;
	bool GoalFlag = false;

    enum State {
        UNDEFINED,
    	CALIBRATION,
        WAITING_FOR_START,
        WALKING,
    	REMOTE,
    	LOOKUPGATE,
		SEESAW,
    	GARAGE,
        GRAY_DETECT,
        FINISH
    };

private:


    Tracer* mTracer;
    const Button* mButton;
    Calibration* mCalibration;
	Remote* mRemote;
	MeasureDistance* mMeasureDistance;// = new MeasureDistance();
    Sound *mSound = Sound::getInstance();
public:
    State mState;
private:
	int TimeCount;

    void execUndefined();
	void execCalibration();
    void execWaitingForStart();
    void execWalking();
	void execLookUpGate();
	void execGarage();
	void remote();
	void execSeesaw() ;
    void execGrayDetect();

    TailMotor *mTailMotor = TailMotor::getInstance();
public:
    bool mStartSignal;
};

#endif  // EV3_APP_StateManager_H_
