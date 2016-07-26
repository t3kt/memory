//
//  PhysicsController.h
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#ifndef PhysicsController_h
#define PhysicsController_h

#include "AppActions.h"
#include "AnchorPointBehavior.h"
#include "AttractionBehavior.h"
#include "Bounds.h"
#include "Context.h"
#include "DampingBehavior.h"
#include "ForceFieldBehavior.h"
#include "Params.h"
#include "PhysicsBehavior.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"

class DebugParams;

class EntityPhysicsParams
: public Params {
public:
  EntityPhysicsParams() {
    add(speed
        .setKey("speed")
        .setName("Speed")
        .setValueAndDefault(10)
        .setRange(0, 50));
  }

  TParam<float> speed;
};

class PhysicsController
: public AppActionHandler {
public:
  class Params : public ::Params {
  public:
    Params() {
      add(observers
          .setKey("observers")
          .setName("Observers"));
      add(occurrences
          .setKey("occurrences")
          .setName("Occurrences"));
      add(rebound
          .setKey("rebound")
          .setName("Rebound"));
      add(observerOccurrenceAttraction
          .setKey("observerOccurrenceAttraction")
          .setName("Observer to Occurrence"));
      add(occurrenceObserverAttraction
          .setKey("occurrenceObserverAttraction")
          .setName("Occurrence to Observer"));
      add(observerObserverAttraction
          .setKey("observerObserverAttraction")
          .setName("Observer to Observer"));
      add(observerSpatialNoiseForce
          .setKey("observerSpatialNoiseForce")
          .setName("Observer Spatial Noise"));
      add(occurrenceSpatialNoiseForce
          .setKey("occurrenceSpatialNoiseForce")
          .setName("Occurrence Spatial Noise"));
      add(observerAnchorPointAttraction
          .setKey("observerAnchorPointAttraction")
          .setName("Observer Anchoring"));
      add(occurrenceAnchorPointAttraction
          .setKey("occurrenceAnchorPointAttraction")
          .setName("Occurrence Anchoring"));
      add(observerDamping
          .setKey("observerDamping")
          .setName("Observer Damping"));
      add(occurrenceDamping
          .setKey("occurrenceDamping")
          .setName("Occurrence Damping"));
      rebound.setEnabledValueAndDefault(true);
      observerSpatialNoiseForce.setEnabledValueAndDefault(false);
      occurrenceObserverAttraction.setEnabledValueAndDefault(false);
      observerAnchorPointAttraction.setEnabledValueAndDefault(false);
      occurrenceAnchorPointAttraction.setEnabledValueAndDefault(true);
      observerObserverAttraction.setEnabledValueAndDefault(false);
      observerDamping.setEnabledValueAndDefault(true);
      occurrenceDamping.setEnabledValueAndDefault(true);
    }

    EntityPhysicsParams observers;
    EntityPhysicsParams occurrences;
    ParamsWithEnabled rebound;
    AbstractAttractionBehavior::Params observerOccurrenceAttraction;
    AbstractAttractionBehavior::Params occurrenceObserverAttraction;
    AbstractAttractionBehavior::Params observerObserverAttraction;
    AbstractNoiseForceFieldBehavior::Params observerSpatialNoiseForce;
    AbstractNoiseForceFieldBehavior::Params occurrenceSpatialNoiseForce;
    AbstractAttractionBehavior::Params observerAnchorPointAttraction;
    AbstractAttractionBehavior::Params occurrenceAnchorPointAttraction;
    AbstractDampingBehavior::Params observerDamping;
    AbstractDampingBehavior::Params occurrenceDamping;
  };

  PhysicsController(Params& params,
                    Bounds& bounds,
                    DebugParams& debugParams,
                    Context& context);

  void stopAllEntities();

  void setup();

  void update();
  void draw();

  bool performAction(AppAction action) override;

private:
  void beginEntityUpdate(ParticleObject* entity, const EntityPhysicsParams& params);
  void endEntityUpdate(ParticleObject* entity, const EntityPhysicsParams& params);

  Params& _params;
  Context& _context;
  Bounds& _bounds;
  DebugParams& _debugParams;

  std::shared_ptr<BoundsBehavior> _rebound;
  std::shared_ptr<AttractionBehavior<ObserverEntity, OccurrenceEntity>> _observerOccurrenceAttraction;
  std::shared_ptr<AttractionBehavior<OccurrenceEntity, ObserverEntity>> _occurrenceObserverAttraction;
  std::shared_ptr<AttractionBehavior<ObserverEntity, ObserverEntity>> _observerObserverAttraction;
  std::shared_ptr<NoiseForceFieldBehavior<ObserverEntity>> _observerSpatialNoiseForce;
  std::shared_ptr<NoiseForceFieldBehavior<OccurrenceEntity>> _occurrenceSpatialNoiseForce;
  std::shared_ptr<AnchorPointBehavior<ObserverEntity>> _observerAnchorPointAttraction;
  std::shared_ptr<AnchorPointBehavior<OccurrenceEntity>> _occurrenceAnchorPointAttraction;
  std::shared_ptr<DampingBehavior<ObserverEntity>> _observerDamping;
  std::shared_ptr<DampingBehavior<OccurrenceEntity>> _occurrenceDamping;
};

#endif /* PhysicsController_h */
