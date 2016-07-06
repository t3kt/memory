//
//  PhysicsController.h
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#ifndef PhysicsController_h
#define PhysicsController_h

#include "Bounds.h"
#include "Params.h"
#include "PhysicsWorld.h"
#include "PhysicsBehavior.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include "ObserversController.h"
#include "OccurrencesController.h"

class PhysicsController {
public:
  class Params : public ::Params {
  public:
    Params() {
      add(rebound
          .setKey("rebound")
          .setName("Rebound"));
      add(observerOccurrenceAttraction
          .setKey("observerOccurrenceAttraction")
          .setName("Observer to Occurrence"));
      add(occurrenceObserverAttraction
          .setKey("occurrenceObserverAttraction")
          .setName("Occurrence to Observer"));
      add(observerSpatialNoiseForce
          .setKey("observerSpatialNoiseForce")
          .setName("Observer Spatial Noise"));
      add(occurrenceSpatialNoiseForce
          .setKey("occurrenceObserverAttraction")
          .setName("Occurrence Spatial Noise"));
      observerSpatialNoiseForce.setEnabled(false);
      occurrenceObserverAttraction.setEnabled(false);
    }

    BoundsBehavior::Params rebound;
    AbstractAttractionBehavior::Params observerOccurrenceAttraction;
    AbstractAttractionBehavior::Params occurrenceObserverAttraction;
    AbstractSpatialNoiseForceBehavior::Params observerSpatialNoiseForce;
    AbstractSpatialNoiseForceBehavior::Params occurrenceSpatialNoiseForce;
  };

  PhysicsController(const Params& params,
                    const Bounds& bounds,
                    const State& state);

  void stopAllEntities();

  void setup(ObserversController& observers,
             OccurrencesController& occurrences);

  void update();

private:
  const Params& _params;
  const Bounds& _bounds;
  const State& _state;

  std::shared_ptr<PhysicsWorld> _world;

  std::shared_ptr<BoundsBehavior> _rebound;
  std::shared_ptr<AttractionBehavior<ObserverEntity, OccurrenceEntity>> _observerOccurrenceAttraction;
  std::shared_ptr<AttractionBehavior<OccurrenceEntity, ObserverEntity>> _occurrenceObserverAttraction;
  std::shared_ptr<SpatialNoiseForceBehavior<ObserverEntity>> _observerSpatialNoiseForce;
  std::shared_ptr<SpatialNoiseForceBehavior<OccurrenceEntity>> _occurrenceSpatialNoiseForce;
};

#endif /* PhysicsController_h */
