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

class ParticleObject : public WorldObject {
public:
  class Params : public ::Params {
  public:
    Params()
    : ::Params() {
      add(_damping
          .setKey("damping")
          .setName("Damping")
          .setValueAndDefault(0.01)
          .setRange(0, 1));
      add(_speed
          .setKey("speed")
          .setName("Speed")
          .setValueAndDefault(1)
          .setRange(0, 10));
    }

    float damping() const { return _damping.get(); }
    float speed() const { return _speed.get(); }

  private:
    TParam<float> _damping;
    TParam<float> _speed;
  };

  ParticleObject(ofVec3f pos, const Params& params, const State& state);

  void setInitialCondition(ofVec3f pos, ofVec3f vel);
  void setVelocity(ofVec3f velocity) { _velocity = velocity; }

  void resetForce();
  void addForce(ofVec3f force);
  void addDampingForce();

  ofVec3f* positionPtr() { return &_position; }
  ofVec3f* velocityPtr() { return &_velocity; }
  ofVec3f* forcePtr() { return &_force; }

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
    Params()
    : ::Params() {
      add(_enabled
          .setKey("enabled")
          .setName("Enabled")
          .setValueAndDefault(true));
      add(distanceBounds
          .setKey("distanceBounds")
          .setName("Distance Bounds")
          .setParamValuesAndDefaults(0.04, 0.3)
          .setParamRanges(0, 4));
      add(forceRange
          .setKey("forceRange")
          .setName("Force Range")
          .setParamValuesAndDefaults(0.0001, 0)
          .setParamRanges(-0.05, 0.05));
    }

    bool enabled() const { return _enabled.get(); }

    void setEnabled(bool enabled) { _enabled.set(enabled); }

    FloatValueRange distanceBounds;
    FloatValueRange forceRange;
  private:
    TParam<bool> _enabled;
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
    if (!_params.enabled()) {
      return;
    }
    float lowBound = _params.distanceBounds.lowValue();
    float highBound = _params.distanceBounds.highValue();
    float lowMagnitude = _params.forceRange.lowValue();
    float highMagnitude = _params.forceRange.highValue();
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
    Params()
    : ::Params() {
      add(_enabled
          .setKey("enabled")
          .setName("Enabled")
          .setValueAndDefault(true));
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
          .setValueAndDefault(0.0001)
          .setRange(0, 0.005));
    }

    bool enabled() const { return _enabled.get(); }
    float scale() const { return _scale.get(); }
    float rate() const { return _rate.get(); }
    float magnitude() const { return _magnitude.get(); }

    void setEnabled(bool enabled) { _enabled.set(enabled); }

  private:
    TParam<bool> _enabled;
    TParam<float> _scale;
    TParam<float> _rate;
    TParam<float> _magnitude;
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
    if (!_params.enabled()) {
      return;
    }
    ofVec3f force = getForce(entity.position(), state);
    entity.addForce(force);
  }
};

#endif /* ParticleObject_h */
