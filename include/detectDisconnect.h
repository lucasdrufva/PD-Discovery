#pragma once
#include "fsm.h"
#include "display.h"
#include "detectCable.h"

class DetectDisconnectState : State{
  public:
    void enter() {};
    void exit () {};
    void update();

    static State& getInstance() {
      static DetectDisconnectState singleton;
	    return singleton;
    };

private: 
    int detectMode;
    State* nextState;
};
