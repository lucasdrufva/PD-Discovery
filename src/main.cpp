#include <Arduino.h>
#include <USB-PD.h>
#include "fsm.h"
#include "display.h"
#include "detectCable.h"
#include "buttons.h"
#include "board/board.h"

Display display;
StateMachine sm;
FUSB302 fusb;
extern PD_Engine pd;

unsigned long updateTime;

volatile bool interruptFlag = false;

void isr()
{
  interruptFlag = true;
}

void setup()
{
  // Start board
  Serial.begin(115200);
  Wire.begin();
  // TODO: power things
  
  // Start USB PD
  attachInterrupt(2, isr, FALLING);
  pd.begin(&fusb);

  // Start user interface, ui
  display.begin();
  setupButtons();
  Serial.println("Display started");

  // Start state machine
  sm.setCurrentState(CableModeState::getInstance());

  analogReadResolution(10);
}

void loop()
{
  if (interruptFlag)
  {
    Serial.println("Interrupt recieved");
    fusb.handleInterrupt();
    interruptFlag = false;
  }

  if (millis() - updateTime > 500)
  {
    sm.update();

    readButtons();

    Serial.println("Idle");
    updateTime = millis();
  }
}