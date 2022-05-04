#include "charger.h"

void Charger::enable(){
    // TODO: Check safe voltage
    pinMode(8, OUTPUT);
    digitalWrite(8, HIGH);
}

void Charger::disable(){
    digitalWrite(8, LOW);
}

bool Charger::status(){
    pinMode(30, INPUT);
    return !digitalRead(30);
}