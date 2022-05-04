#pragma once
#include <Arduino.h>

#include "board.h"

namespace Charger 
{
    void begin();
    void enable();
    void disable();
    bool checkSafeVoltage();
    bool status();
}