#include "chargeMode.h"

extern StateMachine sm;
extern Display display;

void ChargeModeState::enter()
{
    sm.mode = CHARGE;
    //Charger::enable();
}

void ChargeModeState::exit() 
{
    //Charger::disable();
}


void ChargeModeState::update()
{
    //uint16_t VbusMilliVolt = Board::readVbusMilliVolt();
    uint16_t VbusMilliVolt = 100;

    if (VbusMilliVolt > 100){
        display.drawCharging(VbusMilliVolt);
    }else {
        display.drawWaitConnection();
    }
    
}