#include "board/board.h"

uint16_t Board::readVbusMilliVolt(){
    int raw = analogRead(A3);
    int voltage = (((raw*1000) / 1024) * 22 );
    return (uint16_t) voltage;
}

uint16_t Board::readBatteryMilliVolt(){
    int raw = analogRead(A0);
    int voltage = (((raw*1000) / 1024) * 631 ) / 100;
    return (uint16_t) voltage;
}

//TODO init pins
void Board::enableOutputSrc(){
    pinMode(29, OUTPUT);
    digitalWrite(29, HIGH);
}

void Board::disableOutputSrc(){
    pinMode(29, OUTPUT);
    digitalWrite(29, LOW);
}

void Board::enableOutputVoltage(){
    pinMode(21, OUTPUT);
    digitalWrite(21, HIGH);
}

void Board::disableOutputVoltage(){
    pinMode(21, OUTPUT);
    digitalWrite(21, LOW);
}

void Board::setOutputSrc5v(){
    pinMode(31, OUTPUT);
    pinMode(28, OUTPUT);
    digitalWrite(31, LOW);
    digitalWrite(28, LOW);
}

void Board::setOutputSrc9v(){
    digitalWrite(31, HIGH);
    digitalWrite(28, LOW);
}

void Board::setOutputSrc12v(){
    digitalWrite(31, HIGH);
    digitalWrite(28, HIGH);
}