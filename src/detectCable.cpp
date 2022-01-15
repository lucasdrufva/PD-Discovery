#include "detectCable.h"


extern StateMachine sm;
extern Display display;

void DetectCableState::enter()
{
    Serial.println("Detect Cable entered");
    if (sm.mode == CABLE)
    {
        detectMode = TCPM_DETECTION_RA;
        nextState = &(CableModeState::getInstance());
    }
    else if (sm.mode == PS)
    {
        detectMode = TCPM_DETECTION_RP;
        nextState = &(PSModeState::getInstance());
    }
}

void DetectCableState::exit()
{
    Serial.println("Detect cable exit");
}

void DetectCableState::update()
{
    Serial.println("Detect Cable update");
    if ((fusb.getConnection() & detectMode))
    {
        sm.connected = true;
        sm.setCurrentState(*nextState);
    }else {
        display.drawWaitConnection();
    }
}
