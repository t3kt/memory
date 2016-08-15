//
//  NavigatorsController.h
//  memory
//
//  Created by tekt on 7/17/16.
//
//

#ifndef NavigatorsController_h
#define NavigatorsController_h

#include <memory>
#include "../core/ObjectManager.h"
#include "../core/Params.h"
#include "../spawning/Spawner.h"

class Context;
class NavigatorEntity;
class NavigatorState;
class ObserverEntity;
class SimulationEvents;

class NavigatorsController {
public:
  class Params : public ::Params {
  public:
    Params() {
      add(observerNavigatorSpawner
          .setKey("observerNavigatorSpawner")
          .setName("Observer Nav Spawner"));
      add(moveRate
          .setKey("moveRate")
          .setName("Move Rate")
          .setValueAndDefault(10)
          .setRange(0, 50));
      add(stepDuration
          .setKey("stepDuration")
          .setName("Step Duration")
          .setValueAndDefault(1)
          .setRange(0, 10));
      add(reachRange
          .setKey("reachRange")
          .setName("Reach Range")
          .setValueAndDefault(10)
          .setRange(0, 50));
      observerNavigatorSpawner.setEnabledValueAndDefault(false);
      observerNavigatorSpawner.setRateValueAndDefault(0.04);
      observerNavigatorSpawner.setRateRange(0, 2);
    }

    RateSpawner<>::Params observerNavigatorSpawner;
    TParam<float> moveRate;
    TParam<float> stepDuration;
    TParam<float> reachRange;
  };

  NavigatorsController(Context& context,
                       Params& params,
                       SimulationEvents& events);

  void setup();
  void update();
  void draw();
private:
  bool spawnObserverNavigator(std::shared_ptr<ObserverEntity> observer);

  Context& _context;
  Params& _params;
  SimulationEvents& _events;
  ObjectManager<NavigatorEntity>& _navigators;
  std::shared_ptr<Spawner> _observerNavSpawner;

  friend class ObserverNavSpawner;
};

#endif /* NavigatorsController_h */
