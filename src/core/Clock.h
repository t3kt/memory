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
#include "AppActions.h"
#include "Params.h"
#include "State.h"

class Clock
: public AppActionHandler {
public:
  class Params : public ::Params {
  public:
    Params() {
      add(_paused
          .setKey("paused")
          .setName("Paused")
          .setValueAndDefault(false));
      add(_rate
          .setKey("rate")
          .setName("Rate")
          .setValueAndDefault(1)
          .setRange(0, 10));
    }

    bool paused() const { return _paused.get(); }
    float rate() const { return _rate.get(); }

    void setPaused(bool paused) { _paused.set(paused); }

  private:
    TParam<bool> _paused;
    TParam<float> _rate;

    friend class MidiRouter;
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
