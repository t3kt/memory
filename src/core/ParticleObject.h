//
//  ParticleObject.h
//  memory-prototype-2
//
//  Created by tekt on 6/26/16.
//
//

#ifndef ParticleObject_h
#define ParticleObject_h

#include <vector>
#include "WorldObject.h"
#include "State.h"
#include "Behavior.h"
#include "Params.h"
#include "Events.h"
#include "Bounds.h"

class ParticleObject : public StandardWorldObject {
public:
  class Params : public ::Params {
  public:
    Params(std::string name);

    ofParameter<float> damping;
    ofParameter<float> speed;
  };

  ParticleObject(ofVec3f pos, const Params& params);

  void setInitialCondition(ofVec3f pos, ofVec3f vel);
  void setVelocity(ofVec3f velocity) { _velocity = velocity; }

  void resetForce();
  void addForce(ofVec3f force);
  void addDampingForce();

  virtual void update(const State& state) override;
protected:
  virtual void outputFields(std::ostream& os) const override;
private:
  const Params& _params;
  ofVec3f _velocity;
  ofVec3f _force;

  friend class AbstractReboundBehavior;
};

class AbstractReboundBehavior {
public:
  AbstractReboundBehavior(const Bounds& bounds);

protected:
  bool updateEntity(ParticleObject& entity, const State& state);

private:
  const Bounds& _bounds;
};

template<typename E>
class ReboundBehavior
: public AbstractReboundBehavior
, public Behavior<E> {
public:
  ReboundBehavior(const Bounds& bounds)
  : AbstractReboundBehavior(bounds) { }

  ofxLiquidEvent<EntityEventArgs<E>> entityRebounded;

  void update(E& entity, const State& state) override {
    if (updateEntity(entity, state)) {
      EntityEventArgs<E> e(state, entity);
      entityRebounded.notifyListeners(e);
    }
  }
};

class AbstractEntityAttraction {
public:
  class Params : public ::Params {
  public:
    Params(std::string name);

    ofParameter<bool> enabled;
    FloatValueRange distanceBounds;
    FloatValueRange forceRange;
  };

  AbstractEntityAttraction(const Params& params) : _params(params) {}

protected:
  const Params& _params;
};

template<typename E, typename O>
class EntityAttraction
: public AbstractEntityAttraction
, public Behavior<E> {
public:
  EntityAttraction(const AbstractEntityAttraction::Params& params)
  : AbstractEntityAttraction(params) {}

  void update(E& entity, const State& state) override {
    if (!_params.enabled.get()) {
      return;
    }
    float lowBound = _params.distanceBounds.lowValue.get();
    float highBound = _params.distanceBounds.highValue.get();
    float lowMagnitude = _params.forceRange.lowValue.get();
    float highMagnitude = _params.forceRange.highValue.get();
    std::vector<shared_ptr<O>> others = getOthers(entity);
    for (auto other : others) {
      ofVec3f posDiff = other->position() - entity.position();
      float dist = posDiff.length();
      if (dist < lowBound || dist > highBound) {
        continue;
      }
      float mag = ofMap(dist, lowBound, highBound, lowMagnitude, highMagnitude, true);
      posDiff.normalize();
      entity.addForce(posDiff * mag);
    }
  }
protected:
  virtual std::vector<shared_ptr<O>> getOthers(E& entity) const = 0;
};

class AbstractSpatialNoiseForce {
public:
  class Params : public ::Params {
  public:
    Params(std::string name);

    ofParameter<bool> enabled;
    ofParameter<float> scale;
    ofParameter<float> rate;
    ofParameter<float> magnitude;
  };

  AbstractSpatialNoiseForce(const Params& params)
  : _params(params) { }
protected:
  ofVec3f getForce(ofVec3f position, const State& state) const;

  const Params& _params;
};

template<typename E>
class SpatialNoiseForce
: public AbstractSpatialNoiseForce
, public Behavior<E> {
public:
  SpatialNoiseForce(const Params& params)
  : AbstractSpatialNoiseForce(params) { }

  void update(E& entity, const State& state) override {
    if (!_params.enabled.get()) {
      return;
    }
    ofVec3f force = getForce(entity.position(), state);
    entity.addForce(force);
  }
};

#endif /* ParticleObject_h */
