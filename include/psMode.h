#pragma once
#include "fsm.h"
#include "oldtcpm.h"
#include "display.h"
#include "detectCable.h"
#include "detectDisconnect.h"

struct SourceCapRecievedEvent: EventData{
  PD::Capabilities cap;
};

class PSModeState : State
{
public:
    void enter();
    void exit ();
    void update();
    void react(SourceCapRecievedEvent *event);

    static State& getInstance() {
      static PSModeState singleton;
	    return singleton;
    };
private:
  bool recieved = false;
  int tries = 0;
  const unsigned int REQUEST_INTERVAL = 1000;
  const int MAX_TRIES = 10;
  unsigned long lastRequestTime;
};