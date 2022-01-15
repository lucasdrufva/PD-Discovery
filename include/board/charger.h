#pragma once
#include <Arduino.h>

#include "pin-definitions.h"

namespace Charger 
{
    void begin();
    void enable();
    void disable();
    bool checkSafeVoltage();
}