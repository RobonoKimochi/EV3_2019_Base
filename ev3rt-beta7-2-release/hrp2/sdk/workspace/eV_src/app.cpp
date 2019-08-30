/******************************************************************************
 *	app.cpp (for LEGO Mindstorms EV3)
 *	Created on: 2015/01/25
 *	Implementation of the Task main_task
 *	Author: Kazuhiro.Kawachi
 *	Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/


#include "app.h"
#include "Tracer.h"
#include "StateManager.h"
// added part
#include "Odmetry.h"
#include "UI.h"
// #include "BlueTooth.h"
#include "Logger.h"

#include "MotorDriver.h"
#include "SensorDriver.h"

#include "Remote.h"
#include "MeasureDistance.h"
#include "RunManager.h"
#include "TailMotor.h"
#include "Sound.h"
#include "LEDColor.h"

#include "Clock.h"
#include "Block.h"
#include "COM.h"
#include "DriveController.h"
#include "CommandMaker.h"
#include "ArmMotor.h"
#include "Parking.h"

#if USE_OUTPUT_CSV
    FILE *fp;
    FILE *fp_ini;
#define CSV_CYCLE (3000)
#endif

// デストラクタ問題の回避
// https://github.com/ETrobocon/etroboEV3/wiki/problem_and_coping
void *__dso_handle=0;

// using宣言
using ev3api::ColorSensor;
using ev3api::GyroSensor;
using ev3api::Motor;
using ev3api::Clock;

// Motor関連
ev3api::Motor gLeftWheel  = MotorDriver::getInstance().getLeftWheel();
ev3api::Motor gRightWheel = MotorDriver::getInstance().getRightWheel();
ev3api::Motor gArm        = MotorDriver::getInstance().getArmMotor();

// Sensor関連
ev3api::TouchSensor gTouchSensor = SensorDriver::getInstance().getTouchSensor();
ev3api::ColorSensor gColorSensor = SensorDriver::getInstance().getColorSensor();
ev3api::SonarSensor gSonarSensor = SensorDriver::getInstance().getSonarSensor();
ev3api::GyroSensor 	gGyroSensor  = SensorDriver::getInstance().getGyroSensor();

// オブジェクトの定義
static LineMonitor	    *gLineMonitor;
static Tracer	        *gTracer;
static Button			*gButton;
static Calibration		*gCalibration;
static PidController	*gPidController;
static StateManager 	*gStateManager;
static MeasureDistance	*gMeasureDistance;
static UI				*gUI;
static Logger			*gLogger;
static TailMotor		*gTailMotor;
static RunManager		*gRunManager;
static Remote			*gRemote;
static Odmetry			*gOdmetry;
static Sound 			*gSound;
static Clock			*gClock;
static Block			*gBlock;
static DriveController	*gDriveController;
static CommandMaker	*gCommandMaker;
static ArmMotor        *gArmMotor;
static Parking         *gParking;

static bool ParkingFlag = false;
#if USE_OUTPUT_CSV
static float          gCSVdata[20][CSV_CYCLE];
static unsigned int  CSVCycle = 0;
static unsigned int  CSVCycle_num = 0;
#endif
 /* ↓処理負荷計測用の変数 */
 /* static float          gCalibTimerS; */
 /* static float          gCalibTimerE; */
 /* gCalibTimerS = (float)gClock->now() / 1000.0f; */
 /* gCalibTimerE = (float)gClock->now() / 1000.0f; */
//static unsigned int gRouteIndex = 42;
//static unsigned int gRouteIndex = 13;

/* 複合パターン2 */
//static unsigned int gRouteIndex = 15;
/* 複合パターン3 */
static unsigned int gRouteIndex = 17;
/* Rコース用 */
//static unsigned int gRouteIndex = 15;

