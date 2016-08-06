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
#include "ObserverEntity.h"
#include "ObjectManager.h"
#include "State.h"
#include "Params.h"
#include "Events.h"
#include "Bounds.h"
#include "Context.h"
#include "SimulationEvents.h"
#include "Spawner.h"

class IntervalObserverSpawner;
class RateObserverSpawner;

class ObserversController
: public AppActionHandler {
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
    IntervalSpawnerParams spawner;
    RateSpawner::Params rateSpawner;
    SimpleRandomVectorSupplier initialVelocity;
  };
  
  ObserversController(const Params& params,
                      const Bounds& bounds,
                      Context& context,
                      SimulationEvents& events);
  
  void setup();
  void update();
  void draw();
  
  bool registerOccurrence(std::shared_ptr<OccurrenceEntity> occurrence);

  void spawnObservers(int count);

  void killObservers(int count);

  ObjectManager<ObserverEntity>& entities() { return _observers; }
  const ObjectManager<ObserverEntity>& entities() const {
    return _observers;
  }

  bool performAction(AppAction action) override;
  
private:
  void spawnRandomObserver();
  
  const Params& _params;
  Context& _context;
  const Bounds& _bounds;
  SimulationEvents& _events;
  ObjectManager<ObserverEntity>& _observers;
  std::shared_ptr<IntervalObserverSpawner> _spawner;
  std::shared_ptr<RateObserverSpawner> _rateSpawner;

  friend class IntervalObserverSpawner;
  friend class RateObserverSpawner;
};

#endif /* ObserversController_h */
