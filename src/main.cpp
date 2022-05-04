#include <Arduino.h>

#include <Adafruit_SSD1306.h>
#include <Wire.h>

#include "fusb302.h"
#include "oldtcpm.h"
#include "USB-PD.h"
#include "display.h"

namespace PD
{

}

Display display;

// Adafruit_SSD1306 oled = Adafruit_SSD1306(128, 64, &Wire, -1);


const char *MENU_ITEMS[] = {"Cable", "Source", "Sink", 0};

int g_menu_index;



FUSB302 fusb;
extern PD_Engine pd;

volatile bool interruptFlag = false;
void isr()
{
  interruptFlag = true;
}

void sourceCapRecieved(PD::Capabilities cap){
  display.drawPS(cap);
    // oled.clearDisplay();
    // oled.println("V reee");
    // oled.display();
    // oled.println(cap.dataObjects[0].sourceFixed.voltage);
    // oled.display();
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
    // oled.clearDisplay();
    // oled.println("C reee");
    // oled.display();
    // oled.println(id.passiveCable.vbusCurrent);
    // oled.display();
}

void analyzeCable(){
  pinMode(3, OUTPUT);
  digitalWrite(3, HIGH);
  fusb.setIsSource(true);
  delay(100);

  // pd.requestIdentity(PD::Destination::SOP_PRIME, *identityRecieved);

  // oled.clearDisplay();
  // oled.println("Connect cable...");
  // oled.display();

  while(true)
  {
    display.drawWaitConnection();
    if ((fusb.getConnection() & TCPM_DETECTION_RA))
    {
        break;
    }
  }

  // oled.clearDisplay();
  // oled.print("Cable detected");
  // oled.display();

  // display.debug();
  delay(500);

  // Serial.println("Cable detected");
  // uint32_t VDM_header = 0;
  //   VDM_header |= PD::VDM::DISCOVER_IDENTITY;
  //   VDM_header |= PD::VDM::COMMAND_TYPE_REQ;
  //   VDM_header |= PD::VDM::STRUCTURED;
  //   VDM_header |= PD::VDM::STRUCTURED_VERSION;
  //   VDM_header |= PD::VDM::POWER_DELIVERY_SVID;

  //   uint16_t header = 0;

  //   //Message type: VDM
  //   header |= 0x0F;
  //   //Revision
  //   header |= 1 << 6;
  //   //Cable plug role
  //   header |= 0 << 8;
  //   //Message id
  //   header |= (fusb.getMessageID() & 0x07) << 9;
  //   //Number of data objects
  //   header |= 1 << 12;

  //   fusb.sendMessage(header, &VDM_header, PD::Destination::SOP_PRIME);

  pd.requestIdentity(PD::Destination::SOP_PRIME, *identityRecieved);

  
}

void setup()
{
  // put your setup code here, to run once:
  Wire.begin();

  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate oled voltage from 3.3V internally
  // if (!oled.begin(SSD1306_EXTERNALVCC, 0x3C))
  // {
  //   while (true)
  //   {
  //     Serial.println("SSD1306 allocation failed");
  //     delay(5000);
  //   }
  // }

  display.begin();

  pinMode(19, INPUT);
  pinMode(9, INPUT);

  // oled.drawLine(0, 0, 0, 10, 1);
  // oled.drawLine(0, 0, 5, 0, 1);
  // oled.drawLine(0, 10, 5, 10, 1);

  // oled.setTextSize(1);
  // oled.setTextColor(WHITE);
  // oled.setCursor(6, 2);
  // oled.println("First Option - 5V");

  // oled.display();
  
  // oled.ssd1306_command(SSD1306_SETCONTRAST);
  // oled.ssd1306_command(255);

  attachInterrupt(2, isr, FALLING);

  pd.begin(&fusb);
}

unsigned long updateTime;

bool done = false;

void loop()
{
  // put your main code here, to run repeatedly:
  if (interruptFlag)
  {
    Serial.println("Interrupt recieved");
    fusb.handleInterrupt();
    interruptFlag = false;
  }

  if(!done)
  {

  if (millis() - updateTime > 300)
  {
    display.drawMenu(MENU_ITEMS, g_menu_index);

    if(digitalRead(19))
    {
      g_menu_index++;
    }

    if(digitalRead(9))
    {
      if(g_menu_index == 0)
      {
        analyzeSource();
        done = true;
      }else if(g_menu_index == 1){
        analyzeCable();
        done = true;
      }
    }


    updateTime = millis();
  }

  }




}