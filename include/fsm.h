#pragma once
#include <map>
#include <Arduino.h>
#include "fusb302.h"

extern FUSB302 fusb;

enum Mode {
  CABLE,
  PS,
  CHARGE
};

struct EventData{};

enum EventType{
  ID,
  SOURCE_CAP,
  CHANGE_MODE,
};

struct Event{
  EventData* data;
  EventType type;
};

struct IDRecievedEvent;
struct SourceCapRecievedEvent;

class State{
public:
    virtual void enter() {};
    virtual void exit () {};
    virtual void update() {};
    virtual void react(EventData *event) {};
    virtual void react(IDRecievedEvent *event) {};
    virtual void react(SourceCapRecievedEvent *event) {};
    State(){};
};

class StateMachine{
public:
    State* getCurrentState()
    {
      return currentState;
    }

    void setCurrentState(State& state)
    {
      if (currentState != 0)
      {
        currentState->exit();
      }

      currentState = &state;

      if (currentState != 0)
      {
        currentState->enter();
      }
    }

    void update()
    {
      if (currentState != 0)
      {
        currentState->update();
      }
    }

    void sendEvent(Event *event);

  Mode mode = CABLE;
  bool connected = false;

protected:
    State* currentState;
};