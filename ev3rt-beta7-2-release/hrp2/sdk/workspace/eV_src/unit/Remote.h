#ifndef EV3_UNIT_REMOTE_H_
#define EV3_UNIT_REMOTE_H_
#include "ev3api.h"
#include "BlueTooth.h"
#include "Sound.h"
#include "ev3api.h"
#include "MotorDriver.h"
#include "LEDColor.h"

class Remote : public BlueTooth
{
public:
	Remote();
	void RemoteControl();
	void OpenSerialPort();
	bool RemoteState();
	bool bt_cmdflg;
	void setState();
	void getsr();

    FILE *mBT;
	uint8_t c_debug; // !! デバック用;
	uint8_t c;
private:
    // FILE *mBT;
#define SPD_LSB 5
	void chgSpeed();
	void setSpeed();
	Sound *mSound = Sound::getInstance();
	ev3api::Motor mRightWheel = MotorDriver::getInstance().getRightWheel();
    ev3api::Motor mLeftWheel  = MotorDriver::getInstance().getLeftWheel();
	ev3api::Motor mArmMotor   = MotorDriver::getInstance().getArmMotor();
    ev3api::Motor mTailMotor  = MotorDriver::getInstance().getTailMotor();

	int32_t   bt_cmd;      /* Bluetoothコマンド 1:リモートスタート */
	int32_t   bt_cmdfor;      /* Bluetoothコマンド 2:リモートスタート */
	int32_t   bt_cmdturn;      /* Bluetoothコマンド 3:リモートスタート */
	int32_t   bt_cmdend;      /* Bluetoothコマンド 4:リモートスタート */
	int32_t   bt_cmdspd;      /* Bluetoothコマンド 4:リモートスタート */
	int32_t   mforward;
	int32_t   mturn;
	int32_t   mTail;
	int32_t   mArm;
	int32_t   counter;
};
#endif  // EV3_UNIT_BLUETOOTH_H_
