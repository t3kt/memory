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
  AbstractForceFieldBehavior(Context& context)
  : AbstractPhysicsBehavior(context) { }

protected:
  virtual ofVec3f getForceForEntity(ParticleObject* entity) = 0;
  void applyToEntity(ParticleObject* entity);
  void debugDrawEntity(ParticleObject* entity);
  void beginDebugDraw() override;
  void endDebugDraw() override;
};

class NoiseForceFieldParams : public ParamsWithEnabled {
public:
  NoiseForceFieldParams() {
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

class AbstractNoiseForceFieldBehavior
: public AbstractForceFieldBehavior {
public:
  using Params = NoiseForceFieldParams;

  AbstractNoiseForceFieldBehavior(Context& context,
                                  const Params& params)
  : AbstractForceFieldBehavior(context)
  , _params(params) { }

protected:
  ofVec3f getForceForEntity(ParticleObject* entity) override;

  const Params& _params;
};

template<typename E>
class NoiseForceFieldBehavior
: public AbstractNoiseForceFieldBehavior {
public:
  NoiseForceFieldBehavior(Context& context, const Params& params)
  : AbstractNoiseForceFieldBehavior(context, params) { }

  void update() override {
    if (!_params.enabled()) {
      return;
    }
    for (auto& entity : _context.getEntities<E>()) {
      applyToEntity(entity.get());
    }
  }

protected:
  void debugDrawBehavior() override {
    if (!_params.enabled()) {
      return;
    }
    for (auto& entity : _context.getEntities<E>()) {
      debugDrawEntity(entity.get());
    }
  }
};

#endif /* ForceFieldBehavior_h */