CommandMaker::coord     gRoute[300] =
{
// {0 , 4},
// {1 , 4},
// {2 , 4},
// {3 , 4},
// {3 , 5},
// {3 , 4},
// {4 , 4},
// {4 , 3},
// {4 , 2},
// {4 , 1},
// {3 , 1},
// {4 , 1},
// {4 , 0},
// {5 , 0},
// {6 , 0},
// {6 , 1},
// {5 , 1},
// {6 , 1},
// {6 , 2},
// {6 , 3},
// {5 , 3},
// {6 , 3},
// {6 , 4},
// {5 , 4},
// {5 , 3},
// {5 , 4},
// {4 , 4},
// {3 , 4},
// {3 , 5},
// {3 , 4},
// {2 , 4},
// {1 , 4},
// {1 , 3},
// {1 , 4},
// {2 , 4},
// {2 , 3},
// {1 , 3},
// {2 , 3},
// {2 , 4},
// {3 , 4},
// {4 , 4},

// {0 , 4},
// {1 , 4},
// {2 , 4},
// {3 , 4},
// {4 , 4},
// {4 , 3},
// {5 , 3},
// {6 , 3},
// {6 , 2},
// {6 , 1},
// {5 , 1},
// {6 , 1},
// {6 , 0},

 /* 複合パターン2 */
// {0 , 4},
// {0 , 3},
// {0 , 2},
// {1 , 2},
// {1 , 3},
// {1 , 4},
// {2 , 4},
// {2 , 3},
// {2 , 2},
// {1 , 2},
// {1 , 3},
// {1 , 2},
// {2 , 2},
// {3 , 2},
// {4 , 2},

 /* 複合パターン3 */
 {0 , 4},
 {1 , 4},
 {1 , 3},
 {1 , 4},
 {0 , 4},
 {0 , 3},
 {1 , 3},
 {0 , 3},
 {0 , 4},
 {1 , 4},
 {2 , 4},
 {2 , 3},
 {1 , 3},
 {0 , 3},
 {0 , 2},
 {0 , 1},
 {0 , 0},

 /* Rコース用  */
// {6 , 4},
// {5 , 4},
// {4 , 4},
// {4 , 3},
// {4 , 2},
// {3 , 2},
// {3 , 1},
// {3 , 2},
// {2 , 2},
// {2 , 3},
// {1 , 3},
// {0 , 3},
// {0 , 4},
// {0 , 5},
// {0 , 6},

};

/**
 * EV3システム生成
 */
static void user_system_create() {
	tslp_tsk(2);
	// オブジェクトの作成
	gLineMonitor	 = new LineMonitor(gColorSensor);
	gButton		 	 = new Button(gTouchSensor);
	gPidController	 = new PidController();
	gMeasureDistance = new MeasureDistance(gSonarSensor);
	gTracer 	 	 = new Tracer(gLineMonitor, gPidController);
	gCalibration	 = new Calibration(gColorSensor, gGyroSensor, gLineMonitor, gButton);
	gRemote 		 = new Remote();
	gUI 			 = new UI();
	gLogger 		 = new Logger();
	gOdmetry		 = Odmetry::getInstance();
	gRunManager		 = RunManager::getInstance();
	gTailMotor		 = TailMotor::getInstance();
	gSound 			 = Sound::getInstance();
	gClock			 = new Clock();
	gCommandMaker    = new CommandMaker(&gRoute[0], gRouteIndex);
	gStateManager 	 = new StateManager(gTracer, gButton, gCalibration, gRemote, gMeasureDistance);
	gDriveController = new DriveController(gLineMonitor,gTracer);
	gBlock		 	 = new Block(gDriveController,gCommandMaker);
	gArmMotor        = ArmMotor::getInstance();
	gParking		 = new Parking();

	// 初期化完了通知
	ev3_led_set_color(LED_ORANGE);

	gSound->ready();
}

/**
 * EV3システム破棄
 */
static void user_system_destroy() {
	gLeftWheel.reset();
	gRightWheel.reset();
	gArm.reset();

	delete gStateManager;
	delete gTracer;
	delete gButton;
	delete gLineMonitor;
	delete gCalibration;
	delete gPidController;
	delete gUI;
	delete gLogger;

/* 追加 */
	gGyroSensor.reset();
	/* colorとtouchセンサはどうする */
	delete gMeasureDistance;
	delete gRemote;
	delete gOdmetry;
	delete gRunManager;
	delete gTailMotor;
//	delete gSound; /* 暫定 */
	delete gClock;
	delete gBlock;
	delete gDriveController;
	delete gCommandMaker;
	delete gArmMotor;
	delete gParking;
}

/**
 * CSVデータ出力
 */
