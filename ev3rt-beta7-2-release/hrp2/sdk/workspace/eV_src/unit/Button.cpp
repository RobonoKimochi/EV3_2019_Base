/******************************************************************************
 *  Button.cpp (for LEGO Mindstorms EV3)
 *  Created on: 2015/01/25
 *  Implementation of the Class Starter
 *  Author: Kazuhiro.Kawachi
 *  Copyright (c) 2015 Embedded Technology Software Design Robot Contest
 *****************************************************************************/

#include "Button.h"

/**
 * コンストラクタ
 * @param touchSensor タッチセンサ
 */
Button::Button(const ev3api::TouchSensor& touchSensor)
    : mTouchSensor(touchSensor) {
}

/**
 * デストラクタ
 */
Button::~Button() {
}

/**
 * 押下中か否か
 * @retval true  押下している
 * @retval false 押下していない
 */
bool Button::isPushed() const
{
    return mTouchSensor.isPressed();
}
