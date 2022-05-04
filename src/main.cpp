#include <Arduino.h>

#include <Wire.h>

#include "fusb302.h"
#include "oldtcpm.h"
#include "USB-PD.h"
#include "display.h"
#include "menu.h"
#include "buttons.h"
#include "pin-definitions.h"

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

void sourceCapRecieved(PD::Capabilities cap){
  display.drawPS(cap);
  button_register_cb(PIN_SELECT, SHORT, *main_menu);
}

void analyzeSource(){
  fusb.setIsSource(false);

  while(true)
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

void analyzeCable(){
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  fusb.setIsSource(true);
  delay(100);

  while(true)
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
const Menu_item MAIN_MENU_ITEMS[MAIN_MENU_SIZE] = {
  {.title = "Analyze Cable", .fn_ptr=*analyzeCable},
  {.title = "Analyze Source", .fn_ptr=*analyzeSource},
  {.title = "Analyze Sink", .fn_ptr=nullptr},
};
void main_menu(){
  pd.reset();
  start_menu(MAIN_MENU_ITEMS, MAIN_MENU_SIZE);
}

void setup()
{
  Wire.begin();
  display.begin();

  setup_buttons();

  attachInterrupt(2, isr, FALLING);

  pd.begin(&fusb);

  main_menu();
  button_register_cb(PIN_SELECT, LONG, *main_menu);
}

unsigned long updateTime;

void loop()
{
  if (interruptFlag)
  {
    Serial.println("Interrupt recieved");
    fusb.handleInterrupt();
    interruptFlag = false;
  }

  run_buttons();
}