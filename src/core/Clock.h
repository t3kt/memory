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
#include "../core/Common.h"
#include "../core/Component.h"
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

class ClockNode
: public ComponentBase
, public NonCopyable {
public:
  using Params = ClockParams;

  ClockNode(Params& params,
            ClockState& state,
            std::shared_ptr<ClockNode> parentClock)
  : _params(params)
  , _state(state)
  , _parentClock(parentClock) { }

  void setup() override;
  void update() override;

  void toggleState();
private:
  void setPaused(bool paused);
  bool isPaused() const;
  float getEffectiveRate() const;
  void start();
  void stop();

  const std::shared_ptr<ClockNode> _parentClock;
  Params& _params;
  ClockState& _state;
  float _lastAbsoluteTime;
};

#endif /* Clock_h */
