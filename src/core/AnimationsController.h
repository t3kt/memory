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
  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(observerDied
          .setKey("observerDied")
          .setName("Observer Died"));
      add(occurrenceDied
          .setKey("occurrenceDied")
          .setName("Occurrence Died"));
      add(occurrenceSpawnFailed
          .setKey("occurrenceSpawnFailed")
          .setName("Occurrence Spawn Failed"));
      setEnabledValueAndDefault(true);
      occurrenceSpawnFailed.radius.setParamValuesAndDefaults(0, 0.01);
      occurrenceSpawnFailed.setDuration(1);
    }

    ExpandingSphereAnimation::Params observerDied;
    ExpandingSphereAnimation::Params occurrenceDied;
    ExpandingSphereAnimation::Params occurrenceSpawnFailed;
  };

  AnimationsController(const Params& params, const ColorTheme& colors);

  void setup();

  void addAnimation(shared_ptr<AnimationObject> animation, const State& state);
  void addTimedAction(shared_ptr<TimedAction> action);
  
  void attachTo(ObserversController& observers);
  void attachTo(OccurrencesController& occurrences);
  
  void update(State& state);
  void draw(const State& state);

  int count() const { return _animations.size(); }
  
private:
  const Params& _params;
  const ColorTheme& _colors;
  ObjectManager<AnimationObject> _animations;
  TimedActionSet _timedActions;
};

#endif /* AnimationsController_h */
