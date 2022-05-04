#pragma once

enum ButtonPressType{
    SHORT,
    LONG,
};

void setup_buttons();
void run_buttons();
void button_register_cb(int pin, ButtonPressType type, void (*cb)());