//
//  PhysicsController.h
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#ifndef PhysicsController_h
#define PhysicsController_h

#include "../physics/AnchorPointBehavior.h"
#include "../app/AppActions.h"
#include "../physics/AttractionBehavior.h"
#include "../core/Bounds.h"
#include "../core/Component.h"
#include "../core/Context.h"
#include "../physics/DampingBehavior.h"
#include "../physics/EntityForceBehavior.h"
#include "../physics/ForceFieldBehavior.h"
#include "../physics/ForceNodeBehavior.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../core/Params.h"
#include "../physics/PhysicsBehavior.h"

using PhysicsBehaviorCollection
= ComponentCollection<AbstractPhysicsBehavior>;

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

class PhysicsParams
: public Params {
public:
  PhysicsParams() {
    add(observers
        .setKey("observers")
        .setName("Observers"));
    add(occurrences
        .setKey("occurrences")
        .setName("Occurrences"));
    add(rebound
        .setKey("rebound")
        .setName("Rebound"));
    add(observerObserverAttraction
        .setKey("observerObserverAttraction")
        .setName("Observer to Observer"));
    add(observerOccurrenceForce
        .setKey("observerOccurrenceForce")
        .setName("Obs/Occ Force"));
    add(occurrenceOccurrenceForce
        .setKey("occurrenceOccurrenceForce")
        .setName("Occ/Occ Force"));
    add(spatialNoiseForce
        .setKey("spatialNoiseForce")
        .setName("Spatial Noise"));
    add(observerAnchorPointAttraction
        .setKey("observerAnchorPointAttraction")
        .setName("Observer Anchoring"));
    add(occurrenceAnchorPointAttraction
        .setKey("occurrenceAnchorPointAttraction")
        .setName("Occurrence Anchoring"));
    add(vortexNodes
        .setKey("vortexNodes")
        .setName("Vortex Nodes"));
    add(damping
        .setKey("damping")
        .setName("Damping"));
    rebound.setEnabledValueAndDefault(true);
    spatialNoiseForce.setEnabledValueAndDefault(false);
    observerAnchorPointAttraction.setEnabledValueAndDefault(false);
    occurrenceAnchorPointAttraction.setEnabledValueAndDefault(true);
    observerObserverAttraction.setEnabledValueAndDefault(false);
    observerOccurrenceForce.setEnabledValueAndDefault(false);
    vortexNodes.setEnabledValueAndDefault(false);
    damping.setEnabledValueAndDefault(true);
  }

  EntityPhysicsParams observers;
  EntityPhysicsParams occurrences;
  ParamsWithEnabled rebound;
  AbstractAttractionBehavior::Params observerObserverAttraction;
  ObserverOccurrenceForceBehavior::Params observerOccurrenceForce;
  OccurrenceOccurrenceForceBehavior::Params occurrenceOccurrenceForce;
  AbstractNoiseForceFieldBehavior::Params spatialNoiseForce;
  AbstractAttractionBehavior::Params observerAnchorPointAttraction;
  AbstractAttractionBehavior::Params occurrenceAnchorPointAttraction;
  AbstractDampingBehavior::Params damping;
  AbstractVortexForceNodeBehavior::Params vortexNodes;
};

class PhysicsController
: public AppActionHandler {
public:
  using Params = PhysicsParams;

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
  PhysicsBehaviorCollection _behaviors;
};

#endif /* PhysicsController_h */
