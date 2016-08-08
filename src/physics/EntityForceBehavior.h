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
#include "Context.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"
#include "Params.h"
#include "ParticleObject.h"
#include "PhysicsBehavior.h"
#include "ValueSequence.h"

using ForceRangeSequence = ValueSequence<float, 2>;

class AbstractEntityForceBehavior
: public AbstractPhysicsBehavior {
public:
  class Params : public ParamsWithEnabled {
  public:
    Params() {
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
    ForceRangeSequence force;
    TParam<float> magnitude;
  };

  AbstractEntityForceBehavior(const Params& params)
  : _params(params) { }

  void debugDraw(Context& context) override {
    if (!_params.enabled.get()) {
      return;
    }
    AbstractPhysicsBehavior::debugDraw(context);
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
  EntityForceBehavior(const Params& params)
  : AbstractEntityForceBehavior(params) { }

  void applyToWorld(Context& context) override {
    if (!_params.enabled()) {
      return;
    }
    performAction(context, [&](T1& entity,
                               T2& other,
                               const ofVec3f& force) {
      entity.addForce(force);
      other.addForce(-force);
    });
  }

protected:
  using Action = std::function<void(T1&, T2&, const ofVec3f&)>;

  void performAction(Context& context,
                     Action action);

  void debugDrawBehavior(Context& context) override {
    performAction(context, [&](T1& entity,
                               T2& other,
                               const ofVec3f& force) {
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
