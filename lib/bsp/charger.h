#pragma once
#include <Arduino.h>

#include "bsp.h"

namespace Charger 
{
    void begin();
    void enable();
    void disable();
    bool checkSafeVoltage();
    bool status();
}