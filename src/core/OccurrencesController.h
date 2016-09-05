//
//  OccurrencesController.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef OccurrencesController_h
#define OccurrencesController_h

#include "../app/AppActions.h"
#include "../core/Bounds.h"
#include "../core/Context.h"
#include "../core/EntityController.h"
#include "../core/Events.h"
#include "../core/ObjectManager.h"
#include "../core/ObserversController.h"
#include "../core/OccurrenceEntity.h"
#include "../spawning/OccurrenceSpawner.h"
#include "../core/Params.h"
#include "../core/State.h"

class OccurrencesController;
class SimulationEvents;

class OccurrenceParams
: public Params {
public:
  OccurrenceParams() {
    add(spawner
        .setKey("spawner")
        .setName("Inteval Spawner"));
    add(rateSpawner
        .setRateRange(0, 5)
        .setRateValueAndDefault(0.5)
        .setKey("rateSpawner")
        .setName("Rate Spawner"));
  }

  IntervalOccurrenceSpawner::Params spawner;
  RateOccurrenceSpawner::Params rateSpawner;
};

class OccurrencesController
: public EntityController<OccurrenceEntity> {
public:
  using Params = OccurrenceParams;

  OccurrencesController(const Params& params,
                        const Bounds& bounds,
                        ObserversController& observers,
                        Context& context,
                        SimulationEvents& events);
  
  void setup() override;
  void update() override;
  void draw() override;

  void spawnOccurrences(int count);

  bool performAction(AppAction action) override;

  bool tryAddEntity(std::shared_ptr<OccurrenceEntity> entity) override;
  
private:
  const Params& _params;
  const Bounds& _bounds;
  ObserversController& _observers;
  std::shared_ptr<IntervalOccurrenceSpawner> _spawner;
  std::shared_ptr<RateOccurrenceSpawner> _rateSpawner;

  friend class IntervalOccurrenceSpawner;
  friend class DescendantOccurrenceSpawner;
};

#endif /* OccurrencesController_h */
