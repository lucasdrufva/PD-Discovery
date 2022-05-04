#include "display.h"

#include "menu.h"

// extern StateMachine sm;

void Display::debug()
{
    // oled.clearDisplay();
    // pinMode(1, INPUT);
    // int up1 = digitalRead(1);
    // pinMode(20, INPUT);
    // int up2 = digitalRead(20);
    // oled.setCursor(6, 2);
    // if(up1){
    //     oled.println("Up1 ");
    // }
    // if(up2){
    //     oled.println("Up 2");
    // }
    oled.clearDisplay();
    oled.print("debug");
    oled.display();
    delay(500);
    //delay(100);
}

void Display::begin()
{

    Serial.println("Try connect to oled");

    // SSD1306_SWITCHCAPVCC = generate oled voltage from 3.3V internally
    if (!oled.begin(SSD1306_EXTERNALVCC, 0x3C))
    {
        while (true)
        {
            Serial.println("SSD1306 allocation failed");
            delay(5000);
        }
    }

    oled.display();

    // Clear the buffer
    oled.clearDisplay();

    // Draw a single pixel in white
    //oled.drawPixel(10, 10, SSD1306_WHITE);

    // Show the oled buffer on the screen. You MUST call oled() after
    // drawing commands to make them visible on screen!
    oled.display();
    //delay(500);

    oled.clearDisplay();

    oled.drawLine(0, 0, 0, 10, 1);
    oled.drawLine(0, 0, 5, 0, 1);
    oled.drawLine(0, 10, 5, 10, 1);

    oled.setTextSize(1);
    //oled.setTextColor(WHITE);
    oled.setCursor(6, 2);
    oled.println("First Option - 5V");

    oled.drawLine(120, 0, 120, 10, 1);
    oled.drawLine(115, 0, 120, 0, 1);
    oled.drawLine(115, 10, 120, 10, 1);

    oled.drawLine(0, 14, 0, 24, 1);
    oled.drawLine(0, 14, 5, 14, 1);
    oled.drawLine(0, 24, 5, 24, 1);

    oled.setTextSize(1);
    oled.setTextColor(WHITE);
    oled.setCursor(6, 16);
    oled.println("Second Option - 9V");

    oled.drawLine(120, 14, 120, 24, 1);
    oled.drawLine(115, 14, 120, 14, 1);
    oled.drawLine(115, 24, 120, 24, 1);

    oled.display();

    oled.ssd1306_command(SSD1306_SETCONTRAST);
    oled.ssd1306_command(255);
}

void Display::drawCable(PD::Identity id)
{
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setCursor(0, 0);
    oled.print(id.passiveCable.vbusCurrent == PD::A_5 ? "5" : "3");
    oled.println("A");
    oled.print("Latency: ");
    oled.print(id.passiveCable.latency);
    oled.println("0ns");
    oled.print("Approx length: ~");
    oled.print(id.passiveCable.latency);
    oled.println("m");

    oled.setCursor(0,56);
    oled.print("> Back");

    oled.display();
}

void Display::drawWaitConnection()
{
    oled.clearDisplay();
    oled.setTextSize(1);
    oled.setCursor(0, 0);
    oled.print("Connect cable");
    for (int i = 0; i < animationFrame; i++)
    {
        oled.print(".");
    }
    animationFrame = (animationFrame + 1) % 5;
    oled.println();
    oled.setCursor(100, 16);
    oled.drawLine(100, 20, 100, 40, WHITE);
    oled.drawLine(100, 40, 90, 35, WHITE);
    oled.drawLine(100, 40, 110, 35, WHITE);
    
    oled.display();
}

void Display::drawPS(PD::Capabilities cap)
{
    oled.clearDisplay();
    oled.setTextSize(1);
    for (int i = 0; i < cap.length; i++)
    {
        oled.setCursor(0, i * 8);
        oled.print(cap.dataObjects[i].sourceFixed.voltage / 1000);
        oled.print("V - ");
        oled.print(cap.dataObjects[i].sourceFixed.current / 1000.0, 1);
        oled.println("A");
    }

    oled.setCursor(0,56);
    oled.print("> Back");

    oled.display();

    oled.display();
}

void Display::drawModeBar()
{
    oled.drawRoundRect(7, 60, 35, 10, 5, 1);
    oled.drawRoundRect(46, 60, 35, 10, 5, 1);
    oled.drawRoundRect(85, 60, 35, 10, 5, 1);

    // oled.fillRoundRect((sm.mode * 39) + 7, 60, 35, 10, 5, 1);

    oled.display();
}

void Display::drawCableDisconnect()
{
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.setTextSize(1);
    oled.println("Cable disconnected");
    oled.display();

    this->drawModeBar();
}

void Display::drawCharging(uint16_t VBusVoltage, uint16_t batteryVoltage, bool status)
{
    oled.clearDisplay();
    oled.setCursor(0, 0);
    oled.setTextSize(1);
    oled.println("Charging");
    oled.print("VBus: ");
    oled.print(VBusVoltage);
    oled.println(" mV");
    oled.print("Battery: ");
    oled.print(batteryVoltage);
    oled.println(" mV");
    oled.print("Status: ");
    oled.println(status ? "charging" : "done");
    oled.display();

    this->drawModeBar();
}

// void Display::drawModeOption(Mode mode){
//     oled.clearDisplay();
//     oled.setCursor(0, 0);
//     oled.setTextSize(2);
//     // oled.println(StateMachine::nameFromMode(mode));
//     oled.display();
// }




void Display::drawMenu(const Menu_item items[], int index, int length)
{
  int start = index - 1;
  if(start < 0)
    start = 0;
  
  int end = start + MENU_MAX_DRAWN;
  if(end > length)
  {
    end = length;
  }


  oled.clearDisplay();
  oled.setCursor(0, 0);

  for(int i = start; i < end; i++)
  {
    if(i == index)
    {
      oled.print(">");
    }
    oled.print(" ");
    oled.println(items[i].title);
  }

  oled.display();
}