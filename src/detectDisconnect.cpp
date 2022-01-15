#include "detectDisconnect.h"

extern StateMachine sm;
extern Display display;

void DetectDisconnectState::update(){
    if(fusb.getConnection() == 0){
        display.drawCableDisconnect();
        delay(100);
        sm.setCurrentState(DetectCableState::getInstance());
    }
}