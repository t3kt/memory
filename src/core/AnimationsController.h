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
#include "Status.h"
#include "Colors.h"

class AnimationsController
: public StatusInfoProvider {
public:
  class Params : public ::Params {
  public:
    Params();

    ofParameter<bool> enabled;
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
  
  void update(const State& state);
  void draw(const State& state);

  const StatusInfo& getStatusInfo() const override { return _status; }
  
private:
  const Params& _params;
  const ColorTheme& _colors;
  ObjectManager<AnimationObject> _animations;
  TimedActionSet _timedActions;
  StatusInfo _status;
  std::size_t STATUS_COUNT;
};

#endif /* AnimationsController_h */
