#include "ArmMotor.h"
#include <math.h>

/**
 * コンストラクタ
 */

ArmMotor::ArmMotor()
	  {
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
void ArmMotor::init( int32_t offset )
{
	mArmMotor.reset();	// モータリセット
	mOffset = offset;	// アームモータのオフセット値設定

	return;				// 【戻り値】無し
}

/**
 * @brief			アームモータの制御
 *
 * @param angle		アームの目標角度
 *
 * @return			無し
 *
 * @detail			引数「angle」からアームモータの目標角度を設定
 */

void ArmMotor::setAngle( int32_t angle )
{
	mAngle = angle;
	return;	// 【戻り値】無し
}

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

void ArmMotor::moveArmMotor()
{
	float s_err;	// 偏差
	float s_p,s_i;
	static float s_integral,s_preerr;

	s_err =  mAngle + mOffset - mArmMotor.getCount()  ;

	s_p = s_err * mP_Gain;
	s_i = s_integral * mI_Gain;

#ifdef USE_PI
	mPWM = s_p + s_i;
#else
	mPWM = s_p ;
#endif

	// 累積積分
	s_preerr = s_err;
	s_integral = s_integral + (s_err + s_preerr) / 2.0 * 0.004;

	// 目標に達したら累積積分リセット
	if ( fabs(s_err) < 2.0f)
	{
		s_integral = 0;
	}

//	mPWM = (float)(( mAngle + mOffset - mArmMotor.getCount() ) * mP_Gain );	// 比例制御

	// PWM出力飽和処理
	if( mPWM > PWM_MAX )		// 制御PWM絶対最大値より大きい？
	{
		mPWM = PWM_MAX;			// 最大値設定(最大値を超えさせない)
	}
	else if( mPWM < -PWM_MAX )	// 制御PWM絶対最大値より小さい？
	{
		mPWM = -PWM_MAX;		// 最小値設定(最小値未満にさせない)
	}

	mArmMotor.setPWM( mPWM );	// アームモータのPWM制御値を設定

	return;	// 【戻り値】無し
}

float ArmMotor::getDiff(){

	return(mP_Gain - mArmMotor.getCount()) ;
}


bool ArmMotor::Complete(){

	if (mPWM < 1.0f) {
		count++;
	} else {
		count = 0;
	}

	if (count >20) {
		flag = true;
	} else {
		flag = false;
	}
	return(flag) ;
}

/**
 * @brief			アームモータの角度取得
 *
 * @param angle		無し
 *
 * @return			アームモータの角度
 *
 * @detail			現在のアームモータの角度を戻り値として返す。
 */

int32_t ArmMotor::getAngle()
{
	return	mArmMotor.getCount();	// 【戻り値】アームモータの角度
}

void ArmMotor::setPWM(int ang)
{
	mArmMotor.setPWM(ang);	// 【戻り値】アームモータの角度
}
