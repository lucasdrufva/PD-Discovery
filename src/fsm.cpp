#include "cableMode.h"
#include "psMode.h"
#include "chargeMode.h"
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
        Serial.println("Change mode...");
        Serial.print("Current mode:");
        Serial.println(mode);
        switch (mode)
        {
        case CABLE:
            Serial.println("Next mode PS");
            this->setCurrentState(PSModeState::getInstance());
            break;
        case PS:
            Serial.println("Next mode Charge");
            this->setCurrentState(ChargeModeState::getInstance());
            break;
        case CHARGE:
            Serial.println("Next mode Cable");
            this->setCurrentState(CableModeState::getInstance());
            break;
        default:
            break;
        }
    }
}