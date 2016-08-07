//
//  OccurrencesController.h
//  memory-prototype-2
//
//  Created by tekt on 6/25/16.
//
//

#ifndef OccurrencesController_h
#define OccurrencesController_h

#include "AppActions.h"
#include "Bounds.h"
#include "Context.h"
#include "EntityController.h"
#include "Events.h"
#include "ObjectManager.h"
#include "ObserversController.h"
#include "OccurrenceEntity.h"
#include "OccurrenceSpawner.h"
#include "Params.h"
#include "SimulationEvents.h"
#include "State.h"

class OccurrencesController;

class OccurrencesController
: public EntityController<OccurrenceEntity> {
public:
  class Params : public ::Params {
  public:
    Params() {
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
