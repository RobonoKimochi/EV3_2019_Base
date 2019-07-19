#ifndef EV3_UNIT_LEDCOLOR_H_
#define EV3_UNIT_LEDCOLOR_H_

#include "ev3api.h"
#include "app_config.h"

class LEDColor
{
    public:
        static LEDColor *getInstance(){
            static LEDColor LEDColor;
            return &LEDColor;
        }

	void LEDYellow();
	void LED_RedBlack();

    private:
	int32_t   YCounter;
	int32_t   RBCounter;

};

#endif  // EV3_UNIT_LEDCOLOR_H_
