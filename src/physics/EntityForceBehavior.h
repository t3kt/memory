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

class ObserverOccurrenceForceBehavior
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

  ObserverOccurrenceForceBehavior(const Params& params)
  : _params(params) { }

  void applyToWorld(Context& context) override;

  void debugDraw(Context& context) override {
    if (!_params.enabled.get()) {
      return;
    }
    AbstractPhysicsBehavior::debugDraw(context);
  }

protected:
  virtual void beginDebugDraw() override;
  virtual void debugDrawBehavior(Context& context) override;
  virtual void endDebugDraw() override;

private:
  using Action = std::function<void(ObserverEntity&,
                                    OccurrenceEntity&,
                                    const ofVec3f&)>;

  void performAction(Context& context,
                     Action action);

  ofVec3f calcAttraction(const ofVec3f& entityPosition,
                         const ofVec3f& otherPosition) const;

  const Params& _params;
};


#endif /* EntityForceBehavior_h */
