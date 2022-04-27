#include "cableMode.h"
#include "psMode.h"
#include "chargeMode.h"
#include "menu.h"
#include "buttons.h"
#include "fsm.h"

void StateMachine::sendEvent(Event *event)
{
    Serial.println("Passed event along");
    if (event->type == ID)
    {
        Serial.println("Event type id");
        currentState->react((IDRecievedEvent *)event->data);
    }
    else if (event->type == SOURCE_CAP)
    {
        Serial.println("Event type Source cap");
        currentState->react((SourceCapRecievedEvent *)event->data);
    }
    else if (event->type == CHANGE_MODE)
    {
        // Serial.println("Change mode...");
        // Serial.print("Current mode:");
        // Serial.println(mode);
        // switch (mode)
        // {
        // case CABLE:
        //     Serial.println("Next mode PS");
        //     this->setCurrentState(PSModeState::getInstance());
        //     break;
        // case PS:
        //     Serial.println("Next mode Charge");
        //     this->setCurrentState(ChargeModeState::getInstance());
        //     break;
        // case CHARGE:
        //     Serial.println("Next mode Cable");
        //     this->setCurrentState(CableModeState::getInstance());
        //     break;
        // default:
        //     break;
        // }
    }
    else if (event->type == BUTTON)
    {
        ButtonPressType button = ((ButtonEvent *)event->data)->event;
        if (button == BUTTON_UP_LONG || button == BUTTON_DOWN_LONG)
        {
            this->setCurrentState(MenuState::getInstance());
        }
        currentState->react((ButtonEvent *)event->data);
    }
    else
    {
        Serial.println("Other event");
        //currentState->react(event->data);
    }
}

Mode StateMachine::nextMode(Mode mode)
{
    switch (mode)
    {
    case CABLE:
        return PS;
    case PS:
        return CHARGE;
    case CHARGE:
        return CABLE;
    default:
        return CABLE;
    }
}

State &StateMachine::stateFromMode(Mode mode)
{
    switch (mode)
    {
    case CABLE:
        return CableModeState::getInstance();
    case PS:
        return PSModeState::getInstance();
    case CHARGE:
        return ChargeModeState::getInstance();
    default:
        return CableModeState::getInstance();
    }
}

const char* StateMachine::nameFromMode(Mode mode){
    switch (mode)
    {
    case CABLE:
        return "CABLE";
    case PS:
        return "PS";
    case CHARGE:
        return "CHARGE";
    default:
        return "UNKOWN";
    }
}