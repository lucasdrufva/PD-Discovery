#include "buttons.h"

extern StateMachine sm;

void setupButtons(){
    pinMode(0, INPUT);
}

void readButtons(){
    if(digitalRead(0)){
        Event e;
        e.type = CHANGE_MODE;
        sm.sendEvent(&e);
    }
}