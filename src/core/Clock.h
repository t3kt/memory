//
//  Clock.h
//  memory-prototype-2
//
//  Created by tekt on 6/28/16.
//
//

#ifndef Clock_h
#define Clock_h

#include <memory>
#include <ofParameter.h>
#include "../app/AppActions.h"
#include "../core/Params.h"
#include "../core/State.h"

class Clock
: public AppActionHandler {
public:
  class Params : public ::Params {
  public:
    Params() {
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

  Clock(Params& params, State& state);

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
