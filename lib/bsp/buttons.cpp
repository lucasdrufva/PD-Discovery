#include "buttons.h"

#include <Arduino.h>
#include "pin-definitions.h"

typedef struct Button{
    int pin;
    bool state;
    unsigned long timestamp;
    void (*short_cb)();
    void (*long_cb)();
    
};

const unsigned long LONG_PRESS_THRESHOLD = 700;
#define BUTTON_COUNT 5
Button g_buttons[BUTTON_COUNT] = {
                        {.pin = PIN_UP_2, .state=false, .timestamp=0, .short_cb=nullptr, .long_cb=nullptr},
                        {.pin = PIN_UP_1, .state=false, .timestamp=0, .short_cb=nullptr, .long_cb=nullptr},
                        {.pin = PIN_DOWN_1, .state=false, .timestamp=0, .short_cb=nullptr, .long_cb=nullptr},
                        {.pin = PIN_DOWN_2, .state=false, .timestamp=0, .short_cb=nullptr, .long_cb=nullptr},
                        {.pin = PIN_SELECT, .state=false, .timestamp=0, .short_cb=nullptr, .long_cb=nullptr}
                    };

void setup_buttons(){
    for(int i = 0; i <BUTTON_COUNT; i++){
        pinMode(g_buttons[i].pin, INPUT);
    }
}

unsigned long g_buttons_last_timestamp = 0;

void run_buttons()
{
    if(millis()-g_buttons_last_timestamp < 100)
        return;


    g_buttons_last_timestamp = millis();

    for(int i = 0; i < BUTTON_COUNT; i++)
    {
        bool new_state = digitalRead(g_buttons[i].pin);
        if(new_state != g_buttons[i].state)
        {
            if(new_state == false)  //Button released
            {
                if(millis()-g_buttons[i].timestamp > LONG_PRESS_THRESHOLD)
                {
                    if(g_buttons[i].long_cb != nullptr)
                        g_buttons[i].long_cb();
                }
                else
                {
                    if(g_buttons[i].short_cb != nullptr)
                        g_buttons[i].short_cb();
                }
            }

            g_buttons[i].timestamp = millis();
            g_buttons[i].state = new_state;
        }
    }
}

void button_register_cb(int pin, ButtonPressType type, void (*cb)())
{
    for( int i = 0; i < BUTTON_COUNT; i++ )
    {
        if( g_buttons[i].pin != pin )
            continue;
        
        if( type == SHORT )
            g_buttons[i].short_cb = cb;
        else if( type == LONG )
            g_buttons[i].long_cb = cb;

        return;
    }
}



