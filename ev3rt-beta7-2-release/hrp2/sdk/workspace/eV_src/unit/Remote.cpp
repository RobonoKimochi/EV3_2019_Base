#include "Remote.h"
/**
 * コンストラクタ
 */
 Remote::Remote()
    //  :  mBT(fp)
:bt_cmdflg(false),
bt_cmd(10),
bt_cmdfor(0),
bt_cmdturn(0),
bt_cmdend(0),
bt_cmdspd(1),
mforward(0),
mturn(0),
mTail(0),
mArm(0)

        {
            // FILE *mBT;
            OpenSerialPort();
 }

void Remote::OpenSerialPort()
{
   mBT = ev3_serial_open_file(EV3_SERIAL_BT);
}

void Remote::getsr()
{
	c = getChar();
    c_debug = c; // デバック用
	chgSpeed();
	setSpeed();
}


void Remote::RemoteControl()
{

	color_yellow(); /* デバッグ用に光らせる */

    /* 左右輪に出力 */
	mRightWheel.setPWM(mforward - mturn);
	mLeftWheel.setPWM(mforward + mturn);

    /* アームに出力 */
	mArmMotor.setPWM(mArm);

	/* テールに出力 */
	mTailMotor.setPWM(mTail);

}

void Remote::chgSpeed()
{

	switch(c) {
    case '0':
    	bt_cmd = 1;
            break;
    default:
        break;
    }

    switch(c) {
    case '0':
        bt_cmdend++;
      	c = 10;
        break;
    default:
        break;
    }


    switch(c) {
    case '5':
        bt_cmdfor = 5;
        break;

    case '8':
        bt_cmdfor = 8;
        break;

    case '2':
         bt_cmdfor = 2;
         break;

    case '4':
         bt_cmdfor = 4;
         break;

    case '6':
         bt_cmdfor = 6;
         break;

    case 'f':
         bt_cmdfor = 20;
         break;

    case 'j':
         bt_cmdfor = 21;
         break;

    case 't':
         bt_cmdfor = 100;
         break;
    default:
         break;
    }

    switch(c) {
    case '3':
    	bt_cmdfor = 3;
         break;

    case '9':
    	bt_cmdfor = 9;
         break;
    default:
         break;
    }

    switch(c) {
    case 'd':
      	if(bt_cmdspd >1)
       	{
       		bt_cmdspd--;
       	}
       	c = 10;
           break;

    case 'u':
       	if(bt_cmdspd< (100 / SPD_LSB))
      	{
       		bt_cmdspd++;
       	}
        	c = 10;
            break;
        default:
            break;
    }

    switch(c) {
    case '7':
    	bt_cmdfor = 31;
        break;

    case '1':
    	bt_cmdfor = 32;
            break;
        default:
            break;
    }

    switch(c) {
    case 'b':
    	if(bt_cmdflg == false)
    	{
    		bt_cmdflg = true;
    		c = 10;
    	}
    	else
    	{
    		bt_cmdflg = false;       // if bt_cmdflg == true
    		c = 10;
    	}
    	mSound->ok(); /* リモート操作用に音を鳴らす */
    	break;

    default:
    	break;
    }

}

void Remote::setSpeed()
{

	if (bt_cmdfor == 5)
	{
		mforward = 0;
   		mturn = 0;
   		mTail = 0;
   		mArm = 0;
   	}
	else if(bt_cmdfor == 100)
	{
   		mTail = 0;
   		mArm = 0;
   	}
	else if(bt_cmdfor == 8)
	{
		mforward = bt_cmdspd * SPD_LSB;
   		mturn = 0;
   	}
	else if(bt_cmdfor == 2)
	{
		mforward = -bt_cmdspd * SPD_LSB;
   		mturn = 0;
   	}
	else if(bt_cmdfor == 4)
	{
		mturn = -5;
		mforward = 0;
   	}
	else if(bt_cmdfor == 6)
	{
		mturn = 5;
		mforward = 0;
   	}
	else if(bt_cmdfor == 20)
	{
		mturn = -5;
   	}
	else if(bt_cmdfor == 21)
	{
		mturn = 5;
   	}
	else if (bt_cmdfor == 3)
	{
		mTail = 20;

	}
	else if (bt_cmdfor == 9)
	{
		mTail = -20;
	}
	else if (bt_cmdfor == 31)
	{
		mArm = 2;

	}
	else if (bt_cmdfor == 32)
	{
		mArm = -2;
	}
	else
	{
		mforward = 0;
   		mturn = 0;
   		mTail = 0;
   		mArm = 0;
   	}

}


bool Remote::RemoteState()
{

	if (bt_cmdflg == true)
	{
		return (true);
	}
	else
	{
		return (false);
	}

}

void Remote::setState()
{

	bt_cmdflg = true;
	return;
}

void Remote::color_yellow()
{

	/* 緑とオレンジを交互に光らせて黄色を作る */
	counter++;
	if (counter < 5)
	{
		ev3_led_set_color(LED_GREEN);
	}
	else
	{
		ev3_led_set_color(LED_ORANGE);
		if (counter > 4)
		{
			counter = 0;
		}
	}

}

