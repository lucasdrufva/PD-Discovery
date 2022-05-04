#include <BSP.h>

uint16_t BSP::readVbusMilliVolt(){
    int raw = analogRead(A3);
    int voltage = (((raw*1000) / 1024) * 22 );
    return (uint16_t) voltage;
}

uint16_t BSP::readBatteryMilliVolt(){
    int raw = analogRead(A0);
    int voltage = (((raw*1000) / 1024) * 631 ) / 100;
    return (uint16_t) voltage;
}

//TODO init pins
void BSP::enableOutputSrc(){
    pinMode(29, OUTPUT);
    digitalWrite(29, HIGH);
}

void BSP::disableOutputSrc(){
    pinMode(29, OUTPUT);
    digitalWrite(29, LOW);
}

void BSP::enableOutputVoltage(){
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);
}

void BSP::disableOutputVoltage(){
    pinMode(21, OUTPUT);
    digitalWrite(21, LOW);
}

void BSP::setOutputSrc5v(){
    pinMode(31, OUTPUT);
    pinMode(28, OUTPUT);
    digitalWrite(31, LOW);
    digitalWrite(28, LOW);
}

void BSP::setOutputSrc9v(){
    digitalWrite(31, HIGH);
    digitalWrite(28, LOW);
}

void BSP::setOutputSrc12v(){
    digitalWrite(31, HIGH);
    digitalWrite(28, HIGH);
}

void BSP::enableVconn(){
    pinMode(PIN_ENABLE_VCONN, OUTPUT);
    digitalWrite(PIN_ENABLE_VCONN, HIGH);
}

void BSP::disableVconn(){
    pinMode(PIN_ENABLE_VCONN, OUTPUT);
    digitalWrite(PIN_ENABLE_VCONN, LOW);
}