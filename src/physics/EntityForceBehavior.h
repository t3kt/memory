//
//  EntityForceBehavior.h
//  memory
//
//  Created by tekt on 8/3/16.
//
//

#ifndef EntityForceBehavior_h
#define EntityForceBehavior_h

#include <ofTypes.h>
#include <ofxChoreograph.h>
#include "../core/Context.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../core/Params.h"
#include "../core/ParticleObject.h"
#include "../physics/PhysicsBehavior.h"
#include "../core/ValueSequence.h"

class AbstractEntityForceBehaviorParams : public ParamsWithEnabled {
public:
  AbstractEntityForceBehaviorParams() {
    add(force
        .setKey("force")
        .setName("Force"));
    force.setValueRanges(-15, 15);
    force.setLengthRanges(0, 100);
    force.startValue.setValueAndDefault(-2);
    force.values[0].setValueAndDefault(2);
    force.values[1].setValueAndDefault(5);
    force.lengths[0].setValueAndDefault(5);
    force.lengths[1].setValueAndDefault(15);
    add(magnitude
        .setKey("magnitude")
        .setName("Magnitude")
        .setRange(0, 3)
        .setValueAndDefault(1));
  }
  ValueSequence<float, 2> force;
  TParam<float> magnitude;
};

class AbstractEntityForceBehavior
: public AbstractPhysicsBehavior {
public:
  using Params = AbstractEntityForceBehaviorParams;

  AbstractEntityForceBehavior(Context& context,
                              const Params& params)
  : AbstractPhysicsBehavior(context)
  , _params(params) { }

  void debugDraw() override {
    if (!_params.enabled.get()) {
      return;
    }
    AbstractPhysicsBehavior::debugDraw();
  }

protected:
  virtual void beginDebugDraw() override;
  virtual void endDebugDraw() override;

  ofVec3f calcAttraction(const ofVec3f& entityPosition,
                         const ofVec3f& otherPosition) const;

  const Params& _params;
};

template<typename T1, typename T2>
class EntityForceBehavior
: public AbstractEntityForceBehavior {
public:
  EntityForceBehavior(Context& context,
                      const Params& params)
  : AbstractEntityForceBehavior(context, params) { }

  void applyToWorld() override {
    if (!_params.enabled()) {
      return;
    }
    performAction([&](T1& entity,
                      T2& other,
                      const ofVec3f& force) {
      entity.addForce(force);
      other.addForce(-force);
    });
  }

protected:
  using Action = std::function<void(T1&, T2&, const ofVec3f&)>;

  void performAction(Action action);

  void debugDrawBehavior() override {
    performAction([&](T1& entity,
                      T2& other,
                      const ofVec3f& force) {
      if (!_context.highlightedEntities.empty() &&
          !_context.highlightedEntities.containsId(entity.id()) &&
          !_context.highlightedEntities.containsId(other.id())) {
        return;
      }
      drawForceArrow(entity.position(), force);
      drawForceArrow(other.position(), -force);
    });
  }
};

using ObserverOccurrenceForceBehavior
= EntityForceBehavior<ObserverEntity, OccurrenceEntity>;
using OccurrenceOccurrenceForceBehavior
= EntityForceBehavior<OccurrenceEntity, OccurrenceEntity>;


#endif /* EntityForceBehavior_h */