static void output_CSV_data() {
#if USE_OUTPUT_CSV
	bool chageflag = false;
	fp = fopen("ev3rt/log/log.csv", "w");
	fprintf(fp,"Logertime,");
	fprintf(fp,"X,");
	fprintf(fp,"Y,");
	fprintf(fp,"Theta,");
	fprintf(fp,"dTheta,");
	fprintf(fp,"bright,");
	fprintf(fp,"R,");
	fprintf(fp,"G,");
	fprintf(fp,"B,");
	fprintf(fp,"Colortype,");
	fprintf(fp,"mainState,");
	fprintf(fp,"dev,");
	fprintf(fp,"R_PWM,");
	fprintf(fp,"L_PWM,");
	fprintf(fp,"mturn,");
	fprintf(fp,"Tturn,");
	fprintf(fp,"ThetaStart,");
	fprintf(fp,"targetTheta,");
	fprintf(fp,"\n");

    for (unsigned int i =0; i < CSVCycle_num; i++) {

    	if (gCSVdata[10][i] == StateManager::FINISH && chageflag == false) {

    		fprintf(fp,"Logertime,");
    		fprintf(fp,"X,");
    		fprintf(fp,"Y,");
    		fprintf(fp,"Theta,");
    		fprintf(fp,"dTheta,");
    		fprintf(fp,"bright,");
    		fprintf(fp,"R,");
    		fprintf(fp,"G,");
    		fprintf(fp,"B,");
    		fprintf(fp,"Colortype,");
    		fprintf(fp,"mainState,");
    		fprintf(fp,"RGB_Bright,");
    		fprintf(fp,"command_index,");
    		fprintf(fp,"command,");
    		fprintf(fp,"CircleState,");
    		fprintf(fp,"setState,");
    		fprintf(fp,"BackState,");
//    		fprintf(fp,"getState,");
    		fprintf(fp,"h,");
    		fprintf(fp,"s,");
    		fprintf(fp,"v,");
    		fprintf(fp,"\n");

    		chageflag = true;
    	}

    	for (unsigned int t =0; t < 20; t++) {
    		fprintf(fp,"%f,",gCSVdata[t][i]);
    	}
    	fprintf(fp,"\n");
    }

    fclose(fp);
	gSound->dryy();
#endif

}


/**
 * メインタスク
 */
void main_task(intptr_t unused) {

	/* プログラムのエントリポイント */
	/* プログラム起動時にはmain_taskの処理が実行される */

	user_system_create();  // センサやモータの初期化処理

	// 周期キャリブレーションタスク開始
	ev3_sta_cyc(EV3_CYC_CALIB);

	// 周期ハンドラ開始
	ev3_sta_cyc(EV3_CYC_ODMETRY);

	// UIタスク開始
//	act_tsk(UI_TASK);

	slp_tsk();	// バックボタンが押されるまで待つ

	// 周期ハンドラ停止
	ev3_stp_cyc(EV3_CYC_TRACER);
	ev3_stp_cyc(EV3_CYC_BLOCK);
	ev3_stp_cyc(EV3_CYC_ARM);
	ev3_stp_cyc(EV3_CYC_LOGGER);
	ev3_stp_cyc(SOUND_TASK);

	output_CSV_data(); /* csv出力 */

	user_system_destroy();	// 終了処理

	ext_tsk();
}

/**
 * トレーサ周期タスク
 */
void ev3_cyc_tracer(intptr_t exinf) {
	act_tsk(TRACER_TASK);
}

/**
 * トレーサタスク
 */
void tracer_task(intptr_t exinf) {
	if (ev3_button_is_pressed(BACK_BUTTON)) {
		wup_tsk(MAIN_TASK);  // バックボタン押下
	} else {

		gStateManager->run();	// 走行
	}

	if (gStateManager->GoalFlag == true) {
		ev3_sta_cyc(EV3_CYC_BLOCK);
		ev3_stp_cyc(EV3_CYC_TRACER);
	}

#if USE_OUTPUT_LOG
  static int start_flag = 0;
  if ((gStateManager->mState == StateManager::WALKING) // 6はStateManager::WALKING
	&& (start_flag == 0)) {
	  ev3_sta_cyc(EV3_CYC_LOGGER);
	  start_flag = 1;
  }
#endif

	ext_tsk();
}
/**
 * ブロックならべ周期タスク
 */
