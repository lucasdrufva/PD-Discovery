#pragma once
#include "fsm.h"
#include "oldtcpm.h"
#include "display.h"
#include "detectCable.h"
#include "detectDisconnect.h"
#include <USB-PD.h>

struct IDRecievedEvent: EventData{
  PD::Identity id;
};


class CableModeState : State
{
public:
    void enter();
    void exit ();
    void update();
    void react(IDRecievedEvent *event);

    static State& getInstance() {
      static CableModeState singleton;
	    return singleton;
    };
private:
  bool recieved = false;
  int tries = 0;
  const unsigned int REQUEST_INTERVAL = 1000;
  const int MAX_TRIES = 10;
  unsigned long lastRequestTime;
};