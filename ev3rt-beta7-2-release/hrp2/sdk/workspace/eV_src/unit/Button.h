/******************************************************************************
 *  Button.h (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Implementation of the Class Starter
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#ifndef EV3_UNIT_BUTTON_H_
#define EV3_UNIT_BUTTON_H_

#include "TouchSensor.h"

class Button {
public:
    explicit Button(const ev3api::TouchSensor& touchSensor);
    virtual ~Button();

    bool isPushed() const;

private:
    const ev3api::TouchSensor& mTouchSensor;
};

#endif  // EV3_UNIT_BUTTON_H_
