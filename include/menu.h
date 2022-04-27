#pragma once
#include "fsm.h"
#include "display.h"
#include "buttons.h"

class MenuState : State
{
public:
    void enter();
    void exit ();
    void update();
    void react(ButtonEvent *event);

    static State& getInstance() {
      static MenuState singleton;
	    return singleton;
    };
private:
    unsigned long lastUpdateTimestamp;
    const unsigned int threshold = 2000;
    Mode mode;
};