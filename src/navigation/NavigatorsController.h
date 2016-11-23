//
//  NavigatorsController.h
//

#pragma once

#include <memory>
#include <ofxTCommon.h>
#include "../control/Params.h"
#include "../core/Component.h"
#include "../core/ObjectManager.h"
#include "../spawning/Spawner.h"

class Context;
class NavigatorEntity;
class NavigatorState;
class ObserverEntity;
class SimulationEvents;

class NavigatorParams
: public Params {
public:
  NavigatorParams() {
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
      observerNavigatorSpawner.enabled.setValueAndDefault(false);
      observerNavigatorSpawner.rate.setValueAndDefault(0.04);
      observerNavigatorSpawner.rate.setRange(0, 2);
    }

    RateSpawner<>::Params observerNavigatorSpawner;
    TParam<float> moveRate;
    TParam<float> stepDuration;
    TParam<float> reachRange;
};

class NavigatorsController
: public ofxTCommon::NonCopyable
, public ComponentBase {
public:
  using Params = NavigatorParams;

  NavigatorsController(Context& context,
                       Params& params,
                       SimulationEvents& events);

  void setup() override;
  void update() override;
  void draw() override;
private:
  bool spawnObserverNavigator(std::shared_ptr<ObserverEntity> observer);
  bool spawnHighlightedObserverNavigator();

  Context& _context;
  Params& _params;
  SimulationEvents& _events;
  ObjectManager<NavigatorEntity>& _navigators;
  std::shared_ptr<Spawner> _observerNavSpawner;

  friend class ObserverNavSpawner;
};

