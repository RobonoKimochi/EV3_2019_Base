#include "LEDColor.h"


/**
 * コンストラクタ
 */
// LEDColor::LEDColor()
//     	{
// }
// }


void LEDColor::LEDYellow()
{

	/* 緑とオレンジを交互に光らせて黄色を作る */
	YCounter++;
	if (YCounter < 5)
	{
		ev3_led_set_color(LED_GREEN);
	}
	else
	{
		ev3_led_set_color(LED_ORANGE);
		if (YCounter > 4)
		{
			YCounter = 0;
		}
	}

}

void LEDColor::LED_RedBlack()
{

	/* 赤と黒の点滅 */
	RBCounter++;
	if (RBCounter < 5)
	{
		ev3_led_set_color(LED_RED);
	}
	else
	{
		ev3_led_set_color(LED_OFF);
		if (RBCounter > 10)
		{
			RBCounter = 0;
		}
	}

}
