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
#include "../core/EntityRules.h"
#include "../core/Events.h"
#include "../core/ObjectManager.h"
#include "../core/ObserverEntity.h"
#include "../spawning/ObserverSpawner.h"
#include "../core/Params.h"
#include "../core/State.h"

class SimulationEvents;
using ObserverRule = EntityRule<ObserverEntity>;

class ObserverParams : public ::Params {
public:
  ObserverParams() {
    add(rateSpawner
        .setKey("rateSpawner")
        .setName("Rate Spawner"));
    rateSpawner.rate.setRange(0, 5);
    rateSpawner.rate.setValueAndDefault(0.5);
  }

  RateObserverSpawner::Params rateSpawner;
};

class ObserversController
: public EntityController<ObserverEntity> {
public:
  using Params = ObserverParams;
  
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
  std::shared_ptr<RateObserverSpawner> _rateSpawner;

  friend class IntervalObserverSpawner;
};

#endif /* ObserversController_h */
