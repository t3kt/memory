//
//  AttractionBehavior.h
//  memory
//
//  Created by tekt on 7/7/16.
//
//

#ifndef AttractionBehavior_h
#define AttractionBehavior_h

#include "../core/Context.h"
#include "../core/ObserverEntity.h"
#include "../core/OccurrenceEntity.h"
#include "../core/Params.h"
#include "../core/ParticleObject.h"
#include "../physics/PhysicsBehavior.h"

class RangedForceParams
: public ParamsWithEnabled {
public:
  RangedForceParams() {
    add(distanceBounds
        .setKey("distanceBounds")
        .setName("Distance Bounds")
        .setParamValuesAndDefaults(5, 100)
        .setParamRanges(0, 500)
        .setParamNames("Near", "Far"));
    add(magnitude
        .setKey("magnitude")
        .setName("Magnitude")
        .setValueAndDefault(4)
        .setRange(0, 20));
    add(reverse
        .setKey("reverse")
        .setName("Reverse")
        .setValueAndDefault(false));
    setEnabledValueAndDefault(true);
  }

  float signedMagnitude() const {
    return magnitude() * (reverse() ? -1 : 1);
  }

  FloatValueRange distanceBounds;
  TParam<float> magnitude;
  TParam<bool> reverse;
};

class AbstractAttractionBehavior
: public AbstractPhysicsBehavior {
public:
  using Params = RangedForceParams;

  AbstractAttractionBehavior(Context& context,
                             const Params& params)
  : AbstractPhysicsBehavior(context)
  , _params(params) { }

  void applyToWorld() override {
    if (!_params.enabled()) {
      return;
    }
    processWorld(ApplyMode::ADD_FORCE);
  }

protected:
  enum class ApplyMode {
    ADD_FORCE,
    DEBUG_DRAW,
  };

  void debugDrawBehavior() override {
    if (!_params.enabled()) {
      return;
    }
    processWorld(ApplyMode::DEBUG_DRAW);
  }

  virtual void processWorld(ApplyMode mode) = 0;

  ofVec3f calcAttractionForce(const ofVec3f& entityPosition,
                              const ofVec3f& otherPosition,
                              float lowBound,
                              float highBound,
                              float magnitude);

  void beginDebugDraw() override;
  void endDebugDraw() override;

  virtual void debugDrawEntity(ParticleObject* entity,
                               const ofVec3f& force);
  void debugDrawEntityRange(ParticleObject* entity);

  const Params& _params;

};

template<typename E, typename O>
class AttractionBehavior
: public AbstractAttractionBehavior {
public:
  AttractionBehavior(Context& context, const Params& params)
  : AbstractAttractionBehavior(context, params) { }

protected:
  void processWorld(ApplyMode mode) override {
    float lowBound = _params.distanceBounds.lowValue();
    float highBound = _params.distanceBounds.highValue();
    float magnitude = _params.signedMagnitude();
    for (auto& entity : _context.getEntities<E>()) {
      if (!entity->alive()) {
        continue;
      }
      for (const auto& other : getEntityOthers(entity.get())) {
        if (!other.second->alive()) {
          continue;
        }
        ofVec3f force = calcAttractionForce(entity->position(),
                                            other.second->position(),
                                            lowBound,
                                            highBound,
                                            magnitude);
        if (force.length() == 0) {
          continue;
        }
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

template<>
class AttractionBehavior<ObserverEntity, ObserverEntity>
: public AbstractAttractionBehavior {
public:
  AttractionBehavior(Context& context, const Params& params)
  : AbstractAttractionBehavior(context, params) { }

protected:
  void processWorld(ApplyMode mode) override {
    float lowBound = _params.distanceBounds.lowValue();
    float highBound = _params.distanceBounds.highValue();
    float magnitude = _params.signedMagnitude();
    for (auto& entity : _context.getEntities<ObserverEntity>()) {
      if (!entity->alive()) {
        continue;
      }
      for (const auto& other : entity->getConnectedObservers()) {
        if (!other.second->alive()) {
          continue;
        }
        ofVec3f force = calcAttractionForce(entity->position(),
                                            other.second->position(),
                                            lowBound,
                                            highBound,
                                            magnitude);
        if (force.length() == 0) {
          continue;
        }
        switch (mode) {
          case ApplyMode::ADD_FORCE:
            entity->addForce(force);
            break;
          case ApplyMode::DEBUG_DRAW:
            if (!_context.highlightedEntities.empty() &&
                !_context.highlightedEntities.containsId(entity->id())) {
              break;
            }
            debugDrawEntity(entity.get(), force);
            break;
        }
      }
    }
  }
};


#endif /* AttractionBehavior_h */
