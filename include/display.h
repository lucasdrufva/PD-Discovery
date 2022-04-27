#pragma once
#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "fsm.h"


class Display{
public:
    void begin();
    void drawModeBar();
    void drawModeOption(Mode mode);
    void drawWaitConnection();
    void drawCableDisconnect();
    void drawCable(PD::Identity id);
    void drawPS(PD ::Capabilities cap);
    void drawCharging(uint16_t VBusVoltage, uint16_t batteryVoltage, bool status);

    void debug();

private:
    Adafruit_SSD1306 oled = Adafruit_SSD1306(128, 64, &Wire, -1);
    int animationFrame = 0;
    int scroll = 0;
};