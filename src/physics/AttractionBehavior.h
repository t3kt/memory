//
//  AttractionBehavior.h
//  memory
//
//  Created by tekt on 7/7/16.
//
//

#ifndef AttractionBehavior_h
#define AttractionBehavior_h

#include "Params.h"
#include "ParticleObject.h"
#include "PhysicsBehavior.h"
#include "PhysicsWorld.h"


class AbstractAttractionBehavior
: public AbstractPhysicsBehavior {
public:
  class Params
  : public ParamsWithEnabled {
  public:
    Params() {
      add(distanceBounds
          .setKey("distanceBounds")
          .setName("Distance Bounds")
          .setParamValuesAndDefaults(0.04, 0.3)
          .setParamRanges(0, 4)
          .setParamNames("Near", "Far"));
      add(_magnitude
          .setKey("magnitude")
          .setName("Magnitude")
          .setValueAndDefault(0.0001)
          .setRange(0, 0.08));
      add(_reverse
          .setKey("reverse")
          .setName("Reverse")
          .setValueAndDefault(false));
      setEnabledValueAndDefault(true);
    }

    float magnitude() const { return _magnitude.get(); }
    bool reverse() const { return _reverse.get(); }

    float signedMagnitude() const {
      return magnitude() * (reverse() ? -1 : 1);
    }

    FloatValueRange distanceBounds;
  protected:
    TParam<float> _magnitude;
    TParam<bool> _reverse;
  };

  AbstractAttractionBehavior(const Params& params)
  : _params(params) { }

  void applyToWorld(PhysicsWorld* world) override {
    processWorld(world, ApplyMode::ADD_FORCE);
  }

protected:
  enum class ApplyMode {
    ADD_FORCE,
    DEBUG_DRAW,
  };

  void debugDrawBehavior(PhysicsWorld* world) override {
    processWorld(world, ApplyMode::DEBUG_DRAW);
  }

  virtual void processWorld(PhysicsWorld* world,
                            ApplyMode mode) = 0;

  ofVec3f calcAttractionForce(ParticleObject* entity,
                              const ParticleObject* other) {
    ofVec3f posDiff = other->position() - entity->position();
    float lowBound = _params.distanceBounds.lowValue();
    float highBound = _params.distanceBounds.highValue();
    float magnitude = _params.signedMagnitude();
    float dist = posDiff.length();
    if (dist < lowBound || dist > highBound) {
      return ofVec3f(0);
    }
    posDiff.normalize();
    float mag = ofMap(dist,
                      lowBound, highBound,
                      magnitude, 0,
                      true);
    return posDiff * mag;
  }

  void beginDebugDraw() override;
  void endDebugDraw() override;

  void debugDrawEntity(ParticleObject* entity,
                       const ofVec3f& force);
  void debugDrawEntityRange(ParticleObject* entity);

  const Params& _params;

};

template<typename E, typename O>
class AttractionBehavior
: public AbstractAttractionBehavior {
public:
  AttractionBehavior(const Params& params)
  : AbstractAttractionBehavior(params) { }

protected:
  void processWorld(PhysicsWorld* world, ApplyMode mode) override {
    if (!_params.enabled()) {
      return;
    }
    for (auto entity : world->getEntities<E>()) {
      if (!entity->alive()) {
        continue;
      }
      for (auto other : getEntityOthers(entity.get())) {
        if (!other.second->alive()) {
          continue;
        }
        ofVec3f force = calcAttractionForce(entity.get(),
                                            other.second.get());
        switch (mode) {
          case ApplyMode::ADD_FORCE:
            entity->addForce(force);
            break;
          case ApplyMode::DEBUG_DRAW:
            debugDrawEntity(entity.get(), force);
            break;
        }
      }
    }
  }

  EntityMap<O>& getEntityOthers(E *entity);
};


#endif /* AttractionBehavior_h */
