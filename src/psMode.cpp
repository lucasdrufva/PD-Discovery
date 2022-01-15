#include "psMode.h"

extern StateMachine sm;
extern Display display;
extern PD_Engine pd;

void sourceCapRecieved(PD::Capabilities cap){
    //display.debug();
    SourceCapRecievedEvent data;
    data.cap = cap;
    Event e;
    e.type = SOURCE_CAP;
    e.data = &data;
    sm.sendEvent(&e);
}

void PSModeState::enter()
{
    // fusb.setIsSource(false);
    // fusb.pdReset();
    pd.configureAsSink();
    sm.mode = PS;
    if(!sm.connected){
        sm.setCurrentState(DetectCableState::getInstance());
    }
    pd.registerCapCB(*sourceCapRecieved);
    Serial.println("Read PS entered");
};
void PSModeState::exit()
{
    recieved = false;
    tries = 0;
    sm.connected = false;
    Serial.println("Read PS exit");
};
void PSModeState::update()
{
    Serial.print("Check vconn:");
    Serial.println(fusb.checkVConn(), BIN);

    Serial.println("Read PS update");
    if (!recieved && millis() - lastRequestTime > REQUEST_INTERVAL && tries < MAX_TRIES)
    {

        pd.requestSourceCap(*sourceCapRecieved);

        fusb.requestSourceCap();
        Serial.println("Sent cap request");
        tries++;
        lastRequestTime = millis();
    }
};

void PSModeState::react(SourceCapRecievedEvent *e)
{
    Serial.println("PS event recieved");
    recieved = true;
    display.drawPS(e->cap);
    sm.setCurrentState(DetectDisconnectState::getInstance());
}