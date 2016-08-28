//
//  ForceFieldBehavior.h
//  memory
//
//  Created by tekt on 7/7/16.
//
//

#ifndef ForceFieldBehavior_h
#define ForceFieldBehavior_h

#include "../core/Context.h"
#include "../core/Params.h"
#include "../core/ParticleObject.h"
#include "../physics/PhysicsBehavior.h"

class AbstractForceFieldBehavior
: public AbstractPhysicsBehavior {
public:

protected:
  virtual ofVec3f getForceForEntity(Context& context,
                                    ParticleObject* entity) = 0;
  void applyToEntity(Context& context, ParticleObject* entity);
  void debugDrawEntity(Context& context, ParticleObject* entity);
  void beginDebugDraw() override;
  void endDebugDraw() override;
};

class AbstractNoiseForceFieldBehavior
: public AbstractForceFieldBehavior {
public:
  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(scale
          .setKey("scale")
          .setName("Scale")
          .setValueAndDefault(10)
          .setRange(0, 200));
      add(rate
          .setKey("rate")
          .setName("Rate")
          .setValueAndDefault(0.1)
          .setRange(0, 0.5));
      add(magnitude
          .setKey("magnitude")
          .setName("Magnitude")
          .setValueAndDefault(5)
          .setRange(0, 20));
    }

    TParam<float> scale;
    TParam<float> rate;
    TParam<float> magnitude;
  };

  AbstractNoiseForceFieldBehavior(const Params& params)
  : _params(params) { }

protected:
  ofVec3f getForceForEntity(Context& context,
                            ParticleObject* entity) override;

  const Params& _params;
};

template<typename E>
class NoiseForceFieldBehavior
: public AbstractNoiseForceFieldBehavior {
public:
  NoiseForceFieldBehavior(const Params& params)
  : AbstractNoiseForceFieldBehavior(params) { }

  void applyToWorld(Context& context) override {
    if (!_params.enabled()) {
      return;
    }
    for (auto& entity : context.getEntities<E>()) {
      applyToEntity(context, entity.get());
    }
  }

protected:
  void debugDrawBehavior(Context& context) override {
    if (!_params.enabled()) {
      return;
    }
    for (auto& entity : context.getEntities<E>()) {
      debugDrawEntity(context, entity.get());
    }
  }
};

#endif /* ForceFieldBehavior_h */
