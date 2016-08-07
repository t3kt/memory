//
//  ObserversController.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef ObserversController_h
#define ObserversController_h

#include "AppActions.h"
#include "Bounds.h"
#include "Context.h"
#include "EntityController.h"
#include "Events.h"
#include "ObjectManager.h"
#include "ObserverEntity.h"
#include "ObserverSpawner.h"
#include "Params.h"
#include "SimulationEvents.h"
#include "State.h"

class ObserversController
: public EntityController<ObserverEntity> {
public:

  class Params : public ::Params {
  public:
    Params() {
      add(lifetime
          .setKey("lifetime")
          .setName("Lifetime Range")
          .setParamValuesAndDefaults(1, 4)
          .setParamRanges(0, 240));
      add(spawner
          .setKey("spawner")
          .setName("Interval Spawner"));
      add(rateSpawner
          .setRateRange(0, 5)
          .setRateValueAndDefault(0.5)
          .setKey("rateSpawner")
          .setName("Rate Spawner"));
      add(initialVelocity
          .setKey("initialVelocity")
          .setName("Initial Velocity")
          .setParamValuesAndDefaults(0, 4)
          .setParamRanges(0, 20));
    }

    RandomValueSupplier<float> lifetime;
    IntervalObserverSpawner::Params spawner;
    RateObserverSpawner::Params rateSpawner;
    SimpleRandomVectorSupplier initialVelocity;
  };
  
  ObserversController(const Params& params,
                      const Bounds& bounds,
                      Context& context,
                      SimulationEvents& events);
  
  void setup() override;
  void update() override;
  void draw() override;
  
  bool registerOccurrence(std::shared_ptr<OccurrenceEntity> occurrence);

  void spawnObservers(int count);

  void killObservers(int count);

  bool performAction(AppAction action) override;

  bool tryAddEntity(std::shared_ptr<ObserverEntity> entity) override;
  
private:
  void spawnRandomObserver();

  const Params& _params;
  const Bounds& _bounds;
  std::shared_ptr<IntervalObserverSpawner> _spawner;
  std::shared_ptr<RateObserverSpawner> _rateSpawner;

  friend class IntervalObserverSpawner;
};

#endif /* ObserversController_h */
