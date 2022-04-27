#include "buttons.h"

extern StateMachine sm;

typedef struct Button{
    int pin;
    bool state;
    unsigned long timestamp;
    ButtonPressType event;
};

const int buttonCount = 5;
Button buttons[buttonCount] = {
                        {.pin = PIN_UP_2, .state=false, .timestamp=0},
                        {.pin = PIN_UP_1, .state=false, .timestamp=0},
                        {.pin = PIN_DOWN_1, .state=false, .timestamp=0},
                        {.pin = PIN_DOWN_2, .state=false, .timestamp=0},
                        {.pin = PIN_SELECT, .state=false, .timestamp=0}
                    };

const unsigned long longPressThreshold = 700;

void setupButtons(){
    for(int i = 0; i <buttonCount; i++){
        pinMode(buttons[i].pin, INPUT);
    }
}

unsigned long lastReadTimestamp = 0;
// uint8_t lastState = 0b0000;




void readButtons(){
    if(millis()-lastReadTimestamp < 100){
        return;
    }
    lastReadTimestamp = millis();

    Event e;
    e.type = BUTTON;
    ButtonEvent data;
    e.data = &data;

    for(int i = 0; i <buttonCount; i++){
        bool newState = digitalRead(buttons[i].pin);
        if(newState != buttons[i].state){
            if(buttons[i].state == true){
                if(millis()-buttons[i].timestamp > longPressThreshold){
                    if(i == 0){
                        data.event = BUTTON_UP_LONG;
                    }else if(i == 3){
                        data.event = BUTTON_DOWN_LONG;
                    }else if(i == 4){
                        data.event = BUTTON_SELECT_LONG;
                    }
                    sm.sendEvent(&e);
                }else{
                    if(i == 1){
                        data.event = BUTTON_UP_SHORT;
                    }else if(i == 2){
                        data.event = BUTTON_DOWN_SHORT;
                        buttons[3].state = false;
                    }else if(i == 4){
                        data.event = BUTTON_SELECT_SHORT;
                    }
                    sm.sendEvent(&e);
                }
            }

            buttons[i].timestamp = millis();
            buttons[i].state = newState;
        }
    }
    // if(digitalRead(0)){
    //     Event e;
    //     e.type = CHANGE_MODE;
    //     sm.sendEvent(&e);
    // }
    // if(millis()-lastRead < 1500){
    //     return;
    // }

    // lastRead = millis();

    // Event e;
    // e.type = BUTTON;
    // ButtonEvent data;

    // uint8_t currentState = 0b0000;

    // currentState += digitalRead(PIN_UP_2) << 0;
    // currentState += digitalRead(PIN_UP_1) << 1;
    // currentState += digitalRead(PIN_DOWN_1) << 2;
    // currentState += digitalRead(PIN_DOWN_2) << 3;

    // if(currentState&(1<<0) == 1 && lastState&(1<<0) == 1){
    //     data.event = BUTTON_UP_LONG;
    //     e.data = &data;
    //     sm.sendEvent(&e);
    // }
    // else if(currentState&(1<<3) == 1 && lastState&(1<<3) == 1){
    //     data.event = BUTTON_DOWN_LONG;
    //     e.data = &data;
    //     sm.sendEvent(&e);
    // }
    // else if(currentState&(1<<1) == 0 && lastState&(1<<1) == 1){
    //     data.event = BUTTON_UP_SHORT;
    //     e.data = &data;
    //     sm.sendEvent(&e);
    // }
    // else if(currentState&(1<<2) == 0 && lastState&(1<<2) == 1){
    //     data.event = BUTTON_DOWN_SHORT;
    //     e.data = &data;
    //     sm.sendEvent(&e);
    // }

    // lastState = currentState;
}