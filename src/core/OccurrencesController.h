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
#include "../control/Params.h"
#include "../core/Context.h"
#include "../core/EntityController.h"
#include "../core/ObjectManager.h"
#include "../core/ObserversController.h"
#include "../core/OccurrenceEntity.h"
#include "../physics/BoundsController.h"
#include "../spawning/OccurrenceSpawner.h"

class OccurrencesController;
class SimulationEvents;

class OccurrenceParams
: public Params {
public:
  OccurrenceParams() {
    add(rateSpawner
        .setKey("rateSpawner")
        .setName("Rate Spawner"));
    rateSpawner.rate.setRange(0, 5);
    rateSpawner.rate.setValueAndDefault(0.5);
  }

  OccurrenceSpawner::Params rateSpawner;
};

class OccurrencesController
: public EntityController<OccurrenceEntity>
, public AppActionHandler {
public:
  using Params = OccurrenceParams;

  OccurrencesController(const Params& params,
                        const BoundsController& bounds,
                        ObserversController& observers,
                        Context& context,
                        SimulationEvents& events);
  
  void setup() override;
  void update() override;

  bool performAction(AppAction action) override;
  
private:
  const Params& _params;
  const BoundsController& _bounds;
  ObserversController& _observers;
  std::shared_ptr<OccurrenceSpawner> _rateSpawner;

  friend class IntervalOccurrenceSpawner;
  friend class DescendantOccurrenceSpawner;
};

#endif /* OccurrencesController_h */
