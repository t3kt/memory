//
//  PhysicsBehavior.h
//  memory
//
//  Created by tekt on 7/5/16.
//
//

#ifndef PhysicsBehavior_h
#define PhysicsBehavior_h

#include "Params.h"
#include "State.h"
#include "PhysicsWorld.h"
#include "ParticleObject.h"
#include "Bounds.h"

class AbstractPhysicsBehavior {
public:
  virtual void applyToWorld(PhysicsWorld* world) = 0;
  virtual void debugDraw(PhysicsWorld* world) {
    beginDebugDraw();
    debugDrawBehavior(world);
    endDebugDraw();
  }
protected:
  virtual void beginDebugDraw() { }
  virtual void debugDrawBehavior(PhysicsWorld* world) {}
  virtual void endDebugDraw() { }
};

class BoundsBehavior
: public AbstractPhysicsBehavior {
public:
  BoundsBehavior(const Params& params, const Bounds& bounds)
  : _params(params)
  , _bounds(bounds) { }

  void applyToWorld(PhysicsWorld* world) override;

private:
  void applyToEntity(PhysicsWorld* world, ParticleObject* entity);

  const Params& _params;
  const Bounds& _bounds;
};

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
      add(forceRange
          .setKey("forceRange")
          .setName("Force Range")
          .setParamValuesAndDefaults(0.0001, 0)
          .setParamRanges(-0.05, 0.05)
          .setParamNames("Near", "Far"));
      setEnabledValueAndDefault(true);
    }

    FloatValueRange distanceBounds;
    FloatValueRange forceRange;
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
    float lowMagnitude = _params.forceRange.lowValue();
    float highMagnitude = _params.forceRange.highValue();
    float dist = posDiff.length();
    if (dist < lowBound || dist > highBound) {
      return ofVec3f(0);
    }
    float mag = ofMap(dist,
                      lowBound, highBound,
                      lowMagnitude, highMagnitude,
                      true);
    posDiff.normalize();
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

class AbstractSpatialNoiseForceBehavior
: public AbstractPhysicsBehavior {
public:
  class Params : public ParamsWithEnabled {
  public:
    Params() {
      add(_scale
          .setKey("scale")
          .setName("Scale")
          .setValueAndDefault(1)
          .setRange(0, 4));
      add(_rate
          .setKey("rate")
          .setName("Rate")
          .setValueAndDefault(0.1)
          .setRange(0, 0.5));
      add(_magnitude
          .setKey("magnitude")
          .setName("Magnitude")
          .setValueAndDefault(0.001)
          .setRange(0, 0.05));
    }

    float scale() const { return _scale.get(); }
    float rate() const { return _rate.get(); }
    float magnitude() const { return _magnitude.get(); }

  private:
    TParam<float> _scale;
    TParam<float> _rate;
    TParam<float> _magnitude;
  };

  AbstractSpatialNoiseForceBehavior(const Params& params)
  : _params(params) { }

protected:
  ofVec3f getForceForEntity(PhysicsWorld* world, ParticleObject* entity);
  void applyToEntity(PhysicsWorld* world, ParticleObject* entity);
  void debugDrawEntity(PhysicsWorld* world, ParticleObject* entity);
  void beginDebugDraw() override;
  void endDebugDraw() override;

  const Params& _params;
};

template<typename E>
class SpatialNoiseForceBehavior
: public AbstractSpatialNoiseForceBehavior {
public:
  SpatialNoiseForceBehavior(const Params& params)
  : AbstractSpatialNoiseForceBehavior(params) { }

  void applyToWorld(PhysicsWorld* world) override {
    if (!_params.enabled()) {
      return;
    }
    for (auto entity : world->getEntities<E>()) {
      applyToEntity(world, entity.get());
    }
  }

protected:
  void debugDrawBehavior(PhysicsWorld* world) override {
    if (!_params.enabled()) {
      return;
    }
    beginDebugDraw();
    for (auto entity : world->getEntities<E>()) {
      debugDrawEntity(world, entity.get());
    }
    endDebugDraw();
  }
};


#endif /* PhysicsBehavior_h */
