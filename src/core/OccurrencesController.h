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
#include "Colors.h"
#include "Context.h"
#include "EntityController.h"
#include "Events.h"
#include "ObjectManager.h"
#include "ObserversController.h"
#include "OccurrenceEntity.h"
#include "Params.h"
#include "SimulationEvents.h"
#include "Spawner.h"
#include "State.h"

class OccurrencesController;

class IntervalOccurrenceSpawner
: public IntervalSpawner<> {
public:
  IntervalOccurrenceSpawner(OccurrencesController& controller);
protected:
  void spawnEntities(Context& context) override;

  OccurrencesController& _controller;
};

class RateOccurrenceSpawner
: public RateSpawner<> {
public:
  RateOccurrenceSpawner(OccurrencesController& controller);

protected:
  void spawnEntities(Context& context, int count) override;

  OccurrencesController& _controller;
};

class OccurrencesController
: public EntityController<OccurrenceEntity> {
public:
  class Params : public ::Params {
  public:
    Params() {
      add(radius
          .setKey("radius")
          .setName("Radius Range")
          .setParamValuesAndDefaults(0, 80)
          .setParamRanges(0, 400));
      add(spawner
          .setKey("spawner")
          .setName("Inteval Spawner"));
      add(rateSpawner
          .setRateRange(0, 5)
          .setRateValueAndDefault(0.5)
          .setKey("rateSpawner")
          .setName("Rate Spawner"));
      add(initialVelocity
          .setKey("initialVelocity")
          .setName("Initial Velocity")
          .setParamValuesAndDefaults(0, 2)
          .setParamRanges(0, 20));
    }

    RandomValueSupplier<float> radius;
    IntervalSpawner<>::Params spawner;
    RateSpawner<>::Params rateSpawner;
    SimpleRandomVectorSupplier initialVelocity;
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
  void spawnRandomOccurrence();

  const Params& _params;
  const Bounds& _bounds;
  ObserversController& _observers;
  std::shared_ptr<IntervalOccurrenceSpawner> _spawner;
  std::shared_ptr<RateOccurrenceSpawner> _rateSpawner;

  friend class IntervalOccurrenceSpawner;
  friend class RateOccurrenceSpawner;
  friend class DescendantOccurrenceSpawner;
};

#endif /* OccurrencesController_h */
