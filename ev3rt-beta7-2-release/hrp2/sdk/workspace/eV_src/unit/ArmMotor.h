#ifndef	EV3_UNIT_ARMMOTOR_H_
#define EV3_UNIT_ARMMOTOR_H_

// #include "Motor.h"
#include "MotorDriver.h"
#include "app.h"
#include "COM.h"

#define USE_PI

class ArmMotor
{
	public:
		/**
		 * インスタンスの取得
		 */
		 static ArmMotor *getInstance(){
			 static ArmMotor ArmMotor;
			 return &ArmMotor;
		 }

		/**
		* @brief 			アームモータの初期化処理
		*
		* @param [IN]		offset：アームモータオフセット値(初期値)
		*
		* @return 			無し
		*
		* @detail 			アームモータのオフセット値の設定、制御の初期化処理を行う。
		*/
		void init( int32_t offset );

		/**
		* @brief			アームモータの制御
		*
		* @param angle		アームの目標角度
		*
		* @return			無し
		*
		* @detail			引数「angle」からアームモータの目標角度を設定
		*/
		void setAngle( int32_t angle );

		/**
		* @brief			アームモータの制御
		*
		* @param 			無し
		*
		* @return			無し
		*
		* @detail			アームモータに設定するPWM制御を現在のmAngleから算出し
		* 					制御PWM最大値以上なら最大値、最小値以下なら最小値に修正後
		*					アームモータにPWM制御値を与える。
		*/
		void moveArmMotor();
		void setPWM(int ang);

	    float getDiff();
	    bool  Complete();
		int32_t  mAngle = 0;				// モーターの角度(°)



/**
	* @brief			アームモータの角度取得
	*
	* @param 			無し
	*
	* @return			アームモータの角度
	*
	* @detail			現在のアームモータの角度を戻り値として返す。
	*/
	int32_t getAngle();
	private:
		ArmMotor();					// コンストラクタ

		int32_t  mOffset = 0;				// オフセット値
	    int      count = 0;
		bool     flag = false;
		float    mPWM = 0;					// PWM制御値
		float    mP_Gain = 0.5F;				// 比例制御ゲイン
		float	  mI_Gain = 0.1F;				// 積分ゲイン
		float    PWM_MAX = 100;				// PWM制御飽和閾値

	    ev3api::Motor mArmMotor = MotorDriver::getInstance().getArmMotor();
};
#endif	// EV3_UNIT_ARMMOTOR_H_