void ev3_cyc_block(intptr_t exinf) {
	act_tsk(BLOCK_TASK);
}

/**
 * ブロックならべタスク
 */
void block_task(intptr_t exinf) {
	if (ev3_button_is_pressed(BACK_BUTTON)) {
		wup_tsk(MAIN_TASK);  // バックボタン押下
	} else {

		gBlock->run();

	}

	if (gBlock->EndFlag == true) {

		ev3_stp_cyc(EV3_CYC_BLOCK);
		gSound->ready();
		ParkingFlag = true;
	}

	if (ParkingFlag == true) {
		gParking->run();
	}

	ext_tsk();
}

/**
 * キャリブレーション周期タスク
 */
void ev3_cyc_calib(intptr_t exinf) {
	act_tsk(CALIB_TASK);
}

/**
 * キャリブレーションべタスク
 */
void calib_task(intptr_t exinf) {

	bool calib_fin = false;
	if (ev3_button_is_pressed(BACK_BUTTON)) {
		wup_tsk(MAIN_TASK);  // バックボタン押下
	} else {

		ev3_led_set_color(LED_ORANGE);
		calib_fin = gCalibration->RunCalibration();

	}

	if (calib_fin == true) {
		act_tsk(CMD_TASK);
		ev3_stp_cyc(EV3_CYC_CALIB);
	}

	ext_tsk();
}

/**
 * サウンド周期タスク
 */
void ev3_cyc_sound(intptr_t exinf) {
	act_tsk(SOUND_TASK);
}

/**
 * サウンドタスク
 */
void sound_task(intptr_t exinf) {

	gSound->sound_main();

	ext_tsk();
}


/**
 * コマンドタスク
 */
void cmd_task(intptr_t exinf) {

//	gCommandMaker->make_command();

	act_tsk(INI_TASK);
	ext_tsk();
}

/**
 * 初期ログタスク
 */
void ini_task(intptr_t exinf) {

#if USE_OUTPUT_CSV
	fp_ini = fopen("ev3rt/log/init_log.csv", "w");
	fprintf(fp_ini,"mBlackThresh , %d \n",gLineMonitor->mBlackThresh);
	fprintf(fp_ini,"mWhiteThresh , %d \n",gLineMonitor->mWhiteThresh);
	fprintf(fp_ini,"mLineThreshold , %d \n",gLineMonitor->mLineThreshold);
	fprintf(fp_ini,"mBlockBlackThresh , %d \n",gLineMonitor->mBlockBlackThresh);
	fprintf(fp_ini,"mBlockWhiteThresh , %d \n",gLineMonitor->mBlockWhiteThresh);
	fprintf(fp_ini,"mBlockLineThreshold , %d \n",gLineMonitor->mBlockLineThreshold);
	fprintf(fp_ini,"mBlueHSV.h , %d \n",gLineMonitor->mBlueHSV.h);
	fprintf(fp_ini,"mBlueHSV.s , %d \n",gLineMonitor->mBlueHSV.s);
	fprintf(fp_ini,"mBlueHSV.v , %d \n",gLineMonitor->mBlueHSV.v);
	fprintf(fp_ini,"mRedHSV.h , %d \n",gLineMonitor->mRedHSV.h);
	fprintf(fp_ini,"mRedHSV.s , %d \n",gLineMonitor->mRedHSV.s);
	fprintf(fp_ini,"mRedHSV.v , %d \n",gLineMonitor->mRedHSV.v);
	fprintf(fp_ini,"mGreenHSV.h , %d \n",gLineMonitor->mGreenHSV.h);
	fprintf(fp_ini,"mGreenHSV.s , %d \n",gLineMonitor->mGreenHSV.s);
	fprintf(fp_ini,"mGreenHSV.v , %d \n",gLineMonitor->mGreenHSV.v);
	fprintf(fp_ini,"mYellowHSV.h , %d \n",gLineMonitor->mYellowHSV.h);
	fprintf(fp_ini,"mYellowHSV.s , %d \n",gLineMonitor->mYellowHSV.s);
	fprintf(fp_ini,"mYellowHSV.v , %d \n",gLineMonitor->mYellowHSV.v);
	fprintf(fp_ini,"mCommand_index , %d \n",gCommandMaker->mCommand_index);
	fprintf(fp_ini,"\n");
	fprintf(fp_ini,"\n");
	fprintf(fp_ini,"\n");
	fprintf(fp_ini,"ROUTE\n");

	for (unsigned int i =0; i < gRouteIndex; i++) {
		fprintf(fp_ini,"%d , (%d . %d), \n",i,gRoute[i].x,gRoute[i].y);
	}

	fprintf(fp_ini,"\n");
	fprintf(fp_ini,"\n");
	fprintf(fp_ini,"\n");
	fprintf(fp_ini,"COMMAND\n");

	for (unsigned int i =0; i < 1000; i++) {
		fprintf(fp_ini,"%d , %d, \n",i,gCommandMaker->mCommand[i]);

		if (gCommandMaker->mCommand[i] == CommandMaker::BC_End) {
			break;
		}
	}
	fprintf(fp_ini,"\n");
    fclose(fp_ini);
#endif

	ev3_sta_cyc(EV3_CYC_TRACER);
	ev3_sta_cyc(EV3_CYC_ARM);
	ev3_sta_cyc(EV3_CYC_SOUND);

	ext_tsk();
}


