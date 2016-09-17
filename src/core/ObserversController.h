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
#include "../rules/ObserverSickness.h"
#include "../core/Params.h"
#include "../core/State.h"

class SimulationEvents;

class ObserverParams : public ::Params {
public:
  ObserverParams() {
    add(rateSpawner
        .setKey("rateSpawner")
        .setName("Rate Spawner"));
    rateSpawner.rate.setRange(0, 5);
    rateSpawner.rate.setValueAndDefault(0.5);
    add(sickness
        .setKey("sickness")
        .setName("Sickness"));
    sickness.setEnabledValueAndDefault(false);
  }

  RateObserverSpawner::Params rateSpawner;
  ObserverSicknessParams sickness;
};

class ObserversController
: public EntityController<ObserverEntity>
, public AppActionHandler {
public:
  using Params = ObserverParams;
  
  ObserversController(const Params& params,
                      const Bounds& bounds,
                      Context& context,
                      SimulationEvents& events);
  
  void setup() override;
  void update() override;

  bool performAction(AppAction action) override;
  
private:
  const Params& _params;
  const Bounds& _bounds;
  std::shared_ptr<RateObserverSpawner> _rateSpawner;
  std::shared_ptr<ObserverSickness> _sickness;

  friend class IntervalObserverSpawner;
};

#endif /* ObserversController_h */
