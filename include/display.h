#pragma once
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "fsm.h"


class Display{
public:
    void begin();
    void drawModeBar();
    void drawWaitConnection();
    void drawCableDisconnect();
    void drawCable(PD::Identity id);
    void drawPS(PD ::Capabilities cap);
    void drawCharging(uint16_t VBusVoltage);

    void debug();

private:
    Adafruit_SSD1306 oled = Adafruit_SSD1306(128, 32, &Wire, -1);
    int animationFrame = 0;
    int scroll = 0;
};