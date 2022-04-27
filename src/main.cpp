#include <Arduino.h>
#include <USB-PD.h>
#include "fsm.h"
#include "display.h"
#include "detectCable.h"
#include "menu.h"
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
  pinMode(10, OUTPUT);
  
  // Start USB PD
  attachInterrupt(2, isr, FALLING);
  pd.begin(&fusb);

  // Start user interface, ui
  display.begin();
  setupButtons();
  Serial.println("Display started");

  // Start state machine
  sm.setCurrentState(MenuState::getInstance());

  analogReadResolution(10);

  Board::disableOutputSrc();
}

void loop()
{
  // if (interruptFlag)
  // {
  //   Serial.println("Interrupt recieved");
  //   fusb.handleInterrupt();
  //   interruptFlag = false;
  // }

  if (millis() - updateTime > 200)
  {
    sm.update();

    readButtons();

    Serial.println("Idle");
    updateTime = millis();
  }

  // Board::enableOutputVoltage();
  // Board::setOutputSrc5v();
  // delay(100);
  // Board::enableOutputSrc();
  // delay(1000);
  // Board::disableOutputSrc();
  // delay(1000);
  // Board::setOutputSrc9v();
  // delay(100);
  // Board::enableOutputSrc();
  // delay(1000);
  // Board::disableOutputSrc();
  // delay(1000);
  // Board::setOutputSrc12v();
  // delay(100);
  // Board::enableOutputSrc();
  // delay(1000);
  // Board::disableOutputSrc();
  // delay(1000);
}