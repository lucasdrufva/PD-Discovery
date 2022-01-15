#pragma once
#include "fsm.h"
#include "oldtcpm.h"
#include "display.h"
#include "detectCable.h"
#include "detectDisconnect.h"
#include "board/board.h"
#include "board/charger.h"

class ChargeModeState : State
{
public:
    void enter();
    void exit ();
    void update();

    static State& getInstance() {
      static ChargeModeState singleton;
	    return singleton;
    };
private:
  
};