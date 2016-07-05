//
//  AnimationsController.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef AnimationsController_h
#define AnimationsController_h

#include "AnimationObject.h"
#include "ObjectManager.h"
#include "Params.h"
#include "Timing.h"
#include "State.h"
#include "Events.h"
#include "OccurrencesController.h"
#include "ObserversController.h"
#include "Colors.h"

class AnimationsController {
public:
  class Params : public ::Params {
  public:
    Params();

    bool enabled() const { return _enabled.get(); }

    ExpandingSphereAnimation::Params observerDied;
    ExpandingSphereAnimation::Params occurrenceDied;
    ExpandingSphereAnimation::Params occurrenceSpawnFailed;
  private:
    TParam<bool> _enabled;
  };

  AnimationsController(const Params& params, const ColorTheme& colors);

  void setup();

  void addAnimation(shared_ptr<AnimationObject> animation, const State& state);
  void addTimedAction(shared_ptr<TimedAction> action);
  
  void attachTo(ObserversController& observers);
  void attachTo(OccurrencesController& occurrences);
  
  void update(const State& state);
  void draw(const State& state);

  int count() const { return _animations.size(); }
  
private:
  const Params& _params;
  const ColorTheme& _colors;
  ObjectManager<AnimationObject> _animations;
  TimedActionSet _timedActions;
};

#endif /* AnimationsController_h */
