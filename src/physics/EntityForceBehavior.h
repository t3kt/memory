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
#include "Params.h"
#include "ParticleObject.h"
#include "PhysicsBehavior.h"
#include "ObserverEntity.h"
#include "OccurrenceEntity.h"

class ObserverOccurrenceForceBehavior
: public AbstractPhysicsBehavior {
public:
  class Params : public ParamsWithEnabled {
  public:
    Params() {
    }
  };

  ObserverOccurrenceForceBehavior(const Params& params)
  : _params(params) { }

  void applyToWorld(Context& context) override;

private:
  const Params& _params;
};

class ForceRange {
public:
  ForceRange& setInnerDist(float dist);
  ForceRange& setInnerForce(float amount);
  ForceRange& setOuterDist(float dist);
  ForceRange& setOuterForce(float amount);
};

using namespace ofxChoreograph;

static Sequence<float>
createForceRangeSequence(float amount0,
                         float dist1,
                         float amount1,
                         float dist2,
                         float amount2) {
  Sequence<float> sequence(amount0);
  sequence.then<RampTo>(amount1, dist1, easeInOutQuad);
  sequence.then<RampTo>(amount2, dist2, easeInOutQuad);
  return sequence;
}

void ObserverOccurrenceForceBehavior::applyToWorld(Context &context) {
  Sequence<float> sequence(0.0f);
  sequence.then(std::make_shared<RampTo<float>>(1.0f, // duration
                                                -1.0f, // start value
                                                0.0f, // end value
                                                easeNone));
  //...
}


#endif /* EntityForceBehavior_h */
