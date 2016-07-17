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
#include "ObjectManager.h"
#include "Params.h"
#include "Spawner.h"

class Context;
class NavigatorEntity;
class NavigatorState;
class ObserverEntity;

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
          .setValueAndDefault(1)
          .setRange(0, 10));
      add(reachRange
          .setKey("reachRange")
          .setName("Reach Range")
          .setValueAndDefault(0.0005)
          .setRange(0, 0.5));
    }
    RateSpawner::Params observerNavigatorSpawner;
    TParam<float> moveRate;
    TParam<float> reachRange;
  };

  NavigatorsController(Context& context,
                       Params& params);

  void setup();
  void update();
  void draw();
private:
  void spawnObserverNavigator(std::shared_ptr<ObserverEntity> observer);

  Context& _context;
  Params& _params;
  ObjectManager<NavigatorEntity>& _navigators;
  std::shared_ptr<Spawner> _observerNavSpawner;
};

#endif /* NavigatorsController_h */
