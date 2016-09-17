//
//  Clock.h
//
//  Controller that keeps track of the current time and updates it
//  on every update cycle. It supports a variable rate controlled by
//  a parameter and it can be paused based on another parameter.
//
//

#ifndef Clock_h
#define Clock_h

#include <memory>
#include <ofParameter.h>
#include "../app/AppActions.h"
#include "../core/Params.h"
#include "../core/State.h"

class ClockParams
: public Params {
public:
  ClockParams() {
    add(paused
        .setKey("paused")
        .setName("Paused")
        .setValueAndDefault(false));
    add(rate
        .setKey("rate")
        .setName("Rate")
        .setValueAndDefault(1)
        .setRange(0, 10));
  }

  TParam<bool> paused;
  TParam<float> rate;
};

class Clock
: public AppActionHandler {
public:
  using Params = ClockParams;

  Clock(Params& params, State& state)
  : _params(params)
  , _state(state) { }

  void setup();

  void toggleState();

  void update();

  bool performAction(AppAction action) override;
private:
  void start();
  void stop();
  void onPausedChanged(bool& paused);

  State& _state;
  Params& _params;
  float _lastTime;
};

#endif /* Clock_h */