/**
 * テールモータ周期タスク
 */
void ev3_cyc_tailmotor(intptr_t exinf) {
	act_tsk(TAILMOTOR_TASK);
}
/**
 * テールモータタスク
 */
void tailmotor_task(intptr_t exinf) {

	ext_tsk();
}

/**
 * アームモータ周期タスク
 */
void ev3_cyc_arm(intptr_t exinf) {
	act_tsk(ARM_TASK);
}
/**
 * アームモータタスク
 */
void arm_task(intptr_t exinf) {

	gArmMotor->moveArmMotor();	// アーム制御
	ext_tsk();
}
/**
 * オドメトリ周期タスク
 */
void ev3_cyc_odmetry(intptr_t exinf) {
	act_tsk(ODMETRY_TASK);
}
/**
 * オドメトリタスク
 */
void odmetry_task(intptr_t exinf) {
	gOdmetry->updateLocation();  // 倒立走行
	ext_tsk();
}
/**
 * ロガー周期タスク
 */
void ev3_cyc_logger(intptr_t exinf) {
	act_tsk(LOGGER_TASK);
}
/**
 * ロガータスク
 */
#define LOG_NUM 11
void logger_task(intptr_t exinf){
	float gLog[LOG_NUM];
	// gLog[0]	= gOdmetry->getX();					// X座標
	// gLog[1]	= gOdmetry->getY();					// Y座標
	// gLog[2]	= gOdmetry->getTheta();				// 角度
	// gLog[3]	= gColorSensor.getBrightness();		// カラーセンサ値
	// gLog[4]	= gGyroSensor.getAnglerVelocity();	// ジャイロ値
	// gLog[5]	= gBalancer->getForward();			// Forward値
	// gLog[6]	= gBalancer->getTurn();				// Turn値
	// gLog[7]	= gOdmetry->getDeltaTheta();		// 角度の時間変化（フィルタ済み）
	// gLog[8]	= ev3_battery_voltage_mV();			// バッテリー
	// gLog[9]	= gStateManager->debug;	// mState
	// gLog[10] = gTailMotor->mAngle;				// テールモータのアングル
	// gLog[11] = gSonarSensor.getDistance();		// 距離センサ
	// gLog[12] = gRunManager->mZone;				// 走行区間
	// gLog[13] = gRunManager->dDist;				// 走行区間
	// gLog[14] = gRunManager->dLine;				// 走行区間
	// gLog[15] = gRunManager->dCount;				// 走行区間(kaunto)

//	gLogger->putString("time;x;y;theta;deltaTheta;battery;state;sonardist;Zone;dist;Line;gyro;color;forward;turn;Rpwm;Lpwm;\n\r");
//	gLogger->putString("time;x;y;theta;deltaTheta;sonar;gyro;color;forward;turn;Rpwm;Lpwm;\n\r");

//	gLog[0]  = gOdmetry->getX();					// X座標
//	gLog[1]  = gOdmetry->getY();					// Y座標
//	gLog[2]  = gOdmetry->getTheta();				// 角度
//	gLog[3]  = gOdmetry->getDeltaTheta();		// 角度の時間変化（フィルタ済み）
//	gLog[4]  = gSonarSensor.getDistance();		// 距離センサ
//
//	gLog[5] = gGyroSensor.getAnglerVelocity();	// ジャイロ値

//	gLog[7]  = gBalancer->getForward();			// Forward値
//	gLog[8]  = gBalancer->getTurn();				// Turn値
//	gLog[9]  = gBalancingWalker->getLeftPwm();		//pwm
//	gLog[10]  = gBalancingWalker->getRightPwm();	//pwm
//	gLog[11]  = gStateManager->debug;	// mState
//	gLog[12]  = gRunManager->mZone;				// 走行区間
//	gLog[13]  = gRunManager->dDist;				// 走行区間
//	gLog[14]  = gRunManager->dLine;				// 走行区間
//	gLog[15]  = ev3_battery_voltage_mV();		// バッテリー
//	gLog[10] = gRunManager->dCount;				//(kaunto)

//	gLog[0]  = gColorSensor.getBrightness();
//	gLog[1]  = gLineMonitor->getDeviation();
//	gLog[2]  = gOdmetry->getX();
//	gLog[3]  = gOdmetry->getY();
//	gLog[4]  = gOdmetry->getTheta();
//	gLog[5]  = gTracer->RightPWM;
//	gLog[6]  = gTracer->LeftPWM;
//	gLog[7]  = gTracer->mturn;
//	gLog[8]  = gTracer->tturn;
//	gLog[9]  = gTracer->tagetTheta;
//	gLog[10] = gTracer->ThetaStart;

//	rgb_raw_t rgb;
//	int t;
//	gColorSensor.getRawColor(rgb);
//	t = gColorSensor.getColorNumber();
//
//	gLog[0]  = rgb.r;
//	gLog[1]  = rgb.g;
//	gLog[2]  = rgb.b;
//	gLog[3]  = t;


#if USE_OUTPUT_CSV

	float logtime;
    Hsv hsv;
    hsv = RGBtoHSV(gLineMonitor->RGBdata);
	logtime = (float)gClock->now() / 1000.0f;
	gCSVdata[0][CSVCycle] = logtime;
	gCSVdata[1][CSVCycle] = gOdmetry->getX();
	gCSVdata[2][CSVCycle] = gOdmetry->getY();
	gCSVdata[3][CSVCycle] = gOdmetry->getTheta();
	gCSVdata[4][CSVCycle] = gOdmetry->getDeltaTheta();
	gCSVdata[5][CSVCycle] = gLineMonitor->mBright;
	gCSVdata[6][CSVCycle] = gLineMonitor->RGBdata.r;
	gCSVdata[7][CSVCycle] = gLineMonitor->RGBdata.g;
	gCSVdata[8][CSVCycle] = gLineMonitor->RGBdata.b;
	gCSVdata[9][CSVCycle] = gLineMonitor->mColorType;
	gCSVdata[10][CSVCycle] = gStateManager->mState;

	if(gStateManager->mState != StateManager::FINISH) {
		gCSVdata[11][CSVCycle] = gLineMonitor->getDeviation();
		gCSVdata[12][CSVCycle] = gTracer->RightPWM;
		gCSVdata[13][CSVCycle] = gTracer->LeftPWM;
		gCSVdata[14][CSVCycle] = gTracer->mturn;
		gCSVdata[15][CSVCycle] = gTracer->tturn;
		gCSVdata[16][CSVCycle] = gTracer->ThetaStart;
		gCSVdata[17][CSVCycle] = gTracer->tagetTheta;
	} else {
		gCSVdata[11][CSVCycle] = gLineMonitor->rgbTobright();
		gCSVdata[12][CSVCycle] = gCommandMaker->mCommand_index;
		gCSVdata[13][CSVCycle] = gCommandMaker->mCommand[gCommandMaker->mCommand_index];
		gCSVdata[14][CSVCycle] = gDriveController->mCircleState;
		gCSVdata[15][CSVCycle] = gDriveController->mSetState;
		gCSVdata[16][CSVCycle] = gDriveController->mBackState;
//		gCSVdata[17][CSVCycle] = gDriveController->mGetState;
		gCSVdata[17][CSVCycle] = hsv.h;
		gCSVdata[18][CSVCycle] = hsv.s;
		gCSVdata[19][CSVCycle] = hsv.v;
	}

	if (CSVCycle_num < CSV_CYCLE) {
		CSVCycle_num++;
	}
	CSVCycle++;
	CSVCycle = CSVCycle % CSV_CYCLE;

	if (ev3_button_is_pressed(BACK_BUTTON)) {
		wup_tsk(MAIN_TASK);  // バックボタン押下
	}

#endif


	gLogger->sendLog(gLog,LOG_NUM);				// BlueToothを使ってログをteratermに送る
}
/**
 * リモート周期タスク
 */
