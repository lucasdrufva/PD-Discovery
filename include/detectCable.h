#pragma once
#include "fsm.h"
#include "oldtcpm.h"
#include "cableMode.h"
#include "psMode.h"

class DetectCableState : State{
  public:
    void enter();
    void exit ();
    void update();
    void react(EventData e){};

    static State& getInstance() {
      static DetectCableState singleton;
	    return singleton;
    };

private: 
    int detectMode;
    State* nextState;
};
