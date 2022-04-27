#pragma once
#include <Arduino.h>

#include "pin-definitions.h"

namespace Board
{
    namespace Charger {}
    namespace Source {}

    uint16_t readVbusMilliVolt();
    uint16_t readBatteryMilliVolt();

    void enableOutputSrc();
    void disableOutputSrc();
    void enableOutputVoltage();
    void disableOutputVoltage();
    void setOutputSrc5v();
    void setOutputSrc9v();
    void setOutputSrc12v();
}