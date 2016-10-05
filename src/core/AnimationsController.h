//
//  AnimationsController.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef AnimationsController_h
#define AnimationsController_h

#include "../core/AnimationObject.h"
#include "../rendering/Colors.h"
#include "../core/Common.h"
#include "../core/Context.h"
#include "../core/ObjectManager.h"
#include "../core/Params.h"

class SimulationEvents;

class AnimationsController
: public NonCopyable {
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
      occurrenceSpawnFailed.radius.setParamValuesAndDefaults(0, 20);
      occurrenceSpawnFailed.radius.setParamRanges(0, 100);
      occurrenceSpawnFailed.setDuration(1);
    }

    ExpandingSphereAnimation::Params observerDied;
    ExpandingSphereAnimation::Params occurrenceDied;
    ExpandingSphereAnimation::Params occurrenceSpawnFailed;
  };

  AnimationsController(const Params& params,
                       SimulationEvents& events,
                       Context& context);

  void setup();

  void addAnimation(std::shared_ptr<AnimationObject> animation);
  
  void update();
  void draw();

  int count() const { return _animations.size(); }
  
private:
  void attachToEvents();

  SimulationEvents& _events;
  const Params& _params;
  Context& _context;
  const ColorTheme& _colors;
  ObjectManager<AnimationObject>& _animations;
};

#endif /* AnimationsController_h */
