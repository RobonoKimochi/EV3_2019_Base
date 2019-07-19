#ifndef EV3_APP_DRIVECONTROLLER_H_
#define EV3_APP_DRIVECONTROLLER_H_

#include "Odmetry.h"
#include "Sound.h"
#include "Mat.h"
#include "COM.h"
#include "LineMonitor.h"
#include "Tracer.h"

#define QTURN ((float)(PI / 2))
#define CIRCLEDISTANCE        (20)
#define LFTTURNCIRCLEDISTANCE (2)
#define RGTTURNCIRCLEDISTANCE (5)
#define UTURNCIRCLEDISTANCE   (5)
#define EDGEDISTANCE          (2)
#define SETDISTANCE           (30)
#define BACKSHALLOWDISTANCE   (30)
#define BACKDEEPDISTANCE      (30)
#define BRITHEOFF             (3)
#define TURNSPEED             (20)
#define FORWARDSPEED          (20)
#define COUNTERNUMTHRESH      (2)

class DriveController {
public:

    typedef enum {
    	TM_Forward    =  0,
		TM_RgtTurn    =  1,
		TM_LftTurn    =  2,
		TM_UTurn      =  3
    } TurnMode;

    typedef enum {
    	BM_Shallow    =  0,
        BM_Deep       =  1
    } BackMode;

    DriveController(LineMonitor* lineMonitor,Tracer* Tracer);
    virtual ~DriveController();
    bool TurnL(void);
    bool TurnR(void);
	bool TraceForward(float targetdistance);
    bool ThroughCircle(void);
    bool RgtTurnCircle(void);
    bool LftTurnCircle(void);
    bool UTurnCircle(void);
    bool SetToBin(void);
    bool BackSallow(void);
    bool BackDeep(void);


private:

    typedef enum {
    	CS_FirstLine     =  0,
		CS_FirstColor    =  1,
		CS_FirstWhite    =  2,
		CS_Turn          =  3,
		CS_SecondWhite   =  4,
		CS_SecondColor   =  5,
		CS_SecondLine    =  6
    } CircleState;

    typedef enum {
		BT_Noline      =  0,
		BT_Line        =  1,
    } BackState;

    typedef enum {
		US_FirstTurn    =  0,
		US_Forward      =  1,
		US_SecondTurn   =  2
    } UTurnState;

	ev3api::Motor mRightWheel = MotorDriver::getInstance().getRightWheel();
    ev3api::Motor mLeftWheel = MotorDriver::getInstance().getLeftWheel();
    Sound *mSound = Sound::getInstance();
    Odmetry *mOdmetry = Odmetry::getInstance();
    LineMonitor* mLineMonitor;
    Tracer* mTracer;
    float mRgtTheta = 0;
    float mRgtTargetTheta = 0;
    bool  mRgtInitFlag = false;
    float mLftTheta = 0;
    float mLftTargetTheta = 0;
    bool  mLftInitFlag = false;
    float mFowInitX = 0;
    float mFowInitY = 0;
    bool  mFowInitFlag = false;
    float mThrInitX = 0;
    float mThrInitY = 0;
    bool  mThrInitFlag = false;
    bool  mBackInitFlag = false;
    int   mBackTargetBri = 0;
    int Bcount = 0;
    int Gcount = 0;
    int Rcount = 0;
    int Ycount = 0;
    int Wcount = 0;
    CircleState mCircleState = CS_FirstLine;
    BackState   mBackState   = BT_Noline;
    UTurnState  mUTurnState  = US_FirstTurn;

    bool Forward(float targetdistance);
    bool Backward(float targetdistance);
    bool SetPWMForward(float targetdistance, float R_PWM, float L_PWM);
    bool UpdateColorState(TurnMode turnMode, float MedWhiteDistance);
    bool UpdateSetState(float setDistance);
    bool BackToLine(BackMode backMode, float setDistance);
    void SetColorCount(colorid_t color);

};


#endif  // EV3_APP_DRIVECONTROLLER_H_
