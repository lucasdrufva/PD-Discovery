#include "cableMode.h"

extern StateMachine sm;
extern Display display;
extern PD_Engine pd;

void identityRecieved(PD::Identity id){
    IDRecievedEvent data;
    data.id = id;
    Event e;
    e.type = ID;
    e.data = &data;
    sm.sendEvent(&e);
}

void CableModeState::enter()
{
    pd.configureAsSource();
    sm.mode = CABLE;
    if(!sm.connected){
        sm.setCurrentState(DetectCableState::getInstance());
    }
};
void CableModeState::exit()
{
    recieved = false;
    tries = 0;
    sm.connected = false;
    Serial.println("Read Cable exit");
};
void CableModeState::update()
{
    Serial.println("Read Cable update");
    if (!recieved && millis() - lastRequestTime > REQUEST_INTERVAL && tries < MAX_TRIES)
    {
        if(pd.requestIdentity(PD::Destination::SOP_PRIME, *identityRecieved) == PD_Engine::Error::DISCONNECT){
            sm.setCurrentState(DetectCableState::getInstance());
        }

        tries++;
        lastRequestTime = millis();
    }
};

void CableModeState::react(IDRecievedEvent *event)
{
    Serial.println("Cable event recieved");
    Serial.print("Cable supports: ");
    Serial.println(event->id.passiveCable.vbusCurrent == 1 ? "3A" : "5A");
    recieved = true;
    display.drawCable(event->id);
}