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
#include <ofxAssimpModelLoader.h>
#include "ObjectManager.h"
#include "Params.h"
#include "Spawner.h"

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
      add(reachRange
          .setKey("reachRange")
          .setName("Reach Range")
          .setValueAndDefault(10)
          .setRange(0, 50));
      observerNavigatorSpawner.setEnabledValueAndDefault(false);
      observerNavigatorSpawner.setRateValueAndDefault(0.04);
      observerNavigatorSpawner.setRateRange(0, 2);
    }

    RateSpawner::Params observerNavigatorSpawner;
    TParam<float> moveRate;
    TParam<float> reachRange;
  };

  NavigatorsController(Context& context,
                       Params& params,
                       SimulationEvents& events);

  void setup();
  void update();
  void draw();
private:
  void spawnObserverNavigator(std::shared_ptr<ObserverEntity> observer);

  Context& _context;
  Params& _params;
  SimulationEvents& _events;
  ObjectManager<NavigatorEntity>& _navigators;
  std::shared_ptr<Spawner> _observerNavSpawner;

  ofxAssimpModelLoader _modelLoader;
  ofMesh _mesh;

  friend class ObserverNavSpawner;
};

#endif /* NavigatorsController_h */
