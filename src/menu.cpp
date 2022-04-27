#include "menu.h"

extern Display display;
extern StateMachine sm;

void MenuState::enter(){
    this->lastUpdateTimestamp = millis();
    this->mode = sm.mode;
}

void MenuState::update(){
    display.drawModeOption(this->mode);
    if(millis()-this->lastUpdateTimestamp > this->threshold){
        sm.setCurrentState(sm.stateFromMode(this->mode));
    }
}

void MenuState::exit(){}

void MenuState::react(ButtonEvent *event) {
    this->lastUpdateTimestamp = millis();
    if(event->event == BUTTON_DOWN_LONG || event->event == BUTTON_DOWN_SHORT ){
        this->mode = sm.nextMode(this->mode);
        display.drawModeOption(this->mode);
    }
    else if(event->event == BUTTON_SELECT_LONG || event->event == BUTTON_SELECT_SHORT ){
        sm.setCurrentState(sm.stateFromMode(this->mode));
    }
}