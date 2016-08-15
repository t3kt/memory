//
//  ObserversController.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef ObserversController_h
#define ObserversController_h

#include "../app/AppActions.h"
#include "../core/Bounds.h"
#include "../core/Context.h"
#include "../core/EntityController.h"
#include "../core/Events.h"
#include "../core/ObjectManager.h"
#include "../core/ObserverEntity.h"
#include "../spawning/ObserverSpawner.h"
#include "../core/Params.h"
#include "../core/State.h"

class SimulationEvents;

class ObserversController
: public EntityController<ObserverEntity> {
public:

  class Params : public ::Params {
  public:
    Params() {
      add(spawner
          .setKey("spawner")
          .setName("Interval Spawner"));
      add(rateSpawner
          .setRateRange(0, 5)
          .setRateValueAndDefault(0.5)
          .setKey("rateSpawner")
          .setName("Rate Spawner"));
    }

    IntervalObserverSpawner::Params spawner;
    RateObserverSpawner::Params rateSpawner;
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
  const Params& _params;
  const Bounds& _bounds;
  std::shared_ptr<IntervalObserverSpawner> _spawner;
  std::shared_ptr<RateObserverSpawner> _rateSpawner;

  friend class IntervalObserverSpawner;
};

#endif /* ObserversController_h */
