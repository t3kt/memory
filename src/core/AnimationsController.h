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
#include "../core/Component.h"
#include "../core/Context.h"
#include "../core/ObjectManager.h"
#include "../control/Params.h"

class SimulationEvents;

// Controller that owns and manages all animation entities, which
// can be created when various events occur (like another entity
// dying)
class AnimationsController
: public NonCopyable
, public ComponentBase {
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
      occurrenceSpawnFailed.radius.setValueRanges(0, 100);
      occurrenceSpawnFailed.radius.values[0].setValueAndDefault(0);
      occurrenceSpawnFailed.radius.values[1].setValueAndDefault(20);
      occurrenceSpawnFailed.setDuration(1);
    }

    ExpandingSphereAnimation::Params observerDied;
    ExpandingSphereAnimation::Params occurrenceDied;
    ExpandingSphereAnimation::Params occurrenceSpawnFailed;
  };

  AnimationsController(const Params& params,
                       SimulationEvents& events,
                       Context& context);

  void setup() override;

  void addAnimation(std::shared_ptr<AnimationObject> animation);
  
  void update() override;
  void draw() override;

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
