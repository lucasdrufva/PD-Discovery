#pragma once
#include <Arduino.h>

#include "pin-definitions.h"

namespace Board
{
    namespace Charger {}

    uint16_t readVbusMilliVolt();
}