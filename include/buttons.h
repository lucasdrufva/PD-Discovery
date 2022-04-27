#pragma once
#include <Arduino.h>

#include "fsm.h"
#include "board/pin-definitions.h"

enum ButtonPressType{
    BUTTON_UP_SHORT,
    BUTTON_UP_LONG,
    BUTTON_DOWN_SHORT,
    BUTTON_DOWN_LONG,
    BUTTON_SELECT_SHORT,
    BUTTON_SELECT_LONG
};

struct ButtonEvent: EventData{
  ButtonPressType event;
};

void setupButtons();

void readButtons();