void ev3_cyc_remote(intptr_t exinf){
	act_tsk(REMOTE_TASK);
}
/**
 * リモートタスク
 */
void remote_task(intptr_t exinf) {

//	char debug[50] = {0};
	gRemote->getsr();										// コマンドを入力
//	sprintf(debug,"\n\ryou put: %c \n\r",gRemote->c_debug); //

	/* ロボット停止コマンド */
	if (gRemote->c == 's') {
//		if (gClock->now() > 4000) {
			wup_tsk(MAIN_TASK);
			ev3_stp_cyc(EV3_CYC_REMOTE);
//			ev3_stp_cyc(EV3_CYC_TAILMOTOR);
			ev3_stp_cyc(EV3_CYC_ODMETRY);
//			ev3_stp_cyc(EV3_CYC_LOGGER);
//		}
	}

}
/**
 * UIタスク
 */
void ui_task(intptr_t exinf){
//	gUI->putString("\n\rMENU\n\r 1.START\n\r 2.Logger\n\r 3.Remote\n\r 9.STOP\n\r");
	gUI->putString("\n\rMENU\n\r s.START\n\r");
#if USE_OUTPUT_LOG
	gUI->putString("WITH LOG\n");
#else
	gUI->putString("WITHOUT LOG\n");
#endif
	while(1){
		if( gUI->getState() == UI::LOGGER){
			// Logger起動時はMenu画面を飛ばす
		}else{
			//
		}
		gUI->setState( gUI->getChar() );
		switch( gUI->getState() ){
		case UI::START:
			gStateManager->mStartSignal = true;
			/* ↓デバッグ用 */
#if USE_REMOTE_CONTROL_MODE
			ev3_sta_cyc(EV3_CYC_REMOTE);
			//gUI->putString("\n\rREMOTE MODE\n\r");
			slp_tsk();
#endif

#if USE_OUTPUT_LOG
			/* デバッグ用 */
			gLogger->init();
			gOdmetry->clearLocation();
			ev3_sta_cyc(EV3_CYC_LOGGER);
			// gUI->putString("\n\rtime,x,y,theta,brightness,gyro,forward,turn,deltaTheta,battery,state,TailAngle,Sonar,Zone,dist,Line\n\r");
//			gUI->putString("\n\rtime,x,y,theta,deltaTheta,battery,state,sonar,Zone,dist,Line,count,gyro\n\r");
#endif
			continue;
		case UI::LOGGER:
			gLogger->init();
			gOdmetry->clearLocation();
//			  ev3_sta_cyc(EV3_CYC_LOGGER);
			// gUI->putString("\n\rtime,x,y,theta,brightness,gyro,forward,turn,deltaTheta,battery,state,TailAngle,Sonar,Zone,dist,Line\n\r");
//			gUI->putString("\n\rtime,x,y,theta,deltaTheta,battery,state,sonar,Zone,dist,Line,count,gyro\n\r");
			continue;
		case UI::REMOTE_MODE:
			 ev3_sta_cyc(EV3_CYC_REMOTE);
			 gUI->putString("\n\rREMOTE MODE\n\r");
			continue;
		case UI::FINISH:
//			ev3_stp_cyc(EV3_CYC_LOGGER);
			wup_tsk(MAIN_TASK);
			continue;
		default:
			/* ↓暫定対応：書き込み直後に表示される。おそらくバグのため要解析 */
 // 		  gUI->putString("\n\rImput 1 to 4");
			continue;
		}
	}
}
