#include <Arduino.h>

#include <Wire.h>
#include <stdlib.h>

#include "fusb302.h"
#include "oldtcpm.h"
#include "USB-PD.h"
#include "display.h"
#include "menu.h"
#include "buttons.h"
#include "pin-definitions.h"
#include "bsp.h"

Display display;

FUSB302 fusb;
extern PD_Engine pd;

volatile bool interruptFlag = false;
void isr()
{
  interruptFlag = true;
}

// Forward declaration
void main_menu();

void sinkCapRecieved(PD::Capabilities cap)
{
  display.drawPS(cap);
  button_register_cb(PIN_SELECT, SHORT, *main_menu);
}

void analyzeSink()
{
  pd.registerCapCB(*sinkCapRecieved);
  pinMode(PIN_ENABLE_VCONN, OUTPUT);
  digitalWrite(PIN_ENABLE_VCONN, HIGH);
  fusb.setIsSource(true);
  delay(100);

  while (true)
  {
    display.drawWaitConnection();
    if ((fusb.getConnection() & TCPM_DETECTION_RD))
    {
      break;
    }
  }

  delay(20);

  // Every source that provides power must support vSave5V, see USB PD rev2.0 6.4.1.2.3
  PD::PDO::PDO baseDataObject;
  baseDataObject.type = PD::PDO::Types::SOURCE_FIXED;
  baseDataObject.sourceFixed.voltage = 5000;
  baseDataObject.sourceFixed.current = 100;
  baseDataObject.sourceFixed.unconstrained = false;

  PD::Capabilities sourceCap;
  sourceCap.dataObjects[0] = baseDataObject;
  sourceCap.length = 1;

  // Sources must announce capapabilities on new connection
  pd.sendSourceCap(sourceCap);

  delay(100);

  pd.requestSinkCap(*sinkCapRecieved);
}

void handlePDO0()
{
  // TODO request power should first send get source cap then immediately respond with requested pdo
  pd.requestPower(0);
}
void handlePDO1() { pd.requestPower(1); }
void handlePDO2() { pd.requestPower(2); }
void handlePDO3() { pd.requestPower(3); }
void handlePDO4() { pd.requestPower(4); }
void handlePDO5() { pd.requestPower(5); }
void handlePDO6() { pd.requestPower(6); }
void handlePDO7() { pd.requestPower(7); }

void (*handlePDO[8])() = {
    *handlePDO0,
    *handlePDO1,
    *handlePDO2,
    *handlePDO3,
    *handlePDO4,
    *handlePDO5,
    *handlePDO6,
    *handlePDO7,
};

// TODO refactor size constant to max PDO constant
Menu_item psMenuItems[9];

void sourceCapRecieved(PD::Capabilities cap)
{
  char titles[8][13];
  for (int i = 0; i < cap.length; i++)
  {
    // TODO support other types than source fixed
    PD::PDO::SourceFixed data = cap.dataObjects[i].sourceFixed;
    // TODO change from mA to A
    sprintf(titles[i], "%dV - %dmA", data.voltage / 1000, data.current);
    psMenuItems[i] = {.title = titles[i], .fn_ptr = handlePDO[i]};
  }
  psMenuItems[cap.length] = {.title = "Back", .fn_ptr = *main_menu};

  start_menu(psMenuItems, cap.length + 1);
}

void analyzeSource()
{
  fusb.setIsSource(false);

  while (true)
  {
    display.drawWaitConnection();
    if ((fusb.getConnection() & TCPM_DETECTION_RP))
    {
      break;
    }
  }

  pd.requestSourceCap(*sourceCapRecieved);
  fusb.requestSourceCap();
}

void identityRecieved(PD::Identity id)
{
  display.drawCable(id);
  button_register_cb(PIN_SELECT, SHORT, *main_menu);
}

void analyzeCable()
{
  pinMode(PIN_ENABLE_VCONN, OUTPUT);
  digitalWrite(PIN_ENABLE_VCONN, HIGH);
  fusb.setIsSource(true);
  delay(100);

  while (true)
  {
    display.drawWaitConnection();
    if ((fusb.getConnection() & TCPM_DETECTION_RA))
    {
      break;
    }
  }

  delay(500);

  pd.requestIdentity(PD::Destination::SOP_PRIME, *identityRecieved);
}

#define MAIN_MENU_SIZE 3
Menu_item MAIN_MENU_ITEMS[MAIN_MENU_SIZE] = {
    {.title = "Analyze Cable", .fn_ptr = *analyzeCable},
    {.title = "Analyze Source", .fn_ptr = *analyzeSource},
    {.title = "Analyze Sink", .fn_ptr = *analyzeSink},
};
void main_menu()
{
  pd.reset();
  start_menu(MAIN_MENU_ITEMS, MAIN_MENU_SIZE);
}

void setup()
{
  Wire.begin();
  display.begin();

  setup_buttons();

  // configure FUSB external interrupt
  attachInterrupt(2, isr, FALLING);

  pd.begin(&fusb);

  main_menu();
  button_register_cb(PIN_SELECT, LONG, *main_menu);
}

void loop()
{
  if (interruptFlag)
  {
    fusb.handleInterrupt();
    interruptFlag = false;
  }

  run_buttons();
}