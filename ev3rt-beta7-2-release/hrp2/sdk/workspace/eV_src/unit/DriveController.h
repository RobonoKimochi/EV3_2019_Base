#ifndef EV3_APP_DRIVECONTROLLER_H_
#define EV3_APP_DRIVECONTROLLER_H_

#include "Odmetry.h"
#include "Sound.h"
#include "Mat.h"
#include "COM.h"
#include "LineMonitor.h"
#include "Tracer.h"
#include "LEDColor.h"

//#define QTURN ((float)(PI / 2))
#define QTURN ((float)(2.8015926 / 2)) /* 適合値 */
#define QTURNB ((float)(2.9015926 / 2)) /* 適合値 */
#define AdjustAng ((float)(0.2015926)) /* 適合値 */
#define CIRCLEDISTANCE        (20) /* 要調整 */
#define LFTTURNCIRCLEDISTANCE (6.5)
#define RGTTURNCIRCLEDISTANCE (8.5)
#define UTURNCIRCLEDISTANCE   (5)
#define EDGEDISTANCE          (2)
#define BACKSHALLOWDISTANCE   (30)
#define BACKDEEPDISTANCE      (30)
#define MINGUARDDISTANCE      (4)
#define BRITHEOFF             (10)
#define WHITEOFF              (7)
#define TURNSPEED             (20)
#define TURNSLOW              (2)
#define TURNFAST              (6)
#define FORWARDSPEED          (20)
#define SETSPEED              (10)
#define BACKWARDSPEED         (10)
#define COUNTERNUMTHRESH      (1)
#if RUN_COURSE == RUN_LEFT_COURSE
#define LFTSETDISTANCE        (3.0)
#define RGTSETDISTANCE        (4.5)
#define LFTGETDISTANCE        (24)
#define RGTGETDISTANCE        (26)
#define LFTBACKDISTANCE       (1.8)
#define RGTBACKDISTANCE       (4.0)
#else
#define LFTSETDISTANCE        (4.5)
#define RGTSETDISTANCE        (3.0)
#define LFTGETDISTANCE        (26)
#define RGTGETDISTANCE        (24)
#define LFTBACKDISTANCE       (4.0)
#define RGTBACKDISTANCE       (1.8)
#endif
#define OFFSETTDISTANCE       (2)

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

    typedef enum {
    	CS_FirstLine     =  0,
		CS_FirstColor    =  1,
		CS_FirstWhite    =  2,
		CS_Turn          =  3,
		CS_SecondWhite   =  4,
		CS_SecondColor   =  5,
		CS_SecondLine    =  6,
		CS_AdjustAng     =  7
    } CircleState;

    typedef enum {
		BS_Back        =  0,
		BS_Curv        =  1
    } BackState;

    typedef enum {
    	SS_Curv        =  0,
		SS_Foward      =  1
    } SetState;

    typedef enum {
		US_FirstTurn   =  0,
		US_Forward     =  1,
		US_SecondTurn  =  2
    } UTurnState;

    typedef enum {
		GS_Forward     =  0,
		GS_Curv        =  1
    } GetState;

    DriveController(LineMonitor* lineMonitor,Tracer* Tracer);
    virtual ~DriveController();
    bool TurnN(float TargetTheta, float R_PWM, float L_PWM);
    bool TurnL(bool FowFlag, float R_PWM, float L_PWM);
    bool TurnR(bool FowFlag, float R_PWM, float L_PWM);
	bool TraceForward(float targetdistance);
	bool SetForward(float targetdistance);
    bool ThroughCircle(void);
    bool RgtTurnCircle(void);
    bool LftTurnCircle(void);
    bool UTurnCircle(void);
    bool SetRgt(void);
    bool SetLft(void);
    bool BackLft(void);
    bool BackRgt(void);
    bool GetLft(void);
    bool GetRgt(void);
    void SetColor(void);
    void SetPWMZero(void);
    bool OffsetFlag = false;

    CircleState mCircleState = CS_FirstLine;
    SetState    mSetState    = SS_Curv;
    BackState   mBackState   = BS_Back;
    UTurnState  mUTurnState  = US_FirstTurn;
    GetState    mGetState    = GS_Forward;

private:

	ev3api::Motor mRightWheel = MotorDriver::getInstance().getRightWheel();
    ev3api::Motor mLeftWheel = MotorDriver::getInstance().getLeftWheel();
    Sound *mSound = Sound::getInstance();
    Odmetry *mOdmetry = Odmetry::getInstance();
    LineMonitor* mLineMonitor;
    Tracer* mTracer;
    float mNTheta = 0;
    float mNInitTheta = 0;
    bool  mNInitFlag = false;
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
    int   DeleyCoute = 0;
    int Bcount = 0;
    int Gcount = 0;
    int Rcount = 0;
    int Ycount = 0;
    int Wcount = 0;
    bool mSecondWFlag = false;
    colorid_t colorNum = COLOR_NONE;
    colorid_t colorPre = COLOR_NONE;

    bool Forward(float targetdistance);
    bool Backward(float targetdistance);
    bool SetPWMForward(float targetdistance, float R_PWM, float L_PWM);
    bool UpdateFowColorState();
    bool UpdateFarColorState();
    bool UpdateNearColorState();
    bool Curve(bool Turn_L, bool FowFlag, float R_PWM, float L_PWM);
    bool UpdateSetState(bool Turn_L, float fowDistance, float R_PWM, float L_PWM);
    bool UpDateBackState(bool Turn_L, float BackDistance, float R_PWM, float L_PWM);
    bool UpDateGetState(float BackDistance, float R_PWM, float L_PWM);
    bool BackToLine(BackMode backMode, float setDistance);
    void SetColorCount(colorid_t color);

};


#endif  // EV3_APP_DRIVECONTROLLER_H_
